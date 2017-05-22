#pragma once

#include "ofMain.h"
#include "ofThread.h"

class MyThread : public ofThread {
public:
    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
        serial1.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial1.getDeviceList();

        int baud = 9600;
        serial1.setup("/dev/ttyTHS1", baud);
    }

    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
    }

    // the thread function
    void threadedFunction() {
        string myByte = " ";
        string complete = " ";


                 while(isThreadRunning()){
                    if(serial1.available() > 0){
                        myByte = serial1.readByte();
                        if(myByte != "/n"){
                            complete+=myByte;
                        }
                        else{
                            parseSomeShit(complete);
                            //cout << complete;
                            //cout << endl;
                            //complete = " ";
                        }
                    }
                }
    }

    ofSerial      serial1;


    void parseSomeShit(string complete){
        cout << complete;
        cout << endl;



    }


};


