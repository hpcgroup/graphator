#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

/* We want to wrap entries around, and because mod operator % sometimes
 * misbehaves on negative values. -1 maps to the highest value.
 */
#define DO_COMM 0
#define wrap_x(a)	(((a)+num_blocks_x)%num_blocks_x)
#define wrap_y(a)	(((a)+num_blocks_y)%num_blocks_y)
#define wrap_z(a)	(((a)+num_blocks_z)%num_blocks_z)
#define wrap_t(a)	(((a)+num_blocks_t)%num_blocks_t)

#define index(a,b,c,d)		((a)+(b)*(blockDimX+2)+(c)*(blockDimX+2)*(blockDimY+2)\
				+(d)*(blockDimX+2)*(blockDimY+2)*(blockDimZ+2))
#define calc_pe(a,b,c,d)	((a)+(b)*num_blocks_x+(c)*num_blocks_x*num_blocks_y \
				+(d)*num_blocks_x*num_blocks_y*num_blocks_z)

#define MAX_ITER	1
#define LEFT		1
#define RIGHT		2
#define TOP		3
#define BOTTOM		4
#define FRONT		5
#define BACK		6
#define FORWARD		7
#define BACKWARD	8
#define DIVIDEBY9	0.11111111111111111

int main(int argc, char **argv) {
  int myRank, numPes;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numPes);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Request req[8];
  MPI_Status status[8];

  int blockDimX, blockDimY, blockDimZ, blockDimT;
  int arrayDimX, arrayDimY, arrayDimZ, arrayDimT;
  int noBarrier = 0;

  if (argc != 4 && argc != 10) {
    printf("%s [array_size] [block_size] +[no]barrier\n", argv[0]);
    printf("%s [array_size_X] [array_size_Y] [array_size_Z] [array_size_T] [block_size_X] [block_size_Y] [block_size_Z] [block_size_T] +[no]barrier\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, -1);
  }

  if(argc == 4) {
    arrayDimT = arrayDimZ = arrayDimY = arrayDimX = atoi(argv[1]);
    blockDimT = blockDimZ = blockDimY = blockDimX = atoi(argv[2]);
    if(strcasecmp(argv[3], "+nobarrier") == 0)
      noBarrier = 1;
    else
      noBarrier = 0;
    if(noBarrier && myRank==0) printf("\nSTENCIL COMPUTATION WITH NO BARRIERS\n");
  }
  else {
    arrayDimX = atoi(argv[1]);
    arrayDimY = atoi(argv[2]);
    arrayDimZ = atoi(argv[3]);
    arrayDimT = atoi(argv[4]);
    blockDimX = atoi(argv[5]);
    blockDimY = atoi(argv[6]);
    blockDimZ = atoi(argv[7]);
    blockDimT = atoi(argv[8]);
    if(strcasecmp(argv[9], "+nobarrier") == 0)
      noBarrier = 1;
    else
      noBarrier = 0;
    if(noBarrier && myRank==0) printf("\nSTENCIL COMPUTATION WITH NO BARRIERS\n");
  }

  if (arrayDimX < blockDimX || arrayDimX % blockDimX != 0) {
    printf("array_size_X mod block_size_X != 0!\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
  }
  if (arrayDimY < blockDimY || arrayDimY % blockDimY != 0) {
    printf("array_size_Y mod block_size_Y != 0!\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
  }
  if (arrayDimZ < blockDimZ || arrayDimZ % blockDimZ != 0) {
    printf("array_size_Z mod block_size_Z != 0!\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
  }
  if (arrayDimT < blockDimT || arrayDimT % blockDimT != 0) {
    printf("array_size_T mod block_size_T != 0!\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
  }

  int num_blocks_x = arrayDimX / blockDimX;
  int num_blocks_y = arrayDimY / blockDimY;
  int num_blocks_z = arrayDimZ / blockDimZ;
  int num_blocks_t = arrayDimT / blockDimT;

  int iterations = 0, i, j, k, l;
  double error = 1.0, max_error = 0.0;

  if(myRank == 0) {
    printf("Running Jacobi on %d processors with (%d, %d, %d, %d) elements\n", numPes, num_blocks_x, num_blocks_y, num_blocks_z, num_blocks_t);
    printf("Array Dimensions: %d %d %d %d\n", arrayDimX, arrayDimY, arrayDimZ, arrayDimT);
    printf("Block Dimensions: %d %d %d %d\n", blockDimX, blockDimY, blockDimZ, blockDimT);
  }

  double data;
  for(myRank = 0; myRank < num_blocks_x*num_blocks_y*num_blocks_z*num_blocks_t; myRank++) {
    
    int myXcoord = myRank % num_blocks_x;
    int myYcoord = (myRank % (num_blocks_x * num_blocks_y)) / num_blocks_x;
    int myZcoord = (myRank % (num_blocks_x * num_blocks_y * num_blocks_z)) / (num_blocks_x * num_blocks_y);
    int myTcoord = myRank / (num_blocks_x * num_blocks_y * num_blocks_z);

#if !DO_COMM
    printf("%d %d %zd\n", myRank, calc_pe(wrap_x(myXcoord-1), myYcoord, myZcoord, myTcoord), sizeof(double)*blockDimY*blockDimZ*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(wrap_x(myXcoord+1), myYcoord, myZcoord, myTcoord), sizeof(double)*blockDimY*blockDimZ*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, wrap_y(myYcoord-1), myZcoord, myTcoord), sizeof(double)*blockDimX*blockDimZ*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, wrap_y(myYcoord+1), myZcoord, myTcoord), sizeof(double)*blockDimX*blockDimZ*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, myYcoord, wrap_z(myZcoord-1), myTcoord), sizeof(double)*blockDimX*blockDimY*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, myYcoord, wrap_z(myZcoord+1), myTcoord), sizeof(double)*blockDimX*blockDimY*blockDimT);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, myYcoord, myZcoord, wrap_t(myTcoord-1)), sizeof(double)*blockDimX*blockDimY*blockDimZ);
    printf("%d %d %zd\n", myRank, calc_pe(myXcoord, myYcoord, myZcoord, wrap_t(myTcoord+1)), sizeof(double)*blockDimX*blockDimY*blockDimZ);
#endif
    // if(myRank == 0) printf("Iteration %d %f\n", iterations, max_error);
  } /* end of while loop */

  if(myRank == 0) {
    printf("Completed %d iterations\n", iterations);
  }

  MPI_Finalize();
  return 0;
} /* end function main */

