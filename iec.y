 /**
  * CONTENTS: Define the type of token value
  */
%union {
	char *ID; /* Token value */
}
 /**
  * CONTENTS: Define the token
  */
 /* TOKEN: Keywords */
 /* EN & ENO */
%token EN
%token ENO
 /* TRUE & FALSE */
%token TRUE
%token FALSE
 /* Elementary Data Types */
%token BOOL

%token BYTE
%token WORD
%token DWORD
%token LWORD

%token SINT
%token INT
%token DINT
%token LINT

%token USINT
%token UINT
%token UDINT
%token ULINT

%token REAL
%token LREAL

%token WSTRING
%token STRING

%token TIME
%token DATE
%token DT
%token TOD
%token DATE_AND_TIME
%token TIME_OF_DAY
 /* Derived Data Types */
%token TYPE
%token END_TYPE
%token ARRAY
%token OF
%token STRUCT
%token END_STRUCT
 /* Variables Declaration & Initialization */
%token VAR_INPUT
%token VAR_OUTPUT
%token VAR_IN_OUT
%token VAR_EXTERNAL
%token VAR_GLOBAL
%token VAR_TEMP
%token VAR
%token END_VAR
%token RETAIN
%token NON_RETAIN
%token R_EDGE
%token F_EDGE
%token AT
%token CONSTANT
 /* Functions */
%token FUNCTION
%token END_FUNCTION
 /* Function Blocks */
%token FUNCTION_BLOCK
%token END_FUNCTION_BLOCK
 /* Programs */
%token PROGRAM
%token END_PROGRAM
 /* Configuration Elements */
%token CONFIGURATION
%token END_CONFIGURATION
%token TASK
%token RESOURCE
%token ON
%token END_RESOURCE
%token VAR_CONFIG
%token VAR_ACCESS
%token WITH
%token READ_WRITE
%token READ_ONLY
 /* Public Operators */
%token AND
%token MOD
%token OR
%token XOR
%token NOT
 /* ST Subprogram Control Statements */
%token RETURN
 /* ST Selection Statements */
%token IF
%token THEN
%token ELSIF
%token ELSE
%token END_IF

%token CASE
%token END_CASE
 /* ST Iteration Statements */
%token FOR
%token TO
%token BY
%token DO
%token END_FOR

%token WHILE
%token END_WHILE

%token REPEAT
%token UNTIL
%token END_REPEAT

%token EXIT





 /* TOKEN: Numeric Literals */
%token <ID> integer_token
%token <ID> real_token
%token <ID> fixed_point_token
%token <ID> binary_integer_token
%token <ID> octal_integer_token
%token <ID> hex_integer_token



%%
term:

%%
yyerror(char *s) {
}
