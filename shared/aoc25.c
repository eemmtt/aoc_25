#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int pow_int(int num, int exponent, int* out) {
  if (out == NULL) {
    // null param err
    return 1;
  }

  if (exponent == 0) {
    *out = 1;
    return 0;
  }

  int product = num;
  if (exponent > 0) {
    for (int i = 1; i < exponent; i++) {
      product *= num;
    }
    *out = product;
    return 0;
  } else {
    exponent *= -1;
    for (int i = 1; i < exponent; i++) {
      product *= num;
    }
    product = 1 / product;  // integer division!!
    *out = product;
    return 0;
  }
}

int string_to_int(char* str, size_t offset, size_t size, int* out) {
  if (str == NULL || out == NULL) {
    // null param err
    return 1;
  }

  if (offset + size > strlen(str)) {
    // out of bounds
    size = strlen(str) - offset;
  }

  int sum = 0;
  for (size_t i = 0; i < size; i++) {
    int digit = str[offset + i] - '0';
    int powTen;
    pow_int(10, size - i - 1, &powTen);
    sum += digit * powTen;
  }
  *out = sum;
  return 0;
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