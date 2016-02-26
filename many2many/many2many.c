/** \file many2many.c
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

#define calc_pe(a,b,c)	((a)+(b)*num_blocks_x+(c)*num_blocks_x*num_blocks_y)

int main(int argc, char **argv)
{
  int my_rank, ycoord;

  if(argc != 5) {
    printf("%s [num_blocks_x] [num_blocks_y] [num_blocks_z] [msg_size]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int num_blocks_x = atoi(argv[1]);
  int num_blocks_y = atoi(argv[2]);
  int num_blocks_z = atoi(argv[3]);
  int msg_size = atoi(argv[4]);
 
  for(my_rank = 0; my_rank < num_blocks_x*num_blocks_y*num_blocks_z; my_rank++) {
    int my_x_coord = my_rank % num_blocks_x;
    int my_y_coord = (my_rank % (num_blocks_x * num_blocks_y)) / num_blocks_x;
    int my_z_coord = (my_rank % (num_blocks_x * num_blocks_y * num_blocks_z)) / (num_blocks_x * num_blocks_y);
    for(ycoord = 0; ycoord < num_blocks_y; ycoord++) {
      if(ycoord == my_y_coord) continue;
      printf("%d %d %d\n", my_rank, calc_pe(my_x_coord,ycoord,my_z_coord), msg_size);
    }
  }

  exit(EXIT_SUCCESS);
}
