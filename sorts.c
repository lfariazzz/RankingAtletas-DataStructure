#include "atleta.h"

/**
 * Bubble Sort para Lista Dinâmica.
 * Ordena por tempoSegundos (crescente).
 */
void bubble_sort_dinamico(No *inicio) {
    if (inicio == NULL || inicio->proximo == NULL) return;

    int trocou;
    No *atual;
    No *ultimo = NULL;

    do {
        trocou = 0;
        atual = inicio;

        while (atual->proximo != ultimo) {
            // Critério de Aceite: Menor tempo primeiro
            if (atual->dado.tempoSegundos > atual->proximo->dado.tempoSegundos) {
                Atleta temp = atual->dado;
                atual->dado = atual->proximo->dado;
                atual->proximo->dado = temp;
                trocou = 1;
            }
            atual = atual->proximo;
        }
        ultimo = atual;
    } while (trocou);
}

/**
 * Bubble Sort para Lista Estática.
 * Ordena por tempoSegundos (crescente).
 */
void bubble_sort_estatico(NoEstatico lista[], int inicio) {
    if (inicio == -1) return;

    int trocou;
    int i;
    int ultimo = -1;

    do {
        trocou = 0;
        i = inicio;

        while (lista[i].proximo != -1 && lista[i].proximo != ultimo) {
            int prox = lista[i].proximo;

            // Critério de Aceite: Menor tempo primeiro
            if (lista[i].dado.tempoSegundos > lista[prox].dado.tempoSegundos) {
                Atleta temp = lista[i].dado;
                lista[i].dado = lista[prox].dado;
                lista[prox].dado = temp;
                trocou = 1;
            }
            i = lista[i].proximo;
        }
        ultimo = i;
    } while (trocou);
}