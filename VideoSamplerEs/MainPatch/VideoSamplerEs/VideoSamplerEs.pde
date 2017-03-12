//Video Sampler Latest Version 
// 3/12/17



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
//static final String[] FILMS = { "/home/ubuntu/Documents/SourceVideos/Lizard.mp4",
//                                "/home/ubuntu/Documents/SourceVideos/Lizard.mp4",
//                                "/home/ubuntu/Documents/SourceVideos/Lizard.mp4"
//};


static final String[] FILMS = { "C:/Users/Esmail/Downloads/source videos/Lizard_1024_576_Lossless.mp4",
                                "C:/Users/Esmail/Downloads/source videos/Fractal.mp4",
                                "C:/Users/Esmail/Downloads/source videos/IMB_SF_C9_512kb.mp4",
};

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
boolean pix, clrg, clrr, clrb, clra, ghost, rotate = false;

int clrmix, clrscl = 0;
int pixpos = 0; 
int sense = 0;
float press = 0; 
int ghostnum;
int ccmix = 0; 





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
  
  //connects the movie index to m
  for (String s : FILMS)  {
  (movies[idx++] = new Movie(this, s)).playbin.connect(FINISHING);
  }
  (m = movies[idx = 0]).play();
   m.stop();
   
 
 
 //trigger video sequence timing
 
wait = 900;
time = millis();


//pixelation stuff 

  numPixelsWide = width / blockSize;
  numPixelsHigh = height / blockSize;
  movColors = new color[numPixelsWide * numPixelsHigh];

//midi stuff

//MidiBus.list();
busA = new MidiBus(this, 0, 0);

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
    clrmix = 20; 
  }
    
    
 if(rotate == true){   
 rotatePicture();
 }

  
  if(pix == true ){ //| amp.analyze() > 0.5 ){
  Pixelate();
  }
  else{
      DryVideo();
      
  }
  
  

  if(ghost == true){
    GhostFX(ccmix, ghostnum);
  }
  
   if(clrr == true | clrg == true | clrb == true | clra == true){
    TintFX(clrmix);   
  }
  else{
    noTint();
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
 
 
// println(clrmix);
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


  //draws uneffected video to the screen
  void DryVideo(){
    
    //background is set to black so previous artifacts from older frames do not hold true. 
    //maybe make this optional somehow? 
    background(0);
    image(m,width/2,height/2,width,height);   
    
  }
  
  
  
  

  

  