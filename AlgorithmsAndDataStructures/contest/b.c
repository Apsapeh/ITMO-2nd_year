#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 150000

static char g_stackName[STACK_SIZE];
static int g_stackIdx[STACK_SIZE];
static int g_stackPointer = 0;

static int g_result[STACK_SIZE];

static char g_inputString[STACK_SIZE];

int main(void) {
  int animal_counter = 0;
  int trap_counter = 0;

  int readed = fread(g_inputString, 1, STACK_SIZE, stdin);
  g_inputString[readed] = 0;

  char *cursor = g_inputString;

  while (*cursor) {
    // char ch = (char)getchar();

    char ch = *(cursor++);

    // printf("Char: %c", ch);
    if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
      continue;
    }

    if (g_stackPointer == 0 ||
        abs(g_stackName[g_stackPointer - 1] - ch) != ('a' - 'A')) {
      g_stackName[g_stackPointer] = ch;
      g_stackIdx[g_stackPointer] =
          ch <= 'Z' ? trap_counter++ : animal_counter++;

      // printf("Stack push: %c - %d\n", ch, trap_counter);
      ++g_stackPointer;
    } else {
      // ch это ловушка
      if (ch <= 'Z') {
        // printf("TRAP: %c - %d\n", ch, trap_counter);
        g_result[trap_counter++] = g_stackIdx[--g_stackPointer];
      } else {
        // printf("ANIMAL: %c - %d\n", ch, animal_counter);
        g_result[g_stackIdx[--g_stackPointer]] = animal_counter++;
      }
    }
  }

  // printf("Possible\n2 1\n");
  // return 0;
  // if (trap_counter == 0) {
  //   return 0;
  // }

  if (g_stackPointer == 0) {
    printf("Possible\n");
    for (int i = 0; i < trap_counter; ++i) {
      printf("%d ", g_result[i] + 1);
    }
    printf("\n");
  } else {
    printf("Impossible\n");
  }

  return 0;
}
