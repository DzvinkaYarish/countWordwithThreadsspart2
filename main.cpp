#include <iostream>
#include <thread>
#include "workingFunctions.h"
#include "timeMeasuring.h"
#include <unistd.h>
#include <zconf.h>





int main() {
    char* cwd;
    char buff[PATH_MAX + 1];

    cwd = getcwd( buff, PATH_MAX + 1 );

    string pathToSettingsFile(cwd);

    pathToSettingsFile += "/infile.txt";

    argsForWorkers argsForWorkers1;


    vector<string> params = readFromFile(pathToSettingsFile);

    int numbofThreads = stoi(params[3]);
    argsForWorkers1.filename = params[0];
    argsForWorkers1.numbOfConsumers = numbofThreads/2;

    auto startTime = getCurrentTimeFenced();
    thread consumingThreads[numbofThreads / 2];
    thread mergingThreads[numbofThreads/2 + numbofThreads%2];
    thread prod = thread(producer, ref(argsForWorkers1));

    for (int i  = 0; i < numbofThreads / 2; i++) {
        consumingThreads[i] = thread(consumer, ref(argsForWorkers1));
    }

    prod. join();
    for (int i = 0; i <  numbofThreads/2; i++)
    {
        consumingThreads[i].join();
    }
    auto endCount = getCurrentTimeFenced();
    for (int i  = 0; i < numbofThreads / 2 + numbofThreads % 2; i++) {
        mergingThreads[i] = thread(MapsMerger, ref(argsForWorkers1));
    }
    for (int i  = 0; i < numbofThreads / 2 + numbofThreads % 2; i++) {
        mergingThreads[i].join();
    }





    writeResToFile(params[1], params[2], argsForWorkers1.formaps[0]);
    auto endWrite = getCurrentTimeFenced();
    cout << "Total time: " << toMilliSec(endWrite - startTime) << " milisec"<< endl;
    cout << "Counting words time: " << (int)toMilliSec(endCount - startTime) << " milisec"<< endl;
    cout << "Merging/Writing time: " << toMilliSec(endWrite - endCount) << " milisec" <<endl;
    cout << "Number of threads: " <<  numbofThreads  + 1 << endl;
    cout << "Number of consumers: " << numbofThreads / 2 << endl;
    cout << "Number of mergers: " << numbofThreads / 2  + numbofThreads % 2 << endl;


//    string a = "a";
//    processWord(a);
//    cout << a << endl;

}