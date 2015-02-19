#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

int main(int argc, char**argv) {

  if(argc != 6) {
    printf("Correct usage: %s <numP> <minD> <maxD> <neighborhood> <msg_size_doubles>\n", argv[0]);
    return 1;
  }

  int numP = atoi(argv[1]);
  int minD = atoi(argv[2]);
  int maxD = atoi(argv[3]);
  int neighborhood = atoi(argv[4]);
  int msg = atoi(argv[5])*8;

  vector<int> degree;

  degree.resize(numP);
  
  srand(time(NULL));
  int range = maxD - minD;
  long long sum = 0;
  for(int i = 0; i < numP; i++) {
    degree[i] = minD + (rand() % range);
    sum += degree[i];
  }

  long long count = 50*sum;
  sum /= 2;
  long long bsum = sum;
  
  long long ecount = 0;
  while(sum > 0 && count > 0) {
    count--;
    int src = rand() % numP;
    if(degree[src] == 0) continue;
    int multby = 1;
    if(rand() % 2) multby = -1;
    int dst = ((src + multby*(rand() % neighborhood)) + numP) % numP;
    if(degree[dst] == 0) continue;
    if(src == dst) continue;
    degree[src]--;
    degree[dst]--;
    printf("%d %d %d\n",src,dst,msg);
    printf("%d %d %d\n",dst,src,msg);
    sum--;
    ecount++;
  }
  fprintf(stderr,"Expected edges %lld\n",bsum);
  fprintf(stderr,"Produced edges %lld\n",ecount);
}
