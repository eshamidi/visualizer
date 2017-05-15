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
    ofSerial      serial;
    void get_serial_message(char *data);
    void message_handler_loop();
    void serial_handler_loop();

    void *serial_communication_thread(void *data);
    void *message_processing_thread(void *data);
    GAsyncQueue*  message_queue;
    pthread_t     serial_thread;
    pthread_t     message_thread;


    //video switching/playback/finding
    vector <string> findVideos(string directory);

    void switchVideo(int targetVid);


    ofVideoPlayer myMovies[6][2];
    int currentVid = 0;
    bool toggle = false;



    //fx

    bool colorfx = false;
    bool ghostfx = false;
    int numGhosts = 1;
    int red, green, blue;
    int cycle = 2000;
    int timer = 0;
    bool inc = true;
    int step = 0;
    ofPixels pixelated;
	
    int zoom = 0; 
    int zoomx = 0;
    ofColor drawcolor;

    bool tilefx;
    int drawx = 0;
    int drawy = 0;
    int numTiles = 1;
    int numhoriz = 1;
    int numvert = 1;


    //buffer stuff

    //ofFbo      fbo;
    //ofTexture  tex [5];
    int bufIndex = 0;




    //audio stuff

    void audioIn(ofSoundBuffer & input);


    vector <float> left;
    vector <float> right;
    vector <float> volHistory;

    int 	bufferCounter;
    int 	drawCounter;

    float smoothedVol;
    float scaledVol;

    ofSoundStream soundStream;
};


