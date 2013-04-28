#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "def.h"

char* nextline(char *out, int max, FILE *f);


int main(void)
{
    FILE *obj;
    FILE *source;
    obj = fopen("test.obj", "w+");
    source = fopen("../lc3assembler/input.asm", "r");
    LC3_WORD_TYPE data[2];
    data[0] = 0x1301;
    fwrite(data, sizeof(LC3_WORD_TYPE), 1, obj);

    char line[9999];
    while (nextline(line, 9999, source)) {
        char **position;
        char *entry[5];
        char *input = line;
        for (int i = 0; i < 5; ++i) {
            entry[i] = NULL; // Memory leak!
        }

        position = entry;
        while ((*position = strsep(&input, " ")) != NULL) {
            if (**position != '\0') {
                if (++position >= &entry[5]) {
                    break;
                }
            }
        }

        for (int i = 0; i < 5; ++i) {
//            int len = strlen(entry[i]);
//            if (len <= 0) break;

            if (entry[i] == NULL) continue;

            printf(" {\"");
            printf(entry[i]);
            printf("\"");

            char **position2;
            char *entry2[5];
            for (int j = 0; j < 5; ++j) {
                entry2[j] = NULL;
            }
            char *input2 = entry[i];
            position2 = entry2;
            while ((*position2 = strsep(&input2, ",")) != NULL) {
                if (**position2 != '\0') {
                    if (++position2 >= &entry2[5]) {
                        break;
                    }
                }
            }

            for (int j = 0; j < 5; ++j) {
                if (entry2[j] == NULL) continue;
                printf(" [");
                printf(entry2[j]);
                printf("]");
            }

            printf("}");
        }

//        printf(line);
        printf("\n");
    }

    fclose(source);
    fclose(obj);
    return 0;
}

char *nextline(char *out, int max, FILE *f) {
    char *result = fgets(out, max, f);
    int len = strlen(out);
    if (len > 0) {
        if (out[len - 1] == '\n') {
            out[len - 1] = '\0'; // No trailing newline
        }
    }
    return result;
}
