#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct whitespace_command{
    int  notation_length;
    char *notation;
    void (*execute) ();
} command;

int read_input();

int main()
{
  printf("Not implemented yet");
  return 0;
}

int read_input()
{
    const int BUFFERSIZE = 100;
    char *text = calloc(1,1), buffer[BUFFERSIZE];
    bool stop = false;
    char quit[] = "quit\n";
    while(!stop && fgets(buffer, BUFFERSIZE , stdin)) /* break by typing enter then "quit" then enter*/
    {
    if (strcmp(buffer, quit)==0)
    {
        stop = true;
    }
    text = realloc(text, strlen(text)+1+strlen(buffer));
    strcat(text, buffer);
    }
    printf("\ntext:\n%s",text);

    return 0;
 }