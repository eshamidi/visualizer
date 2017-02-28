//Video Sampler Latest Version 
// 2/26/17



//Libraries 

import processing.video.*;
import org.gstreamer.elements.PlayBin2;
import ddf.minim.*;
import ddf.minim.analysis.*;
import themidibus.*;



//declaration of objects -- midi, amplitude, audio input 
Minim minim;
MidiBus busA; 

AudioInput in;
BeatDetect beat;





//create array of movie file paths 
static final String[] FILMS = { "C:/Users/Esmail/Downloads/source videos/2015-03-06 00.29.38.mp4", 
                                "C:/Users/Esmail/Downloads/source videos/Cassette Tape H264-1.mp4",
                                "C:/Users/Esmail/Downloads/source videos/IMB_SF_C9_512kb.mp4"};

//declares array of movies -- empty right now 
final Movie[] movies = new Movie[FILMS.length];


//variables for frame index, position
int counter = 0; 
int tr1, tr2 = 0; 
//movie array index
int idx;
int i = 0;
int tempo[] = new int[10];
//framerate

int val; 
Movie m; 

//timer variables 
int time, wait, b4, a4;
int lastBeat;
boolean tapListen = false; 
boolean beatFlag = false; 

//pixelation variables
int numPixelsWide, numPixelsHigh;
int blockSize = 5;
color movColors[];
//int clrmix = 0;
boolean pix, clrg, clrr, clrb, ghost, rotate = false;

int clrmix, clrscl = 0;
int pixpos = 0; 
int sense = 0;
float press = 0; 






static final PlayBin2.ABOUT_TO_FINISH FINISHING = new PlayBin2.ABOUT_TO_FINISH() {
  @ Override void aboutToFinish(PlayBin2 elt) {
  }
};

void setup() {
  
//window size
  fullScreen(P3D);
 imageMode(CENTER);
 noStroke();

  //audio analysis and playback
  minim = new Minim(this);
  in = minim.getLineIn();

  
  beat = new BeatDetect();
  
 // loband = new LowPass(this);
 // loband.process(in,800);
  
  
  //connects the movie index to m
  for (String s : FILMS)  {
  (movies[idx++] = new Movie(this, s)).playbin.connect(FINISHING);
  }
  (m = movies[idx = 0]).play();
 

 
 
 //trigger video sequence timing
 
wait = 900;
time = millis();


//pixelation stuff 

  numPixelsWide = 720 / blockSize;
  numPixelsHigh = 720 / blockSize;
  movColors = new color[numPixelsWide * numPixelsHigh];

//midi stuff -use later 


//MidiBus.list();
//busA = new MidiBus(this, "Teensy MIDI", "Teensy MIDI");
//busA.addInput("Teensy MIDI"); 

}

void movieEvent(Movie m) {
  m.read();
 
}

//DRAW LOOP 
//
//
//
//
//

void draw() {
  b4 = millis();
  
  //detect if beat 
  beatFlag = UpdateTempo();

  
  
  if(beatFlag == true){
     clrmix = 255;
  }
  else{
    clrmix = 0; 
  }
    //println(clrmix + "   " + amp.analyze());
    
    
 if(rotate == true){   
 rotatePicture();
 }

  
  if(pix == true ){ //| amp.analyze() > 0.5 ){
  Pixelate();
  }
  else{
      DryVideo();
      
  }
  
  
  if(clrr == true | clrg == true | clrb == true){
    TintFX(clrmix);   
  }
  else{
    noTint();
  }

  if(ghost == true){
    GhostFX(clrmix);
  }
  
    
  
  
  
 // timer to automatically trigger transitions 
 
 if(tapListen == true){
    if(millis() - time >= wait){
    thread("Trigger1");//if it is, do something
    time = millis();//also update the stored time
    }
    //else{
    //}
 // //a4 = millis();
 // //println(a4-b4);
 
 }
}

//END OF DRAW LOOP
//
//
//
//
//




void Trigger1(){
  m.jump(0);
  
}

//video switcher 
//odd behavior in p2d renderer -- skipping ahead, need to investigate ? 
void VidSwitch(int val){
  if(val >=97 &  val <= 99){
  m.stop();
  m = movies[val-97];
  m.jump(0);
  m.play();
 

  }
}

//takes keyboard input to switch videos, control, etc.  
void keyPressed(){
  
  val = key; 
  
  
  //switch video 
  if(val == 'a' || val == 'b' || val == 'c'){
  VidSwitch(val);
  }
  

 
  switch(val){
    
    
    //pixelation effects
    
    case 'd':
    //toggle pixelate on/off 
    if(pix == true)
    pix = false;
    else
    pix = true; 
    break; 
    
    //control pixelation quality
    case 'e':
    if(blockSize < 30)
    blockSize+=2;
    else
    blockSize = 30;
    break;
    
    case 'w':
    if(blockSize > 5)
    blockSize-=2;
    else
    blockSize = 5;
    break; 
    
    //color effect
    case 'o':
    clrscl+=1;
    break; 
    
    case 'p':
    clrscl-=1;
    break;
    
    case 'r':
    pixpos+=5;
    break;
    
    case 't':
    pixpos-=5;
    break;
   
    
    
    // manual beat detection controls - y for tap tempo
    //u toggles tap tempo control on/off
    
    case 'y':
    TapUpdate();
    break;
    
    case 'u':
    if(tapListen == false){
    tapListen = true; 
    }
    else{
      tapListen = false;
    }
    break;
    
        
    // tint section -- 
    //k green, j red, l blue 
    
    
    case 'j':
    if(clrr == false){
    clrr = true; 
    clrg = false;
    clrb = false;
    }
    else if(clrr == true){
      clrr = false;
    }
    break;
    

    
    case 'k':
    if(clrg == false){
    clrg = true; 
    clrr = false;
    clrb = false;
    }
    else if(clrg == true){
      clrg = false;
    }
    break;  
    
    case 'l':
    if(clrb == false){
    clrb = true; 
    clrr = false;
    clrg = false;
    }
    else if(clrb == true){
      clrb = false;
    }
    break;  
    
    case 'm':
    if(ghost == false){
    ghost = true; 
    }
    else if(ghost == true){
      ghost = false;
    }
    break;     
    
    //end of tint section
    
    case 'x':
    sense+=10;
    break;
    
    case 'z':
    if(sense == 0){
      sense = 0;
    }
    else{
    sense-=10;
    }
    break; 
    
    case 'n':
    if(rotate == false){
      rotate = true;
    }
    else{
      rotate = false;
    }
    break;
    
    default:
    break;
    
    
  }
  
  
  
}


//pixelation effect - need to figure out how to convert output into image so tint can be layered 
void Pixelate(){
  

      m.loadPixels();
    int count = 0;
    for (int j = 0; j < numPixelsHigh; j++) {
      for (int i = 0; i < numPixelsWide; i++) {
        movColors[count] = m.get(i*3*blockSize, j*2*blockSize) + clrmix ; 
        count++;
      }
    }
   
   
   // for messing with colors? 
   
   // println(movColors.length);
    
    //count = 0;
    //for(int i = 0; i<200; i++){
      
    //   for(int j = 20; j<100; j++){
    //    movColors[count] = 0;
    //    count++;
    //  }
    //}
    
      background(255);
  for (int j = 0; j < numPixelsHigh; j++) {
    for (int i = 0; i < numPixelsWide; i++) {
      fill(movColors[j*numPixelsWide + i]  );//+ clrscl * int(random(100,250))); //+ clrscl*int(10*amp.analyze()));
      rect(i*blockSize, j*blockSize, blockSize, blockSize);
  

      
    }
  }
  
  
}


  //draws uneffected video to the screen
  void DryVideo(){

    image(m,width/2,height/2);   
    
  }
  
  
   //color tint -- mappable to audio 
  void TintFX(int clrmix){


     
    
   
      if(clrr == true){
    tint(clrmix*2,0 , 0); 
      }
      else if(clrg == true){
    tint(0, clrmix*2, 0); 
      } 
      else if(clrb == true){
    tint(0,0,clrmix*2); 
      }
    else{
     noTint();
    }
    
    
  }
  
  
  void GhostFX(int clrmix){
    
    int clrmix_map = int(map(clrmix, 0, 255, 40, 100));
    tint(clrmix*2,255-clrmix , 0, clrmix_map);
    
    image(m,width/2,height/2, width/2,height/2);
    image(m,width/2,height/2, width/4,height/4);
    
    
    
  }
  
  void rotatePicture(){
    
  imageMode(CENTER);
  rotate(press);
  //translate(0,500*press,0);
  press+=0.05;
  if(press>=2*PI){
    press =0;
  }
  
    
    
    
    
    
  }

  
  //tap tempo - unfinished
  void TapUpdate(){
    wait = millis() - lastBeat; 
    
    
    lastBeat = millis(); 
    println("Wait is"+wait);
  tempo[i] = wait;
  i++;
  if(i >= 10){
    i = 0;
  }
  //trying to display bpm array
  textSize(40);
  text(i, width/4, height*8/10);

  
  }
  
  boolean UpdateTempo(){
    
  beat.setSensitivity(sense); 
  beat.detect(in.mix);
  // display sensitivity 
  text(sense, width/4, height*9/10);
  if(beat.isOnset()){
  TapUpdate();
  
  return true;
  }
  else{
    return false;
    
  }
  
    
    
    
  }
  
  
  
  
  //midi stuff  - use later 
      
  void controllerChange(int channel, int number, int value) {
  // Receive a controllerChange
  println();
  println("Controller Change:");
  println("--------");
  println("Channel:"+channel);
  println("Number:"+number);
  println("Value:"+value);

  blockSize = value/3; 
}

void noteOn(int channel, int pitch, int velocity) {
  println();
  println("Note On:");
  println("--------");
  println("Channel:"+channel);
  println("Pitch:"+pitch);
  println("Velocity:"+velocity);

}

  

  