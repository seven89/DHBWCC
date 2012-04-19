#ifndef _DHBWCC_SYMBOLTABLE_H
#define _DHBWCC_SYMBOLTABLE_H
/*
 * symboltable.h
 *
 *  Created on: Apr 3, 2012
 *      Author: NA
 */
#include <stdio.h>
#include "include/utlist.h"

struct variable{
	int type;
	int isArray;
	int size;
	struct symbol *scope;
};

struct function{
	int returntype;
	int isProto;
	int hasParams;
	struct symbol *local_table;
	struct symbol *param_list;
};

typedef struct symbol {
    const char *name;
    int isFunc;
    union{
    	struct variable var;
    	struct function func;
    }is;
    struct symbol *next;
} symbol;

/**
 * @brief inserts the variable "name" in the global symbol table or the local table of current scope
 * 		  also checks if the symbol already exists
 * @param name of the variable
 * @return reference to the created symbol
 */
struct symbol *pushVar(char const *name);

/**
 * @brief inserts the variable "name" in the global symbol table
 * 		  also checks if the symbol already exists
 * @param returntype of the function
 * @param name of the function
 * @return reference to the created symbol
 */
struct symbol *pushFunc(int type, char const *name);

/**
 * @brief deletes the function "name" from global symboltable
 * @param name of the function
 */
void deleteFunc(char const *name);

/**
 * @brief adds a list of parameters to a function
 * @param pointer to the function
 * @param pointer to the parameterlist
 */
void addParam(struct symbol* function,struct symbol* params);

/**
 * @brief reset scope to global
 */
void resetScope();

/**
 * @brief pretty print of all symbols
 */
void debug_printSymbolTable();

#endif


