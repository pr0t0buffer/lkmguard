#include "integrity.h"

static int start_integrity_checks_timer(void);
static int stop_integrity_checks_timer(void);
static void check_syscall_table(struct timer_list *timer);

static struct timer_list integrity_timer;
extern lkmguard_settings* current_settings;

/* 	Timer section
 *  	we have local methods and 2 exported - start/stop
 */
static int start_integrity_checks_timer(){
	int ret;
	
	printk(KERN_INFO "Starting timer");
	timer_setup(&integrity_timer, check_syscall_table, 0);
	ret = mod_timer(&integrity_timer, jiffies + CHECK_INTERVAL);
	
	if(ret)
		printk(KERN_ERR "Error in timer mod");
	
	return ret;
}

static int stop_integrity_checks_timer(){
	int ret;
	
	printk(KERN_INFO "Stop timer");
	ret = del_timer(&integrity_timer);
	if(ret)
		printk(KERN_ERR "Error in deleting timer");
		
	return ret;
}

static void check_syscall_table(struct timer_list *timer){
	int ret;
	
	printk(KERN_INFO "Checking syscall table");
	ret = check_syscall_addresses();
	if(ret)
		printk(KERN_ERR "Syscall hooked!");
	ret = mod_timer(&integrity_timer, jiffies + CHECK_INTERVAL);
	if(ret)
		printk(KERN_ERR "Error in timer mod");
}


int start_integrity_checks(){

	int ret = 0;
	printk(KERN_INFO "Start integrity checks");
	ret = hook_functions();
	if(ret)
		printk(KERN_INFO "Hook not ok");
	else{
		printk(KERN_INFO "Hooked!");
		current_settings->hooks_flag = TRUE;
		start_integrity_checks_timer();
	}
	return ret;
}


int stop_integrity_checks(){
	
	printk(KERN_INFO "Stopping integrity timer, current state is %lu", current_settings->integrity_checks_flag);
	stop_integrity_checks_timer();
	
	if(current_settings->hooks_flag == TRUE){
		current_settings->hooks_flag = FALSE;
		unhook_functions();
	}
	
	return 0;
}


