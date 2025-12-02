#ifndef AOC25_H
#define AOC25_H

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define i64 int64_t
#define i32 int32_t
#define i16 int16_t

#define f32 float
#define f64 double

i64 pow_i64(i64 num, i64 exponent, i64* out);
int char_to_int(char c);
int str_to_int(char* str);
u64 decimal_places(u64 val);
int reverse_uint(int val);

int u64_to_str(u64 value, u64 valueLen, char* out);
int substr_to_u64(char* str, size_t offset, size_t size, u64* out);
int slice(char* source, u64 sourceLen, int offset, int size, char* dest);

int Open(char* path, int oflags);
void Fstat(int fd, struct stat* statBuffer);
void Read(int fd, size_t fileSize, char buf[]);

#endif