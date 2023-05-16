#include "movies.h"
#include <zephyr/sys/printk.h> /* for printk()*/

volatile int up_down = 0, select = 0, return_credit = 0, credit = 0;
/*State variables*/
state_t current_state = CREDIT;
state_t next_state = MOVIE;
state_t historic = MOVIE; // default value

/* Menus options */
char menu_MOVIE[2][20] = {"Movie A", "Movie B"};

char menu_SESSION_A[3][20] = {"19H00 [9€]", "20H00 [12€]", "23H00 [10€]"};
int price_A[3] = {9, 12, 10};

char menu_SESSION_B[2][20] = {"19H00 [10€]", "20H00 [12€]"};
int price_B[2] = {10, 12};

int menu_iter = 0, first_time= 1, movie_sel, session_sel, wallet = 0, deduction, view_group = 1,button_done=0;

void update_menu(char menu[][20], int len_menu)
{
    menu_iter = (menu_iter + up_down) % len_menu;
    if (menu_iter < 0)
    {
        menu_iter = len_menu-1;
    }
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
    printk("---------------------------------\n");
}

void reset_buttons(void)
{
    up_down = 0;
    select = 0;
    credit = 0;
    return_credit = 0;
    button_done = 0;
}

void state_machine(void)
{

    switch (current_state)
    {
    case CREDIT:
    if(first_time==1)
    {
        printk("Welcome to the 5D Cinema experience!\n\nInsert Credit or proceed to movies by pressing [SELECT]!\n");
        first_time=0;
    }
        if (select != 0)
        {
            next_state = historic;
            first_time=1;
            button_done=1;
        }
        else if(return_credit!=0)
        {
            next_state=CREDIT;
            first_time=1;
            historic = MOVIE;
            if(wallet>0)
            {
                printk("Returned exchange: %d €\n",wallet);
                printk("---------------------------------\n");
            wallet=0;
            }
            else
            {
                printk("Your Credit balance is 0, nothing was returned\n");
                printk("---------------------------------\n");
            }
        button_done=1;
        }
        else if (credit != 0)
        {
            wallet += credit;
            printk("Inserted %d €       | Credit: %d €\n", credit, wallet);
            printk("---------------------------------\n");
            next_state = CREDIT;
            button_done=1;
        }
        else
        {
            next_state = current_state;
        }
        break;
    case MOVIE:
        if(first_time==1)
        {
            printk("Movies:             | Credit: %d €\n", wallet);
            update_menu(menu_MOVIE, 2);
            first_time=0;
        }
        if (up_down != 0)
        {
            printk("Movies:             | Credit: %d €\n", wallet);
            update_menu(menu_MOVIE, 2);
            next_state = MOVIE;
            button_done=1;
        }
        else if (select != 0)
        {
            printk("Movie Selected! -> %s\n", menu_MOVIE[menu_iter]);
            movie_sel = menu_iter;
            menu_iter = 0;
            next_state = SESSION;
            first_time = 1;
            button_done=1;
        }
        else if (credit != 0)
        {
            historic = current_state;       /*In case of lacking credit it comes back here later*/
            next_state = CREDIT;            /*Pass to Credit state if money is given*/
            button_done=1;
        }
        else if(return_credit !=0)
        {
            next_state = CREDIT;
            first_time = 1;
            button_done = 1;
            historic = MOVIE;
        }
        else
        {
            next_state = MOVIE;
        }
        break;
    case SESSION:
        if(first_time==1)
        {
            printk("Session[Cost]:      | Credit: %d €\n", wallet);
            if (movie_sel == 0)
            {
                update_menu(menu_SESSION_A, 3);
            }
            else
            {
                update_menu(menu_SESSION_B, 2);
            }
            first_time=0;
        }
        if (up_down != 0)
        {
            printk("Session[Cost]:      | Credit: %d €\n", wallet);
            if (movie_sel == 0)             /* When moving around to print again menu*/
            {
                update_menu(menu_SESSION_A, 3);
            }
            else
            {
                update_menu(menu_SESSION_B, 2);
            }
            next_state = SESSION;
            button_done=1;
        }
        else if (select != 0)
        {
            printk("Session selected ->");
            if (movie_sel == 0)         /* To know which Movie the user choose on the next state after picking*/
            {
                printk("%s\n", menu_SESSION_A[menu_iter]);
            }
            else
            {
                printk("%s\n", menu_SESSION_B[menu_iter]);
            }
            session_sel = menu_iter;
            menu_iter = 0;
            next_state = GROUP;
            first_time=1;
            button_done=1;
        }
        else if (credit != 0)
        {
            historic = current_state;
            next_state = CREDIT;
            button_done=1;
        }
        else if(return_credit !=0)
        {
            next_state = CREDIT;
            first_time = 1;
            button_done = 1;
            historic = MOVIE;
        }
        else
        {
            next_state = current_state;
        }
        break;
    case GROUP:
        if(first_time==1)
        {
            printk("Number of participants: %d\n",view_group);
            first_time=0;
        }
        if (up_down!=0)
        {
            view_group=view_group - up_down;
            if(view_group < 1)
            {
                view_group = 1;
            }
            printk("Number of participants: %d\n",view_group);
            button_done=1;
        }
        else if(select!=0)
        {
            next_state=TICKET;
            button_done=1;
        }
        else if(return_credit !=0)
        {
            next_state = CREDIT;
            first_time = 1;
            button_done = 1;
            historic = MOVIE;
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
            deduction = wallet - price_A[session_sel]*view_group;
        }
        else
        {
            printk("%s %s \n", menu_MOVIE[movie_sel], menu_SESSION_B[session_sel]);
            deduction = wallet - price_B[session_sel]*view_group;
        }

        if (deduction < 0)
        {
            printk("Insufficient credit!\n You are missing: %d €\n", -(deduction));
            printk("---------------------------------\n");
            historic = current_state;
            next_state = CREDIT;
            first_time=1;
        }
        else
        {
            printk("%d ticket bought! Remaining Credit: %d €\n",view_group,deduction);
            printk("\n We hope you enjoy the Movie!\n",deduction);
            printk("---------------------------------\n");
            wallet = deduction;
            historic = MOVIE; // default
            next_state = CREDIT;
            first_time=1;
            view_group=1;
        }
        break;
    default:
        break;
    }
    if(button_done==1)         /*Make sure only when a button is pressed the call to button_reset is made*/
    {
        reset_buttons();
    }
    current_state = next_state;
}