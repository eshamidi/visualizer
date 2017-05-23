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
        startThread(true,true);
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
			    lock();
			    params = parseInput(complete);
			    unlock();
                        //    cout << complete;
			unsigned char stuff = 117;
		        bool isWritten =  serial1.writeByte(stuff);
			

                          //  cout << endl;
                            complete = " ";
                        }
                    }
                
    }
}
    ofSerial      serial1;
    vector <int> params;

vector <int> parseInput(string input){

    vector <int> params_1;
    istringstream stream(input);
    int tmp;

    while (stream >> tmp)
      params_1.push_back(tmp);

    if( params_1[9] != 9 ){
//	cout << "Encoder pressed: " << params[9] << endl;
    }

    for(int i = 0; i<params_1.size(); i++)
  //      cout << params[i] << ", ";

    //cout << endl;

    return params_1;

} 

//void sendFileNames(){
   //   byte write = 114;
  //    bool byteWrite =  serial1.writeByte('r');
//	if(byteWrite == true){
//		cout << "byte sent" << endl;
//		byteWrite = false;
//}

//    }


vector <int> pushParams(){
	return params;
}

};


