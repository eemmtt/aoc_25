// invalid ids:
//   an ID is invalid if it is made only of some sequence of digits repeated
//   at least twice. So, 12341234 (1234 two times), 123123123 (123 three times),
//   1212121212 (12 five times), and 1111111 (1 seven times) are all invalid
//   IDs.

// gcc -g d02_2.c ../shared/aoc25.c -o pt2 -lm

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
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
  u64 id_len = decimal_places(id);

  // check all potential factors of lexical length
  for (u64 stride = 1; stride <= id_len;
       stride++) {  // there's definitly a more efficient way to find the max
                    // factor...
    if (id_len % stride != 0) {
      // bail if not cleanly divisible
      continue;
    }

    if (id_len / stride == 1) {
      // bail if we're only checking one subnum
      continue;
    }

    // turn stride into a mask for decimal processing
    i64 mask;
    pow_i64(10, stride, &mask);

    // iterate over id and split off into numbers of length Stride
    u64 curr = id;
    u64 subNums[id_len / stride];
    for (u64 i = 0; i < id_len / stride; i++) {
      subNums[i] = curr % mask;
      curr = curr / mask;
    }

    // if the subnums are all equal, the id is invalid
    int isInvalid = 1;
    for (u64 k = 1; k < id_len / stride; k++) {
      if (subNums[k] != subNums[k - 1]) {
        isInvalid = 0;
        break;
      }
    }
    // the id is inValid, return false
    if (isInvalid == 1) {
      return 0;
    }
  }

  // didn't find an inValid arrangement of subNumbers, return true
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
        printf("Found invalid id: %ld. Start: %ld, End: %ld, Sum: %ld\n", id,
               r.start, r.end, sum);
      }
    }
  }

  printf("Sum of invalid IDs: %ld\n", sum);
  exit(EXIT_SUCCESS);
}