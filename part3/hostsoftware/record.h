#ifndef record_h_
#define record_h_

// These defines set the sizes of the record file!!!
#define RECORDLEN 36
#define NUMRECORDS (24 * 3)
#define RECORD_FILESIZE (NUMRECORDS * RECORDLEN)

int _records_so_far;

int update_record(char *record, unsigned char tmp, unsigned char prs,
                  unsigned char hmd, unsigned char rained,
                  char timeString[12]) {

  /*
   * Makes sure that we do not write over the length of the file
   */
  if (_records_so_far >= NUMRECORDS) {
    return -1;
  }

  /*
   *  Write out data in a CSV format
   *  and counts the number of records archived
   */

  //TODO: your code here
	// CSV means comma separated values, same format in host.c
	sprintf(record, "%d,%d,%d,%d,%s", tmp, prs, hmd, rained, timeString);

	// TODO RECORD IS NULL, MALLOC HERE OR THERE?
		// testing there
	_records_so_far++;


  return 0;
}

/*
 * construct_record is responsible for opening the file and mmaping the file
 * the file descriptor (fd) is loaded into user provided pointer
 * the address of the mapped region (buffer) is loaded into another user provided
 * pointer return value is 0 on success
 */
int construct_record(char *record_fname, int *fd, char **buffer) {
  _records_so_far = 0;
  /*
   * Open a file for reading and writing with permission granted to user
   * Use error checking!
   */
  
  //TODO: your code here
	int fdtemp = open(record_fname, O_CREAT | O_RDWR, (mode_t)0777);

	if(fdtemp == -1)
		return -1;

	*fd = fdtemp;


  /*
   * Seek to the end of the file and write a \0 to extend the file
   * Use error checking!
   */
  
  //TODO: your code here
	off_t offset = lseek(*fd, RECORD_FILESIZE, SEEK_SET);

	if(offset == -1) {
		close(*fd);
		return -1;
	}

	if(write(*fd, "", 1) == -1) {
		close(*fd);
		return -1;
	}


  /*
   * Map the proper region of the file into the user's address space
   * Use error checking!
   */
  
  //TODO: your code here

	buffer = mmap(0, RECORD_FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);

	if(buffer == MAP_FAILED) {
		close(*fd);
		return -1;
	}

  return 0;
}

int deconstruct_record(int record_fd, char *record) {
  /*
   * No need to sync the file, unless there are multiple reading processes
   * so simply un-map and close
   */
  
  //TODO: your code here
	if(munmap(&record_fd, strlen(record)) == -1) {
		close(record_fd);
		return -1;
	}

	close(record_fd);

  return 0;
}

#endif
