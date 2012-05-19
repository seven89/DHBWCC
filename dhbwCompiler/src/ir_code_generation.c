/*
 * ir_code_generation.c
 *
 *  Created on: Apr 3, 2012
 *      Author: FS
 */

#include "ir_code_generation.h"
#include "include/utlist.h"
#include "symboltable.h"
#include "parser.tab.h"
#include "stdio.h"
#include "stdlib.h"

int num_of_codes;

code *code_list = NULL;

void append_code(enum code_ops operation, struct symbol *address_0,
		struct symbol *address_1, struct symbol *address_2, int jmpTo) {
	code *c = NULL;
	c = (struct code*) malloc(sizeof(struct code));
	c->counter_id = num_of_codes;
	c->operation = operation;
	c->address_0 = address_0;
	c->address_1 = address_1;
	c->address_2 = address_2;
	c->jmpTo = jmpTo;
	LL_APPEND(code_list,c);
	num_of_codes++;
}

void print_all_codes(){
	code *c = NULL;
	LL_FOREACH(code_list,c){
		printf("code no. %d\n",c->counter_id);
	}
}

//
////temporary array; dimension is given by count
//symbol irtemp(int count) {
//	int size = count;
//	struct symbol *temp;
//	temp = (symbol *) malloc(size * sizeof(symbol));
//	if (temp == NULL) {
//		printf("Error with malloc...\n");
//		//TODO: further steps in case error
//	}
//	return *temp;
//}
////zwischenvariable im parser nach oben geben
//
//void genQuad(enum code_ops *op, struct symbol *result, struct symbol *exp1,
//		struct symbol *exp2, struct symbol *exp3, int label, int count) {
//	//TODO: Changes to symboltable and error correction of array-assignments
//	//dynamic array allocation
//	//changes necessary to symboltable ???
//	*tempArray = irtemp(count);
//	//Building quadrupelcode: 1:result; 2:op; 3:operand; 4:operand
//	//tempArray[0] = &(symbol)result;
//	//tempArray[1] = (symbol)op;
//	//tempArray[2] = (symbol)exp1;
//	//tempArray[3] = (symbol)exp2;
//}
//
//void addCalc(enum code_ops *op, struct symbol *result, struct symbol *exp1,
//		struct symbol *exp2) {
//	genQuad(op, result, exp1, exp2, NULL, NULL, 4);
//	//TODO:print();
//	//TODO:overwrite value in symboltable
//	//TODO:free allocation
//}
//
//void addWhile(struct symbol *var, enum code_ops *op, struct symbol *condition,
//		struct symbol *step, int label) {
//	//ob cond true /false --> sprünge + wenn schleife zu ende wieder an anfang springen
//	/**At first the condition is checked for the first time.
//	 * Afterwards quadrupel code is generated in order to change the checked condition variable with defined "step" e.g. i-- or i++
//	 * During Code generation the value of var has changed in symboltable and the new condition is backpatched before addwhile is called again
//	 * in case of a true condition...
//	 **/
//	//step is a value, which manipulates the var, which builds the result of the condition
//	//label equals "1" iff one loop iteration is ready
//	if (condition == 0) {
//		//do not entry loop
//		printf("while_loop is ready; zero times");
//	} else {
//		//jump into loop
//		genQuad(op, var, step, NULL, NULL, label, 3);
//		if (label == 1) {
//			//TODO: Backpatching
//			//*condition=backpatch();
//			//Error: Undefined reference for addwhile-->???
//			//addwhile(var,op,condition,step,label);
//		} else {
//			printf("while_loop is ready; n-times");
//		}
//	}
//}
//
//void addWhileDo(struct symbol *var, enum code_ops *op, struct symbol *condition,
//		struct symbol *step, int label) {
//	/** Difference to While-Loop: the loop has minimal one iteration**/
//	//TODO: Backpatching
//	//*condition=backpatch();
//	if (condition == 0) {
//		printf("while_loop is ready; 1-time");
//	} else {
//		//Error: Undefined reference for addwhile-->???
//		//addwhile(var,op,condition,step,label);
//	}
//
//}
//
//void addIf(enum code_ops *op, struct symbol *condition, int label) {
//	/**Assumption: Else equals If for a compiler**/
//	/**label is a marker, which let the compiler know, when the last condition is read. **/
//	if (label == 0) {
//		if (condition == 0) {
//			backpatch(); //TODO: Backpatching --> FalseList
//		}
//		if (condition == 1) {
//			backpatch(); //TODO: Backpatching --> TrueList
//		}
//	} else {
//		printf("if-statement is ready...");
//	}
//}
//
//void addIfGoTo() {
//	//???
//}
//
//void backpatch() {
////true&false list
//}
//
//void addFunc(enum code_ops *op, struct symbol *type, struct symbol *name,
//		int countParam) {
//	//???
//}
//
//void addReturn() {
//	//???
//}
//
//void addAssign(struct symbol *res, struct symbol *val, enum code_ops *op) {
//	//normal assignment
//	if (&op == NULL) {
//		*res = *val;
//	}
////	else if (&op == MINUS)
////	{
////		//negative assignment
////		genQuad(op, res, val, NULL, NULL, NULL, 4);
////	}
//}
//
////void printRes()
////{
////for(int i = 0; i<4; i++)
////{
////printf("%symbol",(tempArray+i));
////}
////}
