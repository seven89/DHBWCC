/*
 * ir_code_generation.h
 *
 *  Created on: Apr 3, 2012
 *      Author: FS
 */

#ifndef _DHBWCC_IR_CODE_GENERATION_H
#define _DHBWCC_IR_CODE_GENERATION_H

enum operator {
	opDo, opWhile, opGt,
	opIf, opElse,
	opLogicalOr, opLogicalAnd, opLogicalNot,
	opNe, opEq, /*opGt,*/ opLs, opGtEq, opLsEq,
	opShiftRight, opShiftLeft,
	opAdd, opSub, opMul, opDiv, opMinus,
	opReturn, opReturnParam,
	opAssign,
	opCallFunc, opCallFuncOnParam,
	opMem_LD, opMem_ST, opAddr
};

char* opStrings [] = {
    "DO", "WHILE", "GOTO",
    "IF", "ELSE",
    "LOGICAL_OR", "LOGICAL_AND", "LOGICAL_NOT",
    "NE", "EQ", "GT", "LS", "GT_EQ", "LS_EQ",
    "SHIFT_RIGHT", "SHIFT_LEFT",
    "ADD", "SUB", "MUL", "DIV", "Minus",
    "RETURN", "RETURN_PARAM",
    "ASSIGN", "CALL_FUNC", "CALL_FUNC_ON_PARAM",
    "MEM_LD", "MEM_ST", "ADDR"
};

#endif





