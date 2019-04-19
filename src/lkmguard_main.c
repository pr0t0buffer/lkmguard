#include "lkmguard_main.h"

unsigned int log_level = 1;
static int integrity = 0;
static int keylogger = 0;
lkmguard_settings* current_settings;

static int __init lkmguard_init(void){
	int ret;
	printk(KERN_INFO "Welcome and thank you for installing this!\n");
	printk(KERN_INFO "Anyway, i think it is not good idea to use unknown lkm's from unknown sources.\n");
	printk(KERN_INFO "But, thank you for your confidence.\n");
	printk("Kernel version %s\n", utsname()->version);
	printk("Kernel release %s\n", utsname()->release);
	
	// Init settings 
	current_settings = kmalloc(sizeof(struct lkmguard_settings), GFP_KERNEL);
	 
	//Integrity part
	if(integrity != 0){	
		printk(KERN_INFO "Starting integrity module\n");
		current_settings->integrity_checks_flag = TRUE;
		ret = start_integrity_checks();
		if(ret)
			printk(KERN_ERR "Unable to start intergrity checks");
	}
	
	if(keylogger != 0){	
		printk(KERN_INFO "Starting keylogger module\n");
		current_settings->keylogger_flag = TRUE;
		start_keylogger();
	}

	return 0;
}

static void __exit lkmguard_exit(void){
	
	if(current_settings->integrity_checks_flag == TRUE){
		current_settings->integrity_checks_flag = FALSE;
		stop_integrity_checks();
	}
	
	if(current_settings->keylogger_flag == TRUE){
		current_settings->keylogger_flag = FALSE;
		stop_keylogger();
	}	
		
	
	
	
	printk(KERN_INFO "Oh no(( bye!");
}

module_param(log_level, uint, 0644);
MODULE_PARM_DESC(log_level, "Log level (1 by default)");

module_param(integrity, uint, 0644);
MODULE_PARM_DESC(input_syscall_table_addr, "Integrity setting: should we run hooking and timer - 1 for yes");

module_param(keylogger, uint, 0644);
MODULE_PARM_DESC(keylogger, "Keylogger settings: should we track keyboard input - 1 for yes");

module_init(lkmguard_init);
module_exit(lkmguard_exit);

MODULE_LICENSE("GPL\0Proprietary");
MODULE_AUTHOR("pr0t0buffer <galaktion@pm.me>");
MODULE_DESCRIPTION("LKMGuard - full pack lkm with dlp and sort-of-av functionality. Made for fun");
MODULE_VERSION("0.0.1");


