//Video Sampler Patch 5/14/17
// Developed on bump-a-grape

#include "ofApp.h"
#include <dirent.h>
#include "protocol.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //serial comms init;

    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    //need to set up custom to Jetson serial port

    int baud = 57600;
    //serial.setup(0, baud); //open the first device and talk to it at 57600 baud

    // Initialize threads and queue for serial comms
    message_queue = g_async_queue_new();


    //unknown purpose - TODO- ask Chris Buchter about this
//    if(pthread_create(&message_thread, NULL, message_processing_thread, this)) {
//        exit();
//    }
//    if(pthread_create(&serial_thread, NULL, serial_communication_thread, this)) {
//        exit();
//    }

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



    //still need to verify audio functionality on Jetson 5/3/17
    //audio functionality verified on Jetson 5/14/17


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

}

//--------------------------------------------------------------
void ofApp::update(){

    //2 dimensional array of videos
    myMovies[currentVid][toggle].update();



    //this is a sloppy timer for progressive color tint effects. Increment amount controls the speed.

    if(colorfx == true) timer+=3;
    drawcolor.a = 255;
        //progressive color tint
    //gets more blue
    if(step == 0){
        drawcolor.r = 255-timer;
        drawcolor.g = 255-timer;
        drawcolor.b = 255;

    }
    //gets more green
    if(step == 1){
        //drawcolor.r = 0;
        drawcolor.g = timer;
        drawcolor.b = 255-timer;

    }
    //gets more red
    if(step == 2){
        drawcolor.r = timer;
        drawcolor.g = 255-timer;
        //drawcolor.b = 0;

    }
    //gets more green
    if(step == 3){
        drawcolor.r = 255;
        drawcolor.g = timer;
        //drawcolor.b = 0;

    }
    //gets more blue
    if(step == 4){
        drawcolor.r = 255;
        drawcolor.g = 255;
        drawcolor.b = timer;

    }


    if(timer > 254){
        timer = 0;
        if(colorfx == true) step++;
        if(step == 5) step = 0;

    }



    //audio update envelope

	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    numGhosts = ofMap(scaledVol, 0.0, 1.0,1,40,true);
        //lets record the volume into an array
        volHistory.push_back( scaledVol );

        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory.size() >= 400 ){
                volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }
}

//-----------------------------------------------------
void ofApp::draw(){

//    ofTranslate(1920/2,1080/2,0);
//    ofRotate(ang*5,0,0,1);
//    ofTranslate(-1920/2,-1080/2,0);

    ofSetColor(drawcolor);
   if(zoom>0 || tilefx == true){
   if(zoom>0){
       drawcolor.a = 50;
       myMovies[currentVid][toggle].draw(-50*zoom*zoomx,-300*zoom,1920*(zoom+1),1080*(zoom+1));
   }


   if(tilefx == true) {

       for(int f = 0; f < numhoriz; f++){

       //steps thru vertical dim
        drawx = (1920/numhoriz)*f;
        ofTranslate(drawx, 0,0);

            for(int v = 0; v < numvert; v++){
           //horizontal drawing across

            drawy = (1080/numvert)*v;
            ofTranslate(0,drawy,0);

                ofTranslate((1920/(2*numhoriz))*1, (1080/(2*numvert))*1,0);
                ofRotate(ang*5,0,0,1);
                ofTranslate(-(1920/(2*numhoriz))*1,-(1080/(2*numvert))*1,0);

            //myMovies[currentVid][toggle].draw(drawx,drawy,1920/numhoriz,1080/numvert);
            myMovies[currentVid][toggle].draw(0,0,1920/numhoriz,1080/numvert);


            ofTranslate((1920/(2*numhoriz))*1, (1080/(2*numvert))*1,0);
            ofRotate(-ang*5,0,0,1);
            ofTranslate(-(1920/(2*numhoriz))*1,-(1080/(2*numvert))*1,0);


            ofTranslate(0,-drawy,0);



            }
         ofTranslate(-drawx,0,0);
       }
   }
   }
   //re-adding zoom to see what I can do with it -- beware
   else  myMovies[currentVid][toggle].draw(0,0,1920,1080);


    if(ghostfx == true){
        drawcolor.a = 50;
        for(int g = 0; g < numGhosts; g++){

            ofSetColor(drawcolor);
            //TODO - need to figure out transformation of ghost position
            //ghost position is roughly centered now
            myMovies[currentVid][toggle].draw(50*g,25*g,1920-100*g,1080-50*g);

       }
    }






ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);

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
            if(tempName.substr(tempName.find_last_of(".") + 1) == "avi" || tempName.substr(tempName.find_last_of(".") + 1) == "mp4") {

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

        case '7':
            //color tint
            colorfx = !colorfx;
            break;

        case '8':
            //ghost
            ghostfx = !ghostfx;
            break;

        case '9':
            numGhosts++;
        if(numGhosts > 20) numGhosts = 20;
            break;

        case '0':
            numGhosts--;
            if(numGhosts < 0) numGhosts = 0;
            break;
        case 'q':
            zoom--;
            if(zoom<0) zoom = 0;
            break;
        case 'w':
            zoom++;
            if(zoom>9) zoom = 9;
            break;
       case 'a':
            zoomx++;
            if(zoomx > 20) zoomx = 0;
            break;
        case 's':
            zoomx--;
            if(zoomx < 0) zoomx = 20;
            break;
        case 'e':
            tilefx = !tilefx;
        break;
        case 'r':
            numhoriz--;
            numvert--;
            if(numhoriz < 1) numhoriz = 1;
            if(numvert < 1) numvert = 1;
        break;
        case 't':
            numhoriz++;
            numvert++;
            if(numhoriz > 15) numhoriz = 15;
            if(numvert > 15 ) numvert = 15;

        break;


        case 'y':
            ang++;
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


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


//--------------------------------------------------------------
void ofApp::message_handler_loop() {
    while(1) {
        protocol_message_t *message = (protocol_message_t *)g_async_queue_pop(message_queue);
        //Handle message
        if(message) {
            free(message);
        }

    }
}


// parses thru serial message - still gotta figure this out and talk to Chris
void ofApp::serial_handler_loop() {
    int controldata;
    while(1) {
        if(serial.available()){
            controldata = serial.readBytes(0,3);
        }
    };
}


// serial communication thread - receives data from serial comms - empty 5/3/17
void *serial_communication_thread(void *data) {


    ofApp *app = (ofApp *)data;
    app->serial_handler_loop();

    return 0;
}


// does processing for fx/input without hanging up display - empty as of 5/3/17
void *message_processing_thread(void *data) {
    ofApp *app = (ofApp *)data;
    app->message_handler_loop();
        return 0;
}


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

