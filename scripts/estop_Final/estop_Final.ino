#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUM_LEDS 112
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define Estop 13
#define UR3 A1    
#define Driver1 A2
#define Driver2 A3
#define Gripper A0

bool Stop;
bool Stop_New;
bool Stop_Old;
bool Ros = LOW; //Change this to LOW for init OFF on start up
bool EStop_Presed = true;
bool Estop_New = true;
bool Estop_Old = false;
bool fresh = false;

bool Disco = LOW;
bool Warning = LOW;
bool Auto = LOW;

int16_t i = 0;
int16_t j = NUM_LEDS/2;
int16_t k = i - 20;
int16_t l = j - 20;
 
int Stop_Ros = 0; //Change this to 0 for init OFF on start up
char swoosh[1];
int dim = 1; // To dim the led light

/***********************************************************************************************************************************/
void setup() {
  strip.begin();
  strip.show();
  setAll(255,0,255/dim);
  
  pinMode(Estop, INPUT);
  pinMode(UR3, OUTPUT);
  pinMode(Driver1, OUTPUT);
  pinMode(Driver2, OUTPUT);
  pinMode(Gripper, OUTPUT);

  digitalWrite(UR3, LOW); 
  digitalWrite(Driver1, LOW); 
  digitalWrite(Driver2, LOW); 
  digitalWrite(Gripper, LOW);
  
  Stop_New = digitalRead(Estop);
  Stop_Old = Stop_New;

  Serial.begin(115200);
  delay(1000);
}
/***********************************************************************************************************************************/
void loop() {
  if (Serial.available() > 0){
    CheckMessage();
  }
  Stop_New = (bool) digitalRead(Estop);
  
  if (Stop_New != Stop_Old){
    Debounce();
  }
  
  if((Stop_New == LOW) || (Ros == LOW)){
    SystemOff();
  }
  else{
    SystemOn();
  }
  
  if (Estop_New != Estop_Old){
    SendUpdate();
  }
  
  if (Warning == HIGH){ // Don't use both
    colorSWipe(255, 100, 0, 4);
    //PoliceLED(255, 0, 255, 0);
    
  }
  if( Disco == HIGH){
    while(digitalRead(Estop) != LOW and Serial.available() == 0){
      rainbowCycle(0);
    }
    Disco = LOW;
    fresh = false;
  }
}

/***********************************************************************************************************************************/
void SendUpdate(){
  Serial.write(EStop_Presed);
  fresh = false;
  Estop_Old = Estop_New;
}
/***********************************************************************************************************************************/
void Debounce(){
  delayMicroseconds(50); //change delay to filter more or less
  Stop_Old = Stop_New;
}
/***********************************************************************************************************************************/
void CheckMessage(){
  Serial.readBytes(swoosh, 1);
  Stop_Ros = swoosh[0];
  //StopRos -= 48; //to use the arduino terminal
  if(Stop_Ros == 2){
      //Status
      setAll(255,0,255);
      Serial.write(EStop_Presed);
      fresh = false;
    }
    if(Stop_Ros == 1){ 
      //Estop unpressed
      Ros = HIGH;
      Auto = LOW;
    }
    if(Stop_Ros == 0){
      //Estop pressed
      Ros = LOW;
    }
    
    if(Stop_Ros > 9){ 
      //error
      setAll(255,255,0);
      fresh = true;
    }
    if(Stop_Ros == 9){ 
      //Autonomous off
      Ros = HIGH;
      Auto = LOW;
      fresh = false;
    }
    if(Stop_Ros == 8){ 
      //lets off
      setAll(0,0,0);
      fresh = true;
    }
    if(Stop_Ros == 7){ 
      //Autonomous on
      Ros = HIGH;
      Auto = HIGH;
      fresh = false;
    }
    if(Stop_Ros == 6){ 
      //Warning off
      Warning = LOW;
      fresh = false;
    }
    if(Stop_Ros == 5){ 
      //Nothing Idle
      setAll(0,255/dim,255/dim);
      fresh = true;
    }
    if(Stop_Ros == 4){ 
      //Warning on
      Warning = HIGH;
    }
    if(Stop_Ros == 3){ 
      //Rainbow on
      Disco = HIGH;
      Ros = LOW;
    } 
 }

/***********************************************************************************************************************************/
void SystemOff(){
    digitalWrite(UR3, HIGH); 
    digitalWrite(Driver1, HIGH); 
    digitalWrite(Driver2, HIGH); 
    digitalWrite(Gripper, HIGH);
    EStop_Presed = true;
    Estop_New = true; 
    Ros = LOW; //Disable this for ON after this engached the Estop
    if( fresh == false){
      setAll(255/dim,0,0);
      fresh = true;
      //Serial.println("Estop = pressed");
    }
}
/***********************************************************************************************************************************/
void SystemOn(){  
    digitalWrite(UR3, LOW); 
    digitalWrite(Driver1, LOW); 
    digitalWrite(Driver2, LOW); 
    digitalWrite(Gripper, LOW);
    EStop_Presed = false;
    Estop_New = false;
    if( fresh == false){
      if(Auto == HIGH){
        setAll(0,255/dim,0);
      }
      else{
        setAll(0,0,255/dim);
      }
      fresh = true;
      //Serial.println("Estop = unpressed");
    }
}
/***********************************************************************************************************************************/
void PoliceLED (byte red, byte green, byte blue, int SpeedDelay){
  setPixel(i, red, green, 0);
  setPixel(j, 0, green, blue);
  delay(SpeedDelay);
  setPixel(k, 0,0,0);
  setPixel(l, 0,0,0);
  
  showStrip();
  
  i++;
  j++;
  k++;
  l++;
  
  if(i > NUM_LEDS){
    i = 0;
  }
  if(j > NUM_LEDS){
    j = 0;
  }
  if(k > NUM_LEDS){
    k = 0;
  }
  if(l > NUM_LEDS){
    l = 0;
  }
}
/***********************************************************************************************************************************/
void colorSWipe(byte red, byte green, byte blue, int SpeedDelay) {
  setPixel(i, red, green, blue);
  setPixel(j, red, green, blue);
  
  //setPixel(i, 255, 0, 0);
  //setPixel(j, 0, 0, 255);
    
  delay(SpeedDelay);
  setPixel(k, 0,0,0);
  setPixel(l, 0,0,0);
  
  showStrip();
  
  i++;
  j++;
  k++;
  l++;
  
  if(i > NUM_LEDS){
    i = 0;
  }
  if(j > NUM_LEDS){
    j = 0;
  }
  if(k > NUM_LEDS){
    k = 0;
  }
  if(l > NUM_LEDS){
    l = 0;
  }
}
/***********************************************************************************************************************************/
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}

/***********************************************************************************************************************************/
void showStrip() {
   strip.show();
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
