/*TRIFAN BOGDAN-CRISTIAN , 312CD, TEMA 2*/
#ifndef _STRUCT_COADA_H_
#define _STRUCT_COADA_H_
#include "struct_arbore.h"

typedef struct nod_queue {
    arbore nod;
    struct nod_queue *urm;    // adresa catre urmatorul nod din queue
} nod_queue;

typedef struct queue {
    nod_queue *inceput;       // va memora nodul de inceput al listei
    nod_queue *final;         // va memora nodul final al listei
} queue;

nod_queue *creare_nod_coada(arbore nod)
{
    // cream un nou nod de tip cada
    nod_queue *nod_nou = malloc(sizeof(nod_queue));
    nod_nou->urm = NULL;
    nod_nou->nod = nod;
    return nod_nou;
}

queue *creare_coada(void)
{
	// cream o queue noua
	queue *coada_noua = malloc(sizeof(queue));
	coada_noua->inceput = NULL;
	coada_noua->final = NULL;
	return coada_noua;
}

void coada_inserare_final(queue *coada_input, arbore nod)
{
    if (!coada_input) {
        // queue este vida
        coada_input = creare_coada();
        coada_input->inceput = creare_nod_coada(nod);
        coada_input->inceput = coada_input->final;
        return;
    }
    if (coada_input && !coada_input->inceput) {
        // queue nu este initializata, ea exista, dar nu retine nimic
        coada_input->inceput = creare_nod_coada(nod);
        coada_input->final = coada_input->inceput;
        return;
    }
    // queue are cel putin un element
    nod_queue *new_last = creare_nod_coada(nod);
    coada_input->final->urm = new_last;
    coada_input->final = new_last;
    return;
}

void coada_delete_first_node(queue *coada_input)
{
    if (!coada_input)
        return;

    if (!coada_input->inceput)
        return;

    // sterge primul element al cozii , dar nu si queue
    nod_queue *stergator = coada_input->inceput;
    coada_input->inceput = coada_input->inceput->urm;
    free(stergator);
}

void coada_full_delete(queue *coada_input)
{
	if (!coada_input) {
        free(coada_input);
        return;
    }
	while (coada_input->inceput) {
		// stergem succeiv primul element al cozii
		coada_delete_first_node(coada_input);
	}
	free(coada_input);
}

#endif
