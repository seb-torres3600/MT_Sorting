#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <math.h>
#include <ctime>

using namespace std;

class MergeSort{
    private:
        vector<thread*> threads;
        barrier<> *bar;
        struct timespec startTime, endTime;
        vector<vector<int>> split_data;

    public:
        MergeSort(int num_of_threads);
        ~MergeSort();
        vector<int> FinalMerging(vector <int> vectorA, vector <int> vectorB);
        void Merge(vector<int> &data, int start, int mid, int end);
        void MergeSort_alg(vector<int> &data, int start, int end);
        void ForkedSorting(int tid);
        void DataSplit(vector <int> &data, int size_to_split, int number_of_threads);
        void MainDriver(vector <int> &data, int num_of_threads);
};