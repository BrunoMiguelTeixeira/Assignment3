/**
 * @file movies.h
 */
#ifndef MOVIES_H
#define MOVIES_H

extern volatile int up_down, select, return_credit, button_touch, credit;

/**
 * \brief State machine's states enum
 */
typedef enum
{
	NULL_CREDIT = 0,
	CREDIT,
	MOVIE,
	SESSION,
	TICKET
} state_t;

/**
 *  \brief State machine function
 *	State machine diagram:
 *  \dot
 *     	digraph State_Machine{
 *	   	node [shape=oval, fontname=Helvetica, fontsize=10];
		edge [shape=circle, fontname=Helvetica, fontsize=10];

 *	   	n_cre [label="NULL_CREDIT"];
 *	   	add_cre [ label="ADD CREDIT"];
		ret[shape="point" label=" "]

 		subgraph cluster{
			style = rounded
	   		mov [label="MOVIE"];
	   		sess [labe="SESSION"];
	   		tic [label="TICKET"];
			node [shape=circle, fontname=Helvetica, fontsize=10];
			his [label="H"];
			def[shape="point" label=" "]

			def -> mov [ arrowhead="vee" label = ""];
			mov -> sess [ arrowhead="vee" label = "Select"];
			sess -> sess [ arrowhead="vee" label = "Down"]
			mov -> mov [ arrowhead="vee" label = "Down"]
			sess -> sess [ arrowhead="vee" label = "Up"]
			mov -> mov [ arrowhead="vee" label = "Up"]
			sess -> tic [ arrowhead="vee" label = "Select"]
		}
		ret -> n_cre [arrowhead="vee" label = "Return"];
		mov -> add_cre [arrowhead="vee" label = "Credit"];
		sess -> add_cre [arrowhead="vee" label = "Credit"];
		tic -> add_cre [ arrowhead="vee" label = "X"]
  	   	n_cre -> add_cre [arrowhead="vee" label = "Credit"];
		add_cre -> his[label = "Select"]
 *		
 *   }
 *  \enddot
 */
void state_machine(void);

/**
 * \brief Function responsible for print menu
 * \param char[][20] menu array of menu options
 * \param int len_menu number of menu options
*/
void update_menu(char menu[][20], int len_menu);

/**
 * \brief Reset variables altered by the buttons
 * 
*/
void reset_buttons(void);

#endif