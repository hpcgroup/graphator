//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
//
// Written by:
//     Nikhil Jain <nikhil.jain@acm.org>
//     Abhinav Bhatele <bhatele@llnl.gov>
//
// LLNL-CODE-678959. All rights reserved.
//
// This file is part of Graphator. For details, see:
// https://github.com/LLNL/graphator
// Please also read the LICENSE file for our notice and the LGPL.
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char**argv) {
  if(argc != 6) {
    printf("%s <num_pes> <min_nbors> <max_nbors> <distance> <msg_size>\n", argv[0]);
    return 1;
  }

  int num_pes = atoi(argv[1]);
  int min_nbors = atoi(argv[2]);
  int max_nbors = atoi(argv[3]);
  int distance = atoi(argv[4]);
  int msg_size = atoi(argv[5]);

  vector<int> degree;

  degree.resize(num_pes);
  
  srand(time(NULL));
  int range = max_nbors - min_nbors;
  long long num_edges = 0;
  for(int i = 0; i < num_pes; i++) {
    degree[i] = min_nbors + (rand() % range);
    num_edges += degree[i];
  }

  long long count = 50 * num_edges;
  num_edges /= 2;
  long long exp_num_edges = num_edges;
  
  long long gen_num_edges = 0;
  while(num_edges > 0 && count > 0) {
    count--;
    int src = rand() % num_pes;
    if(degree[src] == 0) continue;
    int multby = 1;
    if(rand() % 2) multby = -1;
    int dst = ((src + multby*(rand() % distance)) + num_pes) % num_pes;
    if(degree[dst] == 0) continue;
    if(src == dst) continue;
    degree[src]--;
    degree[dst]--;
    printf("%d %d %d\n", src, dst, msg_size);
    printf("%d %d %d\n", dst, src, msg_size);
    num_edges--;
    gen_num_edges++;
  }

#if DEBUG
  printf("Expected edges %lld\n", exp_num_edges);
  printf("Produced edges %lld\n", gen_num_edges);
#endif
}
