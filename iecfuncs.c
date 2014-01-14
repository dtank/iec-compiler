/* Companion source code for "flex & bison", published by O'Reilly
 * Media, ISBN 978-0-596-15597-1
 * Copyright (c) 2009, Taughannock Networks. All rights reserved.
 * See the README file for license conditions and contact info.
 * $Header: /home/johnl/flnb/code/RCS/fb3-2funcs.c,v 2.1 2009/11/08 02:53:18 johnl Exp $
 */
/*
 * helper functions for fb3-2
 */
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include <string.h>
#  include <math.h>
#  include "iec.h"

/* symbol table */
/* hash a symbol */
static unsigned
symhash(char *sym)
{
  unsigned int hash = 0;
  unsigned c;

  while(c = *sym++) hash = hash*9 ^ c;

  return hash;
}
void init_symtab(void) {
	char *sym = "ADD";
	struct symbol *arg2 = newsymbol("arg2", 0, NULL, NULL);
	struct symlist *arg2list = newsymlist(arg2, NULL);
	struct symbol *arg1 = newsymbol("arg1", 0, NULL, NULL);
	struct symlist *arg1list = newsymlist(arg1, arg2list);
    struct symbol *sp = &symtab[symhash(sym)%NHASH];
    sp->name = sym;
    sp->value = 0;
    sp->func = NULL;
    sp->syms = arg1list;
}
struct symbol *newsymbol(char *name, double value, struct ast *func, struct symlist *syms) {
	struct symbol *sym = malloc(sizeof(struct symbol));
	if(!sym) {
		yyerror("out of space");
		exit(0);
	}
	sym->name = name; /* maybe strdup() is necessary */ 
	sym->value = value;
	sym->func = func;
	sym->syms = syms;
	return sym;
}
struct symbol *
lookup(char* sym)
{
  struct symbol *sp = &symtab[symhash(sym)%NHASH];
  int scount = NHASH;		/* how many have we looked at */

  while(--scount >= 0) {
    if(sp->name && !strcmp(sp->name, sym)) { return sp; }

    if(!sp->name) {		/* new entry */
      sp->name = strdup(sym);
      sp->value = 0;
      sp->func = NULL;
      sp->syms = NULL;
      return sp;
    }

    if(++sp >= symtab+NHASH) sp = symtab; /* try the next entry */
  }
  yyerror("symbol table overflow\n");
  abort(); /* tried them all, table is full */

}



struct ast *
newast(int nodetype, struct ast *l, struct ast *r)
{
  struct ast *a = malloc(sizeof(struct ast));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *
newnum(double d)
{
  struct numval *a = malloc(sizeof(struct numval));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'K';
  a->number = d;
  return (struct ast *)a;
}


struct ast *
newfunc(struct symbol *s, struct ast *l)
{
  struct fncall *a = malloc(sizeof(struct fncall));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'F';
  a->l = l;
  a->s = s;
  return (struct ast *)a;
}


struct ast *
newref(struct symbol *s)
{
  struct symref *a = malloc(sizeof(struct symref));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = 'N';
  a->s = s;
  return (struct ast *)a;
}

struct ast *
newasgn(struct symbol *s, struct ast *v)
{
  struct symasgn *a = malloc(sizeof(struct symasgn));
  
  if(!a) {
    yyerror("out of space");
    exit(0);
  }
  a->nodetype = '=';
  a->s = s;
  a->v = v;
  return (struct ast *)a;
}


struct symlist *
newsymlist(struct symbol *sym, struct symlist *next)
{
  struct symlist *sl = malloc(sizeof(struct symlist));
  
  if(!sl) {
    yyerror("out of space");
    exit(0);
  }
  sl->sym = sym;
  sl->next = next;
  return sl;
}

void
symlistfree(struct symlist *sl)
{
  struct symlist *nsl;

  while(sl) {
    nsl = sl->next;
    free(sl);
    sl = nsl;
  }
}


static double callbuiltin(struct fncall *);

double
eval(struct ast *a)
{
  double v;

  if(!a) {
    yyerror("internal error, null eval");
    return 0.0;
  }

  switch(a->nodetype) {
    /* constant */
  case 'K': v = ((struct numval *)a)->number; break;

    /* name reference */
  case 'N': v = ((struct symref *)a)->s->value; break;

    /* assignment */
  case '=': v = ((struct symasgn *)a)->s->value =
      eval(((struct symasgn *)a)->v); break;

    /* expressions */
  case '+': v = eval(a->l) + eval(a->r); break;
  case '-': v = eval(a->l) - eval(a->r); break;
  case '*': v = eval(a->l) * eval(a->r); break;
  case '/': v = eval(a->l) / eval(a->r); break;
  case '|': v = fabs(eval(a->l)); break;
  case 'M': v = -eval(a->l); break;

  case 'L': eval(a->l); v = eval(a->r); break;

  case 'F': v = callbuiltin((struct fncall *)a); break;

  default: printf("internal error: bad node %c\n", a->nodetype);
  }
  return v;
}

static double
callbuiltin(struct fncall *f)
{
  struct symbol *fn = f->s;
  struct ast *args = f->l;
  struct symlist *sl;
  double *oldval, *newval;
  double sum;
  int nargs;
  int i;

  /* count the arguments */
  sl = fn->syms;
  for(nargs = 0; sl; sl = sl->next)
    nargs++;
  /* prepare to save them */
  newval = (double *)malloc(nargs * sizeof(double));
  /* evaluate the arguments */
  for(i = 0; i < nargs; i++){
    if(args->nodetype == 'L') {	/* if this is a list node */
      newval[i] = eval(args->l);
      args = args->r;
    } else {			/* if it's the end of the list */
      newval[i] = eval(args);
      args = NULL;
    }
  }
  /* output the sequence of calling functions & pointer to args to file "prog" */
 /* fprintf(progfp, "function:%s  ", fn->name);
  for(i = 0; i < nargs; i++) {
	  fprintf(progfp, "arg%d:%d ", i+1, rownumber+i);
  }
  fprintf(progfp, "\n");
  /* output the list of args(Key/Value mapping) to file "args" */
  /*fprintf(argsfp, "args of %s:\n", fn->name);
  for(i = 0; i < nargs; i++){
    fprintf(argsfp, "%d %g\n", rownumber++, newval[i]);
    sum += newval[i];
  }*/
  free(newval);
    return sum;
}
void emitfunc(struct fncall *f) {
	
  struct symbol *fn = f->s;
  struct ast *args = f->l;
  struct symlist *sl;
  double *oldval, *newval;
  int nargs;
  int i;

  /* count the arguments */
  sl = fn->syms;
  for(nargs = 0; sl; sl = sl->next)
    nargs++;
  /* prepare to save them */
  newval = (double *)malloc(nargs * sizeof(double));
  /* evaluate the arguments */
  for(i = 0; i < nargs; i++){
    if(args->nodetype == 'L') {	/* if this is a list node */
      newval[i] = eval(args->l);
      args = args->r;
    } else {			/* if it's the end of the list */
      newval[i] = eval(args);
      args = NULL;
    }
  }
  /* output the sequence of calling functions & pointer to args to file "prog" */
  fprintf(progfp, "function:%s  ", fn->name);
  for(i = 0; i < nargs; i++) {
	  fprintf(progfp, "arg%d:%d ", i+1, argno+i);
  }
  fprintf(progfp, "\n");
  /* output the list of args(Key/Value mapping) to file "args" */
  /*fprintf(argsfp, "args of %s:\n", fn->name);
  for(i = 0; i < nargs; i++){
    fprintf(argsfp, "%d %g\n", argno++, newval[i]);
  }*/
}
void emitasgn(struct symasgn *ast) {
    double value = eval(ast->v);
	fprintf(argsfp, "%d %g\n", argno++, value);
}
/* TODO emitexplist */

void
treefree(struct ast *a)
{
  switch(a->nodetype) {

    /* two subtrees */
  case '+':
  case '-':
  case '*':
  case '/':
  case 'L':
    treefree(a->r);

    /* one subtree */
  case '|':
  case 'M': case 'F':
    treefree(a->l);

    /* no subtree */
  case 'K': case 'N':
    break;

  case '=':
    free( ((struct symasgn *)a)->v);
    break;

  default: printf("internal error: free bad node %c\n", a->nodetype);
  }	  
  
  free(a); /* always free the node itself */

}

void
yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int
main()
{
	init_symtab();
	argsfp = fopen("args", "w");
	progfp = fopen("prog", "w");
    printf("> "); 
    yyparse();
	fclose(argsfp);
	fclose(progfp);
	return 0;
}

/* debugging: dump out an AST */
int debug = 0;
void
dumpast(struct ast *a, int level)
{

  printf("%*s", 2*level, "");	/* indent to this level */
  level++;

  if(!a) {
    printf("NULL\n");
    return;
  }

  switch(a->nodetype) {
    /* constant */
  case 'K': printf("number %4.4g\n", ((struct numval *)a)->number); break;

    /* name reference */
  case 'N': printf("ref %s\n", ((struct symref *)a)->s->name); break;

    /* assignment */
  case '=': printf("= %s\n", ((struct symref *)a)->s->name);
    dumpast( ((struct symasgn *)a)->v, level); return;

    /* expressions */
  case '+': case '-': case '*': case '/': case 'L':
    printf("binop %c\n", a->nodetype);
    dumpast(a->l, level);
    dumpast(a->r, level);
    return;

  case '|': case 'M': 
    printf("unop %c\n", a->nodetype);
    dumpast(a->l, level);
    return;
	              
  case 'F':
    printf("builtin %s\n", ((struct fncall *)a)->s->name);
    dumpast(a->l, level);
    return;

  default: printf("bad %c\n", a->nodetype);
    return;
  }
}
