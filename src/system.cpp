// System dependent routines
// File: system.cpp

#include "system.h"
#include <stdlib.h>

void seedRandom(unsigned int seed)
// seed for random number generator.
{
  srand(seed);
  return;
}

double seconds()
// cpu time in seconds since start of run.
{
  double secs;

  secs = (double)(clock() / 1000.0);
  return (secs);
}
