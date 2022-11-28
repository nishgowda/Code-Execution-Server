#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/* 
 * Public test 4 (public04.c)
 *
 * Tests executing one compilation command, then executing one test command
 * (running the resulting executable program).
 *
 * This test will always unavoidably have memory leaks if you run it under
 * valgrind, so fix any other problems that valgrind identifies, but ignore
 * any memory leaks (or just add the --leak-check=no argument when running
 * this test with valgrind).
 *
 */

int main(void) {
  Commands commands;

  commands= read_commands("public04.compile-cmds", "public04.test-cmds");

  assert(compile_program(commands) == 1);
  assert(test_program(commands) == 1);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
