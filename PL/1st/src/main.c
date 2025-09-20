#include <stdio.h>
#include <stdlib.h>

#define ERROR(str) do {error(__LINE__, __FILE__, str);} while (0)

void error(int line, const char* file, const char* str);

void encrypt(char* word, int shift);
void decrypt(char* word, int shift);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        ERROR("Недостатотчно аругментов");
    }

    char* word = argv[1];
    int shift = atoi(argv[2]);
    shift = ((shift % 26) + 26) % 26;



    encrypt(word, shift);
    printf("Encrypted: %s\n", word);

    decrypt(word, shift);
    printf("Decrypted: %s\n", word);
}

void encrypt(char* word, int prev_shift) {
    for (int i = 0; word[i]; ++i) {
        char current_char = word[i];
        // int shift = prev_shift > 0 ? prev_shift % 26 : (26 + prev_shift) % 26;
        int shift = prev_shift;
        printf("%c %d\n", current_char, shift);
        if (current_char >= 'a' && current_char <= 'z') {
            word[i] = 'a' + (current_char - 'a' + shift) % 26;
        } else if (current_char >= 'A' && current_char <= 'Z') {
            word[i] = 'A' + (current_char - 'A' + shift) % 26;
        }
    }
}

void decrypt(char* word, int shift) {
    char* current_char = word;
    for (; *current_char; ++current_char) {
        int real_shift = shift > 0 ? shift % 26 : (26 + shift) % 26;
        //int real_shift = shift > 0 ? shift % 26 : (26 + shift) % 26;

        if (*current_char >= 'a' && *current_char <= 'z') {
            *current_char += real_shift > (*current_char - 'a') ? 26 - real_shift : -real_shift;
        } else if (*current_char >= 'A' && *current_char <= 'Z') {
            *current_char += real_shift > (*current_char - 'A') ? 26 - real_shift : -real_shift;
        }
    }
}

void error(int line, const char* file, const char* str) {
    printf("Error [%s@%d]: %s\n", file, line, str);
    exit(1);
}
