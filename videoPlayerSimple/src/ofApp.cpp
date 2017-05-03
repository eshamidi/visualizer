//Video Sampler Patch 5/3/17
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
        myMovies[i].load(videos[i]);
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
}

//--------------------------------------------------------------
void ofApp::update(){
    myMovies[currentVid].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    myMovies[currentVid].draw(0,0,1920,1080);
}


//keyboard control...to be taken out when controller is operational

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    // switches current video with 1,2,3 button presses

    switch(key){
        case '1':
            myMovies[currentVid].stop();
            myMovies[0].play();
            currentVid = 0;
            break;
        case '2':
            myMovies[currentVid].stop();
            myMovies[1].play();
            currentVid = 1;
            break;
        case '3':
            myMovies[currentVid].stop();
            myMovies[2].play();
            currentVid = 2;
            break;
        case '4':
            myMovies[currentVid].stop();
            myMovies[3].play();
            currentVid = 3;
            break;
        case '5':
            myMovies[currentVid].stop();
            myMovies[4].play();
            currentVid = 4;
            break;
        case '6':
            myMovies[currentVid].stop();
            myMovies[5].play();
            currentVid = 5;
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
