#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct whitespace_command{
    char** notation;
    void (**execute) (char* argument);
    bool *has_argument;
} commands;

int read_input();
int define_commands();
int ws_argument_to_int(char*);

void push(char*);
void output_number(char*);

int* stack_ptr;

int *heap;

int main()
{
    printf("Not implemented yet");
    read_input();
    return 0;
}

int define_commands()
{

    stack_ptr = (int*) calloc(562, sizeof(int));
    heap = (int*) calloc(562, sizeof(int));


    const int max_command_size = 4;
    const int number_of_commands = 2;
    commands.notation = calloc(number_of_commands, sizeof(char*));
    commands.execute = calloc(number_of_commands, sizeof(void(*)(char *)));

    *commands.notation = '\40\40';   // 40 is ascii code for space in octal
    *commands.execute = &push;
    *commands.has_argument = true;

    *(commands.notation+1) = '\11\12\40\40';
    *(commands.execute+1) = &output_number;
    *(commands.has_argument+1) = false;

      return 0;
}
int read_input()
{
    char *current_command = calloc(1,4);
    char *current_argument = calloc(1,64);
    char *buffer = calloc(1,64);
    bool stop = false;
    char quit[] = "quit\n";
    while(!stop && fgets(buffer, 64, stdin)) /* break by typing enter then "quit" then enter*/
    {
        if (strcmp(buffer, quit)==0)
        {
            stop = true;
        }
        else
        {
            printf("%s",buffer);
        }
    }

    return 0;
 }

int ws_argument_to_int(char* ws_number)
{
    int dec_number = 0;
    for (int count = 0; count< strlen(ws_number); ++count)
    {
        dec_number += *(ws_number+count)==20 ? 0 : 2^count;
    }
    return dec_number;
}

void push(char* argument)
{
  int n = ws_argument_to_int(argument);
  *stack_ptr = n;
  stack_ptr++;
}

void output_number(char* argument)
{
  int n = *(stack_ptr-1);
  printf("%d", n);
  stack_ptr--;
}

