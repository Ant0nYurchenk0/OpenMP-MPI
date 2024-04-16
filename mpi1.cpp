#include <mpi.h>
#include <stdio.h>
#include <chrono>
#include <math.h>
#include <string> 

using namespace std;
using namespace std::chrono;

#define NUMBERS 10000000


long long gethash(string str, int prime){
  long long result = 0;
  for(string::size_type i = 0; i < str.size(); ++i) {
    result += str[i]*pow(prime, i);
  }
  return result;
;
}

void findprime(int rank, int procs){
  MPI_Status status;
  string str = "some text";
  long long hash;

  if (rank == 0){
    int prime = 31;
    hash = gethash(str, prime);
    printf("Sending hash %lld\n", hash);
    printf("Broadcasting\n");
  }
  MPI_Bcast((void *)&hash, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

  if(rank == 0){
    MPI_Barrier(MPI_COMM_WORLD);
    int prime;
    int result = 0;
    for(int i = 1; i < procs; i++){
      MPI_Recv((void *)&prime, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      printf("Received\n");
      if(prime != 0){
        result = prime;
      }
    }
    printf("Prime is: %d\n",result);
  }
  else{
    printf("%d: Received hash %lld\n", rank, hash);
    int result = 0;
    for (int i = (rank-1)*NUMBERS/(procs-1); i < (rank)*NUMBERS/(procs-1); i++){
      if(hash == gethash(str, i)){
        result = i;
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("%d: Sending back %d\n",rank, result);
    MPI_Send((void *)&result, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    printf("%d: Sent\n", rank);
  }
}

int main(int argc, char **argv){
  int num_procs;
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  auto start = high_resolution_clock::now();
  findprime(rank, num_procs);
  auto stop = high_resolution_clock::now();

  auto duration = duration_cast<milliseconds>(stop - start);
  printf("Execution time: %ld\n", duration.count());

  MPI_Finalize();
  return 0;
}

// mpic++ -o mpi mpi1.cpp
// mpiexec -n 4 ./mpi
// mpirun ./mpi