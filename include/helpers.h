#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// nice typedefs for different types
// unsigned
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
// signed
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
// floats
typedef float f32;
typedef double f64;
typedef long double f128;

// own defer macro for making defer a bit cleaner
#define defer __attribute__((__cleanup__(cleanup_free)))

// cleanup function for "all" data types
static inline void cleanup_free(void *data) {
	free(*(void**) data);
}

static clock_t s_benchmark_time = 0;
static double s_benchmark_total = 0;

// set current clock to the time variable
static inline void benchmark_start() {
	s_benchmark_time = clock();
}

// print elapsed and total time
static inline void benchmark_end() {
	double elapsed_time = (double)(clock() - s_benchmark_time) / CLOCKS_PER_SEC;
	s_benchmark_total += elapsed_time;
	printf("time elapsed: %fs time total: %fs\n", elapsed_time, s_benchmark_total);
}

// clear the benchmark timers
static inline void benchmark_clear() {
	s_benchmark_time = 0;
	s_benchmark_total = 0;
}

static inline void strip_newline(char *line) {
	if (line[strlen(line) - 1] == '\n') {
		line[strlen(line) - 1] = '\0';
	}
}

#endif
