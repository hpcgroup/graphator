#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MP_X 0
#define MP_Y 1
#define MP_Z 2

#define MAX_ITER 1

#define calc_pe(a,b,c)	((a)+(b)*num_blocks_x+(c)*num_blocks_x*num_blocks_y)

int main(int argc, char **argv)
{
  MPI_Init(&argc,&argv);

  int i,myrank, numranks, groupsize;

  if(argc != 5) {
    printf("Correct usage: %s <dim_x> <dim_y> <dim_z> <msg_size_doubles>\n",argv[0]);
    return 1;
  }

  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  MPI_Comm_size(MPI_COMM_WORLD,&numranks);

  int num_blocks_x = atoi(argv[1]);
  int num_blocks_y = atoi(argv[2]);
  int num_blocks_z = atoi(argv[3]);
  int perrank = atoi(argv[4])*8;
 
  for(myrank = 0; myrank < num_blocks_x*num_blocks_y*num_blocks_z; myrank++) {
    int myXcoord = myrank % num_blocks_x;
    int myYcoord = (myrank % (num_blocks_x * num_blocks_y)) / num_blocks_x;
    int myZcoord = (myrank % (num_blocks_x * num_blocks_y * num_blocks_z)) / (num_blocks_x * num_blocks_y);
    for(int ycoord = 0; ycoord < num_blocks_y; ycoord++) {
      if(ycoord == myYcoord) continue;
      printf("%d %d %d\n", myrank, calc_pe(myXcoord,ycoord,myZcoord),perrank);
    }
  }

  MPI_Finalize();
}
