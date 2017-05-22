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
                        if(myByte != "\n"){
                            complete+=myByte;
                        }
                        else{
                            //parseSomeShit(complete);
			    params = parseInput(complete);
                           // cout << complete;
                           // cout << endl;
                            complete = " ";
                        }
                    }
                }
    }

    ofSerial      serial1;

    vector <int> params;

vector <int> parseInput(string input){

    vector <int> params;
    istringstream stream(input);
    int tmp;

    while (stream >> tmp)
      params.push_back(tmp);

    if( params[9] != 9 ){
	cout << "Encoder pressed: " << params[9] << endl;
    }

    for(int i = 0; i<params.size(); i++)
        cout << params[i] << ", ";

    cout << endl;

    return params;

} 

void parseSomeShit(string complete){
        cout << complete;
        cout << endl;



    }


};


