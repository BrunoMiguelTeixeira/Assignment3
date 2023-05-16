/**
 * @file movies.h
 */
#ifndef MOVIES_H
#define MOVIES_H

extern volatile int up_down, select, return_credit, credit;

/**
 * \brief State machine's states enum
 */
typedef enum
{
	CREDIT = 0,
	MOVIE,
	SESSION,
	GROUP,
	TICKET
} state_t;


/**
 *  \brief State machine function
 *	State machine diagram:
 *  \dot
 *     	digraph State_Machine{
 *	   	node [shape=oval, fontname=Helvetica, fontsize=12];
		edge [shape=circle, fontname=Helvetica, fontsize=8];

 *	   	
 *	   	add_cre [ label="ADD CREDIT"];
		ret[shape="point" label=" "]

 		subgraph cluster{
			style = rounded
	   		mov [label="MOVIE"];
	   		sess[label="SESSION"];
	   		tic [label="TICKET"];
	   		group [label="GROUP"];
			his [shape="circle" label="H"];
			def[shape="point" label=" "]

			def -> mov [ arrowhead="vee" label = ""];
			mov -> sess [ arrowhead="vee" label = "Select"];
			sess -> sess [ arrowhead="vee" label = "Down"]
			mov -> mov [ arrowhead="vee" label = "Down"]
			sess -> sess [ arrowhead="vee" label = "Up"]
			mov -> mov [ arrowhead="vee" label = "Up"]
			sess -> group [ arrowhead="vee" label = "Select"]
			group -> group [ arrowhead="vee" label = "Up"]
			group -> group [ arrowhead="vee" label = "Down"]
			group-> tic [ arrowhead="vee" label = "Select"]
		}
		ret -> add_cre [arrowhead="vee" label = "Return"];
		mov -> add_cre [arrowhead="vee" label = "Credit"];
		sess -> add_cre [arrowhead="vee" label = "Credit"];
		group -> add_cre [arrowhead="vee" label = "Credit"];
		tic -> add_cre [ arrowhead="vee" label = "X"]
		add_cre -> his[label = "Select"]
 *		
 *   }
 *  \enddot
 */


void state_machine(void);

/**
 * \brief Function responsible for print menu
 * \param menu[][20] array of menu options
 * \param len_menu number of menu options
*/
void update_menu(char menu[][20], int len_menu);

/**
 * \brief Reset variables altered by the buttons
 * 
*/
void reset_buttons(void);

#endif