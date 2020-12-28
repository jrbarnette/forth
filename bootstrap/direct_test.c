/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "direct.h"
#include "names.h"
#include "cmdline.h"

#undef PICK
#undef DEPTH

/*
 * direct_test.c - Direct-threaded test program.
 */

#define CALL(f)		X(i_call) { .ip = (f) },


#define AND		X(x_and)
#define C_FETCH		X(x_c_fetch)
#define C_STORE		X(x_c_store)
#define DEPTH		X(x_depth)
#define DROP		X(x_drop)
#define DUP		X(x_dup)
#define EMIT		X(x_emit)
#define EQUALS		X(x_equals)
#define EXIT		X(x_exit)
#define FETCH		X(x_fetch)
#define FILL		X(x_fill)
#define GREATER_THAN	X(x_greater_than)
#define INVERT		X(x_invert)
#define LESS_THAN	X(x_less_than)
#define LSHIFT		X(x_lshift)
#define MINUS		X(x_minus)
#define MOD		X(x_mod)
#define MOVE		X(x_move)
#define M_STAR		X(x_m_star)
#define NEGATE		X(x_negate)
#define OR		X(x_or)
#define OVER		X(x_over)
#define PICK		X(x_pick)
#define PLUS		X(x_plus)
#define QUESTION_DUP	X(x_question_dup)
#define ROT		X(x_rot)
#define RSHIFT		X(x_rshift)
#define R_FETCH		X(x_r_fetch)
#define R_FROM		X(x_r_from)
#define SLASH		X(x_slash)
#define SLASH_MOD	X(x_slash_mod)
#define STAR		X(x_star)
#define STORE		X(x_store)
#define SWAP		X(x_swap)
#define TO_R		X(x_to_r)
#define TWO_R_FETCH	X(x_two_r_fetch)
#define TWO_R_FROM	X(x_two_r_from)
#define TWO_SLASH	X(x_two_slash)
#define TWO_STAR	X(x_two_star)
#define TWO_TO_R	X(x_two_to_r)
#define U_LESS		X(x_u_less)
#define U_M_SLASH_MOD	X(x_u_m_slash_mod)
#define U_M_STAR	X(x_u_m_star)
#define XOR		X(x_xor)

#define CHAR_PLUS	L(CHAR_SIZE) PLUS
#define CELL_PLUS	L(CELL_SIZE) PLUS

#define OPERAND		DUP FETCH SWAP CELL_PLUS

#define STR(s)		S(s) N(sizeof (s) - 1)
#define PRIM(nm, hdlr)	CALL(addname) X(hdlr) STR(nm)
#define FLAGS(f)	CALL(setflags) N(NAME_TYPE_ ## f)

#define DICTIONARY_CELLS	(0x10000)
#define DICTIONARY_SIZE		(sizeof (dictionary))

cell_ft dictionary[DICTIONARY_CELLS];


PRIM_HDLR(i_call)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, ip + 1);
    return ip->ip;
}


DIRECT_FORTH(aligned) // {
    // ( here )
    L(CELL_SIZE-1) PLUS L(-CELL_SIZE) AND
END_DIRECT // }

DIRECT_FORTH(c_comma) // {
    // ( here cell )
    OVER C_STORE CHAR_PLUS
END_DIRECT // }

DIRECT_FORTH(comma) // {
    // ( here cell )
    OVER STORE CELL_PLUS
END_DIRECT // }

DIRECT_FORTH(addname) // {
    // ( wordlist here )
    DUP ROT CALL(comma)
    R_FROM SWAP TO_R
    OPERAND OPERAND OPERAND
    R_FROM SWAP TO_R
    // ( wordlist hdlr c-addr len here )
    OVER CALL(c_comma) OVER OVER PLUS CALL(aligned) TO_R SWAP MOVE
    R_FROM SWAP CALL(comma)
    // ( wordlist here )
END_DIRECT // }


DIRECT_FORTH(setflags) // {
    // ( wordlist here )
    OVER R_FROM OPERAND TO_R SWAP
    // ( wordlist here flags name )
    CELL_PLUS DUP TO_R C_FETCH OR R_FROM C_STORE
    // ( wordlist here )
END_DIRECT // }


DIRECT_FORTH(direct_program) // {
    L(0) L(dictionary)
    PRIM(">R",		x_to_r)			FLAGS(NO_INTERPRET)
    PRIM("?DUP",	x_question_dup)
    PRIM("DEPTH",	x_depth)
    PRIM("DROP",	x_drop)
    PRIM("DUP",		x_dup)
    PRIM("OVER",	x_over)
    PRIM("R>",		x_r_from)		FLAGS(NO_INTERPRET)
    PRIM("R@",		x_r_fetch)		FLAGS(NO_INTERPRET)
    PRIM("ROT",		x_rot)
    PRIM("SWAP",	x_swap)

    PRIM("2>R",		x_two_to_r)		FLAGS(NO_INTERPRET)
    PRIM("2R>",		x_two_r_from)		FLAGS(NO_INTERPRET)
    PRIM("2R@",		x_two_r_fetch)		FLAGS(NO_INTERPRET)
    PRIM("PICK",	x_pick)

    PRIM("+",		x_plus)
    PRIM("-",		x_minus)
    PRIM("2*",		x_two_star)
    PRIM("2/",		x_two_slash)
    PRIM("<",		x_less_than)
    PRIM("=",		x_equals)
    PRIM(">",		x_greater_than)
    PRIM("AND",		x_and)
    PRIM("INVERT",	x_invert)
    PRIM("LSHIFT",	x_lshift)
    PRIM("NEGATE",	x_negate)
    PRIM("OR",		x_or)
    PRIM("RSHIFT",	x_rshift)
    PRIM("U<",		x_u_less)
    PRIM("XOR",		x_xor)

    PRIM("!", 		x_store)
    PRIM("@",		x_fetch)
    PRIM("C!",		x_c_store)
    PRIM("C@",		x_c_fetch)
    PRIM("MOVE",	x_move)

    PRIM("DO-LITERAL",	do_literal)
END_DIRECT // }


struct options forth_options;


int
main(int argc, char *argv[])
{
    vmstate_ft	vmstate;
    char_ft	ident[NAME_MAX_LENGTH + 1];

    process_args(argc, argv, &forth_options);
    direct_execute(&vmstate, direct_program);
    POP(&vmstate);
    name_ft wordlist = (name_ft) POP(&vmstate);
    while (wordlist != NULL) {
	size_t len = NAME_LENGTH(wordlist);
	memcpy(ident, wordlist->ident, len);
	ident[len] = '\0';
	printf("%s\n", ident);
	wordlist = wordlist->prev;
    }
    return EXIT_SUCCESS;
}
