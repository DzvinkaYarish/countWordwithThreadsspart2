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

void processWord(string& s);
bool compareFunc(const vector<string>& v1, const vector<string>& v2);
lines_storing_t mapToVector(words_counter_t & m1);
void writeResToFile(const string& filename1, const string& filename2);
vector<string> readFromFile(const string& filename);
int producer(const string& filename);
int consumer();



#endif //COUNTWORDSPART2_WORKINGFUNCTIONS_H
