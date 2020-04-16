#ifndef hist_h_
#define hist_h_
#include <fcntl.h>
#include <sys/mman.h>

// These defines set the sizes of the histograms!!!
#define NBUCKETS (256 / 16)
#define FILESIZE (24 * NBUCKETS)

int update_hist(char *hist, unsigned char value, int time) {
  /*
   * the hist file is treated as a two dimension array of characters
   * each of these characters represents how many observations of that bucket have been made
   * the rows in this two dimensional array represent each hour 0-23, while each byte in the row
   * represents the number of observations for that bucket.  There are 16 buckets, which store
   * observation for a range of size 16.  For instance, the 0th bucket stores observation 0-15
   */
  
  //TODO: your code here
	hist[time * NBUCKETS + (value/16)]++;

  return 0;
}

/*
 * Prints out the histogram. Nothing to do here (but study the code!)
 */
int print_hist(char *hist) {
  printf("Hour|   16   32   48   64   80   96  112  128  114  160  176  192  208  224  240  256\n");
  printf("-------------------------------------------------------------------------------------\n");
  for (int t = 0; t < 24; t++) {
    printf("% 3d |", t);
    for (int b = 0; b < NBUCKETS; b++) {
      printf("  % 3d", (unsigned char)hist[t * NBUCKETS + b]);
    }
    printf("\n");
  }
  printf("    --------------------------------------------------------------------------------\n\n");

  return 0;
}

/*
 * construct_hist is responsible for opening the file and mmaping the file
 * reports a file descriptor (fd) by filling a user provided pointer
 * reports the mapped region address (buffer) by filling another user provided pointer
 * return value is 0 on success
 */
int construct_hist(char *hist_fname, int *fd, char **buffer) {
  /*
   * Open a file for reading and writing with permission granted to user
   * Use error checking!
   */
  
  // TODO: Your code here
  	int fdtemp = open(hist_fname, O_CREAT | O_RDWR, (mode_t)0777);

	if(fdtemp == -1) {
		perror("Error");
		return -1; // error
	}

	*fd = fdtemp;
  
  /*
   * Seek to the end of the file and write a \0 to extend the file
   * use error checking!
   */
  
  //TODO: your code here
	off_t offset = lseek(*fd, FILESIZE, SEEK_SET);

	if(offset == -1) {
		close(*fd);
		return -1; // error couldn't stretch file
	}

	if(write(*fd, "", 1) == -1) {
		close(*fd);
		return -1; // error writing to last byte of the file
	}


  /*
   * Map the proper region of the file into the user's address space
   * Use error checking!
   */
  
  //TODO: your code here
	*buffer = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);

	if(*buffer == MAP_FAILED) {
		close(*fd);
		return -1; // error mmaping the file
	}

	return 0;
}

int deconstruct_hist(int hist_fd, char *hist) {

  /*
   * No need to sync the file, unless there are multiple reading processes
   * so simply un-map and close
   */
  
  //TODO: your code here
	if (munmap(&hist_fd, strlen(hist)) == -1) {
		close(hist_fd);
		return -1;
	}

	close(hist_fd);
  return 0;
}
#endif
