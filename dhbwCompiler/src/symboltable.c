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
int symtablelock = 0; //locks the symtable if an error occurred

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
 * @brief compares two parameterlists element by element
 * @param parameterlist1
 * @param parameterlist2
 * @return 1 if they are the same , 0 in all other cases
 */
int checkparams(struct symbol *s1, struct symbol *s2) {
	struct symbol *el;
	int cnt1 = 0;
	int cnt2 = 0;
	int check = 1;

	if (DEBUG)
		printf("Checking parameters\n");

	LL_FOREACH(s1,el)
		cnt1++;
	LL_FOREACH(s2,el)
		cnt2++;

	if (cnt1 != cnt2)
		return (0);

	while (s1 != NULL && s2 != NULL) {
		if (s1->is.var.type != s2->is.var.type || strcmp(s1->name, s2->name)
				|| s1->is.var.isArray != s2->is.var.isArray)
			check = 0;
		else if (s1->is.var.size != s2->is.var.size)
			check = 0;

		s1 = s1->next;
		s2 = s2->next;
	}

	return (check);
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
	if (!symtablelock) {
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
				printf("ERROR	--	multiple declaration of variable \"%s\n",
						name);
		} else if (exists_Param(name) == NULL) {
			if (exists_Sym_loc(name) == NULL) {
				s->is.var.scope = currentScope;
				if (DEBUG)
					printf(
							"appending local variable \"%s\" to function \"%s\"\n",
							name, currentScope->name);
				LL_APPEND(currentScope->is.func.local_table, s);
				return (s);
			}
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
struct symbol * pushFunc(int type, char const *name, struct symbol *paramlist) {
	struct symbol *s = NULL;
	struct symbol *ref = NULL;

	s = (struct symbol*) malloc(sizeof(struct symbol));
	s->name = (char *) malloc(strlen(name) + 1);

	s->name = name;
	s->isFunc = 1;
	s->is.func.returntype = type;
	s->is.func.isProto = 0;

	if (paramlist != NULL) {
		s->is.func.hasParams = 1;
		addParam(s, paramlist);
		deleteFunc("-pseudo-");
	}

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

		if (ref->is.func.hasParams)

			if (!checkparams(ref->is.func.param_list, s->is.func.param_list)
					|| ref->is.func.returntype != s->is.func.returntype) {
				printf("ERROR	--	type missmatch in function \"%s\"\n",
						ref->name);
				symtablelock = 1;
				return (ref);
			}

		ref->is.func.isProto = 0;
		currentScope = ref;
		if (DEBUG) {
			printf("set current scope to %s\n", currentScope->name);
			printf(
					"-----------------------------------------------------------------\n");
		}
		return (ref);
	} else
		printf("ERROR	--	multiple declaration of function \"%s\"\n", name);

	return (s);

}

/**
 * @brief reset scope to global, unlock the symboltable again
 */
void resetScope() {
	currentScope = NULL;
	symtablelock = 0;
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
		printf("added parameter list to function \"%s\"", function->name);
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
				LL_FOREACH(s->is.func.param_list,el) {
					if (el->is.var.isArray)
						printf("\t\t\t\t\t\t|int name:%s[%d] |\n", el->name, // local array
								el->is.var.size);
					else
						printf("\t\t\t\t\t\t|int name:%s| \n", el->name); //local var
				}
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

