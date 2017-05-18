//Video Sampler Patch 5/16/17
// Developed on bump-a-grape

#include "ofApp.h"
#include <dirent.h>
#include "protocol.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //timer init
    colorTimer.setup(colorspeed_p);

     ofAddListener( colorTimer.TIMER_COMPLETE , this, &ofApp::colorTimerCompleteHandler ) ;

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
    //string dirString = "/media/ubuntu/fdrive/";
     string dirString = "/media/root/fdrive/";

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


    colorTimer.start(true);
    step = 0;

}

//--------------------------------------------------------------
void ofApp::update(){

    //audio update envelope

    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    numGhosts = ofMap(scaledVol, 0.0, 1.0,1,maxGhosts,true);
    if(clrdep_new > 0) clrUpdate = ofMap(scaledVol,0.0,1,clrdep_new,true);
        //lets record the volume into an array
        volHistory.push_back( scaledVol );

        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory.size() >= 400 ){
                volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }

    colorTimer.update();



    //2 dimensional array of videos
    myMovies[currentVid][toggle].update();

    if(colorfx) colorModulator(step);
    if(!colorfx) colorAudio(clrUpdate);









}

//-----------------------------------------------------
void ofApp::draw(){

    ofSetColor(drawcolor);



//LOOP FOR TILING
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

            drawcolor.a = 50;
            for(int g = 0; g < numGhosts; g++){

                ofSetColor(drawcolor);
                //TODO - need to figure out transformation of ghost position
                //ghost position is roughly centered now
                myMovies[currentVid][toggle].draw(50*g,25*g,(1920/numhoriz)-100*g,(1080/numvert)-50*g);

           }
           drawcolor.a = 255;

            ofTranslate((1920/(2*numhoriz))*1, (1080/(2*numvert))*1,0);
            ofRotate(-ang*5,0,0,1);
            ofTranslate(-(1920/(2*numhoriz))*1,-(1080/(2*numvert))*1,0);


            ofTranslate(0,-drawy,0);



            }
         ofTranslate(-drawx,0,0);
       }




       //LOOP FOR GHOSTING





drawcolor.a = 255;
ofSetColor(drawcolor);


//debug stuff -- get rid of in final version

int fr = int(ofGetFrameRate());
ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
ofDrawBitmapString("red" + ofToString(drawcolor.r * 1.0, 0), 500, 500);
ofDrawBitmapString("green" + ofToString(drawcolor.g * 1.0, 0), 500, 520);
ofDrawBitmapString("blue" + ofToString(drawcolor.b * 1.0, 0), 500, 540);
ofDrawBitmapString("step" + ofToString(step * 1.0, 0), 500, 560);
ofDrawBitmapString("clrdep_new" + ofToString(clrdep_new * 1.0, 0), 500, 600);
ofDrawBitmapString("chgamt" + ofToString(chgamt * 1.0, 0), 500, 620);
ofDrawBitmapString("framerate" + ofToString(fr * 1.0, 0), 500, 640);

colorTimer.draw(40,40);

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


        case '9':
            maxGhosts++;
        if(maxGhosts > 20) maxGhosts = 20;
            break;

        case '0':
            maxGhosts--;
            if(maxGhosts < 0) maxGhosts = 0;
            break;
        case 'q':
            colorfx = !colorfx;
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

        case 'g':
            colorspeed_p-=50;
            if(colorspeed_p < 50) colorspeed_p = 50;


        break;

        case 'h':
            colorspeed_p+=50;


        break;

        case 'j':
            clrdep_new+=10;
            if(clrdep_new > 250)clrdep_new = 250;
        break;

        case 'k':
            clrdep_new-=10;
            if(clrdep_new < 0)clrdep_new = 0;
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

void ofApp::colorAudio(int clrUpdate){

   drawcolor.r = 255 - clrUpdate;
   drawcolor.g = 128 + clrUpdate;
   drawcolor.b = 128 + clrUpdate*2;










}


