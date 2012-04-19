/*
 * symboltable.c
 *
 *  Created on: Apr 3, 2012
 *      Author: NA
 */

#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utlist.h"

symbol *symtable = NULL; //points to global symbol table
symbol *currentScope = NULL; //points to current function or NULL if scope is global

int DEBUG = 0; // set 1 for printf-output

/**
 * @brief searches the element "name" in global symbol table
 * @param name of the global symbol
 * @return reference to the global symbol or NULL if does not exist
 */
symbol *exists_Sym_glob(char const *name) {
	symbol *s = NULL;

	if (DEBUG)
		printf("searching for symbol \"%s\" in global table\n", name);

	if (symtable == NULL) {
		if (DEBUG)
			printf("global symboltable is empty\n");
		return NULL;
	}

	LL_FOREACH(symtable,s) {
		if (!strcmp(name, s->name)) {
			return (s);
		}
	}

	if (DEBUG)
		printf("cannot find a symbol with the same identifier\n");

	return NULL;
}

/**
 * @brief searches the element "name" in the local symbol table of the function referenced in currentScope
 * @param name of the local symbol
 * @return reference to the local symbol or NULL if does not exist
 */
symbol * exists_Sym_loc(char const *name) {
	symbol *s = NULL;
	if (DEBUG)
		printf("searching for symbol \"%s\" in table \"%s\"\n", name,
				currentScope->name);
	if (currentScope->is.func.local_table == NULL) {
		if (DEBUG)
			printf("local table is empty\n");
		return NULL;
	}

	LL_FOREACH(currentScope->is.func.local_table,s) {
		if (!strcmp(name, s->name)) {
			printf("ERROR	--	multiple declaration of variable \"%s\n", name);
			return (s);
		}
	}
	if (DEBUG)
		printf("cannot find a symbol with the same identifier\n");
	return NULL;
}

/**
 * @brief searches the element "name" in the parameter list of the function referenced in currentScope
 * @param name of the local symbol
 * @return reference to the parameter symbol or NULL if does not exist
 */
symbol * exists_Param(char const *name) {
	symbol *s = NULL;
	if (DEBUG)
		printf("searching for param \"%s\" in table \"%s\"\n", name,
				currentScope->name);
	if (currentScope->is.func.param_list == NULL) {
		if (DEBUG)
			printf("parameter list is empty\n");
		return NULL;
	}

	LL_FOREACH(currentScope->is.func.param_list,s) {
		if (!strcmp(name, s->name)) {
			printf("ERROR	--	there is already a parameter \"%s\"\n", name);
			return (s);
		}
	}

	if (DEBUG)
		printf("cannot find a parameter with the same identifier\n");
	return NULL;
}

/**
 * @brief deletes the function "name" from global symboltable
 * @param name of the function
 */
void deleteFunc(char const *name) {
	struct symbol *ref = NULL;
	ref = exists_Sym_glob(name);

	if (ref != NULL) {
		if (DEBUG)
			printf("deleting function \"%s\"\n", ref->name);
		LL_DELETE(symtable, ref);
	}
}

/**
 * @brief inserts the variable "name" in the global symbol table or the local table of current scope
 * 		  also checks if the symbol already exists
 * @param name of the variable
 * @return reference to the created symbol
 */
struct symbol *pushVar(char const *name) {
	struct symbol *s = NULL;
	s = (struct symbol*) malloc(sizeof(struct symbol));
	s->name = (char *) malloc(strlen(name) + 1);
	s->name = name;
	s->isFunc = 0; //s is not a function
	s->is.var.type = 1; //s is of type int
	s->is.var.isArray = 0; //s is initialized as normal variable
	s->is.var.size = 0; //therefore we have no size

	if (currentScope == NULL) {
		if (exists_Sym_glob(name) == NULL) {
			s->is.var.scope = NULL;
			if (DEBUG)
				printf("appending global variable %s\n", name);
			LL_APPEND(symtable, s);
			return (s);
		} else
			printf("ERROR	--	multiple declaration of variable \"%s\n", name);
	} else if (exists_Param(name) == NULL) {
		if (exists_Sym_loc(name) == NULL) {
			s->is.var.scope = currentScope;
			if (DEBUG)
				printf("appending local variable \"%s\" to function \"%s\"\n",
						name, currentScope->name);
			LL_APPEND(currentScope->is.func.local_table, s);
			return (s);
		}
	}

	return NULL;
}

/**
 * @brief inserts the variable "name" in the global symbol table
 * 		  also checks if the symbol already exists
 * @param returntype of the function
 * @param name of the function
 * @return reference to the created symbol
 */
struct symbol * pushFunc(int type, char const *name) {
	struct symbol *s = NULL;
	struct symbol *ref = NULL;
	s = (struct symbol*) malloc(sizeof(struct symbol));
	s->name = (char *) malloc(strlen(name) + 1);
	s->name = name;
	s->isFunc = 1;
	s->is.func.returntype = type;
	s->is.func.isProto = 0;

	ref = exists_Sym_glob(name);

	if (ref == NULL) {

		LL_APPEND(symtable, s);

		if (DEBUG) {
			printf("appending function \"%s\" to global table \n", name);
			printf(
					"-----------------------------------------------------------------\n");
		}

		currentScope = s;

		if (DEBUG) {
			printf("set current scope to %s\n", currentScope->name);
			printf(
					"-----------------------------------------------------------------\n");
		}

		return (s);
	} else if (ref->is.func.isProto) {
		if (DEBUG)
			printf("found prototype \"%s\"\n", ref->name);
		ref->is.func.isProto = 0;
		currentScope = ref;
		if (DEBUG) {
			printf("set current scope to %s\n", currentScope->name);
			printf(
					"-----------------------------------------------------------------\n");
		}
		return (ref);
	} else
		printf("ERROR	--	multiple declaration of variable \"%s\n", name);

	return (s);

}

/**
 * @brief reset scope to global
 */
void resetScope() {
	currentScope = NULL;
	if (DEBUG) {
		printf("reset scope to global\n");
		printf(
				"-----------------------------------------------------------------\n");
	}
}

/**
 * @brief adds a list of parameters to a function
 * @param pointer to the function
 * @param pointer to the parameterlist
 */
void addParam(struct symbol* function, struct symbol* params) {
	if (DEBUG)
		printf("added parameter list to function \"%s\"",function->name);
	if (function->is.func.param_list == NULL)
		LL_CONCAT(function->is.func.param_list, params);
}

/**
 * @brief pretty print of all symbols
 */
void debug_printSymbolTable() {
	symbol *s = NULL;
	symbol *el = NULL;

	printf("\n\n\t\t - DEBUG ___ SYMBOL _ TABLE ___ DEBUG - \n\n");
	printf(
			"-----------------------------------------------------------------\n");
	printf("     global \t\t|     local\t\t|   param\n");
	printf(
			"-----------------------------------------------------------------\n");

	LL_FOREACH(symtable,s) {

		// is function
		if (s->isFunc) {
			if (s->is.func.isProto)
				printf("isProto"); // is prototype

			if (s->is.func.returntype) //select returntype
				printf("|int name:%s| \n", s->name);
			else
				printf("|void name:%s| \n", s->name);

			LL_FOREACH(s->is.func.local_table,el) {
				if (el->is.var.isArray)
					printf("\t\t\t|int name:%s[%d] |\n", el->name, // local array
							el->is.var.size);
				else
					printf("\t\t\t|int name:%s| \n", el->name); //local var
			}

			if (s->is.func.hasParams) {
				LL_FOREACH(s->is.func.param_list,el)
					printf("\t\t\t\t\t\t|int name:%s|\n", el->name);
			}
		}

		//is global variable
		else {
			if (s->is.var.isArray)
				printf("|int name:%s[%d] |\n", s->name, s->is.var.size);
			else
				printf("|int name:%s| \n", s->name); //global var

		}
	}
}

