#include "hooks.h"

static int resolve_hook_address(struct ftrace_hook *hook);
static void notrace ftrace_thunk(unsigned long ip, unsigned long parent_ip,
								struct ftrace_ops* ops, struct pt_regs* regs);
static int install_hook(struct ftrace_hook* hook);
static void remove_hook( struct ftrace_hook* hook);
static int install_hooks(struct ftrace_hook* hooks, size_t count);
static void remove_hooks(struct ftrace_hook* hooks, size_t count);
static int check_syscall_address(struct ftrace_hook* hook);
					
static asmlinkage long (*original_execve)(const char __user *filename,
											const char __user *const __user argc,
											const char __user *const __user envp);
static asmlinkage long new_execve(const char __user *filename,
											const char __user *const __user argc,
											const char __user *const __user envp){
	long ret;
	printk(KERN_INFO "execve(): %s %p  %p\n",filename, argc, envp);
	printk(KERN_INFO "current proc: %s pid %i\n", current->comm, current->pid);
	ret = original_execve(filename, argc, envp);
	return ret;
}
	
asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);

asmlinkage int new_write(unsigned int fd, const char __user *buf, size_t count){
	return (*original_write)(fd, buf, count);
}

asmlinkage int (*original_read)(unsigned int, const char __user *, size_t);

asmlinkage int new_read(unsigned int fd, const char __user *buf, size_t count){
	return (*original_read)(fd, buf, count);
}

static int resolve_hook_address(struct ftrace_hook *hook){
	hook->address = kallsyms_lookup_name(hook->name);

	if (!hook->address) {
		pr_debug("Unresolved symbol: %s\n", hook->name);
		return -ENOENT;
	}
#if USE_FENTRY_OFFSET
	*((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
#else
	*((unsigned long*) hook->original) = hook->address;
#endif

	return 0;
}

static void notrace ftrace_thunk(unsigned long ip, unsigned long parent_ip,
								struct ftrace_ops* ops, struct pt_regs* regs){
									
	struct ftrace_hook* hook = container_of(ops, struct ftrace_hook, ops);
	
#if USE_FENTRY_OFFSET
	regs->ip = (unsigned long) hook->function;
#else
	if (!within_module(parent_ip, THIS_MODULE))
		regs->ip = (unsigned long) hook->function;
#endif
}

static int install_hook(struct ftrace_hook* hook){
	int err;
	
	err = resolve_hook_address(hook);
	if (err)
		return err;
	
	hook->ops.func = ftrace_thunk;
	hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
	                | FTRACE_OPS_FL_RECURSION_SAFE
	                | FTRACE_OPS_FL_IPMODIFY;
	                
	err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
	if(err){
		printk(KERN_ERR "set filter ip error %d", err);
		return err;
	}
	
	err = register_ftrace_function(&hook->ops);
	if(err){
		printk(KERN_ERR "register ftrace func err %d", err);
		ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
		return err;
	}
	
	return 0;
}

static void remove_hook( struct ftrace_hook* hook){
	int err;
	
	err = unregister_ftrace_function(&hook->ops);
	if(err)
		printk(KERN_ERR "unregister ftrace func err %d", err);
	
	ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
	if(err)
		printk(KERN_ERR "set filter ip err %d", err);
}

static struct ftrace_hook hooks[] = {
	HOOK("sys_read",   new_read,   &original_read),
	HOOK("sys_write",  new_write,  &original_write),
	HOOK("sys_execve", new_execve, &original_execve),
};


static int install_hooks(struct ftrace_hook* hooks, size_t count){
	int err;
	size_t i;
	
	for (i = 0; i < count; i++) {
		err = install_hook(&hooks[i]);
		if (err)
			goto error;
	}
	return 0;

error:
	while (i != 0) {
		remove_hook(&hooks[--i]);
	}

	return err;
}

static void remove_hooks(struct ftrace_hook* hooks, size_t count){
	size_t i;
	
	for(i = 0; i < count; i++)
		remove_hook(&hooks[i]);
}

int hook_functions(void){
	int err;
	err = install_hooks(hooks, ARRAY_SIZE(hooks));
	if (err)
		return err;
	return 0;
}

void unhook_functions(void){
	remove_hooks(hooks, ARRAY_SIZE(hooks));
}

static int check_syscall_address(struct ftrace_hook* hook){
	int ret = 0;
	
	if(hook->address != kallsyms_lookup_name(hook->name)){
		printk(KERN_ERR "Possible hook! func %s cur addr %lu orig addr %lu\n",hook->name, hook->address,kallsyms_lookup_name(hook->name));
		ret = 1; 
	}
	return ret;

}

int check_syscall_addresses(){
	int i,ret;
	for (i=0; i < 2; i++)
		ret = check_syscall_address(&hooks[i]);
	return ret;
}






	
