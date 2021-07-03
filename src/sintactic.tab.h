/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Header: sintactic.tab.h
 * Definitions of macros and functions used by
 * the sintactic analyzer
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#ifndef SINTACTIC_TAB_H
#define SINTACTIC_TAB_H

#include "ST.h"


// Colors to print the output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* Token type */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
    enum yytokentype
    {
        NUM = 258,
        VAR = 259,
        CONST = 260,
        FNCT = 261,
        COMMAND = 262,
        COMMAND_P = 263,
        COMMAND_2P = 264,
        STRING = 265,
        NEG = 266
    };
#endif

/* Value type  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
union YYSTYPE{
    double      val;        // Type for numbers
    info_row    *id;        // Type for pointers to symbol table
    char        *str;       // Type for strings
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

/* Variable that controls if an assignment value is displayed */
#define ECHO_OFF    0
#define ECHO_ON     1
extern int echo;

/* Variable that controls if an error has ocurred */
extern int flag_error;

/* 
 * Function that starts de sintactic and lexical analyzer
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 */
void startExecution(symbol_table *st);

/*
 * Function called by yyparse to manage general errors
 * 
 * @param st: Pointer to the symbol table that contains the alphanumeric strings
 * @param s: String with some information about the error
 */ 
void yyerror(symbol_table *st, char *s);

#endif /* SINTACTIC_TAB_H */