# coroutine.h

Basic coroutine functionality implemented with C macros in a dinky single header file!
Sort of like a flexible [Duff's device](https://en.wikipedia.org/wiki/Duff%27s_device).



The basic steps are:

 	1) Define a function using 'COROUTINE_DEFINE' and 'COROUTINE_ENDDEF'
 	   (and optionally declare it somewhere else using 'COROUTINE_DECLARE').

 	2) Pass the pointer to this function to a constructed 'Coroutine'.

 	3) Call 'next' on this 'Coroutine' to step through it.

 	4) If 'next' returns -1 the 'Coroutine' is finished, use 'reset' to start over.
 



NB: Use the sized variables ('i64', etc.) to store mutable, persistent info.
    Use #defines for constant, persistent info (e.g. loop counts).




Below is an example of how to get coroutine-ing!



```

#include <stdio.h>
#include "coroutine.h"

#define STEP_THRU
#define TEST_BREAK 1

// You can place this in a header file...
COROUTINE_DECLARE(test);

// ... and put this in a source file.
COROUTINE_DEFINE(test)
{
#define count 10
	for (c->i64 = 0; c->i64 < count; c->i64++)
 	{
		float t = (float)c->i64 / (count - 1);
 		printf("t:%f\n", t);
 		yield_return;
 	}

 	printf("First part done!\n");
 	yield_return;

#define counti 5
#define countj 3
#define countk 4

 	for (c->i16 = 0; c->i16 < counti; c->i16++)
 	{
 		printf("\ni:%d\n", c->i16);
 		for (c->j16 = 0; c->j16 < countj; c->j16++)
 		{
 			printf("\n\tj:%d\n", c->j16);
 			for (c->k16 = 0; c->k16 < countk; c->k16++)
 			{
 				printf("\t\tk:%d\n", c->k16);
 				yield_return;
 			}
 		}
 	}

 	if (TEST_BREAK)
 	{
 		printf("I'm tired of this, end early!\n");
 		yield_break;
 	}

 	printf("Second part done!\n");
 	yield_return;

 	printf("Guess that's it!\n");

}
COROUTINE_ENDDEF

int main()
{
 	struct Coroutine testing = { .coroutine = &test };
 	int n = 0;

#ifdef STEP_THRU
 	while (getchar())
#else
 	while (1)
#endif
 	{
 		int line = next(&testing);
 		printf("line: %d, count: %d\n", line, n++);

 		if (line == -1)
 		{
 			break;
 		}
 	}

 	return 0;
}

```

