/**
 * Loop. 
 * 
 * Shows how to load and play a QuickTime movie file.  
 *
 */

import processing.video.*;
import themidibus.*; //Import the library

MidiBus myBus; // The MidiBus
int fr;
boolean tt = false;
Movie movie;

void setup() {
  size(640, 360, P3D);
  background(0);
  MidiBus.list();
  myBus = new MidiBus(this, 0,0); 
  // Load and play the video in a loop
  movie = new Movie(this, "/media/ubuntu/LEXAR/MVI_0050.avi");

 
}

void movieEvent(Movie m) {
  m.read();
}

void draw() {
  //if (movie.available() == true) {
  //  moviegst-.read(); 
  //}
  image(movie, 0, 0, width, height);

    

  fr = int(frameRate);
//println(fr);
}

void noteOn(int channel, int pitch, int velocity) {
  // Receive a noteOn
  println();
  println("Note On:");
  println("--------");
  println("Channel:"+channel);
  println("Pitch:"+pitch);
  println("Velocity:"+velocity);
  if(pitch == 23){
  movie.jump(0);  
  movie.play();
  }
}
void controllerChange(int channel, int number, int value) {
   //Receive a controllerChange
  //println();
  //println("Controller Change:");
  //println("--------");
  //println("Channel:"+channel);
  //println("Number:"+number);
  //println("Value:"+value);
  if(number == 46 ){
    if(value == 1){
      tt= true;
    }
    else{
      tt=false;
      tint(255,255,255);
    }
  }
  if(number == 45){
    if(tt ==true){
      tint(0,value*2,0);
    }
  }

  
}