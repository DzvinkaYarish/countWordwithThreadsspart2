//
// Created by dzvinka on 22.05.17.
//

#include "workingFunctions.h"


deque<vector<string>> d;
map<string, int> wordsCountMap;
condition_variable cv;
mutex mux;
mutex mux_map;
atomic <bool> done = {false};

void processWord(string& s)
{
    string new_s = "";
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    for (int i = 0; i <= s.size(); i++)
    {
        if (((s[i] <= 122) && (s[i] >= 97)))
        {
            new_s += s[i];
        }
    }
    s = new_s;
}

bool compareFunc(const vector<string>& v1, const vector<string>& v2)
{
    return stoi(v1[1]) > stoi(v2[1]);
}

vector<vector<string>> mapToVector(map<string, int>& m1)
{
    vector<vector<string>> v;
    for(auto mapIterator = m1.begin();
        mapIterator != m1.end();
        mapIterator++)
    {
        vector<string> temp;
        temp.push_back(mapIterator->first);
        temp.push_back(to_string(mapIterator->second));
        v.push_back(temp);
    }
    return  v;
}

void writeResToFile(string filename1, string filename2)
{
    ofstream fileWithSortedAlph;
    ofstream fileWithSortedNumb;

    fileWithSortedAlph.open(filename1);
    fileWithSortedNumb.open(filename2);

    for(auto wordsCountIterator = wordsCountMap.begin();
        wordsCountIterator != wordsCountMap.end();
        wordsCountIterator++)
    {
        fileWithSortedAlph << wordsCountIterator->first << " : "<< wordsCountIterator->second<< endl;
    }

    vector<vector<string>> v = mapToVector(wordsCountMap);
    sort(v.begin(), v.end(), compareFunc);
    for (vector<string> subv: v)
    {
        fileWithSortedNumb << subv[0] << " : " << subv[1] << endl;
    }

    fileWithSortedAlph.close();
    fileWithSortedNumb.close();
}





int producer(string filename) {
    fstream fin(filename); //full path to the file
    if (!fin.is_open()) {
        cout << "error reading from file";
        return  0;
    }
    string line;
    vector<string> lines;
    int numbLinesPerBlock = 0;
    while(getline(fin, line))
    {
        lines.push_back(line);
        if (numbLinesPerBlock == NUMB_LINES_IN_BLOCK)
        {
            {
                lock_guard<mutex> ll(mux);
                d.push_back(lines);
            }
            cv.notify_one();
            lines.clear();
            numbLinesPerBlock = 0;
        } else
        {
            numbLinesPerBlock++;
        }

    }
    if (lines.size() != 0)
    {
        {
            lock_guard<mutex> ll(mux);
            d.push_back(lines);
        }
        cv.notify_one();
    }
    cv.notify_all(); //notify all consumers that words have finished
    done = true;
    return 0;
}
int consumer()
{
    while(true)
    {
        unique_lock<std::mutex> lk(mux);
        if (!d.empty()) {
            vector<string> v {d.front()};  //take our data
            d.pop_front();
            lk.unlock();
            string word;
            for(int i = 0; i < v.size(); i++) {
                std::istringstream iss(v[i]);
                while(iss >> word)
                    //cout << word << endl;
                    processWord(word);
                if(word !="") {
                    lock_guard<mutex> lg(mux_map);
                    ++wordsCountMap[word];
                }
            }
        } else {
            if(done)
                break;
            else
                cv.wait(lk);  ///when there is  no wait on any of cv notify will go away
            /// wait also locks and unlocks mutexes
        }
    }
    return 0;

}

vector<string> readFromFile(string filename) {
    vector<string> words;
    fstream fin(filename); //full path to the file
    if (!fin.is_open()) {
        cout << "error reading from file";
        return words;
    }

    string line;

    while (getline(fin, line)) {
        std::istringstream iss(line);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }

    }
    return words;
}