#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "protocol.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void gotMessage(ofMessage msg);
    void exit();

    //serial comms
    void get_serial_message(char *data);
    void message_handler_loop();
    void serial_handler_loop();

    void *serial_communication_thread(void *data);
    void *message_processing_thread(void *data);
    GAsyncQueue*  message_queue;
    pthread_t     serial_thread;
    pthread_t     message_thread;

    MyThread thread;


    //video switching/playback/finding
    vector <string> findVideos(string directory);

    void switchVideo(int targetVid);


    ofVideoPlayer myMovies[6][2];
    int currentVid = 0;
    bool toggle = false;


    //timers

    ofxSimpleTimer colorTimer;
    ofxSimpleTimer rotateTimer;

    void colorTimerCompleteHandler( int &args ) ;


    //fx

    //function for auto-modulating color
    void colorModulator(int step) ;

    //function for modulating color based on audio
    void colorAudio(int clrUpdate);

    bool colorfx = true;
    int clrUpdate = 0;
    bool ghostfx = false;
    int maxGhosts = 1;
    int numGhosts = 1;
    float ghostSize = 1;
    int cycle = 2000;
    int timer = 0;
    bool inc = true;


    //color fade stuff

    int step = 0;
    int colorspeed_p = 500;
    int colorspeed;
    int colordepth;
    int clrdep_new;
    int chgamt = 0;
	ofColor drawcolor;



    //tiling and drawing variables


    int drawx = 0;
    int drawy = 0;
    bool tilefx = false;
    int numTiles;
    int tileAudio = 0;
    int numhoriz = 1;
    int numvert = 1;


    //rotation variables
    float xang = 0;
    float rotate_amt = 0.2;
    int rotate_spd = 10;
    void rotateTimerCompleteHandler( int &args ) ;
    int TotalDraws;
    int ghostLimit = 32;
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


