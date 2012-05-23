/*
 * ir_code_generation.c
 *
 *  Created on: Apr 3, 2012
 *      Author: Fabian Schaefer
 */

#include "ir_code_generation.h"
#include "include/utlist.h"
#include "symboltable.h"
#include "parser.tab.h"
#include "stdio.h"
#include "stdlib.h"

int num_of_codes;

//Initialize
ir_code *code_list = NULL;
ir_code *truelist = NULL;
ir_code *falselist = NULL;

void genQuad(enum code_ops *operation, struct symbol *address_0,
		struct symbol *address_1, struct symbol *address_2, int jmpTo) {
	/**Quadrupel Code Generation**/
	ir_code *c = NULL;
	c = (struct ir_code*) malloc(sizeof(struct ir_code));
	c->code.counter_id = num_of_codes;
	c->code.operation = operation;
	c->code.address_0 = address_0;
	c->code.address_1 = address_1;
	c->code.address_2 = address_2;
	c->code.jmpTo = jmpTo;
	LL_APPEND(code_list,c);
	num_of_codes++;
}

void print_all_codes(){
	ir_code *c = NULL;
	LL_FOREACH(code_list,c);{
		printf("code no. %d\n",c->code.counter_id);
	}
}

void addReturn()
{
	//???
}
void addAssign(enum code_ops *op, struct symbol *var, struct symbol *value)
{
	genQuad(op,var,value,NULL,NULL);
}
void addCalc(enum code_ops *op, struct symbol *result, struct symbol *exp1,
		struct symbol *exp2)
{
	genQuad(op, result, exp1, exp2,NULL);
}
void addWhile(enum code_ops *op, struct symbol *exp1, struct symbol *exp2)
{
	/**After each loop iteration, the GOTO label points to the row in before the while-header and
	 * the while condition is checked again...
	 * In case of a false while condition the row after the while loop will be aimed
	 * So for the truelist entry, the GOTO label at the end of the while loop is needed
	 */
	falselist->FALSELIST.row_id = code_list->code.counter_id;
	genQuad(op, exp1, exp2, NULL, NULL);
}
void addWhileDo(enum code_ops *op, struct symbol *exp1, struct symbol *exp2);
void addIf(enum code_ops *op, struct symbol *exp1, struct symbol *exp2)
{
	/**While calling the addif function a jmpto parameter is not necessary, because it would be always NULL for the first time**/
	//save rows for jumps in order to backpatch the right jump address later
	truelist->TRUELIST.row_id = code_list->code.counter_id;
	falselist->FALSELIST.row_id = code_list->code.counter_id+1;
	genQuad(op, exp1, exp2, NULL,NULL);
}

void addGoTo();
void addFunc(enum code_ops *op, struct symbol *type, struct symbol *name,
		int countParam);

void mergeBLists ()
{
	/**This function handles multiple if-operations and merges therefore true-/falselists**/
}
void backpatch (int *marker, int check)
{
	/** marker is real jump address; check decides for true-/falselist --> 1:true; 0:false
	 * At first one searches the row in the ir_code by using the true-/falselists.
	 * Afterwards the real jump address is filled in...
	**/
	//TODO: consideration of mergeBLists
	if(check == 1)
	{
		//TODO: getting right row of truelist
		//TODO: putting real jump address to ir_code
	}
	else
	{

	}


}
