#ifndef hist_h_
#define hist_h_
#include <fcntl.h>
#include <sys/mman.h>

#define NBUCKETS (256 / 16)
#define FILESIZE (24 * NBUCKETS)

int update_hist(char *hist, unsigned char value, int time) {
  /*
   * the hist file is treated as a two dimension array of characters
   * each of these characters represents how many observations of that bucket
   * have been made the rows in this two dimensional array represent each hour
   * 0-23, while each byte in the row represents the number of observations for
   * that bucket.  There are 16 buckets, which store observation for a range of
   * size 16.  For instance, the 0th bucket stores observation 0-15
   */
  int index = (time * NBUCKETS) + (value / 16);
  hist[index]++;
  return 0;
}

int print_hist(char *hist) {
  printf("Hour|   16   32   48   64   80   96  112  128  114  160  176  192  "
         "208  224  240  256\n");
  printf("---------------------------------------------------------------------"
         "----------------\n");
  for (int t = 0; t < 24; t++) {
    printf("% 3d |", t);
    for (int b = 0; b < NBUCKETS; b++) {
      printf("  % 3d", (unsigned char)hist[t * NBUCKETS + b]);
    }
    printf("\n");
  }
  printf("    "
         "---------------------------------------------------------------------"
         "-----------\n\n");

  return 0;
}

/*
 * construct_hist is responsible for opening the file and mmaping the file
 * reports a file descriptor by filling a user provided pointer
 * reports the mapped region address by filling another user provided
 * pointer return value is 0 on success
 */
int construct_hist(char *hist_fname, int *fd, char **buffer) {
  /*
   * Open a file for reading and writing with permission granted to user
   */
  *fd = open(hist_fname, O_CREAT | O_RDWR, (mode_t)0600);
  if (*fd < 0) {
    perror("issue opening");
    return -1;
  }

  /*
   * Seek to the end of the file and write a \0 to extend the file
   */
  if (-1 == lseek(*fd, FILESIZE, SEEK_SET)) {
    perror("issue with lseek");
    return -1;
  }
  int result = write(*fd, "", 1);
  if (result != 1) {
    perror("issue extending file");
    return -1;
  }

  /*
   * Map the proper region of the file into the user's address apce
   */
  *buffer =
      (char *)mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
  if (*buffer == MAP_FAILED) {
    perror("Issue mmapping hist file");
    return -1;
  }
  return 0;
}

int deconstruct_hist(int hist_fd, char *hist) {

  /*
   * No need to sync the file, unless there are multiple reading processes
   */
  munmap(hist, FILESIZE);
  close(hist_fd);
  return 0;
}
#endif
