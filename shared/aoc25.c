#include "../shared/aoc25.h"

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

i64 pow_i64(i64 num, i64 exponent, i64* out) {
  if (out == NULL) {
    // null param err
    return 1;
  }

  if (exponent == 0) {
    *out = 1;
    return 0;
  }

  i64 product = num;
  if (exponent > 0) {
    for (i64 i = 1; i < exponent; i++) {
      product *= num;
    }
    *out = product;
    return 0;
  } else {
    exponent *= -1;
    for (i64 i = 1; i < exponent; i++) {
      product *= num;
    }
    product = 1 / product;  // integer division!!
    *out = product;
    return 0;
  }
}

int substr_to_u64(char* str, size_t offset, size_t size, u64* out) {
  if (str == NULL || out == NULL) {
    // null param err
    return 1;
  }

  if (offset + size > strlen(str)) {
    // out of bounds
    size = strlen(str) - offset;
  }

  u64 sum = 0;
  for (size_t i = 0; i < size; i++) {
    int digit = str[offset + i] - '0';
    i64 powTen;
    pow_i64(10, size - i - 1, &powTen);
    sum += digit * (u64)powTen;
  }
  *out = sum;
  return 0;
}

int str_to_int(char* str) {
  size_t size = strlen(str);
  i64 sum = 0;
  for (size_t i = 0; i < size; i++) {
    int digit = str[i] - '0';
    i64 powTen;
    pow_i64(10, size - i - 1, &powTen);
    sum += digit * powTen;
  }
  return (int)sum;
}

int u64_to_str(u64 value, u64 valueLen, char out[]) {
  if (out == NULL) {
    // null err
    return 1;
  }
  if (strlen(out) != valueLen + 1) {
    // out is not the correct size
    return 2;
  }
  char result[valueLen + 1];
  result[valueLen] = '\0';
  u64 currValue = value;
  for (int i = valueLen - 1; i >= 0; i--) {
    u64 rightNum = currValue % 10;
    result[i] = rightNum;
    currValue = currValue / 10;
  }
  *out = *result;
  return 0;
}

int char_to_int(char c) { return (c - '0'); }
int slice(char* source, u64 sourceLen, int offset, int size, char* dest) {
  if (source == NULL || dest == NULL) {
    return 1;
  }

  if (size < 0 || offset < 0) {
    return 2;
  }

  if (size == 0 || offset >= sourceLen) {
    dest[0] = '\0';
    return 0;
  }

  // clamp
  if (offset + size > sourceLen) {
    size = sourceLen - offset;
  }

  memcpy(dest, source + offset, size);
  dest[size] = '\0';
  return 0;
}

u64 decimal_places(u64 val) {
  u64 curr_val = val;
  int counter = 0;
  while (curr_val != 0) {
    curr_val = curr_val / 10;
    counter++;
  }
  return counter;
}

int reverse_uint(int val) {
  int left, right;
  i64 rightRaised;
  int valLen = decimal_places(val);
  int curr = val;
  for (int i = 0; i < valLen; i++) {
    left = curr / 10;
    right = curr % 10;
    pow_i64(right, valLen, &rightRaised);
    curr = (int)rightRaised + left;
  }
  return curr;
}
/* file io */

int Open(char* path, int oflags) {
  int fd = open(path, oflags, 0);
  if (fd == -1) {
    perror("Open() failed with err:");
    exit(EXIT_FAILURE);
  }
  return fd;
}

void Fstat(int fd, struct stat* statBuffer) {
  int rc = fstat(fd, statBuffer);
  if (rc != 0) {
    perror("Fstat() failed with err:");
    exit(EXIT_FAILURE);
  }
};

void Read(int fd, size_t fileSize, char buf[]) {
  char* bufferPos = buf;
  size_t bytesRead = 0;
  while (bytesRead < fileSize) {
    int nbytes = read(fd, bufferPos, fileSize - bytesRead);
    if (nbytes == -1) {
      perror("Read() failed with err:");
      exit(EXIT_FAILURE);
    } else if (nbytes == 0) {
      break;
    }
    bytesRead += nbytes;
    bufferPos += nbytes;
  }
}