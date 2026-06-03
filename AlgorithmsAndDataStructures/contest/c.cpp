#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <

// Да-да, STL контейнеры. Мне лень их реализовывать в каждой задаче, где это
// нужно
#include <cstdint>
#include <unordered_map>
#include <vector>

// У меня все задачи A-D лежат в одной папке
#ifdef DEV
#define FILE_NAME "input_c.txt"
#else
#define FILE_NAME "input.txt"
#endif

static const uint64_t BLOCK_END_VAR = 0xFFFFFFFFFFFFFFFF;

inline void encode_name(uint64_t *code, char ch) {
  *code <<= 5U;
  *code |= (ch - 'a' + 1);
}

inline const char *decode_name(uint64_t code) {
  static char buf[11] = {0};

  int counter = 0;
  for (;; ++counter) {
    uint8_t ch_enc = code & 0x000000000000001F;
    code >>= 5;

    if (ch_enc == 0)
      break;

    buf[10 - counter] = ch_enc - 1 + 'a';
  }
  return &buf[10 - (counter - 1)];
}

int main(void) {
  FILE *input = fopen(FILE_NAME, "r");
  fseek(input, 0, SEEK_END);
  size_t input_size = ftell(input);
  rewind(input);

  char *input_str = (char *)malloc(sizeof(char) * (input_size + 2));
  fread(input_str, sizeof(char), sizeof(char) * input_size, input);
  input_str[input_size] = '\n';
  input_str[input_size + 1] = '\0';

  std::unordered_map<uint64_t, std::vector<int>> variables;
  std::vector<uint64_t> block_changes;

  char *cursor = input_str;
  while (*cursor) {
    char ch = *(cursor++);

    // Пропускаем CR и LF. В данной задаче не особо интересует нормальная
    // обработка ввода
    if (ch == '\n' || ch == '\r')
      continue;

    if (ch == '{') {
      block_changes.push_back(BLOCK_END_VAR);

    } else if (ch == '}') {
      while (1) {
        uint64_t var = block_changes.back();
        block_changes.pop_back();

        if (var == BLOCK_END_VAR)
          break;

        variables[var].pop_back();
      }

    } else {
      char *left_cursor = cursor - 1;
      uint64_t left_var = 0;
      while (*(left_cursor) != '=') {
        encode_name(&left_var, *(left_cursor++));
      }
      *left_cursor = 0;

      // printf("V: %llu\n", left_var);
      // printf("D: %s\n", decode_name(left_var));
      // printf("N: %s\n", cursor - 1);

      int value = 0;

      char *right_cursor = left_cursor + 1;
      if (*right_cursor >= 'a') {
        uint64_t right_var = 0;
        while (*(right_cursor) != '\n' && *(right_cursor) != '\r') {
          encode_name(&right_var, *(right_cursor++));
        }

        auto value_iter = variables.find(right_var);
        if (value_iter != variables.end()) {
          auto &vec = (*value_iter).second;
          if (!vec.empty()) {
            value = vec.back();
          }
        }

        printf("%d\n", value);

      } else {
        uint8_t is_minus = 0;
        if (*right_cursor == '-') {
          is_minus = 1;
          ++right_cursor;
        }

        while (*(right_cursor) != '\n' && *(right_cursor) != '\r') {
          value *= 10;
          value += *(right_cursor++) - '0';
        }

        value = is_minus ? -value : value;
      }

      auto var_iter = variables.find(left_var);
      if (var_iter == variables.end()) {
        variables[left_var] = {};
      }

      variables[left_var].push_back(value);
      block_changes.push_back(left_var);

      cursor = right_cursor;
    }
  }

  free(input_str);
}
