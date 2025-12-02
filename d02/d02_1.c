// invalid ids:
//   - made of only repeated digits (55, 6464, 123123)
//   - leading 0 (0101)
//  find each invalid id in each listed range
//  return the sum of all invalid id's

#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>

#include "../shared/aoc25.h"

typedef struct range_struct {
  u64 start;
  u64 end;
  u64 dist;
} Range;

int range_init(Range* r, u64 start, u64 end) {
  if (r == NULL) {
    // null param
    return 1;
  }

  if (end < start) {
    // invalid range
    return 2;
  }

  r->start = start;
  r->end = end;
  r->dist = end - start;
  return 0;
}

int is_valid_id(u64 id) {
  int id_len = decimal_places(id);
  if (id_len % 2 != 0) {
    // can't be invalid if it ain't symmetrical
    return 1;
  }
  i64 halfSize;
  pow_i64(10, id_len / 2, &halfSize);
  u64 left = id / (u64)halfSize;
  u64 right = id % (u64)halfSize;
  if (left == right) {
    // invalid!!!
    return 0;
  }
  return 1;
}

void main(void) {
  char* path = "d02_input.txt";
  int fd = Open(path, O_RDONLY);

  struct stat fileStat;
  Fstat(fd, &fileStat);
  u64 fileSize = fileStat.st_size;

  char readBuffer[fileSize];
  Read(fd, fileSize, readBuffer);

  // get range count
  u64 rangeCount = 0;
  for (u64 i = 0; i < fileSize; i++) {
    if (readBuffer[i] == '-') {
      rangeCount++;
    }
  }

  // parse readBuffer into Ranges
  Range rangeArr[rangeCount];
  int startLen = 0;
  u64 startOffset = 0;
  int endLen = 0;
  u64 endOffset = 0;
  int isStart = 1;
  u64 rangeIdx = 0;

  for (u64 j = 0; j < fileSize; j++) {
    char curr = readBuffer[j];
    // printf("parsing: idx=%ld, curr=%c\n", j, curr);
    if (curr == '-') {
      // start reading end number
      isStart = 0;
      endLen = 0;

    } else if (curr == ',' || (j == fileSize - 1 && isdigit(curr))) {
      if (j == fileSize - 1 && isdigit(curr)) {
        // if end of file, endLen is off by one...
        endLen++;
      }
      u64 start, end;
      substr_to_u64(readBuffer, startOffset, startLen, &start);
      substr_to_u64(readBuffer, endOffset, endLen, &end);

      Range newRange;
      range_init(&newRange, start, end);
      rangeArr[rangeIdx] = newRange;

      // start reading next start number
      isStart = 1;
      startLen = 0;

      // increment range idx
      rangeIdx++;

    } else if (isdigit(curr)) {
      if (isStart) {
        if (startLen == 0) {
          startOffset = j;
        }
        startLen++;

      } else {
        if (endLen == 0) {
          endOffset = j;
        }
        endLen++;
      }
    }
  }

  // check each range for invalid IDs
  u64 sum = 0;
  for (int k = 0; k < rangeCount; k++) {
    Range r = rangeArr[k];
    printf("range %d: %ld-%ld, dist:%ld\n", k, r.start, r.end, r.dist);
    for (u64 m = 0; m <= r.dist; m++) {  // inclusive of end
      u64 id = r.start + m;
      if (!is_valid_id(id)) {
        sum += id;
        // printf("Found invalid id: %ld. Start: %ld, End: %ld, Sum: %ld\n", id,
        // r.start, r.end, sum);
      }
    }
  }

  printf("Sum of invalid IDs: %ld\n", sum);
  exit(EXIT_SUCCESS);
}