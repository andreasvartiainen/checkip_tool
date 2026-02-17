#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>

#define TEST(CASE, MSG) \
	do {\
	if (!(CASE)) {\
		failed_tests++;\
		printf("\033[0;31m");\
	}\
	printf("%s => %s\n", MSG, (CASE) ? "Pass" : "\033[1;31mFail\033[0;0m");\
	total_tests++;\
	} while (0)

#define BEGIN_TESTS\
		unsigned int failed_tests = 0;\
		unsigned int total_tests = 0;\
		printf("\n");

#define END_TESTS\
	if (failed_tests != 0) {\
		printf("\nTests/Failed: %d/\033[1;31m%d\033[0;0m\n", total_tests, failed_tests);\
		return 1;\
	}\
	return 0;

#endif
