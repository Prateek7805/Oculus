#define FORWARD 0x06
#define REVERSE 0x09
#define LEFT 0x05
#define RIGHT 0x0A

#define SPIFFS_TIME 1000


#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<WebSocketsServer.h>
#include<FS.h>
#include "index.h"
#include "styles.h"
#include "script.h"

uint8_t pins[4] = {14,12,13,5};
float speedVector[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
uint8_t dir[4] = {0,};



IPAddress apIP(192, 168, 4,1);  
IPAddress gateway(192, 168,12, 7);
IPAddress subnet(255, 255, 255, 0);
 
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void returnOK(){
  server.send(200,"text/plain", "");
}
void returnNotFound(String msg){
  server.send(400, "text/plain", msg);
}

void handleOtherFiles(){
  if(checkInFS(server.uri())){
    return;
  }
  returnNotFound("Oops check the uri");
}


bool checkInFS(String path){
  String dataType = "text/plain";
  const char* data;
  if(path.endsWith("/") || path.endsWith(".html")){
    dataType = "text/html";
    data = _index;
  }else if(path.endsWith(".css")){
    dataType = "text/css";
    data = _styles;
  }else if(path.endsWith(".js")){
    dataType = "application/js";
    data = _script;
  }else{
    return false;
  }
  server.send_P(200, dataType.c_str(), data);
  return true;
}

struct stateFields {
  bool headLight; //1 -> ON
  uint16_t speed; //0 - 100
  uint16_t brightness;//0 -100
  bool commType;//1 -> Web Socket 0 -> Fetch
  bool darkMode;//1 -> DARK 0-> LIGHT
} state;

struct credentials {
  String STA_SSID;
  String STA_PASS;
  String ESP_SSID;
  String ESP_PASS;
} creds;

void resetState(){
   File file = SPIFFS.open("/state.txt", "w");
   file.print("0\n1023\n1023\n1\n1\n");
   file.close();
}
void resetCreds(){
  File file = SPIFFS.open("/creds.txt", "w");
  file.print("\n\nOculus\nOculus123\n");
  file.close();
}

void loadState(){
  if(!SPIFFS.exists("/state.txt")){
   resetState();
  }
  File file = SPIFFS.open("/state.txt", "r");
  state.headLight = (bool)file.readStringUntil('\n').toInt();
  state.speed = (uint16_t)file.readStringUntil('\n').toInt();
  state.brightness = (uint16_t)file.readStringUntil('\n').toInt();
  state.commType = (bool)file.readStringUntil('\n').toInt();
  state.darkMode = (bool)file.readStringUntil('\n').toInt();
  file.close();
}

void loadCreds(){
  
  if(!SPIFFS.exists("/creds.txt")){
    resetCreds();
  }
  File file = SPIFFS.open("/creds.txt", "r");
  creds.STA_SSID = file.readStringUntil('\n');
  creds.STA_PASS = file.readStringUntil('\n');
  creds.ESP_SSID = file.readStringUntil('\n');
  creds.ESP_PASS = file.readStringUntil('\n');
  file.close();
  Serial.printf("%s\n%s\n%s\n%s\n",creds.STA_SSID.c_str(), creds.STA_PASS.c_str(), creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
}

String stateJSON(){
  String JSON = "{\"headLight\" : " + (String)state.headLight + ",";
  JSON += "\"speed\" : " + (String)state.speed + ",";
  JSON += "\"brightness\" : " + (String)state.brightness + ",";
  JSON += "\"commType\" : " + (String)state.commType + ",";
  JSON += "\"darkMode\" : " + (String)state.darkMode + ",";
  JSON += "\"voltage\" : " +(String)ESP.getVcc() + ",";
  JSON += "\"STA_SSID\" : \"" + creds.STA_SSID + "\",";
  JSON += "\"STA_PASS\" : \"" + creds.STA_PASS + "\",";
  JSON += "\"AP_SSID\" : \"" + creds.ESP_SSID + "\",";
  JSON += "\"AP_PASS\" : \"" +creds.ESP_PASS + "\"}";
  return JSON;
}

void updateState(){
  File file = SPIFFS.open("/state.txt", "w");
  file.printf("%d\n%d\n%d\n%d\n%d\n", state.headLight,state.speed, state.brightness,state.commType,state.darkMode);
  file.close();
}
void updateCreds(){
  File file = SPIFFS.open("/creds.txt", "w");
  file.printf("%s\n%s\n%s\n%s\n",creds.STA_SSID.c_str(), creds.STA_PASS.c_str(), creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
  file.close();
}
bool validateCreds(String _ssid, String _pass){
   if(_ssid == "")
    return false;
   if(_pass == "")
    return false;
  return true;
}
void connectWiFi(){
  uint8_t wiFiType = (creds.ESP_PASS != "")? 0x01 : 0;
  wiFiType |= (creds.ESP_PASS != "")? 0x02 : 0;
  wiFiType |= (creds.STA_SSID != "")? 0x04 : 0;
  wiFiType |= (creds.STA_SSID != "")? 0x08 : 0;
  uint8_t tries = 0;
  switch(wiFiType){
    case 0x0F : {
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(creds.ESP_SSID.c_str(), creds.ESP_PASS.c_str());
      WiFi.begin(creds.STA_SSID.c_str(), creds.STA_PASS.c_str());
      
      Serial.print("Connecting ...");
      
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(100);
        Serial.println(tries);
        
        if(tries++ == 100){
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
        
        if(tries++ == 100){
          resetCreds();
          connectWiFi();
          break;
        }
      }
      break;
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  for(uint8_t i=0; i<4; i++)
    pinMode(pins[i], OUTPUT);
  SPIFFS.begin();
  loadState();
  loadCreds();
  Serial.begin(115200);

  connectWiFi();
  
  server.on("/c",HTTP_GET,[](){
    byte DIR_index = server.arg(0).toInt();
    byte DIR_state = server.arg(1).toInt();
    dir[DIR_index] = DIR_state;
    Serial.printf("%s %s\n",server.arg(0), server.arg(1));
    //Serial.print(dir[0]);Serial.print(dir[1]);Serial.print(dir[2]);Serial.println(dir[3]);
    updateCar();
    returnOK();
  });
  
  server.on("/off",HTTP_GET,[](){
    for(uint8_t i=0; i<4; i++) dir[i] = 0;
    updateCar();
    returnOK();
  });
  
  server.on("/state", HTTP_GET, [](){server.send(200, "text/json", stateJSON());});
  
  server.on("/cred", HTTP_GET, [](){
    String type = server.arg(0);
    String _SSID= server.arg(1);
    String _PASS= server.arg(2);
    if(validateCreds(_SSID, _PASS)){
      if(type == "STA"){
           creds.STA_SSID = _SSID;
           creds.STA_PASS = _PASS;
      }else if(type == "AP"){
           creds.ESP_SSID = _SSID;
           creds.ESP_PASS = _PASS;
      }
      updateCreds();
    }
    returnOK();
  });

  server.on("/resetstate", HTTP_GET, [](){resetState(); loadState(); returnOK();});
  server.on("/resetcreds", HTTP_GET, [](){resetCreds(); loadCreds(); returnOK();});
    
  server.onNotFound(handleOtherFiles);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(socketHandle);
}

unsigned long updateFS;
void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  webSocket.loop();
  if(millis() - updateFS > SPIFFS_TIME){
    updateState();
    updateFS = millis();   
  }
  wdt_reset();
}


void socketHandle(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    //do all stuff here
    switch(payload[0]){
      case 'c':{
                uint8_t index = payload[1] - '0';
                uint8_t val = payload[2] - '0';
                if(index < 4 && val < 4){
                  dir[index] = val;
                  updateCar();
                }
                break;
               }
      case 's':{
                switch(payload[1]){
                  case 'h':{
                    state.headLight = payload[2] - '0'; //here the headLight is on
                    break;
                  }
                  case 'b':{
                    state.brightness = (uint16_t) strtol((const char *)&payload[2], NULL, 10);
                    break;  
                  }
                  case 's':{
                    state.speed = (uint16_t) strtol((const char *)&payload[2], NULL, 10);
                    break;
                  }
                  case 'r':{
                    updateState();
                    ESP.reset();
                  }
                  case 'd':{
                    updateState();
                    ESP.deepSleep(0);
                  }
                }
                break;
                }
      case 'o':{
                for(uint8_t i=0; i<4; i++) dir[i] = 0;
                updateCar();
                break;
               }
    }
  }
}


void updateCar(){
  uint8_t DIRECTION = 0;
  DIRECTION = (dir[0]? FORWARD : 0);
  DIRECTION |= (dir[1]? REVERSE : 0);
  DIRECTION |= (dir[2]? LEFT : 0);
  DIRECTION |= (dir[3]? RIGHT : 0);
  
    for(uint8_t i=0; i<4; i++){
      analogWrite(pins[i], ((DIRECTION>>i)&0x01)? (uint16_t)state.speed : 0 );
    }
}
