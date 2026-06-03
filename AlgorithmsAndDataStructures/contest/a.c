#include <stdio.h>

int main(void) {
  int num;
  scanf("%d\n", &num);

  int begin_flower = 0;
  int end_flower = 0;
  int begin_flower_max = 0;
  int end_flower_max = 0;

  int prev_flower = -1;
  int prev_flower_length = 0;

  for (int i = 0; i < num; ++i) {
    int flower = 0;
    scanf("%d", &flower);

    if (flower != prev_flower) {
      prev_flower_length = 1;
    } else {
      ++prev_flower_length;

      if (prev_flower_length == 3) {
        if (end_flower - begin_flower > end_flower_max - begin_flower_max) {
          begin_flower_max = begin_flower;
          end_flower_max = end_flower;
        }

        begin_flower = end_flower - 1;
        prev_flower_length = 2;
      }
    }

    ++end_flower;
    prev_flower = flower;
  }

  if (end_flower - begin_flower > end_flower_max - begin_flower_max) {
    begin_flower_max = begin_flower;
    end_flower_max = end_flower;
  }

  printf("%d %d\n", begin_flower_max + 1, end_flower_max);

  return 0;
}
