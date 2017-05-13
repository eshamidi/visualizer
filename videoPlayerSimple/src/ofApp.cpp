//Video Sampler Patch 5/5/17
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
    serial.setup(0, baud); //open the first device and talk to it at 57600 baud

    // Initialize threads and queue for serial comms
//    message_queue = g_async_queue_new();
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

    /* Audio Setup ~for later use
    soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.

    int bufferSize = 256;


    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);

    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;

    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    */


    for(int i =0; i < 3; i++){
        tex[i].allocate(1920, 1080, OF_PIXELS_RGB);
        tex[i].clear();
    }

    pixelated.allocate(1920 ,1080,OF_PIXELS_RGB);

    //fbo.allocate(1920,1080,GL_RGB);

    ofSetVerticalSync(TRUE);

}

//--------------------------------------------------------------
void ofApp::update(){
    myMovies[currentVid][toggle].update();
    //myMovies[currentVid].update();

    //an fbo would probably be best used here. hope it works on the jetson!
    //this pixelation algorithm only runs the loop as long as needed for the downsampling operation.
//    if(myMovies[currentVid].isFrameNew()){
//            ofPixels & pixels = myMovies[currentVid].getPixels();


//            for(int i = 0; i < pixels.size(); i++){

//                pixelated[i] = pixels[i];
//            }
//            //load the pixels
//            tex[1].loadData(pixelated);
//        }



}

//--------------------------------------------------------------
void ofApp::draw(){

    if(colorfx == true) ofSetColor(255,47,255,255);
    if(colorfx == false) ofSetColor(255,255,255,255);

    myMovies[currentVid][toggle].draw(0,0,1920,1080);
    //myMovies[currentVid].draw(0,0,1920,1080);

    if(ghostfx == true){
        for(int i = 1; i < numGhosts; i++){
            ofSetColor(i*25,250/i,255/i,80);
            //TODO - need to figure out transformation of ghost position
            myMovies[currentVid][toggle].draw(50*i,50*i,1920/i,1080/i);
            //myMovies[currentVid].draw(50*i,50*i,1920/i,1080/i);
            ofSetColor(255,255,255,255);
        }
    }
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
            break;

        case '0':
            numGhosts--;
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
