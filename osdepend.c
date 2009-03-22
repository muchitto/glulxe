/* osdepend.c: Glulxe platform-dependent code.
    Designed by Andrew Plotkin <erkyrath@eblong.com>
    http://www.eblong.com/zarf/glulx/index.html
*/

#include "glk.h"
#include "glulxe.h"

/* This file contains definitions for platform-dependent code. Since
   Glk takes care of I/O, this is a short list -- memory allocation
   and random numbers.

   The Makefile (or whatever) should define OS_UNIX, or some other
   symbol. Code contributions welcome. 
*/

#ifdef OS_UNIX

#include <time.h>
#include <stdlib.h>

/* Allocate a chunk of memory. */
void *glulx_malloc(glui32 len)
{
  return malloc(len);
}

/* Resize a chunk of memory. This must follow ANSI rules: if the
   size-change fails, this must return NULL, but the original chunk 
   must remain unchanged. */
void *glulx_realloc(void *ptr, glui32 len)
{
  return realloc(ptr, len);
}

/* Deallocate a chunk of memory. */
void glulx_free(void *ptr)
{
  free(ptr);
}

/* Set the random-number seed; zero means use as random a source as
   possible. */
void glulx_setrandom(glui32 seed)
{
  if (seed == 0)
    seed = time(NULL);
  srandom(seed);
}

/* Return a random number in the range 0 to 2^32-1. */
glui32 glulx_random()
{
  return random();
}

#endif /* OS_UNIX */

#ifdef OS_MAC

/* The Glk library uses malloc/free liberally, so we might as well also. */
#include <stdlib.h>

/* Allocate a chunk of memory. */
void *glulx_malloc(glui32 len)
{
  return malloc(len);
}

/* Resize a chunk of memory. This must follow ANSI rules: if the
   size-change fails, this must return NULL, but the original chunk 
   must remain unchanged. */
void *glulx_realloc(void *ptr, glui32 len)
{
  return realloc(ptr, len);
}

/* Deallocate a chunk of memory. */
void glulx_free(void *ptr)
{
  free(ptr);
}

#define COMPILE_RANDOM_CODE
static glui32 lo_random(void);
static void lo_seed_random(glui32 seed);

/* Return a random number in the range 0 to 2^32-1. */
glui32 glulx_random()
{
  return lo_random();
}

/* Set the random-number seed; zero means use as random a source as
   possible. */
void glulx_setrandom(glui32 seed)
{
  if (seed == 0)
    seed = TickCount() ^ Random();
  lo_seed_random(seed);
}

#endif /* OS_MAC */

#ifdef WIN32

#include <time.h>
#include <stdlib.h>

/* Allocate a chunk of memory. */
void *glulx_malloc(glui32 len)
{
  return malloc(len);
}

/* Resize a chunk of memory. This must follow ANSI rules: if the
   size-change fails, this must return NULL, but the original chunk 
   must remain unchanged. */
void *glulx_realloc(void *ptr, glui32 len)
{
  return realloc(ptr, len);
}

/* Deallocate a chunk of memory. */
void glulx_free(void *ptr)
{
  free(ptr);
}

/* Set the random-number seed; zero means use as random a source as
   possible. */
void glulx_setrandom(glui32 seed)
{
  if (seed == 0)
    seed = time(NULL);
  srand(seed);
}

/* Return a random number in the range 0 to 2^32-1. */
glui32 glulx_random()
{
  return rand();
}

#endif /* WIN32 */

#ifdef COMPILE_RANDOM_CODE

/* Here is a pretty standard random-number generator and seed function. */
static glui32 lo_random(void);
static void lo_seed_random(glui32 seed);
static glui32 rand_table[55]; /* State for the RNG. */
static int rand_index1, rand_index2;

static glui32 lo_random()
{
  rand_index1 = (rand_index1 + 1) % 55;
  rand_index2 = (rand_index2 + 1) % 55;
  rand_table[rand_index1] = rand_table[rand_index1] - rand_table[rand_index2];
  return rand_table[rand_index1];
}

static void lo_seed_random(glui32 seed)
{
  glui32 k = 1;
  int i, loop;

  rand_table[54] = seed;
  rand_index1 = 0;
  rand_index2 = 31;
  
  for (i = 0; i < 55; i++) {
    int ii = (21 * i) % 55;
    rand_table[ii] = k;
    k = seed - k;
    seed = rand_table[ii];
  }
  for (loop = 0; loop < 4; loop++) {
    for (i = 0; i < 55; i++)
      rand_table[i] = rand_table[i] - rand_table[ (1 + i + 30) % 55];
  }
}

#endif /* COMPILE_RANDOM_CODE */

#include <stdlib.h>

/* I'm putting a wrapper for qsort() here, in case I ever have to
   worry about a platform without it. But I am not worrying at
   present. */
void glulx_sort(void *addr, int count, int size, 
  int (*comparefunc)(void *p1, void *p2))
{
  qsort(addr, count, size, (int (*)(const void *, const void *))comparefunc);
}
