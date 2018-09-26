#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char STR[10000];
  int LAST;
  int NEWV;
  int DIF;
  /* TODO: Add more members if necessary */


} WP;

#endif
