#include <stdint.h>

typedef uintptr_t	cell_ft;
typedef cell_ft *	label_ft;

enum primitive {
    prim_exit,
    prim_nop,
    prim_literal,
    prim_dup,
    prim_swap,
    prim_drop,
    prim_minus,
    prim_zequal,
    prim_skip,
    prim_zskip,
    prim_addr,
    prim_fetch,
    prim_store,
    NPRIMITIVES
};

extern void compile(enum primitive, ...);
extern label_ft curloc(void);
extern void patch(label_ft);
extern unsigned long interpret(void);
extern void compile_init(void);
extern void compile_fini(void);

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef struct vmstate *	vmstate_p;

struct vmstate {
    cell_ft *	ip;
    cell_ft *	sp;
    cell_ft *	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
};

extern unsigned long execute(vmstate_p vm, cell_ft *entry_xt);


#define NOP()		(compile(prim_nop))
#define LITERAL(n)	(compile(prim_literal, (n)))
#define DUP()		(compile(prim_dup))
#define SWAP()		(compile(prim_swap))
#define DROP()		(compile(prim_drop))
#define MINUS()		(compile(prim_minus))
#define ZEROP()		(compile(prim_zequal))
#define ADDR()		(compile(prim_addr))
#define FETCH()		(compile(prim_fetch))
#define STORE()		(compile(prim_store))

#define LABEL()		(curloc())
#define SKIP(label)	(compile(prim_skip, (label)))
#define ZSKIP(label)	(compile(prim_zskip, (label)))
#define PATCH(label)	(patch((label)))
