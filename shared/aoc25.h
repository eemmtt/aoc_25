#ifndef AOC25_H
#define AOC25_H

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int pow_int(int num, int exponent, int* out);
int string_to_int(char* str, size_t offset, size_t size, int* out);

int Open(char* path, int oflags);
void Fstat(int fd, struct stat* statBuffer);
void Read(int fd, size_t fileSize, char buf[]);

#endif