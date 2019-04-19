#ifndef HOOKS_HDRS
#define HOOKS_HDRS
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/preempt.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/linkage.h>
#include <linux/version.h>
#include <linux/module.h>
#include <asm/bug.h>

#endif
#define WP_BIT (1<<16)

//detet recursion using function return address
#define USE_FENTRY_OFFSET 0

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

int hook_functions(void);
void unhook_functions(void);
int check_syscall_addresses(void);

struct ftrace_hook {
	const char *name;
	void *function;
	void *original;

	unsigned long address;
	struct ftrace_ops ops;
};

#ifdef PTREGS_SYSCALL_STUBS
#define SYSCALL_NAME(name) ("__x64_" name)
#else
#define SYSCALL_NAME(name) (name)
#endif

#define HOOK(_name, _function, _original)	\
	{										\
		.name = SYSCALL_NAME(_name),		\
		.function = (_function),			\
		.original = (_original),			\
	}





