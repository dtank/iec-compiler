/*
 *  matiec - a compiler for the programming languages defined in IEC 61131-3
 *  Copyright (C) 2003-2011  Mario de Sousa (msousa@fe.up.pt)
 *  Copyright (C) 2007-2011  Laurent Bessard and Edouard Tisserant
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 */

/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/*
 * Declaration of the Abstract Syntax data structure components
 */

/*
 * ABSYNTAX.H
 *
 * This generates the parse tree structure used to bind the components
 * identified by Bison in the correct syntax order. At the end of the
 * Bison analysis the tree is walked in a sequential fashion generating
 * the relavent code.
 */

#ifndef _ABSYNTAX_HH
#define _ABSYNTAX_HH


#include <stdio.h> // required for NULL
#include <map>
#include <string>
#include <vector>
#include <stdint.h>  // required for uint64_t, etc...
#include "main.hh"



/* Forward declaration of the visitor interface
 * declared in the visitor.hh file
 * We cannot include the visitor.hh file, as it will
 * include this same file first, as it too requires references
 * to the abstract syntax classes defined here.
 */
//class visitor_c; // forward declaration


//class symbol_c; // forward declaration







/* Case insensitive string compare */
  /* Case insensitive string compare copied from
   * "The C++ Programming Language" - 3rd Edition
   * by Bjarne Stroustrup, ISBN 0201889544.
   */
class nocasecmp_c {
    public:
      bool operator() (const std::string& x, const std::string& y) const {
        std::string::const_iterator ix = x.begin();
        std::string::const_iterator iy = y.begin();

        for(; (ix != x.end()) && (iy != y.end()) && (toupper(*ix) == toupper(*iy)); ++ix, ++iy);
        if (ix == x.end()) return (iy != y.end());
        if (iy == y.end()) return false;
        return (toupper(*ix) < toupper(*iy));
      };
  };









/* The base class of all symbols */
class symbol_c {

  public:
    /* WARNING: only use this method for debugging purposes!! */
    virtual const char *absyntax_cname(void) {return "symbol_c";};

    /*
     * Line number for the purposes of error checking.
     * Annotated (inserted) by stage1_2
     */
    int first_line;
    int first_column;
    const char *first_file;  /* filename referenced by first line/column */
    long int first_order;    /* relative order in which it is read by lexcial analyser */
    int last_line;
    int last_column;
    const char *last_file;  /* filename referenced by last line/column */
    long int last_order;    /* relative order in which it is read by lexcial analyser */


    /*
     * Annotations produced during stage 3
     */    
    /*** Data type analysis ***/
    std::vector <symbol_c *> candidate_datatypes; /* All possible data types the expression/literal/etc. may take. Filled in stage3 by fill_candidate_datatypes_c class */
    /* Data type of the expression/literal/etc. Filled in stage3 by narrow_candidate_datatypes_c 
     * If set to NULL, it means it has not yet been evaluated.
     * If it points to an object of type invalid_type_name_c, it means it is invalid.
     * Otherwise, it points to an object of the apropriate data type (e.g. int_type_name_c, bool_type_name_c, ...)
     */
    symbol_c *datatype;

    /*** constant folding ***/
    /* During stage 3 (semantic analysis/checking) we will be doing constant folding.
     * That algorithm will anotate the abstract syntax tree with the result of operations
     * on literals (i.e. 44 + 55 will store the result 99).
     * Since the same source code (e.g. 1 + 0) may actually be a BOOL or an ANY_INT,
     * or an ANY_BIT, we need to handle all possibilities, and determine the result of the
     * operation assuming each type.
     * For this reason, we have one entry for each possible type, with some expressions
     * having more than one entry filled in!
     */
    typedef enum { cs_undefined,   /* not defined/not yet evaluated --> const_value is not valid! */
                   cs_non_const,   /* we have determined that expression is not a const value --> const_value is not valid! */
                   cs_const_value, /* const value is valid */
                   cs_overflow     /* result produced overflow or underflow --> const_value is not valid! */
                 } const_status_t;
 
    typedef struct const_value_real64_s {const_status_t status;  real64_t  value;  const_value_real64_s (): status(cs_undefined), value(0.0)   {} } const_value_real64_t;
    typedef struct const_value_int64_s  {const_status_t status;   int64_t  value;  const_value_int64_s  (): status(cs_undefined), value(0)     {} } const_value_int64_t;
    typedef struct const_value_uint64_s {const_status_t status;  uint64_t  value;  const_value_uint64_s (): status(cs_undefined), value(0)     {} } const_value_uint64_t;
    typedef struct const_value_bool_s   {const_status_t status;      bool  value;  const_value_bool_s   (): status(cs_undefined), value(false) {} } const_value_bool_t;

    typedef struct {
      const_value_real64_t _real64; /* status is initialised to UNDEFINED */
      const_value_int64_t   _int64; /* status is initialised to UNDEFINED */
      const_value_uint64_t _uint64; /* status is initialised to UNDEFINED */
      const_value_bool_t     _bool; /* status is initialised to UNDEFINED */
    } const_value_t;
    const_value_t const_value;
    
    /*** Enumeration datatype checking ***/    
    /* Not all symbols will contain the following anotations, which is why they are not declared here in symbol_c
     * They will be declared only inside the symbols that require them (have a look at absyntax.def)
     */
    typedef std::multimap<std::string, symbol_c *, nocasecmp_c> enumvalue_symtable_t;
    

  public:
    /* default constructor */
    symbol_c(int fl = 0, int fc = 0, const char *ffile = NULL /* filename */, long int forder=0, /* order in which it is read by lexcial analyser */
             int ll = 0, int lc = 0, const char *lfile = NULL /* filename */, long int lorder=0  /* order in which it is read by lexcial analyser */
            );

    /* default destructor */
    /* must be virtual so compiler does not complain... */ 
    virtual ~symbol_c(void) {return;};

   // virtual void *accept(visitor_c &visitor) {return NULL;};
};

class token_c: public symbol_c {
  public:
    /* WARNING: only use this method for debugging purposes!! */
    virtual const char *absyntax_cname(void) {return "token_c";};

    /* the value of the symbol. */
    const char *value;

  public:
    token_c(const char *value, 
            int fl = 0, int fc = 0, const char *ffile = NULL /* filename */, long int forder=0, /* order in which it is read by lexcial analyser */
            int ll = 0, int lc = 0, const char *lfile = NULL /* filename */, long int lorder=0  /* order in which it is read by lexcial analyser */
           );
};

#endif /*  _ABSYNTAX_HH */
