#define FORWARD 0x06
#define REVERSE 0x09
#define LEFT 0x0A
#define RIGHT 0x05

uint8_t pins[4] = {14,12,13,5};
uint16_t SPEED = 1023;
float speedVector[5] = {0.2, 0.4, 0.6, 0.8, 1.0};

void runCar(uint8_t COMMAND, uint16_t SPEED){
  for(uint8_t j=0; j<5; j++){
    for(uint8_t i=0; i<4; i++){
      analogWrite(pins[i], ((COMMAND>>i)&0x01)? (int)SPEED*speedVector[j] : 0 );
    }
  }
}

void setup() {
  // put your setup code here, to run once:
for(uint8_t i=0; i<4; i++){
  pinMode(pins[i], OUTPUT);
}
}

void loop() {
  // put your main code here, to run repeatedly:
  runCar(FORWARD,SPEED);
  delay(1000);
  runCar(REVERSE, SPEED);
  delay(1000);
  runCar(LEFT, SPEED);
  delay(1000);
  runCar(RIGHT, SPEED);
  delay(1000);
  
}
