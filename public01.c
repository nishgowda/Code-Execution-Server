#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/* 
 * Public test 1 (public01.c)
 *
 * Tests executing one "compliation" command.  (In this test the compilation
 * command does not run the compiler- it's just a simple echo command that
 * prints something, which just tests the ability to execute one compilation
 * command.)
 *
 * This test will always unavoidably have memory leaks if you run it under
 * valgrind, so fix any other problems that valgrind identifies, but ignore
 * any memory leaks (or just add the --leak-check=no argument when running
 * this test with valgrind).
 *
 */

int main(void) {
  Commands commands;

  commands= read_commands("public01.compile-cmds", "public01.test-cmds");

  assert(compile_program(commands) == 1);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
