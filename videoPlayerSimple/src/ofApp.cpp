#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // Manually find flash drive and name "/media/es/fdrive"
    system("sudo ~/unmountFlash.sh");
    system("sudo ~/mountFlash.sh");

    myMovies[0].load("/media/es/fdrive/animation_demo.avi");
    myMovies[0].play();

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
    myMovies[0].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    myMovies[0].draw(0,0,1920,1080);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
