#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <errno.h> 
#include <sysexits.h> 
#include <fcntl.h>
#include "simple-sserver.h"
#include "safe-fork.h"
#include "split.h"

/*
 * simple-sserver.c
 *
 * A simple submit server simulation using exec processes and process
 * control. This program will run commands to compile given student code and
 * others to test the compiled student code. Uses a dynamic linear data structure
 * Target and Commands to handle the data.
 *
 */ 


/* a simple helper function used to detect the number
 * of lines in a given file name */
static int line_numbers(const char file[]) {
  FILE *f;
  int count = 0;
  char c;
  /* open the file in readonly mode */
  f =  fopen(file, "r");
  for (c = getc(f); c != EOF; c= getc(f)) {
    if (c == '\n')
      count++; /* increment line when we see newline */
  }
  fclose(f);
  return count;
}

/* helper function to get the proper size of
 * a dynamically allocated array */
static int get_arr_size(char *arr[]) {
  int i = 0;
  while (arr[i] != NULL) 
    i++;
  return i;
}

/* helper function to free all memory in a dynamic
 * array of strings and the array itself */
static void clear_arr_list(char *arrs[]) {
    int i, n;
    if (arrs != NULL) {
        /* grab the arr size */
        n = get_arr_size(arrs);
        /* loop through each element */
        for (i = 0; i < n; i ++) {
            if (arrs[i] != NULL) {
                free(arrs[i]); /* free element in array */
                arrs[i] = NULL; /* set to NULL */
            }
        }
        /* free array itself and set it to NULL */
        free(arrs);
        arrs = NULL;
    }
}

/* helper function to handle input redirection in test_command, 
 * just adjusts the array and sets the filename */
static int redirect_input(char **args, char *input_filename) {
  int i, j;
  for(i = 0; args[i] != NULL; i++) {

    /* check if we find input redirection */
    if (args[i][0] == '<') {
      free(args[i]); /* free the argument */
      args[i] = NULL;
      
    /* now assign the input filename will be the next argument */  
      if (args[i+1] != NULL)
          strcpy(input_filename, args[i+1]);
      else 
	      return -1;
      
      /* loop through array and move element to the next
       * in the array */
      for (j = i; args[j-2] != NULL; j++)
        args[j] = args[j+2];
       
      return 1;
    }
  }

  return 0;
}

/* helper function to handle output redirection in test_command, 
 * just adjusts the array and sets the filename */
static int redirect_output(char **args, char *output_filename) {
  int i, j;

  for(i = 0; args[i] != NULL; i++) {
  
    /* check for output redirection in array */
    if(args[i][0] == '>') {
      free(args[i]); /* free the sign in the array */

      /* copy the filename to the output filename */
      if (args[i+1] != NULL)
          strcpy(output_filename, args[i+1]);
      else
	      return -1;
      
  
      /* loop through array and move element to the next
       * in the array */
      for(j = i; args[j-1] != NULL; j++) {
	      args[j] = args[j+2];
      }

      return 1;
    }
  }
  return 0;
}

/* helper function to test if the array has input redirection */
static int has_io(char **arrs) {
  int i = 0;

  /* loop through the array */
  while (arrs[i] != NULL) {
  /* check if we find a > or < and return 1 */
    if (strcmp(arrs[i], ">") == 0 || strcmp(arrs[i], "<") == 0)
      return 1;
    i++;
  }
  return 0;
}

/* read_commands reads two files to copy each line and set it in the Command
 * data structure and returns it. If both filenames are not valid we exit early
 * with a status of 1 */
Commands read_commands(const char compile_cmds[], const char test_cmds[]) {
  Commands cmd;
  char line1[LINE_MAX], line2[LINE_MAX]; 
  Target *new_target;
  FILE *comp_file, *test_file;

  /* early set the command to NULL head and tail */
  cmd.num_targets = 0;
  cmd.head = NULL;
  cmd.tail = NULL;
  /* grab the compile file and test file */
  comp_file = fopen(compile_cmds, "r");
  test_file = fopen(test_cmds, "r");
  /* early exit with 1 if both are not valid */
  if (comp_file == NULL || test_file == NULL)
      exit(1);

  /* case 1: the files have different number of lines this can affect
   * how we read the data in and assign it in the datastructure */
  if (line_numbers(compile_cmds) != line_numbers(test_cmds)) {
   /* loop through each line of the first file */
    while(fgets(line1, LINE_MAX, comp_file)) {

            /* create a target and assign the compilation commands */
              new_target = malloc(sizeof(Target));
              new_target->comp_cmds = split(line1);
              new_target->tid = cmd.num_targets;
              /* increment the number of targets */
              cmd.num_targets++;
              
              /* assign the command to the list */
              /* if the tial is null we have an empty list so we can
               * just assign it to the head and tail */
              if (cmd.tail == NULL) {
                  cmd.tail = new_target;
                  cmd.head = new_target;
                  cmd.tail->next = NULL;
              } else if (cmd.tail != NULL) {
                /* assign the next's tail to the target */
                  cmd.tail->next = new_target;
                  cmd.tail = new_target;
                  new_target->next = NULL;
              }  
      }
    /* close both files and return the command */
    fclose(test_file);
    fclose(comp_file);
    return cmd;
  } 
  /* case 2: both files have an equal number of commands */
  /* loop through both files and assign data to target */
  while (fgets(line1, LINE_MAX, comp_file), fgets(line2, LINE_MAX, test_file)) {
              /* create the target and assign the compile and test commands
               * to the result of split(line1) and split(line2) */
              new_target = malloc(sizeof(Target));
              new_target->comp_cmds = split(line1);
              new_target->test_cmds = split(line2);
              new_target->tid = cmd.num_targets;
              /* increment the number of targets cmd has */
              cmd.num_targets++;
              /* if list is empty just assign the target to the tail */
              if (cmd.tail == NULL) {
                  cmd.tail = new_target;
                  cmd.head = new_target;
                  cmd.tail->next = NULL;
              } else if (cmd.tail != NULL) {
                  cmd.tail->next = new_target; /* assign the next to target */
                  cmd.tail = new_target;
                  new_target->next = NULL;
              }  
      }
  /* close both files we opened */
    fclose(test_file);
    fclose(comp_file);
    /* return command */
    return cmd;
}

/* clear commands frees all dynamically allocated memory in
 * a given command datastructure */
void clear_commands(Commands *const commands) {
   Target *curr = NULL; 
   if (commands != NULL) {
        /* loop through each element in commands */
     while (commands->head != NULL) {
       /* set curr to head */
          curr = commands->head;
          /* move head to next */
          commands->head = commands->head->next;
          /* free all memory in both string arrays of comp_cmds and test_cmds */
          clear_arr_list(curr->comp_cmds);
          clear_arr_list(curr->test_cmds);
          /* free curr and set it to NULL */
          free(curr);
          curr = NULL;
        }
    }
}

/* compile_program executes the compilation commands in the given command
 * datastructure. If the status code of a command is not 0 we return 
 * FAILED_COMPILATION otherwise we return SUCCESSFUL_COMPILATION */
int compile_program(Commands commands) {
    Target *curr;
    pid_t child;
    int status;
    /* set curr to the head */
    curr = commands.head;
    while (curr != NULL) {
      /* fork the child */
        child = safe_fork();
        if (child > 0) {
          /* wait for the status of the exec */
            wait(&status);
            /* check the status of the exec if it's not valid return failed */
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
              return FAILED_COMPILATION;
        } else if (child == 0) {
            /* execute the compilation command */
            if (curr != NULL)
               execvp(curr->comp_cmds[0], curr->comp_cmds);
        }
        curr = curr->next;
    }
    /* return success */
    return SUCCESSFUL_COMPILATION;
}

/* test_program executes the test commands in the given command 
 * datastructure. Returns the number of successful exec commands
 * and also handles input and output redirection */
int test_program(Commands commands) {
    Target *curr;
    pid_t child;
    int status;
    int n;
    int inp, out, fd;
    char *inp_filename, *out_filename;
    int total_success = 0;
    /* check if we have targets */
    if (commands.num_targets > 0) {
      /* set curr to the head */
        curr = commands.head;
        while (curr != NULL) {
            n = get_arr_size(curr->test_cmds);
            /* check if there's input and output redirection */
            if (has_io(curr->test_cmds)) {
              /* allocate enough memory for the filenames */
              inp_filename = malloc(strlen(curr->test_cmds[n-1]));
              out_filename = malloc(strlen(curr->test_cmds[n-1]));
              /* check if there exists input and output redirecion in the 
               * current test commands */
              inp = redirect_input(curr->test_cmds, inp_filename);
              out = redirect_output(curr->test_cmds, out_filename);
              /* if we have input redirect then open the file in 
               * read only mode to pass the data later to the exec command */
              if (inp) 
                  fd = open(inp_filename, O_RDONLY, 0666);
              /* if we have output redirection then open the file with the 
               * file flags for permissions */
              if (out)
                fd = open(out_filename, FILE_FLAGS, FILE_MODE);
            }
            /* fork the child now */
            child = safe_fork();
            if (child > 0) {
              /* wait to get the status of the command */
                wait(&status);
                /* if the status is valid then increment the total_success */
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) 
                    total_success++;
            } else if (child == 0) {
                if (curr != NULL && curr->test_cmds != NULL) {
                  /* if inp occurs then dup and close the file */
                  if (inp) {
                    dup2(fd, 0);
                    close(fd);
                  } 
                  /* if out occurs then dup and close the file */
                  if (out){
                    dup2(fd, 0);
                    close(fd);
                  }
                 /* now we can execute the commands regardless of io redirection*/ 
                   execvp(curr->test_cmds[0], curr->test_cmds);
                  }
                 }
            curr = curr->next;
        }
    }
    /* return the number of success */
    return total_success;
}

