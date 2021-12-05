#define FORWARD 0x06
#define REVERSE 0x09
#define LEFT 0x05
#define RIGHT 0x0A

#define SPIFFS_INTERVAL 1000
#define NOTIFY_INTERVAL 200
#define NOTIFY_ENABLE 4
#define NOTIFY_DISABLE 0
#define SPEED_LOW 80

#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<WebSocketsServer.h>
#include<FS.h>
#include "index.h"
#include "styles.h"
#include "script.h"

uint8_t pins[4] = {14, 12, 13, 5};
uint8_t dir[4] = {0,};
uint8_t headLightPin = 15;
uint8_t notify = 0;

IPAddress apIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 12, 7);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void returnOK() {
  server.send(200, "text/plain", "");
}
void returnNotFound(String msg) {
  server.send(400, "text/plain", msg);
}
void returnErr(String msg) {
  server.send(500, "text/plain", msg);
}
void handleOtherFiles() {
  if (checkInFS(server.uri())) {
    return;
  }
  returnNotFound("Oops check the uri");
}


bool checkInFS(String path) {
  String dataType = "text/plain";
  const char* data;
  if (path.endsWith("/") || path.endsWith(".html")) {
    dataType = "text/html";
    data = _index;
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
    data = _styles;
  } else if (path.endsWith(".js")) {
    dataType = "application/js";
    data = _script;
  } else {
    return false;
  }
  server.send_P(200, dataType.c_str(), data);
  return true;
}

struct stateFields {
  bool headLight; //1 -> ON
  uint8_t speed; //0 - 100
  uint8_t brightness;//0 -100
} state;

struct credentials {
  String STA_SSID;
  String STA_PASS;
  String ESP_SSID;
  String ESP_PASS;
} creds;

void resetState() {
  File file = SPIFFS.open("/state.txt", "w");
  file.print("0\n255\n255\n");
  file.close();
}
void resetCreds() {
  File file = SPIFFS.open("/creds.txt", "w");
  file.print("\n\nOculus\nOculus123\n");
  file.close();
}

void loadState() {
  if (!SPIFFS.exists("/state.txt")) {
    resetState();
  }
  File file = SPIFFS.open("/state.txt", "r");
  state.headLight = (bool)file.readStringUntil('\n').toInt();
  state.speed = (uint8_t)file.readStringUntil('\n').toInt();
  state.brightness = (uint8_t)file.readStringUntil('\n').toInt();
  file.close();
}

void headLightUpdate() {
  analogWrite(headLightPin, state.headLight ? state.brightness : 0);
}

void loadCreds() {
  if (!SPIFFS.exists("/creds.txt")) {
    resetCreds();
  }
  File file = SPIFFS.open("/creds.txt", "r");
  creds.STA_SSID = file.readStringUntil('\n');
  creds.STA_PASS = file.readStringUntil('\n');
  creds.ESP_SSID = file.readStringUntil('\n');
  creds.ESP_PASS = file.readStringUntil('\n');
  file.close();
  Serial.printf("%s\n%s\n%s\n%s\n", creds.STA_SSID.c_str(), creds.STA_PASS.c_str(), creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
  headLightUpdate();
}

String stateJSON() {
  String JSON = "{\"headLight\" : " + (String)state.headLight + ",";
  JSON += "\"speed\" : " + (String)state.speed + ",";
  JSON += "\"brightness\" : " + (String)state.brightness + ",";
  JSON += "\"voltage\" : " + (String)ESP.getVcc() + ",";
  JSON += "\"STA_SSID\" : \"" + creds.STA_SSID + "\",";
  JSON += "\"STA_PASS\" : \"" + creds.STA_PASS + "\",";
  JSON += "\"AP_SSID\" : \"" + creds.ESP_SSID + "\",";
  JSON += "\"AP_PASS\" : \"" + creds.ESP_PASS + "\"}";
  return JSON;
}

void updateState() {
  File file = SPIFFS.open("/state.txt", "w");
  file.printf("%d\n%d\n%d\n", state.headLight, state.speed, state.brightness);
  file.close();
}

void updateCreds() {
  File file = SPIFFS.open("/creds.txt", "w");
  file.printf("%s\n%s\n%s\n%s\n", creds.STA_SSID.c_str(), creds.STA_PASS.c_str(), creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
  file.close();
}

bool validateCreds(String _ssid, String _pass) {
  if (_ssid == "" || _ssid.length()<1 || _ssid.length()>32)
    return false;
  if (_pass == "" || _pass.length()<8 || _pass.length()>63)
    return false;
  return true;
}

void connectWiFi() {
  uint8_t wiFiType = (creds.ESP_PASS != "") ? 0x01 : 0;
  wiFiType |= (creds.ESP_PASS != "") ? 0x02 : 0;
  wiFiType |= (creds.STA_SSID != "") ? 0x04 : 0;
  wiFiType |= (creds.STA_PASS != "") ? 0x08 : 0;

  uint8_t tries = 0;
  switch (wiFiType) {
    case 0x0F : {
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
        WiFi.begin(creds.STA_SSID.c_str(), creds.STA_PASS.c_str());

        Serial.print("Connecting ...");

        while (WiFi.status() != WL_CONNECTED)
        {
          delay(100);
          if (tries++ == 100) {
            creds.STA_SSID = "";
            creds.STA_PASS = "";
            connectWiFi();
            break;
          }
        }
        break;
      }
    case 0x03 : {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
        break;
      }
    case 0x0C : {
        WiFi.mode(WIFI_STA);
        WiFi.begin(creds.STA_SSID.c_str(), creds.STA_PASS.c_str());

        Serial.print("Connecting ...");
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(100);
          Serial.print(".");

          if (tries++ == 100) {
            resetCreds();
            connectWiFi();
            break;
          }
        }
        break;
      }
  }
}

unsigned long t_updateFS;

void updateFS() {
  if (millis() - t_updateFS > SPIFFS_INTERVAL) {
    t_updateFS = millis();
    updateState();
  }
}
void setup() {
  // put your setup code here, to run once:
  for (uint8_t i = 0; i < 4; i++)
    pinMode(pins[i], OUTPUT);
  pinMode(headLightPin, OUTPUT);
  SPIFFS.begin();
  loadState();
  loadCreds();
  Serial.begin(115200);

  connectWiFi();
  loadCreds();

  notify = NOTIFY_ENABLE;

  server.on("/off", HTTP_GET, []() {
    stopCar();
    returnOK();
  });

  server.on("/state", HTTP_GET, []() {
    server.send(200, "text/json", stateJSON());
  });

  server.on("/cred", HTTP_POST, []() {
    if (server.hasArg("type") && server.hasArg("ssid") && server.hasArg("pass") && (server.arg("type") != NULL) && (server.arg("ssid") != NULL) && (server.arg("pass") != NULL)) {
      String type = server.arg(0);
      String _SSID = server.arg(1);
      String _PASS = server.arg(2);
      Serial.println(type);
      Serial.println(_SSID);
      Serial.println(_PASS);

      if (validateCreds(_SSID, _PASS)) {
        if (type == "STA") {
          creds.STA_SSID = _SSID;
          creds.STA_PASS = _PASS;
        } else if (type == "AP") {
          creds.ESP_SSID = _SSID;
          creds.ESP_PASS = _PASS;
        }
        updateCreds();
      }
      returnOK();
    } else {
      returnErr("Check the entered credentials");
    }
  });

  server.on("/resetstate", HTTP_GET, []() {
    resetState();
    loadState();
    returnOK();
  });
  server.on("/resetcreds", HTTP_GET, []() {
    resetCreds();
    loadCreds();
    returnOK();
  });

  server.onNotFound(handleOtherFiles);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(socketHandle);
}



void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  webSocket.loop();
  updateFS();
  notifyBlinkAsync();
  wdt_reset();
}


void socketHandle(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: {
        stopCar();
        break;
      }
    case WStype_CONNECTED: {
        notify = NOTIFY_ENABLE;
        break;
      }
    case WStype_TEXT: {
        //do all stuff here
        switch (payload[0]) {
          case 'c': {
              uint8_t index = payload[1] - '0';
              uint8_t val = payload[2] - '0';
              if (index < 4 && val < 4) {
                dir[index] = val;
                updateCar();
              }
              break;
            }
          case 's': {
              switch (payload[1]) {
                case 'h': {
                    state.headLight = payload[2] - '0'; //1- on 0- off
                    headLightUpdate();
                    break;
                  }
                case 'b': {
                    state.brightness = (uint8_t) strtol((const char *)&payload[2], NULL, 10);
                    headLightUpdate();
                    break;
                  }
                case 's': {
                    state.speed = (uint8_t) strtol((const char *)&payload[2], NULL, 10);
                    break;
                  }
                case 'r': {
                    updateState();
                    notifyBlinkDelay();
                    ESP.reset();
                  }
                case 'd': {
                    updateState();
                    notifyBlinkDelay();
                    ESP.deepSleep(0);
                  }
              }
              break;
            }
          case 'o': {
              stopCar();
              break;
            }
        }
      }
  }
}


void updateCar() {
  uint8_t DIRECTION = 0;
  uint8_t scaledSpeed = map(state.speed, 0, 255, 50, 255);
  DIRECTION = (dir[0] ? FORWARD : 0);
  DIRECTION |= (dir[1] ? REVERSE : 0);
  DIRECTION |= (dir[2] ? LEFT : 0);
  DIRECTION |= (dir[3] ? RIGHT : 0);

  for (uint8_t i = 0; i < 4; i++) {
    analogWrite(pins[i], ((DIRECTION >> i) & 0x01) ? scaledSpeed : 0 );
  }
}

void stopCar() {
  for (uint8_t i = 0; i < 4; i++) dir[i] = 0;
  updateCar();
}

unsigned long t_notify = 0;
void notifyBlinkAsync() {
  if (state.headLight && notify) {
    if (millis() - t_notify > NOTIFY_INTERVAL) {
      t_notify = millis();
      notify--;
      analogWrite(headLightPin, (notify % 2) ? 0 : state.brightness);
    }
  } else {
    notify = NOTIFY_DISABLE;
  }
}
void notifyBlinkDelay() {
  if (state.headLight) {
    for (uint8_t i = 0; i < 2; i++) {
      analogWrite(headLightPin, state.brightness);
      delay(NOTIFY_INTERVAL);
      analogWrite(headLightPin, 0);
      delay(NOTIFY_INTERVAL);
    }
  }
}
