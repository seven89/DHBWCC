/*
 * ir_code_generation.h
 *
 *  Created on: Apr 3, 2012
 *      Author: Fabian Schaefer
 */

#ifndef _DHBWCC_IR_CODE_GENERATION_H
#define _DHBWCC_IR_CODE_GENERATION_H
#include "symboltable.h"
enum code_ops {
	opASSIGN,
	opADD,
	opSUB,
	opMUL,
	opMINUS,


	opLOGICAL_AND,
	opLOGICAL_OR,
	opLOGICAL_NOT,
	opNE,
	opEQ,
	opGT,
	opGE,
	opLT,
	opLE,

	opIF,
	opGOTO,
	opWHILE_BEGIN,
	opDO_WHILE_BEGIN,

	opRETURN,
	opPARAM,
	opCALL,
	opFUNC_DEF,
	opFUNC_DEF_END,
};

typedef struct ir_code{
	/**ir_code pre-definitions**/
	struct code
	{
		int counter_id;
		enum code_ops *operation;
		struct symbol *address_0;
		struct symbol *address_1;
		struct symbol *address_2;
		int jmpLabel;
		int jmpTo;
		struct code *next;
	}code;

	union TRUELIST
	{
		int row_id;
		union TRUELIST *next;
	}TRUELIST;

	union FALSELIST
	{
		int row_id;
		union FALSELIST *next;
	}FALSELIST;

	struct ir_code *next;
}ir_code;

static char* enumStrings[] = { "ASSIGN", "ADD", "SUB", "MUL", "MINUS",

		"LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT", "NE", "EQ", "GT", "GE",
		"LT", "LE",

		"IF", "GOTO", "WHILE_BEGIN", "DO_WHILE_BEGIN",

		"RETURN", "PARAM", "CALL", "FUNC_DEF",
		"FUNC_DEF_END"

};

void print_all_codes();
void addAssign(enum code_ops *op, struct symbol *var, struct symbol *value);
void addCalc(enum code_ops *op, struct symbol *result, struct symbol *exp1,
		struct symbol *exp2);
void addIf(enum code_ops *op, struct symbol *exp1, struct symbol *exp2);


void addWhile(enum code_ops *op, struct symbol *exp1, struct symbol *exp2);
void addWhileDo(enum code_ops *op, struct symbol *exp1, struct symbol *exp2);

void addIfGoTo();
void addFunc(enum code_ops *op, struct symbol *type, struct symbol *name,
		int countParam);
void addReturn();





#endif

