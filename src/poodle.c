#include <stdio.h>
#include <stdlib.h>
#include <mpc/mpc.h>
#include <llvm-c/Core.h>

mpc_parser_t *Number;
mpc_parser_t *Symbol;
mpc_parser_t *String;
mpc_parser_t *Comment;
mpc_parser_t *Sexpr;
mpc_parser_t *Qexpr;
mpc_parser_t *Expr;
mpc_parser_t *Poodle;

/*
typedef struct {
  int val;
} poodle_val;
*/

typedef struct {
  LLVMContextRef llvm_ctx;
} poodle_ctx_t;

/*
poodle_val poodle_exec(mpc_result_t parse_result) {
  poodle_exec_ctx *ctx = malloc(sizeof(poodle_exec_ctx));
}
*/

poodle_ctx_t *poodle_ctx_new(void) {
  poodle_ctx_t *ctx = malloc(sizeof(poodle_ctx_t));
  ctx->llvm_ctx = LLVMContextCreate();
  // initialize llvm context
  ctx->llvm_ctx
  return ctx;
}

void poodle_ctx_del(poodle_ctx_t *ctx) {
  LLVMContextDispose(ctx->llvm_ctx);
  free(ctx);
}

void poodle_add_builtin_sum(LLVModuleRef module) {
  LLVMTypeRef param_types[] = { LLVMInt32Type(), LLVMInt32Type() };
  LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0);
  LLVMAddFunction(module, "sum", ret_type);
}

//void poodle_read(poodle_ctx_t *ctx, mpc_ast_t *ast) {
void poodle_read(LLVMModuleRef module, mpc_ast_t *ast) {
  if (strstr(ast->tag, "symbol")) {
    printf("symbol: %s\n", ast->contents);
  }
  if (strstr(ast->tag, "number")) {
    printf("number: %s\n", ast->contents);
  }
}


/*
  alloc an execution context
  convert parse tree into poodle tree (do we need to?)
  walk parse/poodle tree
    does that go right into llvm?
    i guess so?


  step back:

  (+ 1 2) should use add
  (+ 1 2 3 4) could use add vector for cdr of input
    or that could be an optimization...

  environments!

  should be an llvm struct
  has a parent
  would be interesting if immutable
    should you just copy or search over parents?
    seems like we should just copy. cheap.

  anyway, all this stuff needs to get into llvm land.
  how much massaging do we do?
*/

static char input[2048];

int main(int argc, char **argv) {
  puts("Poodle Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");

  Number   = mpc_new("number");
  Symbol   = mpc_new("symbol");
  String   = mpc_new("string");
  Comment  = mpc_new("comment");
  Sexpr    = mpc_new("sexpr");
  Qexpr    = mpc_new("qexpr");
  Expr     = mpc_new("expr");
  Poodle   = mpc_new("poodle");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                              \
      number  : /-?[0-9]+/ ;                       \
      symbol  : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ; \
      string  : /\"(\\\\.|[^\"])*\"/ ;             \
      comment : /;[^\\r\\n]*/ ;                    \
      sexpr   : '(' <expr>* ')' ;                  \
      qexpr   : ''' <expr>* ;                      \
      expr    : <number> | <symbol> | <string>     \
              | <comment> | <sexpr> | <qexpr>;     \
      poodle  : /^/ <expr>* /$/ ;                  \
    ",
    Number, Symbol, String, Comment,
    Sexpr, Qexpr, Expr, Poodle);

  while (1) {
    fputs("poodle> ", stdout);
    fgets(input, 2048, stdin);

    mpc_result_t r;

    //LLVMContextRef llvm = LLVMContextCreate();

    //poodle_ctx_t *ctx = poodle_ctx_new();

    if (mpc_parse("<stdin>", input, Poodle, &r)) {
      //poodle_read(ctx, r.output);
      LLVMModuleRef module = LLVMModuleCreateWithName("poodle");
      poodle_read(module, r.output);
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    poodle_ctx_del(ctx);

    //LLVMContextDispose(llvm);
  }

  return 0;
}
