// Code example 1.
int base(int *p [[clang::lifetimebound]]);

int callback() {
    int i = 5;
    return base(&i);
}

/*
  This code examples does get analyzed by the UseAfterLifetimeEnd checker and reported. However, current checkers does not catch this error. Why?
  It is because the analyzer conservatively treats `base` as an opaque call since it could not inline it. Therefore, it has no information when
  an argument will be passed to `base` on what would happen with it. If it can inline it then it could reason about the code just like UseAfterLifetimeEnd.
  See Code example 2.
*/

// Code example 2.
int *base_non_op(int *p) {
  // We do something irrelevant here just to let the analyzer inline the function
  return p; 
}

int *set_ptr_to_int() {
  int i = 5;
  int *ptr = base_non_op(&i);
  return ptr;
}

/*
The analyzer can inline `base_non_op`, so it can reason about the return of stack address escape.
*/

/*------------------------------------------------------------------------------*/
/* Show the binding between the annotated parameter and the object bound to it  */
/*------------------------------------------------------------------------------*/

int foo(int *p [[clang::lifetimebound]]) {
  int some_local_num = 5;
  p = &some_local_num; // The memory regions of the parameter and the local variable are bound to each other

  // rest of the code...
}
