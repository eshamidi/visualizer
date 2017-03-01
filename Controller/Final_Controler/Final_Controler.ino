//Visualizer Controller Firmware, by Jason Lewinski
//Latest Rev 2/12/17

//-------Naming--------------
//Video Control Buttons pins
int b1 = 0; // video trigger 1
int b2 = 1; // video trigger 2
int b3 = 2; // video trigger 3
/*
  int b4 = 2; // video trigger 3
  int b5 = 2; // video trigger 3
  int b6 = 2; // video trigger 3
*/

//Video Effect bank1
int encoder1SwitchPin = 12;
int encoderPin1_1 = 3;
int encoderPin2_1 = 4;

//LEDs
int encoderRed = 8;
int encoderGreen = 7;
int encoderBlue = 6;
int faderGreen = 5;

//LED Color
int Green = 0;
int Red = 0;
int Blue = 0;

//------States------------
//Video Control Buttons
int b1State = 0;
int b2State = 0;
int b3State = 0;

//Video Effect Bank
int encoderSwitchPinState = 0;
int encoderSwitchState = 0;
int fader1Value = 0 ;

//-------Encoder Rotation--------------
volatile int lastEncoded = 0;
volatile long encoderValue_1 = 63;
int lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;


//setup code
void setup() {

  Serial.begin(9600);

  //Video button Pin setup
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);

  //Encoder pin setup
  pinMode(encoderPin1_1, INPUT);
  pinMode(encoderPin2_1, INPUT);

  //Encoder Push button setup
  pinMode(encoder1SwitchPin, INPUT_PULLDOWN);

  //LEDs
  pinMode(encoderRed, OUTPUT);
  pinMode(encoderGreen, OUTPUT);
  pinMode(encoderBlue, OUTPUT);
  pinMode(faderGreen, OUTPUT);

  digitalWrite(encoderRed, LOW);
  digitalWrite(encoderGreen, LOW);
  digitalWrite(encoderBlue, LOW);
  digitalWrite(faderGreen, LOW);

  //Pin resistor condition
  digitalWrite(b1, HIGH); //turn pullup resistor on
  digitalWrite(b2, HIGH); //turn pullup resistor on
  digitalWrite(b3, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin1_1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2_1, HIGH); //turn pullup resistor on



  //call updateEncoder() when a high/low change is seen (pin#, function call, based off of what)
  attachInterrupt(encoderPin1_1, updateEncoder, CHANGE);
  attachInterrupt(encoderPin2_1, updateEncoder, CHANGE);
}


void loop() {

  b1State = digitalRead(b1);
  if (b1State == LOW) {
    debounce(&b1State, b1, 70);
    Serial.println("B1 pressed");
    usbMIDI.sendNoteOn(21,64,1);
    usbMIDI.sendNoteOff(21,127,1);
  }

  b2State = digitalRead(b2);
  if (b2State == LOW) {
    debounce(&b2State, b2, 70);
    Serial.println("B2 pressed");
    usbMIDI.sendNoteOn(22,64,1);
    usbMIDI.sendNoteOff(22,127,1);
  }

  b3State = digitalRead(b3);
  if (b3State == LOW ) {
    debounce(&b3State, b3, 70);
    Serial.println("B3 pressed");
    usbMIDI.sendNoteOn(23,64,1);
    usbMIDI.sendNoteOff(23,127,1);
  }

  encoderSwitchPinState = digitalRead(encoder1SwitchPin);
  if (digitalRead(encoder1SwitchPin) == HIGH) {
    debounce(&encoderSwitchPinState, encoder1SwitchPin, 60);
    ((encoderSwitchState == 0) ? encoderSwitchState = 1 : encoderSwitchState = 0);
  } else {
    //nothing
  }

  if (encoderSwitchState == 1) {
    //call effect control function
    //determine which effect back is called

  } else {
    Serial.println("effect not in affect");
    analogWrite(faderGreen, 0);
    analogWrite(encoderRed, 0);
    analogWrite(encoderGreen, 0);
    analogWrite(encoderBlue, 0);
  }
  delay(75); //just here to slow down the output, and show it will work  even during a delay
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1_1);
  int LSB = digitalRead(encoderPin2_1);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 13 || sum == 4 || sum == 2 || sum == 11)
  {
    if (encoderValue_1 > 0) encoderValue_1 --;
  }
  if (sum == 14 || sum == 7 || sum == 1 || sum == 8)
  {
    if (encoderValue_1 < 127) encoderValue_1 ++;
  }

  lastEncoded = encoded;
}

void debounce(int *buttonstate, int pin, int delayAmount) {
  int comparestate = *buttonstate;
  delay(delayAmount);
  int buttonstate2 = digitalRead(pin);
  if (comparestate == buttonstate2) {
    //nothing
  } else {
    *buttonstate = buttonstate2;
  }
}

void effectcontrol(int bank_number) {

  Serial.println("Encoder: " + String(encoderValue_1) + "\n");
  fader1Value = analogRead(A0);
  fader1Value = map(fader1Value, 0, 1024, 0, 128);
  Serial.println("Fader: " + String(fader1Value) + "\n");

  int fadercolor = (fader1Value);
  int encodercolor = encoderValue_1 * 2;

  Green = encodercolor;
  Red = 255 - encodercolor;
  Blue = 0;//254 - encodercolor;

  analogWrite(faderGreen, fadercolor);

  analogWrite(encoderGreen, Green);
  analogWrite(encoderBlue, Blue); //Red  0: LOW | 255: HIGH
  analogWrite(encoderRed, Red);
}








