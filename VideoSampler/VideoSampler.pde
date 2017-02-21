// Video Sampler
// Updated: 2/21/17


//Libraries 
import processing.video.*;
import org.gstreamer.elements.PlayBin2;
import ddf.minim.*;
import ddf.minim.analysis.*;
import processing.sound.*;
import themidibus.*;
import java.io.File;
import java.io.FilenameFilter;


//declaration of objects -- midi, amplitude, audio input 
//MidiBus busA; 
Amplitude amp;
AudioIn in;
BeatDetect beat;

//LowPass loband;
//SoundFile in;

static String videoDirectory = "C:/Users/Jeremy/Google Drive/Drexel/Winter 2017/ECE-12 Senior Design/Source Videos/";
static String videoExtension = ".avi";


// generate video list
static final String[] videos = findVideos(videoDirectory, videoExtension);

//legacy code:
//create array of movie file paths 
//static final String[] videos = { "C:/Users/Jeremy/Google Drive/Drexel/Winter 2017/ECE-12 Senior Design/Source Videos/2015-03-06 00.29.38.mp4", 
//                                "C:/Users/Jeremy/Google Drive/Drexel/Winter 2017/ECE-12 Senior Design/Source Videos/Cassette Tape H264-1.mp4",
//                                "C:/Users/Jeremy/Google Drive/Drexel/Winter 2017/ECE-12 Senior Design/Source Videos/IMB_SF_C9_512kb.mp4"};

//declares array of movies -- empty right now 
final Movie[] movies = new Movie[videos.length];


//variables for frame index, position
int counter = 0; 
int tr1, tr2 = 0; 
//movie array index
int idx;
//framerate
int FPS = 30; 
int val; 
Movie m; 

//timer variables 
int time, wait, b4, a4;
int lastBeat;
boolean tapListen = false; 

//pixelation variables
int numPixelsWide, numPixelsHigh;
int blockSize = 5;
color movColors[];
//int clrmix = 0;
boolean pix, clrg, clrr, clrb, ghost = false;

int clrscl = 0;
int pixpos = 0; 

static final PlayBin2.ABOUT_TO_FINISH FINISHING = new PlayBin2.ABOUT_TO_FINISH() {
  @ Override void aboutToFinish(PlayBin2 elt) {
  }
};

void setup() {
  
  //window size
  size(640,480);
  //framerate
  frameRate(30);
  noStroke();
  //timer
  
  //audio analysis and playback
  amp = new Amplitude(this);
  //in = new SoundFile(this, "D:/Music/dj stuff/02 - Shall Not Fade - It's Been A Long Time.mp3");
  in = new AudioIn(this, 0);
  in.start();
  amp.input(in);
  
  beat = new BeatDetect();
  
  // loband = new LowPass(this);
  // loband.process(in,800);
  
  
  //connects the movie index to m
  for (String s : videos)  {
    (movies[idx++] = new Movie(this, s)).playbin.connect(FINISHING);
  }
  (m = movies[idx = 0]).play();
 
  m.volume(0); 
 
 
  //trigger video sequence timing
  wait = 900;
  time = millis();

  numPixelsWide = 720 / blockSize;
  numPixelsHigh = 720 / blockSize;
  movColors = new color[numPixelsWide * numPixelsHigh];


  //MidiBus.list();
  //busA = new MidiBus(this, "Teensy MIDI", "Teensy MIDI");
  //busA.addInput("Teensy MIDI"); 

}

// necessary for playback
void movieEvent(Movie m) {
  m.read();
}

// main loop
void draw() {
  b4 = millis();
  int clrmix = int(map(amp.analyze(), 0, 0.9, 0, 255)); 
  //println(clrmix + "   " + amp.analyze());

  if(pix == true ){  //| amp.analyze() > 0.5 ){
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
  }
}

// locates files in a certain directory
static String[] findVideos(String directory, final String extension)
{
  // video files directory
  File dir = new File(directory);
  
  // list directory and filter for file extension
  String[] FILMS = dir.list(new FilenameFilter() 
  {
    @Override
    public boolean accept(File dir, String name) 
    {
      return name.toLowerCase().endsWith(extension);
    }
  });
  
  // create array of full paths to files
  for(int i=0; i<FILMS.length; i++)
  {
    FILMS[i] = directory + FILMS[i];
  }

  return FILMS;
}


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
  VidSwitch(val);
 
  switch(val){
    
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
    
    case 'j':
    if(clrr == false){
    clrr = true; 
    }
    else if(clrr == true){
      clrr = false;
    }
    break;
    
    case 'k':
    if(clrg == false){
    clrg = true; 
    }
    else if(clrg == true){
      clrg = false;
    }
    break;  
    
    case 'l':
    if(clrb == false){
    clrb = true; 
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
      movColors[count] = m.get(i*3*blockSize, j*2*blockSize) ; 
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
  image(m, tr1, tr2, 640,480);       
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
    
  image(m,0, -50, 800,600);
  //image(m,0, 0, 1000,800);  
}

  
//tap tempo - unfinished
void TapUpdate(){
  wait = millis() - lastBeat; 
  lastBeat = millis(); 
  println(wait);
}
  

////midi stuff  - use later 
      
//void controllerChange(int channel, int number, int value) {
//  // Receive a controllerChange
//  println();
//  println("Controller Change:");
//  println("--------");
//  println("Channel:"+channel);
//  println("Number:"+number);
//  println("Value:"+value);
  
//  blockSize = value/3; 
//}


//void noteOn(int channel, int pitch, int velocity) {
//  println();
//  println("Note On:");
//  println("--------");
//  println("Channel:"+channel);
//  println("Pitch:"+pitch);
//  println("Velocity:"+velocity);
//}

  

  