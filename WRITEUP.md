# Sebastian Torres - CSCI 4313 (Lab 1)

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