//
// Created by dzvinka on 22.05.17.
//

#ifndef COUNTWORDSPART2_WORKINGFUNCTIONS_H
#define COUNTWORDSPART2_WORKINGFUNCTIONS_H
#define NUMB_LINES_IN_BLOCK 50  //most optimal size of a block

#include <iostream>
#include <deque>
#include <atomic>
#include <mutex>
#include <thread>
#include <map>
#include <fstream>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;
using words_counter_t=map<string, int>;
using lines_storing_t=vector<vector<string>>;

struct argsForWorkers{
    string filename;
    deque<words_counter_t > formaps;
    deque<vector<string>> forwords;
    mutex mux_d1, mux_d2, mux_map;
    condition_variable cv_producer, cv_consumer;
    atomic <bool> done = {false};
    atomic <int>  countConsumers = {0};
    int numbOfConsumers;


};

void processWord(string& s);
lines_storing_t mapToVector(words_counter_t & m1);
void writeResToFile(const string& filename1, const string& filename2, words_counter_t &wordsCountMap);
vector<string> readFromFile(const string& filename);
int producer(argsForWorkers &args);
int consumer(argsForWorkers &args);
int  MapsMerger(argsForWorkers &args);



#endif //COUNTWORDSPART2_WORKINGFUNCTIONS_H
