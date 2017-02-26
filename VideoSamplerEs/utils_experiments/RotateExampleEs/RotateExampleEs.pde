PImage img;
float press=0;
void setup() {
  size(640,480, P2D);
  img = loadImage("C:/Users/Esmail/Pictures/cubapix/club.jpg");
}

void draw() {
  imageMode(CENTER);
  rotate(press);
  image(img, width/2,height/2,width,height);  // Draw image using CENTER mode
}

void mousePressed(){
  
  press+=0.05;
  println(press);
  draw();
}