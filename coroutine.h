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
