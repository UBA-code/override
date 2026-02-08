#include <stdio.h>
#include <string.h>
#include <stdint.h>

int get_unum()
{
  unsigned int v1[3];

  v1[0] = 0;
  fflush(stdout);
  __isoc99_scanf("%u", v1); // read unsigned int
  clear_stdin();
  return v1[0];
}

int store_number(int arrNum)
{
  unsigned int number;
  unsigned int index;

  printf(" Number: ");
  number = get_unum();
  printf(" Index: ");
  index = get_unum();
  if ((index % 3 == 0) || (number >> 24 == 183))
  {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
  }
  else
  {
    *(unsigned int *)(index * 4 + arrNum) = number;
    return 0;
  }
}

int read_number(int arrNum)
{
  unsigned int index;

  printf(" Index: ");
  index = get_unum();
  printf(" Number at data[%u] is %u\n", index, *(unsigned int *)(arrNum + 4 * index));
  return 0;
}

int main(int argc, const char **argv, const char **envp)
{
  char numArr[400];
  int number;
  char command[4];
  int v9;
  int v10;
  int v11;
  int v12;
  unsigned int v13;

  v13 = __readgsdword(20);
  number = 0;
  *(char *)command = 0;

  v9 = 0;
  v10 = 0;
  v11 = 0;
  v12 = 0;

  memset(numArr, 0, sizeof(numArr));

  //* Clear argv and envp to remove any sensitive data
  while (*argv)
  {
    memset((void *)*argv, 0, strlen(*argv));
    ++argv;
  }

  //* Clear environment variables
  while (*envp)
  {
    memset((void *)*envp, 0, strlen(*envp));
    ++envp;
  }

  puts(
      "----------------------------------------------------\n"
      "  Welcome to wil's crappy number storage service!   \n"
      "----------------------------------------------------\n"
      " Commands:                                          \n"
      "    store - store a number into the data storage    \n"
      "    read  - read a number from the data storage     \n"
      "    quit  - exit the program                        \n"
      "----------------------------------------------------\n"
      "   wil has reserved some storage :>                 \n"
      "----------------------------------------------------\n");

  //* Main command loop */
  while (1)
  {
    printf("Input command: ");
    number = 1;
    fgets(command, 20, stdin);
    command[strlen(command) - 1] = 0;

    //* Process commands */
    //? Store command
    if (!memcmp(command, "store", 5))
    {
      number = store_number(numArr);
      goto jumpHere;
    }
    //? Read command
    if (!memcmp(command, "read", 4))
    {
      number = read_number(numArr);
      goto jumpHere;
    }
    //? Quit command
    if (!memcmp(command, "quit", 4))
      return 0;

  jumpHere:
    if (number)
      printf(" Failed to do %s command\n", command);
    else
      printf(" Completed %s command successfully\n", command);
    *(char *)command = 0;
    v9 = 0;
    v10 = 0;
    v11 = 0;
    v12 = 0;
  }
}
