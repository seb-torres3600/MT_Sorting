
## Summary 
This lab familiarized me with fork/join parallelism, barriers, and locks.  


## Fork/Join Quick/Mergesort
Your fork/join algorithm should be an implementation of either Mergesort or Quicksort.  Your parallelization strategy should use fork/join parallelism, implemented using fork, join, and barriers.  Locks should not be necessary. You are allowed to use pre-written unsorted data structures (e.g. STL's vector), and your Lab 0 code, but you may not use any pre-written sorted data structure or sorting algorithm (e.g. STL's map) for this algorithm.


## Locking BucketSort
Your second algorithm should be an implementation of BucketSort using sorted data structures (e.g. STL's map - note that sorted data structures are allowed in this algorithm, but not the other).  BucketSort normally uses unsorted buckets, using sorted buckets makes the task significantly easier - you need not recurse to sort each bucket after the first insertion.  Pseudocode for this algorithm is as follows:

```
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
```

Your implementation of Locking BucketSort should avoid idling threads if at all possible. For instance, a problem decomposition that assigns the bucket ranges evenly across threads will result in idle threads when the source distribution is highly skewed e.g. (1,2,3,4,5,6,7,1001,1000,1002).  Instead, you should work to ensure that all threads are busy at all times, even at the cost of synchronizing across buckets.  The `skew` workloads in `autograde_tests` have a highly skewed distribution.

## Examples

Examples of your `mysort` program's syntax
```
### print your name
./mysort --name
# prints:
Your Full Name

### Consider an unsorted file
printf "3\n2\n1\n" > 321.txt
cat 321.txt
# prints
3
2
1

### Sort the text file and print to file
./mysort 321.txt -o out.txt -t5 --alg=forkjoin
# prints time taken in nanoseconds for 5 threads on fork/join sort:
294759

cat out.txt
# prints:
1
2
3
```

### Compilation and Execution:
Your submission should contain a Makefile and the project should build using a single `make` command.  The generated executable should be called `mysort`.  The `mysort` command should have the following syntax (pay attention, it is slightly
different from Lab 0):

`mysort [--name] [source.txt] [-o out.txt] [-t NUMTHREADS] [--alg=<forkjoin,lkbucket>] `

Using the `--name` option should print your name.  Otherwise, the program should sort the source file.  The source file is a text file with a single integer on each line.  The `mysort` command should then sort all integers in the source file and print them sorted one integer per line to an output file (specified by the `-o` option). The time taken to sort the file (excluding the time for file I/O and to initially launch / finally join threads) should be printed to standard out in nanoseconds (see `text.c` for examples).  The `-t` option specifies the number of threads that should be used, including the master thread, to accomplish the sorting. 

See below for `mysort` syntax examples.  As before, you can assume that all input values are non-negative, less than or equal to `INT_MAX`, and that there are no duplicates.

See the included `exampleC.c` , `exampleCPP.cpp` and `Makefile` for boilerplate thread launching and timing code using the high resolution `CLOCK_MONOTONIC` timer and the pthread and C++ threading interface.  You are welcome to use any/all of this code for building your applications. Note that the example code does not appropriately handle the `mysort` command line syntax. 

### Strategies:
My strategies for both were very similar. Split the data by the number of threads we were
going to use and have each thread sort or place their data into a bucket.

I decided to do Merge Sort as my sorting algorithm. I split the data into chunks and had each 
thread call MergeSort on its chunk. That way each individual chunk is sorted. I later joined the threads
and joined their vectors into a main vector. 

For bucket sort, since we could use sorted data structures I created a vector of maps and used the 
keys to sort my values. There was n number of buckets which represents the number of threads we were using.
I once again split the data in n number of chunks, and had each thread place their data in the appropriate buckets.
The only shared info the threads had was the buckets so I had to put a lock around that to avoid having a data race. 

### Performance:
On the smaller data sets you don't really notice a speed up, once you get into larger data sets and start using a
lot more threads then you can see significant speed improvements. I think that lkbucket was slower when it came to
the skewed data sets but overall it seemed to be quicker than Merge Sort. Maybe cause I was
presorting the data with map, not sure.

### Organization:
    main.cpp - main driver file
    read_write.*pp - files holding functions to read and write data
    lk_bucket.*pp - files that contain my bucket sorting algorithm
    fork_join,*pp - files holding my functions for merge sort
    Makefile - make file to get one executable
    Examples folder - holds fix and class examples
    
### File Description:
main.cpp -  main driver file, gathers info from command line and calls appropriate algo with the number of threads
specified. First reads from source file, puts data into a vector, calls algo, writes vector to 
output file. A lot of reused code from lab 0.

fork_join.hpp - header file for fork_join.cpp, creates MergeSort class

fork_join.cpp - Merge sort algorithm lives here. We have a main driver function which calls our function to split 
our data n times depending on number of threads, creates n threads,  each thread starting at our forkingAlgo. 
Each thread then merge sorts on their own data chunk,they come back and join together, and the data chunks get 
merged into one vector that gets written to our output file. A lot of reused code from lab 0.

lkbucket.hpp - header file for lkbucket.cpp, creares Bucket class

lkbucket.cpp - BucketSort algorithm lives here. We have a driver function in here which gets the size we need to split
the data, it creates n threads which start at the Bucket sort algo, each thread takes their chunk of data and
places it in the appropriate bucket. The critical section is when we are writing to the buckets so there is a lock
around that to make sure we dont have a data race. We end up getting a map of values where the 
key is the value we care about, we join the threads at the driver function, we merge the keys of the map
into one main vector which then gets written to our output file.

read_write.hpp -  header file for read_write.cpp

read_write.cpp - same as lab 0. Has functions that read from source file and place data in a vector, also has
function to write vector back into a text file. 

### Bugs:
Currently none but its multi-threaded, so I'm sure there's one currently hiding in their.