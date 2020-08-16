#include "types.h"
//assignment2//
struct sigaction {
  void (*sa_handler)(int);
  uint sigmask;
};