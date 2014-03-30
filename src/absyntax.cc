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
 * Definition of the Abstract Syntax data structure components
 */

#include <stdio.h>
#include <stdlib.h>	/* required for exit() */
#include <string.h>

#include "absyntax.hh"
//#include "../stage1_2/iec.hh" /* required for BOGUS_TOKEN_ID, etc... */

#include "main.hh" // required for ERROR() and ERROR_MSG() macros.



/* The base class of all symbols */
symbol_c::symbol_c(
                   int first_line, int first_column, const char *ffile, long int first_order,
                   int last_line,  int last_column,  const char *lfile, long int last_order ) {
  this->first_file   = ffile,
  this->first_line   = first_line;
  this->first_column = first_column;
  this->first_order  = first_order;
  this->last_file    = lfile,
  this->last_line    = last_line;
  this->last_column  = last_column;
  this->last_order   = last_order;
  this->datatype     = NULL;
  this->const_value._real64.status   = cs_undefined;
  this->const_value._int64.status    = cs_undefined;
  this->const_value._uint64.status   = cs_undefined;
  this->const_value._bool.status     = cs_undefined;
}
