#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/*
 * Public test 2 (public02.c)
 *
 * Tests executing multiple compilation commands.  (In this test the
 * compilation commands do not run the compiler- they're just simple echo
 * commands that print something, which just tests the ability to execute
 * multiple compilation commands.)
 *
 * This test will always unavoidably have memory leaks if you run it under
 * valgrind, so fix any other problems that valgrind identifies, but ignore
 * any memory leaks (or just add the --leak-check=no argument when running
 * this test with valgrind).
 *
 */

int main(void) {
  Commands commands;

  commands= read_commands("public02.compile-cmds", "public02.test-cmds");

  assert(compile_program(commands) == 1);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
