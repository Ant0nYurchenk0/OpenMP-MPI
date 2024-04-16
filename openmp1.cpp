// C++ program for Merge Sort
#include <bits/stdc++.h>
#include <chrono>
#include <limits.h>
#include <omp.h>
#include <thread>
#include <map>
#include <string>
#include <math.h>
#include <stdio.h>

using namespace std;
using namespace std::chrono;

#define NUMBERS 10000000
#define WORKERS 12

long long gethash(string str, int prime){
  long long result = 0;
  for(string::size_type i = 0; i < str.size(); ++i) {
    result += str[i]*pow(prime, i);
  }
  return result;
}
int main() 
{ 

  auto start = high_resolution_clock::now();
  int prime = 31;
  string str = "some text";
  long long hash = gethash(str, prime);
  cout<<"Hash is: "<<hash<<endl;
  int result = 0;
  #pragma omp parallel for num_threads(WORKERS)
  for (int i =0; i < omp_get_thread_num()+1; i++){
    #pragma omp task
    {
      for (int j = i*NUMBERS/WORKERS; j<(i+1)*NUMBERS/WORKERS; j++){
        // cout<<j<<endl;
        if(hash == gethash(str, j)){
          #pragma omp critical
          {
            result = j;
          }
        }
      }
      
    }

  }

  auto stop = high_resolution_clock::now();

  cout<<"Prime is: "<<result<<endl;

  auto duration = duration_cast<milliseconds>(stop - start);
  cout << "Execution time: " << duration.count() << endl;
  return 0; 
} 

