//Video Sampler Patch 5/5/17
// Developed on bump-a-grape

#include "ofApp.h"
#include <dirent.h>
#include "protocol.h"


void *serial_communication_thread(void *data);
void *message_processing_thread(void *data);

int currentVid = 0;
int numVids = 0;

//--------------------------------------------------------------
void ofApp::setup(){

    //serial comms init

    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    //need to set up custom to Jetson serial port

    int baud = 57600;
    //serial.setup(0, 57600); //open the first device and talk to it at 57600 baud




    // Initialize threads and queue for serial comms
    message_queue = g_async_queue_new();
    if(pthread_create(&message_thread, NULL, message_processing_thread, this)) {
        exit();
    }
    if(pthread_create(&serial_thread, NULL, serial_communication_thread, this)) {
        exit();
        }



    // Manually find flash drive and name "/media/root/fdrive"
    // put these scripts in home directory (~)
    //seg fault issue on Jetson - had to do with size of array  - 5/3/17
    //does not break if there are more/less than 6 videos


    system("sudo ~/unmountFlash.sh");
    system("sudo ~/mountFlash.sh");

    string tempName;
    string videos [6] ;
    string dirString = "/media/root/fdrive/";

    DIR *dir;
    struct dirent *entries;

    if (( dir = opendir("/media/root/fdrive/")) != NULL) {;

        int count = 0;
        while ((entries = readdir (dir)) != NULL && count <6) {

            tempName = entries->d_name;

            if(tempName.substr(tempName.find_last_of(".") + 1) == "avi" || tempName.substr(tempName.find_last_of(".") + 1) == "mp4") {
                videos[count] = dirString + tempName;
                count++;
            }
        }
        closedir (dir);
    }

    // display videos found
    cout << "videos in array\n" ;

    //TODO: figure out how to limit the size of video array efficiently
    for( int i=0; i<6; i++){
        cout << videos[i] << "\n" ;
    }

    /* Hard-coded paths - LEGACY
    string vids [3] = {
        "/media/root/fdrive/Lizard_Full.mp4",
        "/media/root/fdrive/animation_demo.avi",
        "/media/root/fdrive/jhomerun.avi" };
    */

    // load videos into each ofVideoPlayer
    int i = 0;
    while(videos[i] != ""){
        myMovies[i*2].load(videos[i]);
        myMovies[i*2+1].load(videos[i]);

        //mutes audio in case of HDMI audio output
        myMovies[i*2].setVolume(0);
        myMovies[i*2+1].setVolume(0);
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
    myMovies[currentVid].update();

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

    myMovies[currentVid].draw(0,0,1920,1080);

    if(ghostfx == true){
        for(int i = 1; i < numGhosts; i++){
            ofSetColor(i*25,250/i,255/i,80);
            //TODO - need to figure out transformation of ghost position
            myMovies[currentVid].draw(50*i,50*i,1920/i,1080/i);
            ofSetColor(255,255,255,255);
        }
    }







}


//keyboard control...to be taken out when controller is operational

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    // switches current video with 1,2,3 button presses

    switch(key){
        case '1':
        //pause current, rewind to first frame, play new. This produces improvement in switching hi quality videos
        //TODO: functionize

            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();

            if(currentVid == 0){
                myMovies[1].setPaused(false);
                currentVid = 1;
            }
            else{
            myMovies[0].setPaused(false);
            currentVid = 0;
            }

            break;
        case '2':
            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();

            if(currentVid == 2){
                myMovies[3].setPaused(false);
                currentVid = 3;
            }
            else{
            myMovies[2].setPaused(false);
            currentVid = 2;
            }
            break;
        case '3':
            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();

            if(currentVid == 4){
                myMovies[5].setPaused(false);
                currentVid = 5;
            }
            else{
            myMovies[4].setPaused(false);
            currentVid = 4;
            }
            break;
        case '4':
            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();


            if(currentVid == 6){
                myMovies[7].setPaused(false);
                currentVid = 7;
            }
            else{
            myMovies[6].setPaused(false);
            currentVid = 6;
            }
            break;
        case '5':
            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();

            if(currentVid == 8){
                myMovies[9].setPaused(false);
                currentVid = 9;
            }
            else{
            myMovies[8].setPaused(false);
            currentVid = 8;
            }
            break;
        case '6':
            myMovies[currentVid].setPaused(true);
            myMovies[currentVid].firstFrame();

            if(currentVid == 10){
                myMovies[11].setPaused(false);
                currentVid = 11;
            }
            else{
            myMovies[10].setPaused(false);
            currentVid = 10;
            }
            break;
        case '7':
            //color tint
            if(colorfx == false) colorfx = true;
            else colorfx = false;
            break;
        case '8':
            //ghost
            if(ghostfx == false) ghostfx = true;
            else ghostfx = false;
            break;
        case '9':
            numGhosts++;
            break;
        case '0':
            numGhosts--;
            break;




    }
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
