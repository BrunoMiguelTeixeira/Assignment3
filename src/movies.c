#include "movies.h"
#include <zephyr/sys/printk.h> /* for printk()*/

volatile int up_down = 0, select = 0, return_credit = 0, credit = 0;

/*State variables*/
state_t current_state = CREDIT;
state_t next_state = NULL_CREDIT;
state_t historic = MOVIE; // default value

/* Menus options */
char menu_MOVIE[2][20] = {"Movie A", "Movie B"};

char menu_SESSION_A[3][20] = {"19H00 session", "20H00 session", "23H00 session"};
int price_A[3] = {9, 12, 10};

char menu_SESSION_B[2][20] = {"19H00 session", "20H00 session"};
int price_B[2] = {10, 12};

int menu_iter = 0, start = 1, movie_sel, session_sel, wallet = 0, deduction;

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
    if (menu_iter < 0)
    {
        menu_iter = len_menu;
    }
}

void reset_buttons(void)
{
    up_down = 0;
    select = 0;
    credit = 0;
    return_credit = 0;
}

void state_machine(void)
{

    switch (current_state)
    {
    case NULL_CREDIT:
        break;
    case CREDIT:
        if (select != 0)
        {
            next_state = historic;
        }
        else if (credit != 0)
        {
            wallet += credit;
            printk("Incerted %d €       | Credit: %d\n", credit, wallet);
            next_state = CREDIT;
        }
        else
        {
            next_state = current_state;
        }
        break;
    case MOVIE:
        if (up_down != 0)
        {
            printk("Movies:             | Credit: %d\n", wallet);
            update_menu(menu_MOVIE, 2);
            next_state = MOVIE;
        }
        else if (select == 1)
        {
            printk("Movie Selected! -> %s\n", menu_MOVIE[menu_iter]);
            movie_sel = menu_iter;
            menu_iter = 0;
            next_state = SESSION;
        }
        else if (credit != 0)
        {
            historic = current_state;
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
            printk("Session:            | Credit: %d\n", wallet);
            if (movie_sel == 0)
            {
                update_menu(menu_SESSION_A, 3);
            }
            else
            {
                update_menu(menu_SESSION_B, 2);
            }
            next_state = SESSION;
        }
        else if (select == 1)
        {
            printk("Session selected ->");
            if (movie_sel == 0)
            {
                printk("%s\n", menu_SESSION_A[menu_iter]);
            }
            else
            {
                printk("%s\n", menu_SESSION_B[menu_iter]);
            }
            session_sel = menu_iter;
            menu_iter = 0;
            next_state = TICKET;
        }
        else if (credit != 0)
        {
            historic = current_state;
            next_state = CREDIT;
        }
        else
        {
            next_state = current_state;
        }
        break;
    case TICKET:
        if (movie_sel == 0)
        {
            printk("%s %s \n", menu_MOVIE[movie_sel], menu_SESSION_A[session_sel]);
            deduction = wallet - price_A[session_sel];
        }
        else
        {
            printk("%s %s\n", menu_MOVIE[movie_sel], menu_SESSION_B[session_sel]);
            deduction = wallet - price_B[session_sel];
        }

        if (deduction < 0)
        {
            printk("Insufficient credit! %d \n", deduction);
            historic = current_state;
            next_state = CREDIT;
        }
        else
        {
            printk("Ticket!\n");
            printk("Insert Credit or return\n");
            wallet = deduction;
            historic = MOVIE; // default
            next_state = CREDIT;
        }
        break;
    default:
        break;
    }
    reset_buttons();
    current_state = next_state;
}