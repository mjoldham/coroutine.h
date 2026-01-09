#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Coroutine data structure and functions
#define COROUTINE_DECLARE(func_name) int func_name(struct Coroutine *c)
struct Coroutine
{
	uint64_t line;
	union
	{
		uint64_t i64; // max: 1.84e19 - 1
		struct
		{
			uint32_t i32, j32; // max: 4.29e9 - 1
		};
		struct
		{
			uint16_t i16, j16, k16, l16; // max: 65,535
		};
		struct
		{
			uint8_t i8, j8, k8, l8, m8, n8, o8, p8; // max: 255
		};
	};

	COROUTINE_DECLARE((*coroutine));
};

// Go to next yield statement. Returns -1 if coroutine finished.
inline int next(struct Coroutine *c) { return c->coroutine(c); }

// Reset coroutine to the start.
inline void reset(struct Coroutine *c) { c->line = c->i64 = 0; }

// Initiates definition of coroutine, followed by opening braces.
#define COROUTINE_DEFINE(func_name) COROUTINE_DECLARE(func_name) \
{ switch (c->line) { case 0:
// Ends definition of coroutine, placed after closing braces.
#define COROUTINE_ENDDEF default: return c->line = -1; } }

// Pauses coroutine, returns to calling function.
#define yield_return return c->line = __LINE__; case __LINE__:
// Ends coroutine early, returns to calling function.
#define yield_break return c->line = -1;

#ifdef __cplusplus
}
#endif

// Here's an example of how to get coroutine-ing!
// 1) Define a function using 'COROUTINE_DEFINE' and 'COROUTINE_ENDDEF'
// (and optionally declare it somewhere else using 'COROUTINE_DECLARE').
// 2) Pass the pointer to this function to a constructed 'Coroutine'.
// 3) Call 'next' on this 'Coroutine' to step through it.
// 4) If 'next' returns -1 the 'Coroutine' is finished, use 'reset'!
// 
// NB: Use the sized variables ('i64', etc.) to store mutable, persistent info.
//     Use #defines for constant, persistent info (e.g. loop counts).

//#include <stdio.h>
//#include "coroutine.h"
//
//#define STEP_THRU
//#define TEST_BREAK 1
//
//COROUTINE_DEFINE(test)
//{
//#define count 10
//	for (c->i64 = 0; c->i64 < count; c->i64++)
//	{
//		float t = (float)c->i64 / (count - 1);
//		printf("t:%f\n", t);
//		yield_return;
//	}
//
//	printf("First part done!\n");
//	yield_return;
//
//#define counti 5
//#define countj 3
//#define countk 4
//	for (c->i16 = 0; c->i16 < counti; c->i16++)
//	{
//		printf("\ni:%d\n", c->i16);
//		for (c->j16 = 0; c->j16 < countj; c->j16++)
//		{
//			printf("\n\tj:%d\n", c->j16);
//			for (c->k16 = 0; c->k16 < countk; c->k16++)
//			{
//				printf("\t\tk:%d\n", c->k16);
//				yield_return;
//			}
//		}
//	}
//
//	if (TEST_BREAK)
//	{
//		printf("I'm tired of this, end early!\n");
//		yield_break;
//	}
//
//	printf("Second part done!\n");
//	yield_return;
//
//	printf("Guess that's it!\n");
//}
//COROUTINE_ENDDEF
//
//int main()
//{
//	struct Coroutine testing = { .coroutine = &test };
//	int n = 0;
//
//#ifdef STEP_THRU
//	while (getchar())
//#else
//	while (1)
//#endif
//	{
//		int line = next(&testing);
//		printf("line: %d, count: %d\n", line, n++);
//
//		if (line == -1)
//		{
//			break;
//		}
//	}
//
//	return 0;
//}
