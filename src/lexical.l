%{
/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: lexical.l
 * Lexical analysis source code in lex/flex
 * format that recognizes the lexical components 
 * of the input read
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.h"
#include "sintactic.tab.h"
#include "ST.h"


// Redefine 'yylex()' to receive symbol table as parameter
#define YY_DECL int yylex(st) symbol_table* st;

// Definition of function called by yylex to manage lexical errors
void lexical_error(symbol_table *st, char *info, char *symbol);

// Declaration of function to change the input to a file
int change_input_file(char *file);
// Variable to control whether the current input depends on one or more files
unsigned int input_file = 0;

// Stack to control the input files
#define MAX_INPUT_DEPTH 6
YY_BUFFER_STATE input_file_stack[MAX_INPUT_DEPTH];
%}

%option noyywrap
%option noinput


Exponent                [eE][\+\-]?{DecimalInteger}

DecimalInteger          0|[1-9][[:digit:]]*
ExponentInteger         {DecimalInteger}{Exponent}
Integer                 {DecimalInteger}|{ExponentInteger}

DecimalDouble           {DecimalInteger}\.[[:digit:]]+
ExponentDouble          {DecimalDouble}{Exponent}
Double                  {DecimalDouble}|{ExponentDouble}

Binary                  0[bB][01]+
Hexadecimal             0[xX][0-9a-fA-F]+
Number                  {Integer}|{Double}
Id                      [_[:alpha:]][_[:alnum:]]*
String                  \"([^"\n]|\\\")*\"

%%

<<EOF>>             { /* END OF FILE */
                        // If is the end of an input file
                        if(input_file){
                            input_file--;

                            fclose(yyin);
                            yy_delete_buffer(YY_CURRENT_BUFFER);
                            yy_switch_to_buffer(input_file_stack[input_file]);
                        }
                        else{
                            // If is the end of the program execution
                            printf("\n");
                            st_free(st); //Frees memory reserved by the symbol table
                            exit(EXIT_SUCCESS);
                        }
                        
                    }

[ \t]+              { /* NOTHING */ }

{Id}                { /* ALPHANUMERIC STRING */
                        int lc = st_search(*st, yytext);
                        if(lc == -1){
                            st_insert(st, yytext, VAR);
                            lc = VAR;
                        }
                        
                        yylval.id = st_get_info_row(st, yytext);
                        return lc;
                    }

{String}            { /* STRING LITERAL */
                        char *string = malloc((yyleng-1) * sizeof(char));
                        strncpy(string, (yytext+1), yyleng-2); // Obvious the quotes
                        *(string+yyleng-2) = '\0';
                        yylval.str = string;

                        return STRING;
                    }

{Number}            { /* NUMBER LITERAL */
                        yylval.val = atof(yytext);
                        return NUM;
                    }

{Binary}            { /* NUMBER LITERAL: BINARY */
                        // Starts in the 3rd character (ignore prefix '0b')
                        yylval.val = strtoul(&yytext[2], NULL, 2);
                        return NUM;
                    }

{Hexadecimal}       { /* NUMBER LITERAL: HEXADECIMAL */
                        yylval.val = strtoul(yytext, NULL, 16);
                        return NUM;
                    }

\n                  { /* END OF LINE */
                        return '\n';
                    }

"+"                 |
"-"                 |
"*"                 |
"/"                 |
"%"                 |
"^"                 |
"="                 |
"("                 |
")"                 { /* MATHEMATICAL OPERATOR CHARACTER */
                        return *yytext;
                    }

.                   { /* ANY OTHER CHARACTER */
                        lexical_error(st, "Unexpected symbol", yytext);
                    }

%%

// Function to change the input to a file
int change_input_file(char *file){
    if(input_file == MAX_INPUT_DEPTH){
        return -1;
    }
    FILE *new_file_input = fopen(file, "r");

    if(new_file_input == NULL)
        return -2;

    // Adds the current input into the stack
    input_file_stack[input_file++] = YY_CURRENT_BUFFER;
    yyin = new_file_input;
    // Changes to the new input
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

    unput('\n'); // Unputs a '\n' to finalize 'load' action properly

    return 0;
}

// Function called by yylex to manage lexical errors
void lexical_error(symbol_table *st, char *info, char *symbol){
    flag_error = 1;
    char *message = malloc(strlen("LEXICAL ERROR: ") + strlen(info) + 1);
    strcpy(message, "LEXICAL ERROR: ");
    strcat(message, info);
    strcat(message, " ");
    strcat(message, symbol);
    yyerror(st, message);
    free(message);
}