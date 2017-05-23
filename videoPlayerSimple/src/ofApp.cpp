//Video Sampler Patch 5/18/17
// Developed on bump-a-grape

#include "ofApp.h"
#include <dirent.h>
#include "protocol.h"

//--------------------------------------------------------------
void ofApp::setup(){

   // system(" ~/initAudio.sh");

    //timer init
    colorTimer.setup(colorspeed_p);
    rotateTimer.setup(rotate_spd);

     ofAddListener( colorTimer.TIMER_COMPLETE , this, &ofApp::colorTimerCompleteHandler ) ;
     ofAddListener( rotateTimer.TIMER_COMPLETE , this, &ofApp::rotateTimerCompleteHandler ) ;

    //serial comms init;

//    serial.listDevices();
//    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

//    //need to set up custom to Jetson serial port - ttyTHS1
//    //open the first device and talk to it at 57600 baud
//    int baud = 9600;
//    serial.setup(0, baud);

    // Initialize threads and queue for serial comms
    message_queue = g_async_queue_new();




    system("sudo ~/unmountFlash.sh");
    system("sudo ~/mountFlash.sh");


    // location of mounted flash drive - varies per system
    //string dirString = "/media/jere/fdrive/";
    string dirString = "/media/ubuntu/fdrive/";
    // string dirString = "/media/root/fdrive/";

    // locate all videos on connected flash drive
    vector<string> videos = findVideos(dirString);




    // load videos into each ofVideoPlayer, each video has 2 for faster switching
    int i = 0;
    while(i < videos.size()){
        myMovies[i][0].load(videos[i]);
        myMovies[i][1].load(videos[i]);

        //mutes audio in case of HDMI audio output
        myMovies[i][0].setVolume(0);
        myMovies[i][1].setVolume(0);
        i++;
    }


// Audio setup stuff

    soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.

    int bufferSize = 256;


    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);

    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;

    soundStream.setup(this, 0, 2, 48000, bufferSize, 4);

    ofSetVerticalSync(TRUE);
    ofBackground(0,0,0);



    //timer start
    colorTimer.start(true);
    step = 0;


    rotateTimer.start(true);



    serialThread.start();
//    serialThread.sendFileNames();

}

//--------------------------------------------------------------
void ofApp::update(){
//	serialThread.sendFileNames();
    TotalDraws = pow(numhoriz, 2)*maxGhosts;

    if(TotalDraws > 32) maxGhosts--;

    //audio update envelope

    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);


    if(ghostfx) numGhosts = ofMap(scaledVol, 0.0, 1.0,1,maxGhosts,true);
    else numGhosts = maxGhosts;
    if(tilefx) tileAudio = ofMap(scaledVol,0.0, 1.0, 1, numhoriz, false);

    if(clrdep_new > 0) clrUpdate = ofMap(scaledVol,0.0,1,clrdep_new,true);
        //lets record the volume into an array
        volHistory.push_back( scaledVol );

        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory.size() >= 400 ){
                volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }

    //update color timer for ColorModulator
    colorTimer.update();
    rotateTimer.update();


    //update video frames
    myMovies[currentVid][toggle].update();



    //update colorModulator or coloraudio depending on which is selected
    if(colorfx) colorModulator(step);
    if(!colorfx) colorAudio(clrUpdate);
    if(rotateAudio) xang = ofMap(scaledVol, 0.1, 1.0, 0, 150, true);

//    if((rotateTimer.bIsPaused == true) & (rotate_amt != 0)){
//        rotateTimer.togglePause();
//    }
serialThread.lock();
//controlHI = serialThread.params;
//serialThread.unlock();
    controlHI = serialThread.pushParams();
serialThread.unlock();
if(controlHI.size() == 10){
cout << controlHI.at(1) << endl;
}
}

//-----------------------------------------------------
void ofApp::draw(){

    //At the beginning of the draw loop, this sets the draw color to the updated value from effects.

    ofSetColor(drawcolor);

    ofTranslate(0,0,-200);

    //This is where the drawing happens.

    //Mapping tile horizontal amount to audio variable if effect is engaged
    if(tilefx == true) numTiles = tileAudio;
    else numTiles = numhoriz;
    //First nesting loop: pans horizontal thru tiles
       for(int f = 0; f < numTiles; f++){

    //Adjusts draw x coordinates to corner of next tile
        drawx = (1920/numhoriz)*f;
        ofTranslate(drawx, 0,0);

    //Second nested loop: steps thru vertical tiles
            for(int v = 0; v < numvert; v++){

    //Adjusts draw y coordinates to corner of next tile
                drawy = (1080/numvert)*v;
                ofTranslate(0,drawy,0);


                ofTranslate((1920/(2*numhoriz))*1, (1080/(2*numvert))*1,0);
                ofRotate(xang,0,0,1);

                ofTranslate(-(1920/(2*numhoriz))*1,-(1080/(2*numvert))*1,0);
                myMovies[currentVid][toggle].draw(0,0,1920/numhoriz,1080/numvert);
                //sets alpha value to 50  for ghosting
                drawcolor.a = 50;


                //
                for(int g = 0; g < numGhosts; g++){
                    //sets draw color with new alpha value
                    ofSetColor(drawcolor);
                //TODO - need to figure out transformation of ghost position
                //ghost position is roughly centered now
                    myMovies[currentVid][toggle].draw(-50*ghostSize*g,-50*ghostSize*g,(1920/numhoriz)+100*g*ghostSize,(1080/numvert)+100*g*ghostSize);

           }

           drawcolor.a = 255;
           ofSetColor(drawcolor);

           ofTranslate((1920/(2*numhoriz))*1, (1080/(2*numvert))*1,0);
           ofRotate(-xang,0,0,1);
           ofTranslate(-(1920/(2*numhoriz))*1,-(1080/(2*numvert))*1,0);


           ofTranslate(0,-drawy,0);

           }
       ofTranslate(-drawx,0,0);
       }










    drawcolor.a = 255;
    ofSetColor(drawcolor);


//debug stuff -- get rid of in final version

    int fr = int(ofGetFrameRate());
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofDrawBitmapString("red" + ofToString(drawcolor.r * 1.0, 0), 50, 500);
    ofDrawBitmapString("green" + ofToString(drawcolor.g * 1.0, 0), 50, 520);
    ofDrawBitmapString("blue" + ofToString(drawcolor.b * 1.0, 0), 50, 540);
    ofDrawBitmapString("step" + ofToString(step * 1.0, 0), 50, 560);
    ofDrawBitmapString("clrdep_new" + ofToString(clrdep_new * 1.0, 0), 50, 600);
    ofDrawBitmapString("chgamt" + ofToString(chgamt * 1.0, 0), 50, 620);
    ofDrawBitmapString("framerate" + ofToString(fr * 1.0, 0), 50, 640);
    ofDrawBitmapString("xang" + ofToString(xang * 1.0, 0), 50, 660);


    colorTimer.draw(50,40);
    rotateTimer.draw(50,80);

    ofDrawBitmapString("Controls:" , 50, 200);
    ofDrawBitmapString("Video triggers nums 1-7" , 50, 220);
    ofDrawBitmapString("Audio toggles: color = q, ghosting = 8 " , 50, 240);
    ofDrawBitmapString("Rotate: i+, o- :: speed: p+, o-" , 50, 260);

    ofDrawBitmapString("Number tiles " + ofToString((numhoriz), 0), 50, 280);
    ofDrawBitmapString("max ghosts" + ofToString(maxGhosts,0) , 50, 300);
    ofDrawBitmapString("total draw operations " + ofToString(TotalDraws,0) , 50, 320);

}

//--------------------------------------------------------------
vector <string> ofApp::findVideos(string directory){

    vector <string> videos;
    string tempName;
    DIR *dir;
    struct dirent *entries;

    if (( dir = opendir( directory.c_str() )) != NULL) {;

        size_t count = 0;
        while ((entries = readdir (dir)) != NULL && count < 6) {

            tempName = entries->d_name;
            if(tempName.substr(tempName.find_last_of(".") + 1) == "avi" || tempName.substr(tempName.find_last_of(".") + 1) == "mp4" || tempName.substr(tempName.find_last_of(".") + 1) == "mov") {

                videos.push_back( directory + tempName );
                count++;
            }
        }
        closedir (dir);
    }

    // display videos found
    for(int i = 0; i < videos.size(); i++) {
        cout << videos[i] << endl;
    }

    return videos;
}


// keyboard control...to be taken out when controller is operational
void ofApp::keyPressed(int key){

    // switches current video with 1,2,3 button presses
    switch(key){
        //video trigger buttons
        case '1':
            switchVideo(0);
            break;

        case '2':
            switchVideo(1);
            break;

        case '3':
            switchVideo(2);
            break;

        case '4':
            switchVideo(3);
            break;

        case '5':
            switchVideo(4);
            break;

        case '6':
            switchVideo(5);
            break;

        //toggle ghost audio
        case '8':
            ghostfx = !ghostfx;
        break;

        //max amount of ghosts
        case '9':

            maxGhosts++;
      //  if(TotalDraws >= 32) maxGhosts = TotalDraws/pow(numhoriz,2);
            break;
        case '0':
            maxGhosts--;
            if(maxGhosts < 0) maxGhosts = 0;
            break;

        case 'n':
            ghostSize +=0.5;
            if(ghostSize > 7) ghostSize = 7;
        break;
        case 'm':
            ghostSize-=0.5;
            if(ghostSize < 1) ghostSize = 1;
        break;



        //toggle color audio
        case 'q':
            colorfx = !colorfx;
            break;

        //fewer tiles
        case 'r':
            numhoriz--;
            numvert--;
            if(numhoriz < 1) numhoriz = 1;
            if(numvert < 1) numvert = 1;
        break;
        //more tiles
        case 't':
            numhoriz++;
            numvert++;
            if(numhoriz > 5) numhoriz = 5;
            if(numvert > 5 ) numvert = 5;
            //if(TotalDraws >= 32) maxGhosts = TotalDraws/pow(numhoriz,2);
        break;

        case 'e':
            tilefx = !tilefx;
        break;


        //adjust auto color speed
        case 'g':
            colorspeed_p-=50;
            if(colorspeed_p < 50) colorspeed_p = 50;
        break;
        case 'h':
            colorspeed_p+=50;
        break;

        //adjust color mod depth
        case 'j':
            clrdep_new+=10;
            if(clrdep_new > 250)clrdep_new = 250;
        break;
        case 'k':
            clrdep_new-=10;
            if(clrdep_new < 0)clrdep_new = 0;
        break;

        //rotate speed + rotate amount per step
//        case 'i':
//            rotate_spd+=100;
//        break;
//        case 'o':
//            rotate_spd-=100;
//        break;
        case 'l':
        rotate_amt+=0.2;
        break;
        case 'p':
        rotate_amt-=0.2;
        break;
        case 'o':
            rotateAudio = !rotateAudio;
        break;





    }
}

// pause&rewind video, flip toggle, play new video
void ofApp::switchVideo(int targetVid){

    myMovies[currentVid][toggle].setPaused(true);
    myMovies[currentVid][toggle].firstFrame();

    toggle = !toggle;

    myMovies[targetVid][toggle].setPaused(false);
    currentVid = targetVid;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}







//// does processing for fx/input without hanging up display - empty as of 5/3/17
//void *message_processing_thread(void *data) {
//    ofApp *app = (ofApp *)data;
//    app->message_handler_loop();
//        return 0;
//}


void ofApp::audioIn(ofSoundBuffer & input){

    float curVol = 0.0;

    // samples are "interleaved"
    int numCounted = 0;

    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < input.getNumFrames(); i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;

        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }

    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;

    // this is how we get the root of rms :)
    curVol = sqrt( curVol );

    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;

    bufferCounter++;

}


void ofApp::colorTimerCompleteHandler( int &args )
{
    colorTimer.draw(90,90);


    if(step <= 4) step++;
    else{
        colorTimer.setup(colorspeed_p);
        chgamt = 35*clrdep_new/colorspeed_p;
        step = 0;
}




}

void ofApp::colorModulator(int step){

    //this is a sloppy timer for progressive color tint effects. Increment amount controls the speed. should maybe use a separate free-running loop so I can control the timing better
    //dynamically change the amount of colorchg based on depth so speed will remain relatively consistent while depth is changing.
    //Not great when depth is high and speed is low - looks steppy - could incorporate colorspeed in my calculation

    //timer++;
    drawcolor.a = 255;
        //progressive color tint



    //timer = ofGetElapsedTimeMillis();

    //red -, green -, result is blue

    switch(step){

    case 0:

    break;

    case 1:


           if(drawcolor.r > 1) drawcolor.r -= chgamt;
           if(drawcolor.g > 1) drawcolor.g -= chgamt;

    break;

    //green +, blue -, result is green
    case 2:

            if(drawcolor.g < 254) drawcolor.g += chgamt;
            if(drawcolor.b > 1) drawcolor.b -= chgamt;


    break;
    //red +, green -, result is red
    case 3:

        if(drawcolor.r < 254) drawcolor.r += chgamt;
        if(drawcolor.g > 1) drawcolor.g -= chgamt;


    break;
    //green +, result is red + green
    case 4:

        if(drawcolor.g < 254) drawcolor.g += chgamt;

    break;
    //blue +, result is white
    case 5:
       // colorTimer.togglePause();
        if(drawcolor.b < 254) drawcolor.b += chgamt;
        if(drawcolor.r < 254) drawcolor.r += chgamt;
        if(drawcolor.g < 254) drawcolor.g += chgamt;

//        if(drawcolor.b == 254 & drawcolor.g == 254 & drawcolor.r == 254){
//            step = 0;
//            colorTimer.togglePause();
//        }
    break;

    default:
        break;
    }


}

//audio driven color update function

void ofApp::colorAudio(int clrUpdate){

   drawcolor.r = 255 - clrUpdate;
   drawcolor.g = 128 + clrUpdate;
   drawcolor.b = 128 + clrUpdate*2;

}

void ofApp::rotateTimerCompleteHandler(int &args) {

    xang += rotate_amt;
    if(abs(xang) > 360){
        if(rotate_amt == 0)rotateTimer.togglePause();
        xang = 0;
}



}


void ofApp::exit() {

    // stop the thread
    serialThread.stopThread();
}
