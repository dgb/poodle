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
      qexpr   : '{' <expr>* '}' ;                  \
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

    LLVMContextRef llvm = LLVMContextCreate();

    if (mpc_parse("<stdin>", input, Poodle, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    LLVMContextDispose(llvm);
  }


  return 0;
}
