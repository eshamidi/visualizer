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
    
    case 'h':
    ghostnum++;
    break;
    
    case 'g':
    ghostnum--;
    break;
    
    default:
    break;
    
    
  }
  
  
  
}

  
  
  
  
  //midi stuff  - use later
      
  void controllerChange(int channel, int number, int value) {
  // Receive a controllerChange
  int ctrl1 = 46;
  int ctrl2 = 45;
  
  //println();
  //println("Controller Change:");
  //println("--------");
  //println("Channel:"+channel);
  //println("Number:"+number);
  //println("Value:"+value);
    
    if(number == 46 ){
    if(value == 1){
      ghost = true;
    }
    else{
      ghost =false;
    }
  }
  if(number == 45){
    if( ghost ==true){
     ghostnum = value;
    }
  }
  if(number == 47){
    ccmix = 2*value;

  }
}

void noteOn(int channel, int pitch, int velocity) {
  //currently set to 21,22,23 
  println();
  println("Note On:");
  println("--------");
  println("Channel:"+channel);
  println("Pitch:"+pitch);
  println("Velocity:"+velocity);
  
  VidSwitch(pitch+76); //add 76 to get to ASCII values expected by function

}