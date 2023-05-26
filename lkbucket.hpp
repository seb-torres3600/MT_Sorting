#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <ctime>
#include <map>
#include <math.h>

using namespace std;

class BucketSort{
    private:
        struct timespec startTime, endTime;
        vector<thread*> threads;
        barrier<> *bar;
        mutex* lk;
        int NUM_THREADS;
        int MAX;
        int split_size;
        vector <int> complete_data;
        vector<map<int, int>> buckets;

    public:
        BucketSort(int num_of_threads);
        ~BucketSort();
        void MainDriver(vector <int> &data);
        void Bucket(int tid);
};