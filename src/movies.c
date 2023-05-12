
#include "movies.h"
#include <zephyr/sys/printk.h>      /* for printk()*/

volatile int up_down = 0, select = 0, return_credit = 0, button_touch = 0, credit = 0;

state_t current_state = CREDIT;
state_t next_state = NULL_CREDIT;
state_t historic = MOVIE;

char menu_MOVIE[2][20] = {"Movie A", "Movie B"};

char menu_SESSION_A[3][20] = {"19H00 session","20H00 session","23H00 session"};
int price_A[3] = {9,12,10};

char menu_SESSION_B[2][20] = {"19H00 session","20H00 session"};
int price_B[2] = {10,12};

int menu_iter = 0;
int movie_selected;

void update_menu(char menu[][20], int len_menu)
{
    int i;
    for (i = 0; i < len_menu; ++i)
    {
        if (i == menu_iter)
        {
            printk("-->");
        }
        else
        {
            printk("   ");
        }
        printk("%s\n", menu[i]);
    }
    menu_iter = (menu_iter + up_down) % len_menu;
    if (menu_iter < 0) {
        menu_iter = len_menu - 1;
    }
}

void reset_buttons(void)
{
    up_down = 0;
    select = 0;
    return_credit = 0;
    button_touch = 0;
}

void state_machine(void)
{

    switch (current_state)
    {
    case NULL_CREDIT:
        break;
    case CREDIT:
        printk("Insert Credit\n");
        if (select != 0)
        {
            if (historic == MOVIE)
            {
                update_menu(menu_MOVIE, 2);
                reset_buttons();
            }
            next_state = historic;
        }
        else
        {
            next_state = CREDIT;
        }
        
        break;
    case MOVIE:
        if (up_down != 0)
        {
            printk("Movies:\n");
            update_menu(menu_MOVIE, 2);
            reset_buttons();
            next_state = MOVIE;
        }
        else if (select == 1)
        {
            printk("Movie Selected! -> %s\n",menu_MOVIE[menu_iter]);
            movie_selected = menu_iter;
            menu_iter = 0;
            reset_buttons();
            next_state = SESSION;
        }   
        else if(credit != 0)
        {
            historic = SESSION;
            next_state = CREDIT;
        }
        else
        {
            next_state = MOVIE;
        }
        break;
    case SESSION:
        if (up_down != 0)
        {
            printk("Session:%d\n",menu_iter);
            if (movie_selected == 0)
            {
                update_menu(menu_SESSION_A, 3);
            }
            else
            {
                update_menu(menu_SESSION_B,2);
            }    
            reset_buttons(); 
            next_state = SESSION;
        }
        break;
    case TICKET:
        break;
    default:
        break;
    }
    current_state = next_state;
}