//Framerate timer - saves framerate to csv 
//used to compare framerates between different formats 

import processing.video.*;
int frametime, elaptime = 0; 
Table table;
Movie movie;
int count; 
int dd; 

void setup() {
 fullScreen(P2D);
  background(0);
  // Load and play the video
  movie = new Movie(this, "Lizard.mp4");
  movie.play();
  table = new Table();
  
  table.addColumn("format");
  table.addColumn("framerate");
  dd = millis(); 
  
}

void movieEvent(Movie m) {
  m.read();
   count++; 
  

}


void draw() {
  
  image(movie, 0, 0, width, height);

   if( millis() - dd >= 1000){
    println(count);
    count = 0; 
    dd = millis();     
  }
  
}