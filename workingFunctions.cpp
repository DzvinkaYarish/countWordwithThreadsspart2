//
// Created by dzvinka on 22.05.17.
//

#include "workingFunctions.h"



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



lines_storing_t mapToVector(words_counter_t& m1)
{
    lines_storing_t v;
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

void writeResToFile(const string& filename1, const string& filename2, words_counter_t &wordsCountMap)
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
    sort(v.begin(), v.end(),
    [](const vector<string>& v1, const vector<string>& v2){
        return stoi(v1[1]) > stoi(v2[1]);
    });
    for (vector<string> subv: v)
    {
        fileWithSortedNumb << subv[0] << " : " << subv[1] << endl;
    }

    fileWithSortedAlph.close();
    fileWithSortedNumb.close();
}





int producer(argsForWorkers &args) {
    fstream fin(args.filename); //full path to the file
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
                lock_guard<mutex> ll(args.mux_d1);
                args.forwords.push_back(lines);
            }
            args.cv_producer.notify_one();
            lines.clear();
            numbLinesPerBlock = 0;
        } else
        {
            ++numbLinesPerBlock;
        }

    }
    if (lines.size() != 0)
    {
        {
            lock_guard<mutex> ll(args.mux_d1);
            args.forwords.push_back(lines);
        }
        args.cv_producer.notify_one();
    }
    args.cv_producer.notify_all(); //notify all consumers that words have finished
    args.done = true;
    //cout << "done" << endl;
    return 0;
}
int consumer(argsForWorkers &args)
{
    words_counter_t  localWordsCount;
    int count =0;
    while(true)
    {
        unique_lock<std::mutex> lk(args.mux_d2);
        if (!args.forwords.empty()) {
            vector<string> v {args.forwords.front()};  //take our data
            args.forwords.pop_front();
            lk.unlock();
            string word;
            for(int i = 0; i < v.size(); i++) {
                std::istringstream iss(v[i]);
                while(iss >> word)
                    //cout << word << endl;
                    //processWord(word);
                    //cout << "after processsing"<<word << endl;
                if(!word.empty()) {
                    count++;
                    ++localWordsCount[word];
                }
            }
        } else {
            if(args.done)
                break;
            else
                args.cv_producer.wait(lk);  ///when there is  no wait on any of cv notify will go away
            /// wait also locks and unlocks mutexes
        }

    }

    {
        lock_guard<mutex> lg(args.mux_d2);
        args.formaps.push_back(localWordsCount);
    }

    args.cv_consumer.notify_one();
    args.countConsumers++;

    cout << count << endl;
    return 0;

}
int MapsMerger(argsForWorkers &args)
{
    //int count = 0;


    while(true) {
        unique_lock<std::mutex> lk(args.mux_map);
        if (args.formaps.size() > 1) {
            words_counter_t localWordsCountMap1{args.formaps.front()};
            args.formaps.pop_front();
            words_counter_t localWordsCountMap2{args.formaps.front()};
            args.formaps.pop_front();

            lk.unlock();
            for (auto wordsCountIterator = localWordsCountMap2.begin();
                 wordsCountIterator != localWordsCountMap2.end();
                 wordsCountIterator++) {
                //cout << "writer " << wordsCountIterator->first << endl;
                ++localWordsCountMap1[wordsCountIterator->first];
            }
            lk.lock();
            args.formaps.push_back(localWordsCountMap1);
            lk.unlock();

        } else {
            if (args.countConsumers == args.numbOfConsumers) {
                break;
            } else {
                args.cv_consumer.wait(lk);
            }
        }
    }
    return 0;
}


vector<string> readFromFile(const string& filename) {
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