/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EN = 258,
     ENO = 259,
     TRUE = 260,
     FALSE = 261,
     BOOL = 262,
     BYTE = 263,
     WORD = 264,
     DWORD = 265,
     LWORD = 266,
     SINT = 267,
     INT = 268,
     DINT = 269,
     LINT = 270,
     USINT = 271,
     UINT = 272,
     UDINT = 273,
     ULINT = 274,
     REAL = 275,
     LREAL = 276,
     WSTRING = 277,
     STRING = 278,
     TIME = 279,
     DATE = 280,
     DT = 281,
     TOD = 282,
     DATE_AND_TIME = 283,
     TIME_OF_DAY = 284,
     TYPE = 285,
     END_TYPE = 286,
     ARRAY = 287,
     OF = 288,
     STRUCT = 289,
     END_STRUCT = 290,
     VAR_INPUT = 291,
     VAR_OUTPUT = 292,
     VAR_IN_OUT = 293,
     VAR_EXTERNAL = 294,
     VAR_GLOBAL = 295,
     VAR_TEMP = 296,
     VAR = 297,
     END_VAR = 298,
     RETAIN = 299,
     NON_RETAIN = 300,
     R_EDGE = 301,
     F_EDGE = 302,
     AT = 303,
     CONSTANT = 304,
     FUNCTION = 305,
     END_FUNCTION = 306,
     FUNCTION_BLOCK = 307,
     END_FUNCTION_BLOCK = 308,
     PROGRAM = 309,
     END_PROGRAM = 310,
     CONFIGURATION = 311,
     END_CONFIGURATION = 312,
     TASK = 313,
     RESOURCE = 314,
     ON = 315,
     END_RESOURCE = 316,
     VAR_CONFIG = 317,
     VAR_ACCESS = 318,
     WITH = 319,
     READ_WRITE = 320,
     READ_ONLY = 321,
     AND = 322,
     MOD = 323,
     OR = 324,
     XOR = 325,
     NOT = 326,
     RETURN = 327,
     IF = 328,
     THEN = 329,
     ELSIF = 330,
     ELSE = 331,
     END_IF = 332,
     CASE = 333,
     END_CASE = 334,
     FOR = 335,
     TO = 336,
     BY = 337,
     DO = 338,
     END_FOR = 339,
     WHILE = 340,
     END_WHILE = 341,
     REPEAT = 342,
     UNTIL = 343,
     END_REPEAT = 344,
     EXIT = 345,
     integer_token = 346,
     real_token = 347,
     fixed_point_token = 348,
     binary_integer_token = 349,
     octal_integer_token = 350,
     hex_integer_token = 351
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 4 "iec.y"

	char *ID; /* Token value */



/* Line 2068 of yacc.c  */
#line 152 "iec.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


