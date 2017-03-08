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
 // textSize(40);
 // text(i, width/4, height*8/10);

  
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