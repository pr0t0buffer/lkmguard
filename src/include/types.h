typedef unsigned long bool_t;

#define TRUE 1
#define FALSE 0

typedef struct lkmguard_settings {
	bool_t integrity_checks_flag;	
	bool_t hooks_flag;
	bool_t keylogger_flag;
} lkmguard_settings;
