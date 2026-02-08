#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int secret_backdoor()
{
  char s[128];

  fgets(s, 128, stdin);
  return system(s);
}

char *set_msg(long long buffer) // char buffer[140]
{
  char inputBuffer[1024];

  memset(inputBuffer, 0, sizeof(inputBuffer));
  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(inputBuffer, 1024, stdin);
  return strncpy((char *)buffer, inputBuffer, *(int *)(buffer + 180));
}

int set_username(long long buffer) // char buffer[140]
{
  char inputBuffer[140];
  int i;

  memset(inputBuffer, 0, 128u);
  puts(">: Enter your username");
  printf(">>: ");
  fgets(inputBuffer, 128, stdin);

  for (i = 0; i <= 40 && inputBuffer[i]; ++i)
    *(char *)(buffer + i + 140) = inputBuffer[i];

  return printf(">: Welcome, %s", (const char *)(buffer + 140));
}

int handle_msg()
{
  char buffer[140];
  long long v2; // 8 bytes
  long long v3; // 8 bytes
  long long v4; // 8 bytes
  long long v5; // 8 bytes
  long long v6; // 8 bytes
  int charsToCopy;       // 4 bytes

  v2 = 0;
  v3 = 0;
  v4 = 0;
  v5 = 0;
  v6 = 0;
  charsToCopy = 140;
  set_username(buffer);
  set_msg(buffer);
  return puts(">: Msg sent!");
}

int main(int argc, const char **argv, const char **envp)
{
  puts(
      "--------------------------------------------\n"
      "|   ~Welcome to l33t-m$n ~    v1337        |\n"
      "--------------------------------------------");
  handle_msg();
  return 0;
}

