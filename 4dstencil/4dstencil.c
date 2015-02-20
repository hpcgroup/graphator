#include <stdio.h>
#include <stdlib.h>

/* We want to wrap entries around, and because mod operator % sometimes
 * misbehaves on negative values. -1 maps to the highest value. */
#define wrap_x(a)	(((a)+num_blocks_x)%num_blocks_x)
#define wrap_y(a)	(((a)+num_blocks_y)%num_blocks_y)
#define wrap_z(a)	(((a)+num_blocks_z)%num_blocks_z)
#define wrap_t(a)	(((a)+num_blocks_t)%num_blocks_t)

#define calc_pe(a,b,c,d)	((a)+(b)*num_blocks_x+(c)*num_blocks_x*num_blocks_y \
				+(d)*num_blocks_x*num_blocks_y*num_blocks_z)


int main(int argc, char **argv) {
  int my_rank;

  if (argc != 6) {
    printf("%s [num_blocks_x] [num_blocks_y] [num_blocks_z] [num_blocks_t] [msg_size]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int num_blocks_x = atoi(argv[1]);
  int num_blocks_y = atoi(argv[2]);
  int num_blocks_z = atoi(argv[3]);
  int num_blocks_t = atoi(argv[4]);
  int msg_size = atoi(argv[5]);

#if DEBUG
  printf("4D Stencil on %d processors with aspect ratio (%d, %d, %d, %d)\n", (num_blocks_x*num_blocks_y*num_blocks_z*num_blocks_t), num_blocks_x, num_blocks_y, num_blocks_z, num_blocks_t);
#endif

  for(my_rank = 0; my_rank < num_blocks_x*num_blocks_y*num_blocks_z*num_blocks_t; my_rank++) {
    int my_x_coord = my_rank % num_blocks_x;
    int my_y_coord = (my_rank % (num_blocks_x * num_blocks_y)) / num_blocks_x;
    int my_z_coord = (my_rank % (num_blocks_x * num_blocks_y * num_blocks_z)) / (num_blocks_x * num_blocks_y);
    int my_t_coord = my_rank / (num_blocks_x * num_blocks_y * num_blocks_z);

    printf("%d %d %zd\n", my_rank, calc_pe(wrap_x(my_x_coord-1), my_y_coord, my_z_coord, my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(wrap_x(my_x_coord+1), my_y_coord, my_z_coord, my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, wrap_y(my_y_coord-1), my_z_coord, my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, wrap_y(my_y_coord+1), my_z_coord, my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, my_y_coord, wrap_z(my_z_coord-1), my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, my_y_coord, wrap_z(my_z_coord+1), my_t_coord), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, my_y_coord, my_z_coord, wrap_t(my_t_coord-1)), msg_size);
    printf("%d %d %zd\n", my_rank, calc_pe(my_x_coord, my_y_coord, my_z_coord, wrap_t(my_t_coord+1)), msg_size);
  } /* end of for loop */

  exit(EXIT_FAILURE);
} /* end function main */

