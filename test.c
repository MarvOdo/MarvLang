/**
 * test.c
 * Marvin Odobashi
 * The main test-running file. To use, you need testXX.marv source files in the
 * ./tests directory. Currently, there are only tests for the lexer, so the 
 * output of the lexer is tested against the expected testXX.lex files.
 * 
 * To run all tests, simply run "$ make test; ./test"
*/

#include "lexer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_TESTS 2

//main function to run tests
int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "INPUT ERROR: Too many command line arguments\n");
        exit(1);
    }

    FILE *current_source_file;
    char test_name[20] = "./tests/testXX.marv";
    char test_expected[19] = "./tests/testXX.YYY";
    for (int i = 1; i <= NUM_TESTS; i++)
    {
        //fork, use child process to run test, get output, execvp into "diff" command,
        //and get result
        if (fork() == 0)
        {
            //child process

            //open temporary file to get output of test, and then compare output to
            //expected output
            int testout_fd = open("test.out", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH);
            dup2(testout_fd, 1);
            close(testout_fd);

            //get current test names, and open current test file
            test_name[12] = (char) ((i / 10) + 48);
            test_name[13] = (char) ((i % 10) + 48);
            test_expected[12] = (char) ((i / 10) + 48);
            test_expected[13] = (char) ((i % 10) + 48);
            test_expected[15] = 'l';
            test_expected[16] = 'e';
            test_expected[17] = 'x';
            current_source_file = fopen(test_name, "r");

            //get tokens from code stream / source file
            struct TokenList *token_stream = lexer(current_source_file);

            //don't need current_source_file anymore
            fclose(current_source_file);

            //write output to temp file
            for (int j = 0; j < token_stream->size; j++)
            {
                if (token_stream->array[j].code <= 100)
                {
                    printf("Token Code: %ld\n", token_stream->array[j].code);
                }
                else
                {
                    printf("Token Value: %s\n", token_stream->array[j].value);
                }
            }

            char *diff_argv[3];
            diff_argv[0] = "diff";
            diff_argv[1] = test_expected;
            diff_argv[2] = "test.out";

            //flush output to temp file
            fflush(stdout);
            
            execvp("/usr/bin/diff", diff_argv);
        }
        else
        {
            int status = 0;
            wait(&status);

            //unlink / delete .out file, since we're done with it
            unlink("test.out");

            if (WEXITSTATUS(status) == 0) //if no diff between output and expected, test passed
            {
                fprintf(stderr, "TEST %d - LEXER PASSED\n", i);
            }
            else //failed test / there is a diff between output and expected
            {
                fprintf(stderr, "TEST %d - LEXER FAILED\n", i);
            }
        }
    }

    return 0;
}