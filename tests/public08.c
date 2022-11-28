#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/* 
 * Public test 8 (public08.c)
 *
 * Tests executing some test commands where some do not succeed (some
 * commands have a nonzero exit status).
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

  commands= read_commands("public08.compile-cmds", "public08.test-cmds");

  assert(compile_program(commands) == 1);
  /* only two tests in public08-test-cmds succeeded */
  assert(test_program(commands) == 2);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
