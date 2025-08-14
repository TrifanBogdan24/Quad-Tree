/*TRIFAN BOGDAN-CRISTIAN , 312CD, TEMA 2*/
#ifndef _STRUCT_ARBORE_H_
#define _STRUCT_ARBORE_H_
#define ZERO 0
#define UNU 1
#define DOI 2
#define TREI 3
#define PATRU 4
#define CINCI 5
#define ZECE 10
#define DOUA_ZECI 20
#define FLAG_C1 "-c1"
#define FLAG_C2 "-c2"
#define FLAG_D "-d"
#define CHAR_ZERO '0'
#define CHAR_NOUA '9'
#define LITERA_R 'R'
#define LITERA_G 'G'
#define LITERA_B 'B'

typedef struct culoare {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} culoare;

typedef struct nod {
    culoare pixel;
    unsigned int nivel;
    unsigned int x_stanga_sus;
    unsigned int y_stanga_sus;
    unsigned int lungimea_patratului;
    unsigned char tip_nod; // 0 pentru nod , 1 pentru frunza
    struct nod *fiu_unu;
    struct nod *fiu_doi;
    struct nod *fiu_trei;
    struct nod *fiu_patru;
} nod_arbore, *arbore;

arbore create_nod_arbore(void)
{
    arbore radacina =  malloc(sizeof(nod_arbore));
    radacina->x_stanga_sus = ZERO;
    radacina->y_stanga_sus = ZERO;
    radacina->lungimea_patratului = ZERO;
    radacina->tip_nod = ZERO;   // 0 pentru ne-frunza, 1 pentru frunza
    radacina->fiu_unu = NULL;
    radacina->fiu_doi = NULL;
    radacina->fiu_trei = NULL;
    radacina->fiu_patru = NULL;
    radacina->pixel.R = ZERO;
    radacina->pixel.G = ZERO;
    radacina->pixel.B = ZERO;
    return radacina;
}

culoare **alloc_init_matrice(unsigned int lungime_patrat)
{
    culoare **matricea_imaginii = (culoare **)
                                    malloc(lungime_patrat * sizeof(culoare *));
    for (int i = ZERO; i < lungime_patrat; i++) {
        matricea_imaginii[i] = (culoare *)
                                malloc(lungime_patrat * sizeof(culoare));
    }
    for (int i = ZERO; i < lungime_patrat; i++) {
        for (int j = ZERO; j < lungime_patrat; j++) {
            matricea_imaginii[i]->R = ZERO;
            matricea_imaginii[i]->G = ZERO;
            matricea_imaginii[i]->B = ZERO;
        }
    }
    return matricea_imaginii;
}

void display_image(FILE *FOUT, int dimensiune_imagine,
                    culoare **matricea_imaginii)
{
    fprintf(FOUT, "P6\n");
    fprintf(FOUT, "%d ", dimensiune_imagine);
    fprintf(FOUT, "%d\n", dimensiune_imagine);
    fprintf(FOUT, "%d\n", 255);

    for (int i = ZERO; i < dimensiune_imagine; i++) {
        for (int j = ZERO; j < dimensiune_imagine; j++)
            fwrite(&matricea_imaginii[i][j], sizeof(culoare), UNU, FOUT);
    }

    for (int i = ZERO; i < dimensiune_imagine; i++)
        free(matricea_imaginii[i]);

    free(matricea_imaginii);
}

int nr_nivele(arbore radacina)
{
    if (!radacina) {
        return ZERO;
    } else if (!radacina->fiu_unu && !radacina->fiu_doi &&
                !radacina->fiu_trei && !radacina->fiu_patru) {
        // nodul este frunza daca toti copiii lui sunt nuli
        return radacina->nivel;
    } else {
        int distanta_pana_la_unu = nr_nivele(radacina->fiu_unu);
        int distanta_pana_la_doi = nr_nivele(radacina->fiu_doi);
        int distanta_pana_la_trei = nr_nivele(radacina->fiu_trei);
        int distanta_pana_la_patru = nr_nivele(radacina->fiu_patru);
        int nivel_actual = distanta_pana_la_unu;

        if (distanta_pana_la_doi > nivel_actual)
            nivel_actual = distanta_pana_la_doi;

        if (distanta_pana_la_trei > nivel_actual)
            nivel_actual = distanta_pana_la_trei;

        if (distanta_pana_la_patru > nivel_actual)
            nivel_actual = distanta_pana_la_patru;

        return nivel_actual;
    }
}

int nr_frunze(arbore radacina)
{
    if (!radacina) {
        return ZERO;
    } else if (!radacina->fiu_unu && !radacina->fiu_doi &&
                !radacina->fiu_trei && !radacina->fiu_patru) {
        return UNU;
    } else {
        int nr_frunze_subarbore = ZERO;
        nr_frunze_subarbore += nr_frunze(radacina->fiu_unu);
        nr_frunze_subarbore += nr_frunze(radacina->fiu_doi);
        nr_frunze_subarbore += nr_frunze(radacina->fiu_trei);
        nr_frunze_subarbore += nr_frunze(radacina->fiu_patru);
        return nr_frunze_subarbore;
    }
}

int latura_maxima_frunza(arbore radacina)
{
    if (!radacina) {
        return ZERO;
    } else if (!radacina->fiu_unu && !radacina->fiu_doi &&
                !radacina->fiu_trei && !radacina->fiu_patru) {
        return radacina->lungimea_patratului;
    } else {
        int lat_max_unu = latura_maxima_frunza(radacina->fiu_unu);
        int lat_max_doi = latura_maxima_frunza(radacina->fiu_doi);
        int lat_max_trei = latura_maxima_frunza(radacina->fiu_trei);
        int lat_max_patru = latura_maxima_frunza(radacina->fiu_patru);

        int latura_maxima = lat_max_unu;

        if (lat_max_doi > latura_maxima)
            latura_maxima = lat_max_doi;

        if (lat_max_trei > latura_maxima)
            latura_maxima = lat_max_trei;

        if (lat_max_patru > latura_maxima)
            latura_maxima = lat_max_patru;

        return latura_maxima;
    }
}

void citire_nod_arbore(FILE *FIN, arbore *nod)
{
    fread(&((*nod)->tip_nod), sizeof(unsigned char), UNU, FIN);
    if ((*nod)->tip_nod == UNU) {
        fread(&((*nod)->pixel.R), sizeof(unsigned char), UNU, FIN);
        fread(&((*nod)->pixel.G), sizeof(unsigned char), UNU, FIN);
        fread(&((*nod)->pixel.B), sizeof(unsigned char), UNU, FIN);
    }
}

void image_builder(culoare **matrice, int x, int y, int dimensiune, culoare cul)
{
    for (int i = x; i < x + dimensiune; i++) {
        for (int j = y; j < y + dimensiune; j++)
            matrice[i][j] = cul;
    }
    return;
}

void use_of_nod_in_image(arbore nod, culoare **matrice)
{
    // printf("%u ", nod->tip_nod);
    if (nod->tip_nod == UNU) {
        // nodul este de tip frunza
        // printf("%hhu ", nod->pixel.R);
        // printf("%hhu ", nod->pixel.G);
        // printf("%hhu ", nod->pixel.B);
        int x = nod->x_stanga_sus;
        int y = nod->y_stanga_sus;
        int dimensiune = nod->lungimea_patratului;
        culoare cul_pixel = nod->pixel;
        image_builder(matrice, x, y, dimensiune, cul_pixel);
    }
}

void destroy_arobre(arbore radacina)
{
	if (radacina) {
		// printf("%d ", (*root)->value);
        if (radacina->fiu_unu)
            destroy_arobre(radacina->fiu_unu);

        if (radacina->fiu_doi)
            destroy_arobre(radacina->fiu_doi);

        if (radacina->fiu_trei)
            destroy_arobre(radacina->fiu_trei);

        if (radacina->fiu_patru)
            destroy_arobre(radacina->fiu_patru);

		free(radacina);
		// (*root) = NULL;
	}
    return;
}

#endif
