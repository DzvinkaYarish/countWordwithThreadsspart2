#include <iostream>
#include <thread>
#include "workingFunctions.h"
#include "timeMeasuring.h"
#define PATH_TO_SETTINGS_FILE "/home/dzvinka/CLionProjects/countWordspart2/infile.txt"



int main() {

    vector<string> params = readFromFile(PATH_TO_SETTINGS_FILE);
    int numbofThreads = stoi(params[3]);
    auto startTime = getCurrentTimeFenced();
    thread consumingThreads[numbofThreads];
    thread thr1 = thread(producer, params[0]);
    for (int i  = 0; i < numbofThreads; i++) {
        consumingThreads[i] = thread(consumer);
    }

    thr1. join();
    for (int i = 0; i <  numbofThreads; i++)
    {
        consumingThreads[i].join();
    }
    auto endCount = getCurrentTimeFenced();
    writeResToFile(params[1], params[2]);
    auto endWrite = getCurrentTimeFenced();
    cout << "Total time: " << toMilliSec(endWrite - startTime) << endl;
    cout << "Counting words time: " << (int)toMilliSec(endCount - startTime) << endl;
    cout << "Writing time: " << toMilliSec(endWrite - endCount) << endl;
    cout << "Number of threads: " <<  numbofThreads << endl;

}