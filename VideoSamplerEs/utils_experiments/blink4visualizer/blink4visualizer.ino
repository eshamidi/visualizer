//Visualizer LED Prototype 

//One mode just cycles thru colors, the other mode turns on LEDs on a button press. 

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//interrupt pin for button
int interruptPin = 2; 

byte state = LOW;
byte LITE; 

//These are all binary numbers that represent the combos of LEDs on/off required 
//for a certain color on all LEDS on the board. Each LED has 3 numbers corresponding to RGB.
//Common anode so LOW = ON. 
//Example: white is all RGB on = 000 + 000 - black is all RGB off = 111 + 111 = 63 
//Red is only the red LEDs on = 110 + 110 = 54 . And so on. 

byte w = 0;  //white
byte k = 63; //black
byte r = 54; //red
byte g = 45; //green
byte y = 36; //yellow
byte x = 18; //pink
byte q = 9; //lite blue 
byte b = 27; //blue
byte colorarr[] = {k, w, r, x, y, g, b, q };

void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Turnon, CHANGE);
  Serial.begin(9600);
}


int mode = 1; 



void loop() {



switch(mode){
  case 1:
  //Loop that delivers + latches color array. 
    for(int i = 0; i<8; i++){
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:

    shiftOut(dataPin, clockPin, MSBFIRST, colorarr[i]);  
    
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    delay(400);
  
}
break; 

case 2:

    if(state == 1){
      LITE = b;
    }
    if(state == 0){
      LITE = k;
    }
    
    digitalWrite(latchPin, LOW);
    // shift out the bits:

    shiftOut(dataPin, clockPin, MSBFIRST, LITE);  
    
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
      
    
  

 
  

break;

default:
break;
  
}

}

void Turnon(){
  //reading again for some sweet sweet debouncing 
  if(digitalRead(interruptPin) == HIGH){
  state = !state; 
  Serial.print("Int");
  }

}

