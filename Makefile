# $Header: /home/johnl/flnb/code/RCS/Makefile.ch3,v 2.1 2009/11/08 02:53:18 johnl Exp $
# Companion source code for "flex & bison", published by O'Reilly
# Media, ISBN 978-0-596-15597-1
# Copyright (c) 2009, Taughannock Networks. All rights reserved.
# See the README file for license conditions and contact info.

# programs in chapter 3
Target=iec_compiler
Source=iec
all: ${Target}

${Target}:	${Source}.l ${Source}.y
	bison -d -o ${Source}.tab.cc ${Source}.y
	flex -o ${Source}.lex.cc ${Source}.l
	gcc  -o $@ ${Source}.tab.cc ${Source}.lex.cc lexer_utils.cc -lstdc++

clean:
	rm -f ${Target} ${Source}.tab.c ${Source}.tab.h ${Source}.lex.c
