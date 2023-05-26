#include "fork_join.hpp"

// init the barrier
MergeSort :: MergeSort(int num_of_threads){
    bar = new barrier(num_of_threads);

}
//destroy the barrier
MergeSort :: ~MergeSort(){
    delete bar;
}

// other merge function that i used for lab 0  works with continuous memory vector
// doesn't work for merging two vectors
// used this function to do final merging of vectors
// same concept as Merge function underneath this
vector <int> MergeSort :: FinalMerging(vector <int> vectorA, vector <int> vectorB) {

    // vectorC is the output vector
    // it will store the merged vector obtained by merging v1 and v2
    vector<int> vectorC;
    int i = 0;
    int j = 0;
    int n, m;
    i = j = 0;
    n = vectorA.size();
    m = vectorB.size();

    // traverse each element of vectorA and vectorB
    while (i < n && j < m) {

        // merge two parts into vectorC
        if (vectorA[i] <= vectorB[j]) {
            vectorC.push_back(vectorA[i]);
            ++i;
        }
        else {
            vectorC.push_back(vectorB[j]);
            ++j;
        }

    }
    // push the elements left
    while (i < n) {
        vectorC.push_back(vectorA[i]);
        ++i;
    }
    // push the elements left
    while (j < m) {
        vectorC.push_back(vectorB[j]);
        ++j;
    }
    // return merged vector
    return vectorC;

}

// merge function from lab 0
// has to be one vector or data with continuous memory for it to work
void MergeSort :: Merge(vector<int> &data, int start, int mid, int end) {
	
	vector<int> tmp;

	int i = start;
	int j = mid + 1;

	// merge two parts into tmp
	while (i <= mid && j <= end) {
		if (data[i] <= data[j]) {
			tmp.push_back(data[i]);
			i++;
		}
		else {
			tmp.push_back(data[j]);
			j++;
		}
	}
	// Insert all the remaining values from i to mid into tmp
	while (i <= mid) {
		tmp.push_back(data[i]);
		i++;
	}
	// Insert all the remaining values from j to end into tmp
	while (j <= end) {
		tmp.push_back(data[j]);
		j++;
	}
	// Assign sorted data stored in tmp to data vector
	i = start;
	while(i <= end){
		data[i] = tmp[i-start];
		i++;
	}

}

// merge sort algorithm from lab 0
// recursively splits data and then merges
void MergeSort :: MergeSort_alg(vector<int> &data, int start, int end){
    if (start < end){
        int mid = (start + end)/2;
        MergeSort_alg(data, start, mid);
        MergeSort_alg(data, mid+1, end);
        Merge(data, start, mid, end);
    }
}

// have each thread call this function
void MergeSort :: ForkedSorting(int tid){

    //wait for all threads to arrive
    bar->arrive_and_wait();
    // if main thread get start time
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
    // wait for main thread to finish work
	bar->arrive_and_wait();
    // call merge sort algorithm with data that has been split
    // for that thread
	MergeSort_alg(split_data[tid-1],0, split_data[tid-1].size()-1);
    //wait until work is done
	bar->arrive_and_wait();

    if(tid==1){
        clock_gettime(CLOCK_MONOTONIC,&endTime);
    }

	return;
}

// split data into n number of chunks, n = number of threads
void MergeSort :: DataSplit(vector <int> &data, 
                            int size_to_split, int number_of_threads){
    // split initially from 0 to size we are splitting
    int ending_index = size_to_split;
    int starting_index = 0;

    // for the number of threads
    for(int i = 0; i < number_of_threads;i++){
        // handle last remainding chunk of data
        // doesn't always split evenly
        if( i == number_of_threads - 1){
            //get partial vector from data from starting index to end of data
            vector<int> chunk(data.begin() + starting_index,  data.end());
            // push data into our vector of vectors
            split_data.push_back(chunk);
        }
        else{
            //get partial vector from data from starting index to ending index
            vector<int> chunk(data.begin()+starting_index,data.begin()+ending_index );
            // push data into our vector of vectors
            split_data.push_back(chunk);
        }

        //move starting index to our ending index
        starting_index = ending_index;
        // move ending index the size we are splitting
        ending_index += size_to_split;

    }
}

void MergeSort :: MainDriver(vector <int> &data, int num_of_threads){

    // get size we want to split data
    int size_to_split = round(data.size()/num_of_threads);

    // split the data into the number of threads we have
    DataSplit(data, size_to_split, num_of_threads);

	int i;
    // resize vector holding threads
	threads.resize(num_of_threads);
    // make new thread and initialize them at the ForkedSorting function
	for(i=1; i < num_of_threads; i++){
		threads[i] = new thread(&MergeSort::ForkedSorting,this, i+1);
	}
    // main thread also calls ForkedSorting
	i=1;
	ForkedSorting(i);

    // join threads
	for(i = 1; i < num_of_threads; i++){
		threads[i]->join();
		delete threads[i];
	}

    // merge sorted vector data into one
    for(i = 1; i < num_of_threads; i++) {
        split_data[0] = FinalMerging(split_data[0], split_data[i]);
    }

    // set to data
    data = split_data[0];

    //copied from example, get time
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);

}