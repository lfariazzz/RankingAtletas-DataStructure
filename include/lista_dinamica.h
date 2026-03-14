#ifndef LISTA_DINAMICA_H
#define LISTA_DINAMICA_H
 
#include "atleta.h"
 
// LISTA ENCADEADA DINAMICA: NÓS ALOCADOS DINAMICAMENTE COM PONTEIROS
 
typedef struct No {
    Atleta      dado; // Dado do atleta
    struct No  *prox; // Ponteiro para o próximo nó (NULL se for o último)
} No;
 
typedef struct {
    No  *cabeca;    // Ponteiro para o primeiro nó da lista (NULL se vazia)
    int  tamanho;   // Quantidade de elementos ativos na lista
} ListaDinamica;
 
// Operacoes basicas: O(1) para insercao no inicio, O(n) para insercao no fim, busca e remocao

void ld_inicializa(ListaDinamica *lista); // Inicializa a lista, definindo cabeca como NULL e tamanho como 0
int  ld_insere_inicio(ListaDinamica *lista, Atleta atleta); // Retorna 1 se inserido, 0 se erro de alocacao
int  ld_insere_fim(ListaDinamica *lista, Atleta atleta); // Retorna 1 se inserido, 0 se erro de alocacao
void ld_imprime(const ListaDinamica *lista); // Imprime a lista no formato "Posicao | Nome | Tempo | Pace"
No  *ld_busca(const ListaDinamica *lista, int numero); // Retorna ponteiro para o no do atleta ou NULL se nao encontrado
int  ld_remove(ListaDinamica *lista, int numero); // Retorna 1 se removido, 0 se nao encontrado
void ld_libera(ListaDinamica *lista); // Libera toda a memoria alocada para a lista, definindo cabeca como NULL e tamanho como 0

// Utilitarios para ordenacao e atualizacao de posicoes

int  ld_para_vetor(const ListaDinamica *lista, Atleta *vetor, int max); // Converte a lista para um vetor (usado para ordenacao)
void ld_de_vetor(ListaDinamica *lista, const Atleta *vetor, int n); // Reconstrói a lista a partir de um vetor ordenado
void ld_atualizar_posicoes(ListaDinamica *lista); // Recalcula posicoes com base na ordem atual da lista
 
#endif