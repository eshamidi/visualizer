#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"




class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


    //serial comms

    void get_serial_message(char *data);
    void message_handler_loop();
    void serial_handler_loop();


    GAsyncQueue*  message_queue;
    pthread_t     serial_thread;
    pthread_t     message_thread;
    ofVideoPlayer myMovies [6];
    ofSerial      serial;


    //fx

    ofxPostProcessing post;

    itg::NoiseWarpPass::Ptr noiseWarp;
    itg::KaleidoscopePass::Ptr kScope;
    itg::RGBShiftPass::Ptr rgbShift;
    itg::ToonPass::Ptr toon;
    float n = 5;
    float m = 5;
    float b = 0;
    float v = 5;

    bool fx = false;


    //buffer stuff


    ofFbo      fbo;
    ofTexture  tex [40];
    int bufIndex = 0;
};
