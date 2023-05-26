/*
Sebastian Torres
09.16.2022
Lab 1 - CSCI 4313
*/

#include <iostream>
#include <unistd.h>
#include "read_write.hpp"
#include "fork_join.hpp"
#include "lkbucket.hpp"

using namespace std;

int main(int argc, char** argv){
   // print name
   if (string(argv[1]) == "--name"){
      cout << "Sebastian Torres" << endl;
      }
   else{
      // get elements we need from command line
      string algo_to_use;
      // default is 4 threads
      int num_threads = 4;
       if (argc == 6){
           algo_to_use = string(argv[5]);
           string n_threads =  string(argv[4]);
           n_threads.erase(0,2);
           num_threads = stoi(n_threads);
       }
       else if(argc == 7){
           algo_to_use = string(argv[6]);
           string n_threads =  string(argv[5]);
           num_threads = stoi(n_threads);
       }
       else if(argc == 5){
           algo_to_use = string(argv[4]);
       }
       else{
           cout << "Invalid arguments" << endl;
       }

       // output/source file name
      string source_file = string(argv[1]);
      string output_file = string(argv[3]);

      // vector to hold data
      vector <int> data;
      // read from source file and turn into data
      data = read_from_file(source_file);

      // find algo wanted
      if (algo_to_use == "--alg=forkjoin"){
         MergeSort merge_sort(num_threads);
         merge_sort.MainDriver(data,num_threads);
      }
      else if (algo_to_use == "--alg=lkbucket"){
          BucketSort bucket_time(num_threads);
          bucket_time.MainDriver(data);
      }
      else{
          // input is not either algorithm
         cout << algo_to_use << endl;
         cout << "Unknown sorting algorithm";
         return -1;
      }
      // write data to output file 
      write_to_file(output_file, data);
   }
   return 0;
}
