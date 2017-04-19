#include "ofApp.h"
#include <dirent.h>

int currentVid = 0;

//--------------------------------------------------------------
void ofApp::setup(){

    // Manually find flash drive and name "/media/root/fdrive"
    system("sudo ~/unmountFlash.sh");
    system("sudo ~/mountFlash.sh");

    // to be replaced with auto-finder function



    string vids [3] = {
        "/media/root/fdrive/Lizard_Full.mp4",
        "/media/root/fdrive/animation_demo.avi",
        "/media/root/fdrive/jhomerun.avi" };

    myMovies[0].load(vids[0]);
    myMovies[1].load(vids[1]);
    myMovies[2].load(vids[2]);

    //myMovies[0].play();

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
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
