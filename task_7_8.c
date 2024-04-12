

#include <stdio.h>

#define MAX_LINE_LENGTH 500

int main(int argc, char **argv) {
    char *program_name = *argv++, *file_name, line[MAX_LINE_LENGTH], *linep;
    FILE *input_file;
    int page = 1;
    argc--;
    while (argc--) {
        file_name = *argv++;
        input_file = fopen(file_name, "r");
        printf("--- FILE: %s --- [ %d ] \n", file_name, page);
        while (!feof(input_file) &&
               (linep = fgets(line, MAX_LINE_LENGTH, input_file)) != NULL)
            fputs(linep, stdout);
        printf("--- [ %d ]\n\n", page);
        fclose(input_file);
        ++page;
    }
}
