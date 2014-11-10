/*-
 * Copyright (c) 2004 - 2009 CTPP Team
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the CTPP Team nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      CTPP2Syntax.h
 *
 * $CTPP$
 */
#ifndef _CTPP2_SYNTAX_H__
#define _CTPP2_SYNTAX_H__ 1

/**
  @file CTPP2Syntax.h
  @brief Syntax definitions of CTPP2 template language
*/

// First char
#define TMPL_OPEN_SYMBOL          '<'
#define TMPL_CLOSE_SYMBOL         '>'

// Open tag
#define CTPP2_OPEN_TAG            "tmpl"
#define CTPP2_CLOSE_TAG           "tmpl"

// Operators
#define TMPL_VAR_TOK              "var"
#define TMPL_IF_TOK               "if"
#define TMPL_ELSE_TOK             "else"
#define TMPL_ELSIF_TOK            "elsif"
#define TMPL_UNLESS_TOK           "unless"
#define TMPL_FOREACH_TOK          "foreach"
#define TMPL_UDF_TOK              "udf"
#define TMPL_INCLUDE_TOK          "include"
#define TMPL_CALL_TOK             "call"
#define TMPL_COMMENT_TOK          "comment"
#define TMPL_BREAK_TOK            "break"
#define TMPL_BLOCK_TOK            "block"
#define TMPL_VERBOSE_TOK          "verbose"

// Relations
#define TMPL_EQ      "eq"
#define TMPL_NE      "ne"
#define TMPL_GT      "gt"
#define TMPL_LT      "lt"
#define TMPL_GE      "ge"
#define TMPL_LR      "le"

#define TMPL_N_EQ    "=="
#define TMPL_N_NE    "!="
#define TMPL_N_GE    ">="
#define TMPL_N_LE    "<="
#define TMPL_N_GT    ">"
#define TMPL_N_LT    "<"

// Operations
//#define TMPL_PLUS    "+"
//#define TMPL_MINUS   "-"
//#define TMPL_DIV     "/"
//#define TMPL_MUL     "*"
//#define TMPL_IDIV    "DIV"
//#define TMPL_MOD     "MOD"

//#define TMPL_UNPLUS  "+"
//#define TMPL_UNMINUS "-"
//#define TMPL_NOT     "!"

/*
LogicalOrExpr  = LogicalAndExpr { "||" LogicalAndExpr }

LogicalAndExpr = RelExpr { "&&" RelExpr }

RelExpr        = SimpleExpr [Relation SimpleExpr]

SimpleExpr     = Term {AddOp Term}

Term           = Factor {MulOp Factor}

Factor         = Function "(" [ExprList] ")" | number | character | string | "(" LogicalOrExpr ")" | [ UNPLUS | UNMINUS | NOT] Factor

Relation       = N_EQ | N_NE | N_GT | N_LT | N_GE | N_LE | EQ | NE | GT | LT | GE | LE

AddOp          = ADD | SUB

MulOp          = MUL | DIV | IDIV | MOD

ExprList       = LogicalOrExpr {"," LogicalOrExpr}
*/

#endif // _CTPP2_SYNTAX_H__
// End.
