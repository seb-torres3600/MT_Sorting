
/*
function bucketSort(input_array, k) :
  buckets[] : array of k empty sorted lists
  mins[] : array of k+1 minimum values for each list in buckets
  // bucket[m] holds (sorted) values from mins[m] to mins[m+1], so
  // mins[0]=0; mins[k]=INT_MAX;
  for i = 0 to length(input_array) do
    for j = 0 to k do
		if(input_array[i]<mins[j]) // find bucket to insert 
			insert input_array[i] into buckets[j-1]
  return the concatenation of buckets[0], ...., buckets[k-1]
*/

#include "lkbucket.hpp"
// init barrier, mutex, and number of threads
BucketSort ::BucketSort(int num_of_threads) {
    lk = new mutex();
    bar = new barrier(num_of_threads);
    NUM_THREADS = num_of_threads;
}

// delete barrier and lock
BucketSort :: ~BucketSort(){
    delete lk;
    delete bar;
}

// each threads calls this
void BucketSort ::Bucket(int tid) {
    // wait for all to arrive
    bar->arrive_and_wait();
    // main thread get start time
    if(tid == 1){
        clock_gettime(CLOCK_MONOTONIC, &startTime);
    }
    // wait for work to complete
    bar->arrive_and_wait();

    // Handle the last chunk of data
    if(tid == NUM_THREADS) {
        int starting_index = (tid-1) * split_size;
        for(int i = starting_index; i < int(complete_data.size()); i++) {
            // get bucket we need to add data into
            // Ex: floor((4 threads - 1 = number of buckets)* 12(data we are at)/MAX = 29) = bucket 1
            // Bucket data in that case is split [0-7.25][7.26-14.5][14.51-21.75][21.75-29]
            int bucket_index = floor((NUM_THREADS-1) * complete_data[i] / MAX);
            // Lock bucket so no other threads can access it
            lk->lock();
            // insert data into appropriate bucket
            buckets[bucket_index].insert({complete_data[i], 0});
            lk->unlock();
        }
    }

        // Sorting chunks of the data, keeping track using the thread id
    else {
        int starting_index = (tid-1) * split_size;
        int ending_index = ((tid-1) * split_size)+ split_size;
        for(int i = starting_index; i < ending_index; i++) {
            int bucket_index = floor((NUM_THREADS-1) * complete_data[i] / MAX);
            // Lock bucket so no other threads can access it
            lk->lock();
            // insert data into appriopriate bucket which is a map
            buckets[bucket_index].insert({complete_data[i], 0});
            lk->unlock();
        }
    }

    bar->arrive_and_wait();

    if(tid==1){
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }

    return;
}

void BucketSort :: MainDriver(vector<int> &data) {

    // get appropriate number of maps in our vector
    for(int i = 0; i < NUM_THREADS;i++){
        buckets.push_back(map<int,int>());
    }

    // get split size
    split_size = round(data.size()/NUM_THREADS);

    complete_data = data;
    // https://www.includehelp.com/stl/find-the-maximum-largest-element-of-a-vector.aspx
    MAX = *max_element(data.begin(), data.end());

    // resize thread vector to number of threads
    threads.resize(NUM_THREADS);

    int i;
    // create new threads and start at Bucket function
    for(i = 1;i < NUM_THREADS;i++){
        threads[i] = new thread(&BucketSort::Bucket,this, i+1);
    }

    i = 1;
    // main thread should also call Bucket
    Bucket(i);

    // join threads and delete thread
    for(int i =1; i < NUM_THREADS;i++){
        threads[i]->join();
        delete threads[i];
    }

    // merge the map keys into the final vector
    int index = 0;
    // https://www.techiedelight.com/print-keys-values-map-cpp/
    for(int i = 0; i < NUM_THREADS; i++) {
        for (auto const &pair: buckets[i]) {
            data[index] = pair.first;
            index++;
        }
    }

    //copied from example
    unsigned long long elapsed_ns;
    elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
    printf("Elapsed (s): %lf\n",elapsed_s);
}


