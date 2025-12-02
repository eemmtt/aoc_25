#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/aoc25.h"

#define DIAL_SIZE 100
#define DIAL_START_POS 50

int main(int argc, char* argv[]) {
  printf("start program\n");

  // open the problem input
  char* path = "./d01_input.txt";
  int fd = Open(path, O_RDONLY);

  struct stat statBuffer;
  Fstat(fd, &statBuffer);
  off_t fileSize = statBuffer.st_size;

  // read file into buffer
  char readBuffer[fileSize];
  Read(fd, fileSize, readBuffer);

  // step through buffer and parse instructions
  size_t file_offset = 0;
  int dial_pos = DIAL_START_POS;
  int zero_counter = 0;
  int lineNum = 0;  // for debugging
  while (file_offset < fileSize && lineNum < 5000) {
    int sign = 1;
    int num_size = 0;
    for (size_t i = 0; i <= (fileSize - file_offset); i++) {
      int curr_offset = file_offset + i;
      // first char sets direction
      if (i == 0) {
        if (readBuffer[curr_offset] == 'L') {
          sign *= -1;
          continue;
        } else if (readBuffer[curr_offset] != 'R') {
          fprintf(stderr,
                  "encountered unrecognized character '%c' at "
                  "fileoffset %d",
                  readBuffer[curr_offset], curr_offset);
          exit(EXIT_FAILURE);
        }
      } else  // i > 0
      {
        // not at end of line and not at end of file
        if (readBuffer[curr_offset] != '\n' && curr_offset < fileSize) {
          num_size++;
          continue;
        }

        // at end of line, or end of file
        if (readBuffer[curr_offset] == '\n' || curr_offset == fileSize) {
          int num;
          string_to_int(readBuffer, file_offset + 1, num_size, &num);
          int instruction = num * sign;
          dial_pos = (dial_pos + instruction) % DIAL_SIZE;
          if (dial_pos == 0) {
            zero_counter++;
          }

          // move to next line
          file_offset += i + 1;
          break;
        }
      }
    }

    lineNum++;
  }

  printf("number of times dial reached zero: %d\n", zero_counter);
  exit(EXIT_SUCCESS);
}