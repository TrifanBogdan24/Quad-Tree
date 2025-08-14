/*TRIFAN BOGDAN-CRISTIAN , 312CD, TEMA 2*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_arbore.h"
#include "struct_coada.h"

int medie_culoare(culoare **matricea_imaginii, int x, int y,
                    int size, char tip_culoare)
{
    if (size == ZERO) {
        printf("Dimensiunea nu poate fi 0.\n");
        return -UNU;
    }
    unsigned long long val_cul = ZERO;
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            switch (tip_culoare) {
            case LITERA_R:
                val_cul += matricea_imaginii[i][j].R;
                break;

            case LITERA_G:
                val_cul += matricea_imaginii[i][j].G;
                break;

            case LITERA_B:
                val_cul += matricea_imaginii[i][j].B;
                break;

            default:
                break;
            }
        }
    }
    return val_cul / (size * size);
}

int mean(culoare **matricea_imaginii, int x, int y, int size)
{
    if (size == ZERO) {
        printf("Dimensiunea nu poate fi 0.\n");
        return -UNU;
    }
    unsigned long long val_mean = ZERO;
    int red = medie_culoare(matricea_imaginii, x, y, size, LITERA_R);
    int green = medie_culoare(matricea_imaginii, x, y, size, LITERA_G);
    int blue = medie_culoare(matricea_imaginii, x, y, size, LITERA_B);

    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            unsigned long long dif_red = red - matricea_imaginii[i][j].R;
            unsigned long long dif_green = green - matricea_imaginii[i][j].G;
            unsigned long long dif_blue = blue - matricea_imaginii[i][j].B;
            dif_red = dif_red * dif_red;
            dif_green = dif_green * dif_green;
            dif_blue = dif_blue * dif_blue;
            val_mean = val_mean + dif_red + dif_green + dif_blue;
        }
    }
    return val_mean / (TREI * size * size);
}

void constructor_arbore(arbore radacina, int x, int y, int len, int lvl,
                        culoare **matricea_imaginii, int factor)
{
    if (len == ZERO)
        return;

    int red = medie_culoare(matricea_imaginii, x, y, len, LITERA_R);
    int green = medie_culoare(matricea_imaginii, x, y, len, LITERA_G);
    int blue = medie_culoare(matricea_imaginii, x, y, len, LITERA_B);
    int val_mean = mean(matricea_imaginii, x, y, len);
    radacina->pixel.R = red;
    radacina->pixel.G = green;
    radacina->pixel.B = blue;
    radacina->x_stanga_sus = x;
    radacina->y_stanga_sus = y;
    radacina->lungimea_patratului = len;
    radacina->nivel = lvl;

    // printf("red = %d\n", red);
    // printf("green = %d\n", green);
    // printf("blue = %d\n", blue);
    // printf("mean = %d\n", val_mean);
    if (val_mean <= factor) {
        radacina->tip_nod = UNU; // frunza
        return;
    } else {
        // trebuie sa impartim in patru zone

        // patratul stanga-sus
        radacina->fiu_unu = create_nod_arbore();
        radacina->fiu_unu->x_stanga_sus = x;
        radacina->fiu_unu->y_stanga_sus = y;
        radacina->fiu_unu->lungimea_patratului = len / DOI;

        // patratul dreapta-sus
        radacina->fiu_doi = create_nod_arbore();
        radacina->fiu_doi->x_stanga_sus = x;
        radacina->fiu_doi->y_stanga_sus = y + len / DOI;
        radacina->fiu_doi->lungimea_patratului = len / DOI;

        // patratul dreapta-jos
        radacina->fiu_trei = create_nod_arbore();
        radacina->fiu_trei->x_stanga_sus = x + len / DOI;
        radacina->fiu_trei->y_stanga_sus = y + len / DOI;
        radacina->fiu_trei->lungimea_patratului = len / DOI;

        //patraul stanga-jos
        radacina->fiu_patru = create_nod_arbore();
        radacina->fiu_patru->x_stanga_sus = x + len / DOI;
        radacina->fiu_patru->y_stanga_sus = y;
        radacina->fiu_patru->lungimea_patratului = len / DOI;

        // apelam recursiv pentru cele patru zone
        constructor_arbore(radacina->fiu_unu, x, y,
                            len / DOI, lvl + UNU, matricea_imaginii, factor);
        constructor_arbore(radacina->fiu_doi, x, y + len / DOI,
                            len / DOI, lvl + UNU, matricea_imaginii, factor);
        constructor_arbore(radacina->fiu_trei, x + len / DOI, y + len / DOI,
                            len / DOI, lvl + UNU, matricea_imaginii, factor);
        constructor_arbore(radacina->fiu_patru, x + len / DOI, y,
                            len / DOI, lvl + UNU, matricea_imaginii, factor);
    }
}

void fiii_nodului(arbore nod, queue *coada)
{
    if (nod->tip_nod == ZERO && nod->lungimea_patratului / DOI > ZERO) {
        // nodul nu este terminal
        // nodul nu este frunza
        int x = nod->x_stanga_sus;
        int y = nod->y_stanga_sus;
        int lungime = nod->lungimea_patratului / DOI;
        // primul fiu
        arbore fiu_unu = create_nod_arbore();
        fiu_unu->x_stanga_sus = x;
        fiu_unu->y_stanga_sus = y;
        fiu_unu->lungimea_patratului = lungime;
        coada_inserare_final(coada, fiu_unu);

        // al doilea fiu
        arbore fiu_doi = create_nod_arbore();
        fiu_doi->x_stanga_sus = x;
        fiu_doi->y_stanga_sus = y + lungime;
        fiu_doi->lungimea_patratului = lungime;
        coada_inserare_final(coada, fiu_doi);

        // al treilea fiu
        arbore fiu_trei = create_nod_arbore();
        fiu_trei->x_stanga_sus = x + lungime;
        fiu_trei->y_stanga_sus = y + lungime;
        fiu_trei->lungimea_patratului = lungime;
        coada_inserare_final(coada, fiu_trei);

        // al patrulea fiu
        arbore fiu_patru = create_nod_arbore();
        fiu_patru->x_stanga_sus = x + lungime;
        fiu_patru->y_stanga_sus = y;
        fiu_patru->lungimea_patratului = lungime;
        coada_inserare_final(coada, fiu_patru);
    }
}

void get_sons_in_queue(FILE *FOUT, arbore nod, queue *coada)
{
    if (!nod)
        return;
    fwrite(&nod->tip_nod, sizeof(unsigned char), UNU, FOUT);
    // printf("%hhu", nod->tip_nod);
    if (nod->tip_nod == UNU) {
        // nodul este de tip frunza
        fwrite(&nod->pixel.R, sizeof(unsigned char), UNU, FOUT);
        fwrite(&nod->pixel.G, sizeof(unsigned char), UNU, FOUT);
        fwrite(&nod->pixel.B, sizeof(unsigned char), UNU, FOUT);
        // printf("\n%hhu %hhu %hhu", nod->pixel.R,
        //          nod->pixel.G, nod->pixel.B);
    }

    if (nod->fiu_unu)
        coada_inserare_final(coada, nod->fiu_unu);

    if (nod->fiu_doi)
        coada_inserare_final(coada, nod->fiu_doi);

    if (nod->fiu_trei)
        coada_inserare_final(coada, nod->fiu_trei);

    if (nod->fiu_patru)
        coada_inserare_final(coada, nod->fiu_patru);
    // printf("\n");
}

void solve_cerinta_doi(FILE *FOUT, unsigned int lungime, arbore radacina)
{
    // task 2 to be coded
    // BFS -> PARCURGERE IN LATIME
    // with frwite
    fwrite(&lungime, sizeof(unsigned int), UNU, FOUT);
    // printf("%d\n", lungime);
    queue *coada = creare_coada();
    coada_inserare_final(coada, radacina);
    while (coada->inceput) {
        arbore nod = coada->inceput->nod;
        get_sons_in_queue(FOUT, nod, coada);
        coada_delete_first_node(coada);
    }
    coada_full_delete(coada);
}

void solve_primele_doua_cerinte(char const *flag, int factor,
                                FILE *FIN, FILE *FOUT)
{
    char caracter_unu_tip_fisier = ' ';
    char caracter_doi_tip_fisier = ' ';

    // char *tip_fisier = (char *)malloc(DOI * sizeof(char));

    fread(&caracter_unu_tip_fisier, sizeof(char), UNU, FIN);
    fread(&caracter_doi_tip_fisier, sizeof(char), UNU, FIN);
    unsigned int lungime = ZERO, latime = ZERO, max_rgb = ZERO;
    fscanf(FIN, "%u", &lungime);
    fscanf(FIN, "%u", &latime);
    fscanf(FIN, "%u", &max_rgb);

    int poz_cursor = ZERO;
    fseek(FIN, ZERO, SEEK_SET);
    char *linie = (char *)malloc(DOUA_ZECI * sizeof(char));
    fgets(linie, DOUA_ZECI, FIN);
    poz_cursor += strlen(linie);
    fgets(linie, DOUA_ZECI, FIN);
    poz_cursor += strlen(linie);
    fgets(linie, DOUA_ZECI, FIN);
    poz_cursor += strlen(linie);
    free(linie);
    fseek(FIN, poz_cursor, SEEK_SET);
    // printf("%d\n", lungime);
    // printf("Lungime = %d\n", lungime);
    // printf("Latime = %d\n", latime);
    // printf("Max rgb = %d\n", max_rgb);

    culoare **matricea_imaginii = alloc_init_matrice(lungime);
    for (int i = ZERO; i < latime; i++) {
        for (int j = ZERO; j < lungime; j++)
            fread(&matricea_imaginii[i][j], sizeof(culoare), UNU, FIN);
    }

    arbore radacina = create_nod_arbore();
    radacina->lungimea_patratului = lungime;
    radacina->nivel = UNU;
    constructor_arbore(radacina, ZERO, ZERO, lungime, UNU,
                        matricea_imaginii, factor);
    // printf("Lungime = %d\n", lungime);
    // verificare afisare imagine
    /*
    FILE *imagine = fopen("imagine", "wb");
    display_image(imagine, lungime, matricea_imaginii);
    fclose(imagine);
    */
    // terminare verificare
    if (strcmp(flag, FLAG_C1) == ZERO) {
        // rezolvarea primei cerinte
        // printf("Rezolvam cerinta 1 :\n");
        // printf("Numar nivele : %d\n", nr_nivele(radacina));
        // printf("Numar frunze : %d\n", nr_frunze(radacina));
        // printf("Latura maxima a unei frunze : %d\n",
        //    latura_maxima_frunza(radacina));
        fprintf(FOUT, "%d\n", nr_nivele(radacina));
        fprintf(FOUT, "%d\n", nr_frunze(radacina));
        fprintf(FOUT, "%d\n", latura_maxima_frunza(radacina));
    } else if (strcmp(flag, FLAG_C2) == ZERO) {
        // task 2 to be coded
        // BFS -> PARCURGERE IN LATIME
        // with frwite
        solve_cerinta_doi(FOUT, lungime, radacina);
    }
    destroy_arobre(radacina);
    // free(tip_fisier);
    for (int i = ZERO; i < latime; i++)
        free(matricea_imaginii[i]);
    free(matricea_imaginii);
}

void solve_cerinta_trei(FILE *FIN, FILE *FOUT)
{
    unsigned int dimensiune_imagine = TREI;
    fread(&dimensiune_imagine, sizeof(unsigned int), UNU, FIN);
    printf("%u\n", dimensiune_imagine);
    culoare **matricea_imaginii = alloc_init_matrice(dimensiune_imagine);
    printf("good");
    arbore nod = create_nod_arbore();
    queue *coada = creare_coada();
    nod->lungimea_patratului = dimensiune_imagine;
    nod->x_stanga_sus = ZERO;
    nod->y_stanga_sus = ZERO;
    citire_nod_arbore(FIN, &nod);
    coada_inserare_final(coada, nod);
    if (nod->tip_nod == UNU) {
        // nodul este de tip frunza
        use_of_nod_in_image(nod, matricea_imaginii);
    } else {
        // nodul este nod intern grafului
        // radacina sau nu este nod terminal
        fiii_nodului(nod, coada);
    }
    printf("%u", nod->tip_nod);
    if (nod->tip_nod == UNU) {
        printf("%hhu ", nod->pixel.R);
        printf("%hhu ", nod->pixel.G);
        printf("%hhu ", nod->pixel.B);
    }
    while (!feof(FIN)) {
        arbore nod = coada->inceput->nod;
        citire_nod_arbore(FIN, &nod);
        printf("%u", nod->tip_nod);
        if (nod->tip_nod == UNU) {
            printf("%hhu ", nod->pixel.R);
            printf("%hhu ", nod->pixel.G);
            printf("%hhu ", nod->pixel.B);
        }
        printf("\n");
        if (nod->tip_nod == UNU) {
            // nodul este de tip frunza
            use_of_nod_in_image(nod, matricea_imaginii);
        } else {
            // nodul este nod intern grafului
            // radacina sau nu este nod terminal
            fiii_nodului(nod, coada);
        }
        coada_delete_first_node(coada);
    }
    printf("\n");
    display_image(FOUT, dimensiune_imagine, matricea_imaginii);
    return;
}

int string_to_int(char const *sir)
{
    int nr = ZERO;
    for (int i = ZERO; i < strlen(sir); i++) {
        if ((int)CHAR_ZERO <= (int)sir[i] && (int)sir[i] <= (int)CHAR_NOUA) {
            nr = nr * ZECE + (int)sir[i] - (int)CHAR_ZERO;
        } else {
            printf("Factorul '%s' nu este un numar.\n", sir);
            return -UNU;
        }
    }
    return nr;
}

int cmd_verification(int expected_nr_args, int argc, char const *argv[])
{
    if (argc < expected_nr_args) {
        printf("Parametri insuficienti.\n");
        return ZERO;
    }
    if (argc > expected_nr_args) {
        printf("Prea multi parametri.\n");
        return ZERO;
    }
    if (!argv[expected_nr_args - DOI] || !argv[expected_nr_args - UNU]) {
        printf("Eroare la deschiderea fisierului.\n");
        return ZERO;
    }
}

int parametrii_cmds(int argc, char const *argv[])
{
    // returneaza 1 daca parametrii primitii dupa executabili sunt OK
    // si 0, daca parametrii nu sunt valizi (in regula)
    int is_ok = UNU;
    if (strcmp(argv[UNU], FLAG_C1) == ZERO) {
        cmd_verification(CINCI, argc, argv);
    } else if (strcmp(argv[UNU], FLAG_C2) == ZERO) {
        cmd_verification(CINCI, argc, argv);
    } else if (strcmp(argv[UNU], FLAG_D) == ZERO) {
        cmd_verification(PATRU, argc, argv);
    } else {
        printf("FLAG-ul este nu este recunoscut.\n");
        is_ok = ZERO;
    }
    return is_ok;
}

int main(int argc, char const *argv[])
{
    if (!parametrii_cmds(argc, argv)) {
        // verificam daca parametrii primiti sunt valizi
        // functia returneaza 1 pentru parametri valizi
        // functia returneaza 0 pentru parametri invalizi
        return UNU;
    }

    if (strcmp(argv[UNU], FLAG_C1) == ZERO ||
        strcmp(argv[UNU], FLAG_C2) == ZERO) {
        int factor = string_to_int(argv[DOI]);
        if (factor < ZERO)
            return UNU;
     
        // printf("\n%d\n", factor);
        FILE *FIN = fopen(argv[TREI], "rb");
        FILE *FOUT = fopen(argv[PATRU], "wt");
        solve_primele_doua_cerinte(argv[UNU], factor, FIN, FOUT);
        fclose(FIN);
        fclose(FOUT);
    } else if (strcmp(argv[UNU], FLAG_D) == ZERO) {
        // task 3 to be coded
        FILE *FIN = fopen(argv[DOI], "rb");
        FILE *FOUT = fopen(argv[TREI], "wt");
        solve_cerinta_trei(FIN, FOUT);
        fclose(FIN);
        fclose(FOUT);
    }
    return ZERO;
}
