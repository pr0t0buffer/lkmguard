#ifndef INTEGRITY_HDRS
#define INTEGRITY_HDRS
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/unistd.h>
#include "hooks.h"
#include "../include/types.h"
#endif

#define NSEC_PER_SEC 1000000000L
#define CHECK_INTERVAL msecs_to_jiffies(15 * 1000) // seconds

int start_integrity_checks(void);
int stop_integrity_checks(void);



	
