#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>
#include "hello.h"

// C stub, notice how we copy values into OCaml and copy it out
char * echo(char *buf) {
    static value * func = NULL;
    if (func == NULL) func = caml_named_value("echo");
    value v = caml_callback(*func, caml_copy_string(buf));
    return String_val(v);
}

// Init the runtime
__attribute__ ((__constructor__))
void init(void) {
    char *caml_argv[1] = { NULL };
    caml_startup(caml_argv);
}