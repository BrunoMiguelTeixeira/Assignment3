#ifndef MOVIES_H
#define MOVIES_H

extern volatile int up_down, select, return_credit, button_touch, credit;

typedef enum{
		NULL_CREDIT = 0,
		CREDIT,
		MOVIE,
		SESSION,
		TICKET
}state_t;

void update_menu(char menu[][20], int len_menu);

void state_machine(void);

void reset_buttons(void);

#endif