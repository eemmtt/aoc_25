#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/aoc25.h"

#define DIAL_SIZE 100
#define DIAL_START_POS 50

int main(int argc, char* argv[]) {
  // open the problem input
  char* path = "./d01_input.txt";
  int fd = Open(path, O_RDONLY);

  struct stat statBuffer;
  Fstat(fd, &statBuffer);
  off_t fileSize = statBuffer.st_size;

  printf("start program. File size: %ld\n", fileSize);

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

    // loop over characters in a line of input
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
          int full_rotations = abs(instruction) / DIAL_SIZE;
          int part_rotation = instruction % DIAL_SIZE;

          printf("ini %d:\tdialPos: %d,\tzero count: %d,\tinstruction: %d\n",
                 lineNum, dial_pos, zero_counter, instruction);

          // count each full rotation
          zero_counter += full_rotations;

          int new_dial_pos = (dial_pos + part_rotation) % DIAL_SIZE;
          if (new_dial_pos < 0) {
            new_dial_pos += DIAL_SIZE;
          }

          // check if we passed zero during partial rotation
          int wrapped = 0;
          if (part_rotation != 0 && dial_pos != 0) {
            if (sign > 0 && new_dial_pos < dial_pos) {
              // wrapped right
              zero_counter++;
              wrapped = 1;
            } else if (sign < 0 && new_dial_pos > dial_pos) {
              // wrapped left
              zero_counter++;
              wrapped = 1;
            }
          }

          // check if we landed on zero
          if (new_dial_pos == 0 && !wrapped) {
            zero_counter++;
          }

          // move to next line
          dial_pos = new_dial_pos;
          file_offset += i + 1;
          printf(
              "end %d:\tdialPos: %d,\tzero count: %d,\tnext fileOffset: %ld\n",
              lineNum, dial_pos, zero_counter, file_offset);
          break;
        }
      }
    }

    lineNum++;
  }

  printf("number of times dial passed or equaled zero: %d\n", zero_counter);
  exit(EXIT_SUCCESS);
}