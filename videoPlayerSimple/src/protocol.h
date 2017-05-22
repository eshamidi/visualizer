#pragma once

#include "ofMain.h"
#include "ofThread.h"

class MyThread : public ofThread {

    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
        serial1.listDevices();
        vector <ofSerialDeviceInfo> deviceList = serial1.getDeviceList();

        int baud = 9600;
        serial1.setup(1, baud);
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


                 while(isThreadRunning()){
                     cout << "good 2 go";
                     if(serial1.available()){

                     }


        }




    }
    ofSerial      serial1;

};


