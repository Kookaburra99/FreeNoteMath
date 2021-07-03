/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Header: ST.h
 * Definitions of symbol table struct and 
 * functions, implemented as a BST (binary
 * search tree)
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#ifndef ST_H
#define ST_H

/** Information that the symbol table contains in each node **/
typedef struct {
    char *key;      // The alphanumeric string
    int lc;         // The lexic component identifier
    union{          // The value of the variable or function
        double var;
        double (*fnct_d)();
        void (*command)();
    }value;
    unsigned init;  // Checks if the variable has been initialized
} info_row;

/** Type definition as a pointer to node **/
// Struct node is defined on ST.c and it's transparent to the user
typedef struct node *symbol_table;


#include "commands.h"

/**
 * Initialize the symbol table, reserve space and add the commands 
 * passed as a parameter
 * 
 * @param list_commands: List of command names and pointers to their 
 * associated functions. The last row must be all 0
 */ 
symbol_table* st_init(init_commands commands_list[]);

/**
 * Searchs an alphanumeric string on the symbol table
 * If exists, returns the lexical component
 * If not, returns -1
 * 
 * @param st: Symbol table that contains the alphanumeric strings
 * @param key: The alphanumeric string
 * @return The lexical component of the variable/function or -1 if
 * not exists
 */
int st_search(symbol_table st, char *key);

/**
 * Inserts an alphanumeric string, and its lexical component
 * on the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 * @param key: The alphanumeric string
 * @param lc: The lexical component of the alphanumeric string
 * @return 0 if everything went well or -1 if there was an error
 */ 
int st_insert(symbol_table *st, char *key, int lc);

/**
 * Deletes an element from the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 * @param key: The alphanumeric string
 * @return 0 if everything went well or -1 if there was an error
 */ 
int st_delete(symbol_table *st, char *key);

/**
 * Returns all the information associated with an alphanumeric
 * string present in the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 * @param key: The alphanumeric string
 * @return The pointer to the corresponding info_row or NULL if the key isn't
 * on the symbol table
 */ 
info_row* st_get_info_row(symbol_table *st, char *key);

/**
 * Displays the information associated with all the alphanumeric
 * strings present in the symbol table whose lexical component
 * mathces the one passed by parameter
 * 
 * @param st: Symbol table that contains the alphanumeric strings
 * @param lc: The lexical component of the alphanumeric string
 */ 
void st_show_lc_items(symbol_table st, int lc);

/**
 * Deletes the information associated with all the alphanumeric
 * strings present in the symbol table whose lexical component
 * mathces the one passed by parameter
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 * @param lc: The lexical component of the alphanumeric string
 */ 
void st_delete_lc_items(symbol_table *st, int lc);

/**
 * Updates the value of an existing constant in the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the aplhanumeric strings
 * @param key: The alphanumeric string (the constant name)
 * @param value: The value to be assigned to the constant
 * @return 0 if everything went well or -1 if there was an error
 */ 
int st_addValue(symbol_table *st, char *key, double value);

/**
 * Updates the value of an existing command in the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the aplhanumeric strings
 * @param key: The alphanumeric string (the command name)
 * @param command: Pointer to the function associated to the command
 * @return 0 if everything went well or -1 if there was an error
 */ 
int st_addCommand(symbol_table *st, char *key, void (*command)());

/**
 * Updates the value of an existing function in the symbol table
 * 
 * @param st: Pointer to the symbol table that contains the aplhanumeric strings
 * @param key: The alphanumeric string (the function name)
 * @param fnc_ptr: Pointer to the function associated
 * @return 0 if everything went well or -1 if there was an error
 */ 
int st_addFunction(symbol_table *st, char *key, double (*fnc_ptr)());

/**
 * Frees the memory associated to the symbol table
 * Upon completion, the value of the symbol table is NULL
 * 
 * @param st: Pointer to the symbol table to be freed
 */ 
void st_free(symbol_table *st);

#endif /* ST_H */