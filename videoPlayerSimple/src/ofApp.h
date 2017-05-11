#pragma once

#include "ofMain.h"





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
    bool colorfx = false;
    bool ghostfx = false;

    //buffer stuff


    ofFbo      fbo;
    ofTexture  tex [40];
    int bufIndex = 0;
};
