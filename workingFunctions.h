//
// Created by dzvinka on 22.05.17.
//

#ifndef COUNTWORDSPART2_WORKINGFUNCTIONS_H
#define COUNTWORDSPART2_WORKINGFUNCTIONS_H
#define NUMB_LINES_IN_BLOCK 50

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

void processWord(string& s);
bool compareFunc(const vector<string>& v1, const vector<string>& v2);
vector<vector<string>> mapToVector(map<string, int>& m1);
void writeResToFile(string filename1, string filename2);
vector<string> readFromFile(string filename);
int producer(string filename);
int consumer();



#endif //COUNTWORDSPART2_WORKINGFUNCTIONS_H
