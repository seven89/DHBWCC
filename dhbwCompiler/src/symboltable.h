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
	int arr_size;
	int offset;
	int mem_size;
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
struct symbol * pushFunc(int type, char const *name, struct symbol *paramlist);

/**
 * @brief finds a symbol in any table by name - order: param_list, local_list, global_list
 * @param name of the symbol
 * @return pointer to the searched symbol or NULL if it does not exist
 */
struct symbol * findSymbol(const char *name);

/**
 * @brief reset scope to global, unlock global symboltable
 */
void resetScope();

/**
 * @brief pretty print of all symbols
 */
void debug_printSymbolTable();

#endif


