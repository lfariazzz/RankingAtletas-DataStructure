#include "ordenacao.h"
#include "atleta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para trocar dois Atletas
static inline void trocar(Atleta *a, Atleta *b) {
    Atleta tmp = *a;
    *a = *b;
    *b = tmp;
}


// Implementação do insertion_sort
long insertion_sort(Atleta *v, int n, FuncComparacao cmp) {
    long comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Atleta chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            comparacoes++;
            if (cmp(&v[j], &chave) > 0) {
                v[j+1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j+1] = chave;
    }
    return comparacoes;
}


// Implementação do quick_sort
long _quick_sort_rec(Atleta *v, int esq, int dir, FuncComparacao cmp) {
    long comparacoes = 0;
    if (esq >= dir)
        return 0;

    // Escolhe o pivô (aqui usando o elemento do meio)    
    Atleta pivo = v[(esq + dir) / 2];
    int i = esq;
    int j = dir;

    while (i <= j) {
        // Avança i enquanto v[i] < pivo
        while (1) {
            comparacoes++;
            if (cmp(&v[i], &pivo) < 0) i++;
            else break;
        }
        // Recua j enquanto v[j] > pivo
        while (1) {
            comparacoes++;
            if (cmp(&v[j], &pivo) > 0) j--;
            else break;
        }
        if (i <= j) {
            trocar(&v[i], &v[j]);
            i++;
            j--;
        }
    }

    comparacoes += _quick_sort_rec(v, esq, j, cmp);
    comparacoes += _quick_sort_rec(v, i, dir, cmp);
    return comparacoes;
}

long quick_sort(Atleta *v, int n, FuncComparacao cmp) {
    if (n <= 1) return 0;
    return _quick_sort_rec(v, 0, n - 1, cmp);
}

// Implementação do bubble_sort
long bubble_sort(Atleta *v, int n, FuncComparacao cmp) {
    long comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            comparacoes++;
            if (cmp(&v[j], &v[j+1]) > 0) {
                trocar(&v[j], &v[j+1]);
                trocou = 1;
            }
        }
        if (!trocou)
            break; // Otimização: se não houve trocas, o vetor já está ordenado
    }
    return comparacoes;
}

// Implementação do selection_sort 
long selection_sort(Atleta *v, int n, FuncComparacao cmp) {
    long comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int idx_min = i;
        for (int j = i + 1; j < n; j++) {
            comparacoes++;
            if (cmp(&v[j], &v[idx_min]) < 0)
                idx_min = j;
        }
        if (idx_min != i)
            trocar(&v[i], &v[idx_min]);
    }
    return comparacoes;
}

long _merge_sort_rec(Atleta *v, Atleta *aux, int esq, int dir, FuncComparacao cmp) {
    long comparacoes = 0;
    if (esq >= dir)
        return 0;

    int meio = (esq + dir) / 2;

    comparacoes += _merge_sort_rec(v, aux, esq, meio, cmp);
    comparacoes += _merge_sort_rec(v, aux, meio + 1, dir, cmp);

    /* Intercalacao (merge) das duas metades */
    int i = esq, j = meio + 1, k = esq;

    while (i <= meio && j <= dir) {
        comparacoes++;
        if (cmp(&v[i], &v[j]) <= 0) {
            aux[k++] = v[i++];
        } else {
            aux[k++] = v[j++];
        }
    }
    while (i <= meio) aux[k++] = v[i++];
    while (j <= dir)  aux[k++] = v[j++];

    /* Copia de volta para v */
    for (int x = esq; x <= dir; x++)
        v[x] = aux[x];

    return comparacoes;
}

long merge_sort(Atleta *v, int n, FuncComparacao cmp) {
    if (n <= 1) return 0;
    Atleta *aux = (Atleta *) malloc(n * sizeof(Atleta));
    if (aux == NULL) {
        fprintf(stderr, "[ERRO] merge_sort: malloc falhou.\n");
        return -1;
    }
    long comp = _merge_sort_rec(v, aux, 0, n - 1, cmp);
    free(aux);
    return comp;
}