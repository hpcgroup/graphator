/** \file 2dstencil.c
 * Copyright (c) 2014, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * Written by:
 *     Nikhil Jain <nikhil.jain@acm.org>
 *     Abhinav Bhatele <bhatele@llnl.gov>
 *
 * LLNL-CODE-678959. All rights reserved.
 *
 * This file is part of Graphator. For details, see:
 * https://github.com/LLNL/graphator
 * Please also read the LICENSE file for our notice and the LGPL.
 */

#include <stdio.h>
#include <stdlib.h>

/* We want to wrap entries around, and because mod operator % sometimes
 * misbehaves on negative values. -1 maps to the highest value. */
#define wrap_x(a)	(((a)+num_blocks_x)%num_blocks_x)
#define wrap_y(a)	(((a)+num_blocks_y)%num_blocks_y)
#define calc_pe(a,b)	((a)*num_blocks_y+(b))


int main(int argc, char **argv) {
  int my_rank;

  if (argc != 4) {
    printf("%s [num_blocks_x] [num_blocks_y] [msg_size]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int num_blocks_x = atoi(argv[1]);
  int num_blocks_y = atoi(argv[2]);
  int msg_size = atoi(argv[3]);

#if DEBUG
  printf("2D Stencil on %d processors with aspect ratio (%d, %d)\n", (num_blocks_x*num_blocks_y), num_blocks_x, num_blocks_y);
#endif

  for(my_rank = 0; my_rank < num_blocks_x*num_blocks_y; my_rank++) {
    int my_row = my_rank / num_blocks_y;
    int my_col = my_rank % num_blocks_y;

    printf("%d %d %zd\n", my_rank, calc_pe(my_row, wrap_y(my_col-1)), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_row, wrap_y(my_col+1)), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(wrap_x(my_row-1), my_col), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(wrap_x(my_row+1), my_col), msg_size);
  } /* end of for loop */

  exit(EXIT_SUCCESS);
} /* end function main */

