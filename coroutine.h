#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Declares coroutine function with given name.
#define COROUTINE_DECLARE(func_name) int func_name(struct Coroutine *c)
struct Coroutine
{
	uint64_t line; // The line of code to resume from, -1 means it's done.
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

	// The function used by this coroutine.
	COROUTINE_DECLARE((*coroutine));
};

// Goes to next yield statement. Returns -1 if coroutine is finished.
inline int next(struct Coroutine *c) { return c->coroutine(c); }

// Resets coroutine to the start of its function.
inline void reset(struct Coroutine *c) { c->line = c->i64 = 0; }

// Initiates definition of coroutine, insert opening braces after this!
#define COROUTINE_DEFINE(func_name) COROUTINE_DECLARE(func_name) \
{ switch (c->line) { case 0:
// Ends definition of coroutine, place this after closing braces!
#define COROUTINE_ENDDEF default: return c->line = -1; } }

// Pauses coroutine, returns to calling function.
#define yield_return return c->line = __LINE__; case __LINE__:
// Ends coroutine early, returns to calling function.
#define yield_break return c->line = -1;

#ifdef __cplusplus
}
#endif
