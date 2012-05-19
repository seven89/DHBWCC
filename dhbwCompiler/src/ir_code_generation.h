/*
 * ir_code_generation.h
 *
 *  Created on: Apr 3, 2012
 *      Author: FS
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
	opSHIFT_LEFT,
	opSHIFT_RIGHT,

	opLOGICAL_AND,
	opLOGICAL_OR,
	opLOGICAL_NOT,
	opNE,
	opEQ,
	opGT,
	opGTEQ,
	opLS,
	opLSEQ,

	opIF,
	opGOTO,
	opWHILE_BEGIN,
	opDO_WHILE_BEGIN,

	opRETURN,
	opPARAM,
	opCALL,
	opMEM_LD,
	opMEM_ST,
	opADDR,
	opFUNC_DEF,
	opFUNC_DEF_END,
	opNOP

};

typedef struct code
{
	int counter_id;
	enum code_ops operation;
	struct symbol *address_0;
	struct symbol *address_1;
	struct symbol *address_2;
	int jmpLabel;
	int jmpTo;
	struct code *next;
}code;

static char* enumStrings[] = { "ASSIGN", "ADD", "SUB", "MUL", "MINUS",
		"SHIFT_LEFT", "SHIFT_RIGHT",

		"LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT", "NE", "EQ", "GT", "GTEQ",
		"LS", "LSEQ",

		"IF", "GOTO", "WHILE_BEGIN", "DO_WHILE_BEGIN",

		"RETURN", "PARAM", "CALL", "MEM_LD", "MEM_ST", "ADDR", "FUNC_DEF",
		"FUNC_DEF_END", "NOP"

};

void append_code(enum code_ops operation, struct symbol *address_0,
		struct symbol *address_1, struct symbol *address_2, int jmpTo);

void print_all_codes();

void addCalc(enum code_ops *op, struct symbol *result, struct symbol *exp1,
		struct symbol *exp2);
void addWhile(struct symbol *var, enum code_ops *op, struct symbol *condition,
		struct symbol *step, int label);
void addWhileDo(struct symbol *var, enum code_ops *op, struct symbol *condition,
		struct symbol *step, int label);
void addIf(enum code_ops *op, struct symbol *condition, int label);
void addIfGoTo();
void addFunc(enum code_ops *op, struct symbol *type, struct symbol *name,
		int countParam);
void addReturn();
void addAssign(struct symbol *res, struct symbol *val, enum code_ops *op);

#endif

