/*
 * ir_code_generation.h
 *
 *  Created on: Apr 3, 2012
 *      Author: FS
 */

#ifndef _DHBWCC_IR_CODE_GENERATION_H
#define _DHBWCC_IR_CODE_GENERATION_H

enum code_ops
{
opASSIGN, opADD, opSUB, opMUL, opMINUS, opSHIFT_LEFT, opSHIFT_RIGHT,

opLOGICAL_AND, opLOGICAL_OR, opLOGICAL_NOT, opNE, opEQ, opGT, opGTEQ, opLS, opLSEQ,

opIF, opGOTO, opWHILE_BEGIN, opDO_WHILE_BEGIN,

opRETURN, opPARAM, opCALL, opMEM_LD, opMEM_ST, opADDR, opFUNC_DEF, opFUNC_DEF_END, opNOP

};

static char* enumStrings[] = {
"ASSIGN", "ADD", "SUB", "MUL", "MINUS", "SHIFT_LEFT", "SHIFT_RIGHT",

"LOGICAL_AND", "LOGICAL_OR", "LOGICAL_NOT", "NE", "EQ", "GT", "GTEQ", "LS", "LSEQ",

"IF", "GOTO", "WHILE_BEGIN", "DO_WHILE_BEGIN",

"RETURN", "PARAM", "CALL", "MEM_LD", "MEM_ST", "ADDR", "FUNC_DEF", "FUNC_DEF_END", "NOP"

};
#endif





