#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct whitespace_command{
    char ** notation;
    void (** execute) (char * argument);
    bool * has_argument;
} commands;

struct current_state{
    char * command;
    char * argument;
    char * current_str;
    int command_index;
} state;

int read_input();
int define_commands();
void process_input_line(char * line);
void search_command();
void execute_command();

int ws_argument_to_int(char*);

void push(char*);
void output_number(char*);

const char * charset = "\11\12\40";

int * stack_ptr;
int * heap;

int main()
{
    printf("Not fully implemented yet \n");
    define_commands();
    read_input();
    return 0;
}

int define_commands()
{

    stack_ptr = (int*) calloc(562, sizeof(int));
    heap = (int*) calloc(562, sizeof(int));


    const int max_command_size = 4;
    const int number_of_commands = 2;
    commands.notation = calloc(number_of_commands, sizeof(char *));
    commands.execute = calloc(number_of_commands, sizeof(void(*)(char *)));
    commands.has_argument = calloc(number_of_commands, sizeof(bool));


    *commands.notation = calloc(max_command_size, sizeof(char));
    *commands.notation = "\40\40";   // 40 is ascii code for space in octal
    *commands.execute = &push;
    *commands.has_argument = true;

    *(commands.notation+1) = calloc(max_command_size, sizeof(char));
    *(commands.notation+1) = "\11\12\40\11";
    *(commands.execute+1) = &output_number;
    *(commands.has_argument+1) = false;

    *(commands.notation+2) = calloc(max_command_size, sizeof(char));
    *(commands.notation+2) = "\11\12\40\40";
    *(commands.execute+2) = &output_char;
    *(commands.has_argument+2) = false;
    return 0;
}
int read_input()
{
    char * buffer = (char*) calloc(1,64);
    state.command = (char*) calloc(1,4);
    state.argument = (char*) calloc(1,64);
    state.current_str = state.command;

    bool stop = false;
    char *quit = "quit\n";

    while(!stop && fgets(buffer, 64, stdin)) /* break by typing enter then "quit" then enter*/
    {
        if (strcmp(buffer, quit)==0)
        {
            stop = true;
        }
        else
        {
            process_input_line(buffer);
        }
    }

    return 0;
 }

int ws_argument_to_int(char* ws_number)
{
    int count, dec_number = 0, power=0;
    for (count = strlen(ws_number)-2; count>-1; --count)
    {
        if (*(ws_number+count)==32) dec_number += 0;
        if (*(ws_number+count)==9) dec_number += pow (2,power);
        power++;
    }
    return dec_number;
}

void process_input_line(char* line){
    int count;
    for (count = 0; count<strlen(line); ++count){
        if (strchr(charset, *(line+count)) != NULL){
            *(state.current_str+strlen(state.current_str)) = *(line+count);
            if (state.current_str == state.command) search_command();
            if (state.current_str != state.command && *(line+count)==10) execute_command();
        }
    }
}

void search_command(){
    int i;
    for (i = 0; i < 2; ++i) {
        if(strcmp(*(commands.notation+i), state.command) == 0) {
            if (!*(commands.has_argument+i)) (*(commands.execute+i))(state.argument);
            else {
                state.current_str = state.argument;
                state.command_index = i;
            }
            return;
        }
    }
}

void execute_command(){
    (*(commands.execute+state.command_index))(state.argument);
    memset(state.argument, '\0', strlen(state.argument)*sizeof(char));
    memset(state.command, '\0', strlen(state.command)*sizeof(char));
    state.current_str = state.command;
}

void push(char* argument)
{
    printf("Executing Push\n");

    int n = ws_argument_to_int(argument);
    *stack_ptr = n;
    stack_ptr++;
}

void output_number(char* argument)
{
    printf("Executing Output Number\n");

    int n = *(stack_ptr-1);
    printf("%d", n);
    stack_ptr--;
}

void output_char(char* argument)
{
    printf("Executing Output Char\n");

    int n = *(stack_ptr-1);
    printf("%c", n);
    stack_ptr--;
}

