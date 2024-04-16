// C++ program for Merge Sort
#include <bits/stdc++.h>
#include <chrono>
#include <limits.h>
#include <omp.h>
#include <thread>
#include <map>

using namespace std;
using namespace std::chrono;

#define SIZE 100000000

int fib(int n) 
{ 
    if (n <= 1) 
        return n; 
    return fib(n - 1) + fib(n - 2); 
} 

int main() 
{ 

  std::map<int, int> fibs;

  auto start = high_resolution_clock::now();

  // #pragma omp parallel 
  // {

    #pragma omp parallel for num_threads(12)
    for (int i = 0; i < 45; i++){

      #pragma omp task
      {
        long long fibonacci = fib(i);
        #pragma omp critical
        {
          fibs[i] = fibonacci;
        }

      }

    }

  // }
  auto stop = high_resolution_clock::now();

  // for(auto &itr: fibs)
  // {
  //     cout<<itr.first<<" "<<itr.second<<endl;
  // }

  auto duration = duration_cast<milliseconds>(stop - start);
  cout << endl << "Execution time: " << duration.count() << endl;
  return 0; 
} 

// int main() 
// { 

//   std::map<int, int> fibs;

//   auto start = high_resolution_clock::now();


//   for (int i = 0; i < 40; i++){

//     long long fibonacci = fib(i);
//     fibs[i] = fibonacci;
//   }


//   auto stop = high_resolution_clock::now();


//   for(auto &itr: fibs)
//   {
//       cout<<itr.first<<" "<<itr.second<<endl;
//   }


//   auto duration = duration_cast<milliseconds>(stop - start);
//   cout << endl << "Execution time: " << duration.count() << endl;
//   return 0; 
// } 
