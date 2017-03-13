PImage img;
float press=0;
void setup() {
  size(640,480, P2D);
  imageMode(CENTER);
  img = loadImage("C:/Users/Esmail/Pictures/cubapix/club.jpg");
}

void draw() {
  background(0);
  

  translate(width/2,height/2);
  rotate(press*TWO_PI/360);
  translate(-width/2,-height/2);
// Draw image using CENTER mode
image(img, width/2,height/2,width,height);

}

void mousePressed(){
  
  press+=0.55;
  println(press);
  draw();
}