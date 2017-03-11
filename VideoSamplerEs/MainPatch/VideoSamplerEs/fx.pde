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
    
      background(0);
  for (int j = 0; j < numPixelsHigh; j++) {
    for (int i = 0; i < numPixelsWide; i++) {
      fill(movColors[j*numPixelsWide + i]  );//+ clrscl * int(random(100,250))); //+ clrscl*int(10*amp.analyze()));
      rect(i*blockSize+(width/4), j*blockSize+(height/4), blockSize*20, blockSize*20);
  

      
    }
  }
  
  
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
  
  
  void GhostFX(int clrmix, int num){
    int ghostnum_map = int(map(num, 0,127,1,40));
    int clrmix_map = int(map(clrmix, 0, 255, 40, 100));
    
    
    for(int i=1; i<ghostnum_map; i++){
      tint(255,255 , 255, clrmix_map);
      image(m,width/2,height/2, width/i,height/i);
    
    }
    
  }
  
  void rotatePicture(){
    
  imageMode(CENTER);
  rotate(-press);
  translate(-press,-50*press,0);
  press+=0.01;
  if(press>=2*PI){
    press =0;
  }
  
    
    
    
  }