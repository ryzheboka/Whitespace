#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFERSIZE 100

int read_input();
int remove_comments();

int main()
{
  read_input();
  printf("Not implemented yet");
  return 0;
}

int read_input()
{
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