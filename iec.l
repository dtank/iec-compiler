 /**
  * PART 1.1
  * CONTENT: Flex Options
  */
 /* use char *yytext instead of char yytext[] to store the input lexeme */
%pointer
 /* NOT use function yywrap() */
%option noyywrap
 /* Required for the use of yylineno */
%option yylineno
 /* Required for the use of function yy_pop_state() & yy_push_state() */
%option stack
 /**
  * PART 1.2
  * CONTENT: File Including
  */
%{
#include "iec.tab.hh"
#include "lexer_utils.hh"
#define TEST_FLEX
%}
 /**
  * PART 1.6
  * CONTENT: Forward Declaration Of Functions Defined Later
  */
%{
 /* Return the current token back to the input stream */
void unput_text(unsigned int n);
%}
 /**
  * PART 1.7
  * CONTENT: State Declaration
  */
%s header_state
%x vardecl_list_state
%s vardecl_state
%x body_state
%s il_state
%s st_state

 /**
  * PART 1.8
  * CONTENT: Pattern Naming
  * PURPOSE: To use pattern easily
  */
 /* Whitespace */
st_whitespace [ \f\n\r\t\v]*
il_whitespace [ \f\r\t\v]*
 /* Letters, Digits and Identifiers */
letter               [A-Za-z]
digit                [0-9]
octal_digit          [0-7]
hex_digit            {digit}|[A-F]
identifier           ({letter}|(_({letter}|{digit})))((_?({letter}|{digit}))*)
qualified_identifier {identifier}(\.{identifier})+
 /* Numeric Literals */
integer         {digit}((_?{digit})*)
integer_0_59    (0(_?))*([0-5](_?))?{digit}
integer_0_19    (0(_?))*([0-1](_?))?{digit}
integer_20_23   (0(_?))*2(_?)[0-3]
integer_0_23    {integer_0_19}|{integer_20_23}
integer_0_999   {digit}((_?{digit})?)((_?{digit})?)
binary_integer  2#{bit}((_?{bit})*)
bit	            [0-1]
octal_integer   8#{octal_digit}((_?{octal_digit})*)
hex_integer     16#{hex_digit}((_?{hex_digit})*)
exponent        [Ee]([+-]?){integer}
real	    	{integer}\.{integer}{exponent}
fixed_point     {integer}\.{integer}
 /* Directly Represented Variables */
location_prefix          [IQM]
size_prefix              [XBWDL]
direct_variable_standard %{location_prefix}({size_prefix}?){integer}((.{integer})*)
direct_variable          {direct_variable_standard}
incompl_location         %[IQM]\*

%%
 /**
  * PATTERN: State Identifiers
  * ACTIONS: Change State
  */
 /* INITIAL -> header_state */
<INITIAL>{
FUNCTION	   yy_push_state(header_state); return FUNCTION;
FUNCTION_BLOCK yy_push_state(header_state); return FUNCTION_BLOCK;
PROGRAM        yy_push_state(header_state); return PROGRAM;
}
 /* header_state -> vardecl_list_state */
<header_state>{
VAR          |
VAR_INPUT    |
VAR_OUTPUT   |
VAR_IN_OUT   |
VAR_EXTERNAL |
VAR_GLOBAL   |
VAR_TEMP     |
VAR_CONFIG   |
VAR_ACCESS   unput_text(0); BEGIN(vardecl_list_state);
}
 /* vardecl_list_state -> (vardecl_state | body_state | INITIAL) */
<vardecl_list_state>{
VAR_INPUT    |
VAR_OUTPUT   |
VAR_IN_OUT   |
VAR_EXTERNAL |
VAR_GLOBAL   |
VAR_TEMP     |
VAR_CONFIG   |
VAR_ACCESS   |
VAR          unput_text(0); yy_push_state(vardecl_state);

END_FUNCTION	   unput_text(0); BEGIN(INITIAL);
END_FUNCTION_BLOCK unput_text(0); BEGIN(INITIAL);
END_PROGRAM		   unput_text(0); BEGIN(INITIAL);

. unput_text(0); yy_push_state(body_state);
}
 /* vardecl_list_state -> pop to $previous_state (vardecl_list_state) */
<vardecl_state>{
END_VAR	yy_pop_state(); return END_VAR;
}
 /* body_state -> (il_state | st_state) */
<body_state>{

{qualified_identifier}     unput_text(0); BEGIN(st_state);
{direct_variable_standard} unput_text(0); BEGIN(st_state);

RETURN unput_text(0); BEGIN(st_state);
IF     unput_text(0); BEGIN(st_state);
CASE   unput_text(0); BEGIN(st_state);
FOR    unput_text(0); BEGIN(st_state);
WHILE  unput_text(0); BEGIN(st_state);
EXIT   unput_text(0); BEGIN(st_state);
REPEAT unput_text(0); BEGIN(st_state);

:= unput_text(0); BEGIN(st_state);

{identifier} {
    int token = get_identifier_token(yytext);
    if ((token == prev_declared_fb_name_token) || (token == prev_declared_variable_name_token)) {
   	  /*the code has a call to a function block OR has an assingment with a variable as the lvalue */
   	 unput_text(0); BEGIN(st_state);
    } else if (token == prev_declared_derived_function_name_token) {
   	  /*the code has a call to a function - must be IL */
   	 unput_text(0); BEGIN(il_state);
    } else {
   	  /*Might be a lable in IL, or a bug in ST/IL code. We jump to IL */
   	 unput_text(0); BEGIN(il_state);
    }
}

. unput_text(0); BEGIN(il_state); /* Don't know what it could be. This is most likely a bug. Let's just to a random state... */
}
 /* (il_state | st_state) -> pop to $previous_state (vardecl_list_state) */
<il_state,st_state>{
END_FUNCTION       yy_pop_state(); unput_text(0);
END_FUNCTION_BLOCK yy_pop_state(); unput_text(0);
END_PROGRAM        yy_pop_state(); unput_text(0);
}


 /**
  * PATTERN: Whitespace
  * ACTIONS: Do nothing to eat any whitespace
  */
<INITIAL,header_state,body_state,vardecl_list_state,vardecl_state,st_state>{st_whitespace}
<il_state>{il_whitespace}

 /**
  * PATTERN: Public Keywords
  * ACTIONS: Return the token defined by bison
  */
 /* EN & ENO */
EN  return EN;
ENO return ENO;
 /* TRUE & FALSE */
TRUE  return TRUE;
FALSE return FALSE;
 /* Elementary Data Types */
BOOL          return BOOL;

BYTE          return BYTE;
WORD          return WORD;
DWORD         return DWORD;
LWORD         return LWORD;

SINT          return SINT;
INT           return INT;
DINT          return DINT;
LINT          return LINT;

USINT         return USINT;
UINT          return UINT;
UDINT         return UDINT;
ULINT         return ULINT;

REAL          return REAL;
LREAL         return LREAL;

WSTRING       return WSTRING;
STRING        return STRING;

TIME          return TIME;
DATE          return DATE;
DT            return DT;
TOD           return TOD;
DATE_AND_TIME return DATE_AND_TIME;
TIME_OF_DAY   return TIME_OF_DAY;
 /* Derived Data Types */
TYPE       return TYPE;
END_TYPE   return END_TYPE;
ARRAY      return ARRAY;
OF         return OF;
STRUCT     return STRUCT;
END_STRUCT return END_STRUCT;
 /* Variables Declaration & Initialization */
VAR_INPUT    return VAR_INPUT;
VAR_OUTPUT   return VAR_OUTPUT;
VAR_IN_OUT   return VAR_IN_OUT;
VAR_EXTERNAL return VAR_EXTERNAL;
VAR_GLOBAL   return VAR_GLOBAL;
VAR_TEMP     return VAR_TEMP;
VAR          return VAR;
END_VAR      return END_VAR;
RETAIN       return RETAIN;
NON_RETAIN   return NON_RETAIN;
R_EDGE       return R_EDGE;
F_EDGE       return F_EDGE;
AT           return AT;
CONSTANT     return CONSTANT;
 /* Functions */
FUNCTION     return FUNCTION;
END_FUNCTION return END_FUNCTION;
 /* Function Blocks */
FUNCTION_BLOCK     return FUNCTION_BLOCK;
END_FUNCTION_BLOCK return END_FUNCTION_BLOCK;
 /* Programs */
PROGRAM     return PROGRAM;
END_PROGRAM return END_PROGRAM;
 /* Configuration Elements */
CONFIGURATION     return CONFIGURATION;
END_CONFIGURATION return END_CONFIGURATION;
TASK              return TASK;
RESOURCE          return RESOURCE;
ON                return ON;
END_RESOURCE      return END_RESOURCE;
VAR_CONFIG        return VAR_CONFIG;
VAR_ACCESS        return VAR_ACCESS;
WITH              return WITH;
READ_WRITE        return READ_WRITE;
READ_ONLY         return READ_ONLY;
 /* Public Operators */
AND return AND;
OR  return OR;
XOR return XOR;
NOT return NOT;
MOD return MOD;
 /* ST Subprogram Control Statements */
RETURN return RETURN;
 /* ST Selection Statements */
IF       return IF;
THEN     return THEN;
ELSIF    return ELSIF;
ELSE     return ELSE;
END_IF   return END_IF;

CASE     return CASE;
END_CASE return END_CASE;
 /* ST Iteration Statements */
FOR        return FOR;
TO         return TO;
BY         return BY;
DO         return DO;
END_FOR    return END_FOR;

WHILE      return WHILE;
END_WHILE  return END_WHILE;

REPEAT     return REPEAT;
UNTIL      return UNTIL;
END_REPEAT return END_REPEAT;

EXIT       return EXIT;

 /**
  * PATTERN: IL STATE EXCLUSIVE
  * ACTIONS: Return the token defined by bison
  */
 /* EOL */
<il_state>\n return EOL;
 /* IL operators clashing with standard functions keywords */
<il_state>{
ADD return ADD;
SUB return SUB;
MUL return MUL;
DIV return DIV;
EQ  return EQ;
GE  return GE;
GT  return GT;
LE  return LE;
LT  return LT;
NE  return NE;
}
 /* IL operators clashing with SFC action qualifiers */
<il_state>{
S return S;
R return R;
}
 /* IL operators have no clashes */
<il_state>{
LD    return LD;
LDN   return LDN;
ST    return ST;
STN   return STN;
ANDN  return ANDN;
&N    return ANDN2;
ORN   return ORN;
XORN  return XORN;
CAL   return CAL;
CALC  return CALC;
CALCN return CALCN;
RET   return RET;
RETC  return RETC;
RETCN return RETCN;
JMP   return JMP;
JMPC  return JMPC;
JMPCN return JMPCN;
}

 /* TODO: Don't forget the '&' operator */



 /**
  * PATTERN: Values
  * ACTIONS: Store the token value(the same string) in yylval.ID;
  *          Return the token defined by bison
  */
 /* Numeric Literals */
{integer}	     yylval.ID = strdup(yytext); return integer_token;
{real}	     	 yylval.ID = strdup(yytext); return real_token;
{fixed_point}  	 yylval.ID = strdup(yytext); return fixed_point_token;
{binary_integer} yylval.ID = strdup(yytext); return binary_integer_token;
{octal_integer}  yylval.ID = strdup(yytext); return octal_integer_token;
{hex_integer} 	 yylval.ID = strdup(yytext); return hex_integer_token;



 /* TODO: Don't forget change 0 to 'yytext[0]' */
. return 0;
%%
/**
 * @brief  unput_text() return the current token back to the input stream
 * @param  n
 */
void unput_text(unsigned int n) {
	yyless(n);
}
#ifdef TEST_FLEX
int main(int argc, char **argv) {
	int token;
	while(token = yylex()) {
		printf("(line %d)token: %d (%s)\n", yylineno, token, yylval.ID);
	}
	return 0;
}
#endif
