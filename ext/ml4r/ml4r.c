#include <ruby.h>

/* our new native method; it just returns
 * the string "bonjour!" */
static VALUE tce_hello_world(VALUE self) {
  return rb_str_new2("hello world");
}

/* ruby calls this to load the extension */
void Init_ml4r(void) {
  /* assume we haven't yet defined TestCppExtension */
  VALUE klass = rb_define_class("TestCppExtension", rb_cObject);

  /* the hola_bonjour function can be called
   * from ruby as "Hola.bonjour" */
  rb_define_singleton_method(klass, "bonjour", tce_hello_world, 0);
}