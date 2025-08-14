#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


void solve_task1(int factor, FILE *fin, FILE *fout)
{
    // TODO
}


void solve_task2(int factor, FILE *fin, FILE *fout)
{
    // TODO

}

void solve_task3(FILE *fin, FILE *fout)
{
    // TODO

}





void check_command_line_arguments(int argc, char *argv[])
{
    if (argc < 4 || 5 < argc) {
        fprintf(stderr, "[ERR] Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4) {
        if (!strcmp(argv[1], "-d") && strlen(argv[1]) == strlen("-d"))
            return;
        
        fprintf(stderr, "[INFO] Program called with 4 arguments\n");
        fprintf(stderr, "[ERR]  Expected flag '-d'\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 5) {
        if (!strcmp(argv[1], "-c1") && strlen(argv[1]) == strlen("-c1"))
            return;
        if (!strcmp(argv[1], "-c2") && strlen(argv[1]) == strlen("-c2"))
            return;
        
        fprintf(stderr, "[INFO] Program called with 5 arguments\n");
        fprintf(stderr, "[ERR]  Expected either flag '-c1' or '-c2'\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    check_command_line_arguments(argc, argv);

    FILE *fin = fopen(argv[argc - 2], "rb");
    FILE *fout = fopen(argv[argc - 1], "wt");

    if (!fin) {
        fprintf(stderr, "[ERR] Failed to open '%s' file\n", argv[argc - 2]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-c1")) {
        int factor = atoi(argv[2]);
        solve_task1(factor, fin, fout);
    }
    else if (strcmp(argv[1], "-c2")) {
        int factor = atoi(argv[2]);
        solve_task2(factor, fin, fout);
    } else if (strcmp(argv[1], "-d"))
        solve_task3(fin, fout);

    return 0;
}
