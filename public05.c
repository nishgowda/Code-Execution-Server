#include <stdio.h>
#include <assert.h>
#include "simple-sserver.h"

/* 
 * Public test 5 (public05.c)
 *
 * Tests executing multiple compilation commands, then executing several
 * test commands (running several resulting executable programs).
 *
 * This test will always unavoidably have memory leaks if you run it under
 * valgrind, so fix any other problems that valgrind identifies, but ignore
 * any memory leaks (or just add the --leak-check=no argument when running
 * this test with valgrind).
 *
 */

int main(void) {
  Commands commands;

  commands= read_commands("public05.compile-cmds", "public05.test-cmds");

  assert(compile_program(commands) == 1);
  /* three tests in public05.test-cmds succeeded */
  assert(test_program(commands) == 3);

  printf("No assertion remains unsatisfied!\n");
  
  return 0;
}
