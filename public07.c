#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/*
 * Public test 7 (public07.c)
 *
 * Tests executing some compilation commands where the compilation does not
 * succeed (a command has a nonzero exit status).
 *
 * THIS TEST WILL CAUSE WARNINGS TO BE PRINTED.  This is NOT wrong.  It is
 * the EXPECTED behavior.  See the first bullet point in Appendix B in the
 * project assignment.
 *
 * This test will always unavoidably have memory leaks if you run it under
 * valgrind, so fix any other problems that valgrind identifies, but ignore
 * any memory leaks (or just add the --leak-check=no argument when running
 * this test with valgrind).
 *
 */

int main(void) {
  Commands commands;

  commands= read_commands("public07.compile-cmds", "public07.test-cmds");

  assert(compile_program(commands) == 0);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
