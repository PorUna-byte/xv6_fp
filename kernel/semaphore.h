#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

#define MAX_sema 16
struct semaphore {
  struct spinlock lock;
  int count;
};