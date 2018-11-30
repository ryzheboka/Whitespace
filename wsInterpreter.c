#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// contains pointers to three arrays, elements with the same index describe one whitespace instruction
struct whitespace_command{
    char ** notation;   // instruction in whitespace
    void (** execute) (char * argument);   //corresponding function, that should be executed
    bool * has_argument;    // true if the instruction is followed by an argument
} commands;

// describes state of the interpreter
struct current_state{
    char * command;   // command that should be executed next
    char * argument;  // argument that should be passed to the next executed command
    char * current_str;  // same as command, if next symbol belongs to command, else same as argument
    int command_index;  // index, where to find the command, that should be executed next
} state;

int read_input();
int define_commands();
void process_input_line(char * line);
void search_command();
void execute_command();

int ws_argument_to_int(char*);

void push(char*);
void output_number(char*);
void output_char(char*);

const char * charset = "\11\12\40";     // chars that are symbols in Whitespace, everything else is comment

int * stack_ptr;
int * heap;

int main()
{
    define_commands();
    read_input();
    return 0;
}

// fills the arrays that describe implemented ws-commands
int define_commands()
{

    stack_ptr = (int*) calloc(562, sizeof(int));
    heap = (int*) calloc(562, sizeof(int));


    const int max_command_size = 4;
    const int number_of_commands = 3;
    commands.notation = calloc(number_of_commands, sizeof(char *));
    commands.execute = calloc(number_of_commands, sizeof(void(*)(char *)));
    commands.has_argument = calloc(number_of_commands, sizeof(bool));


    *commands.notation = calloc(max_command_size, sizeof(char));
    *commands.notation = "\40\40";   // 40 is ascii code for space in octal
    *commands.execute = &push;
    *commands.has_argument = 1;

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

// reads whole input into a variable, then executes given ws-program
int read_input()
{
    char * buffer = (char*) calloc(1,64);
    char *text = (char*) calloc(1,1);
    state.command = (char*) calloc(1,4);
    state.argument = (char*) calloc(1,64);
    state.current_str = state.command;

    bool stop = false;
    char *quit = "run\n";

    while(!stop && fgets(buffer, 64, stdin)) /* execute by typing enter then "run" then enter*/
    {
        if (strcmp(buffer, quit)==0)
        {
            stop = true;
        }
        else
        {
            text = realloc(text, strlen(text)+1+strlen(buffer));
            strcat(text, buffer);
        }
    }
    process_input_line(text);
    return 0;
 }


// converts a number that is written according to the specification of ws into an integer
int ws_argument_to_int(char* ws_number)
{
    int count, dec_number = 0, power=0;
    for (count = strlen(ws_number)-2; count>0; --count)
    {
        if (*(ws_number+count)==32) dec_number += 0;
        if (*(ws_number+count)==9) dec_number += pow (2,power);
        power++;
    }
    if (*(ws_number)==9)  dec_number = -dec_number;
    return dec_number;
}

// given a sequence of characters, ignores comments and executes commands in the same order
void process_input_line(char* line){
    int count;
    for (count = 0; count<strlen(line); ++count){
        if (strchr(charset, *(line+count)) != NULL){
            *(state.current_str+strlen(state.current_str)) = *(line+count);
            if (state.current_str == state.command) search_command();
            else if (state.current_str != state.command && *(line+count)==10) execute_command();
        }
    }
}
// Searches the current command in the array of commands.
// if the command does't take any argument, executes it and initializes current command to empty
// else stores the index of the command and returns
// in the second case, the command will be executed after getting the argument
void search_command(){
    int i;
    for (i = 0; i < 2; ++i) {
        if(strcmp(*(commands.notation+i), state.command) == 0) {
            if (!*(commands.has_argument+i)) {
            (*(commands.execute+i))(state.argument);
            memset(state.command, '\0', strlen(state.command)*sizeof(char));
            }
            else {
                state.current_str = state.argument;
                state.command_index = i;
            }
            return;
        }
    }
}
// executes the command with current index and initializes current_str, current command and current argument.
void execute_command(){
    (*(commands.execute+state.command_index))(state.argument);
    memset(state.argument, '\0', strlen(state.argument)*sizeof(char));
    memset(state.command, '\0', strlen(state.command)*sizeof(char));
    state.current_str = state.command;
}

//function equivalent to whitespace command "push"
// pushes it's argument onto stack
void push(char* argument)
{

    int n = ws_argument_to_int(argument);
    *stack_ptr = n;
    stack_ptr++;
}
//function equivalent to whitespace command "output number"
// outputs the top integer from the stack
void output_number(char* argument)
{

    int n = *(stack_ptr-1);
    printf("%d", n);
    stack_ptr--;
}
//function equivalent to whitespace command "output character"
// outputs the character corresponding to the top integer from the stack
void output_char(char* argument)
{

    int n = *(stack_ptr-1);
    printf("%c", n);
    stack_ptr--;
}

