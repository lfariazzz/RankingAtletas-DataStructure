#ifndef LISTA_ESTATICA_H
#define LISTA_ESTATICA_H
 
#include "atleta.h"
 
//LISTA ENCADEADA ESTATICA: VETOR DE NOS COM INDICES PARA PROXIMO E LIVRE
 
#define LE_NIL  (-1)  
 
typedef struct {
    Atleta dado;
    int    proximo;   // Indice do proximo elemento ou LE_NIL se for o ultimo
    int    livre;     // Indice do proximo slot livre ou LE_NIL se nao houver mais
} NoEstatico;
 
typedef struct {
    NoEstatico nos[MAX_LISTA_ESTATICA]; // Array de nos
    int cabeca;      // Indice do primeiro elemento ativo (LE_NIL se vazio)
    int topo_livre;  // Indice do primeiro slot livre (LE_NIL se cheio)
    int tamanho;     // Quantidade de elementos ativos na lista
    int capacidade;  // Capacidade total da lista (MAX_LISTA_ESTATICA)
} ListaEstatica;
 
// Operacoes basicas: O(1) para insercao no inicio, O(n) para insercao no fim, busca e remocao 

void le_inicializa(ListaEstatica *lista); // Inicializa a lista, definindo cabeca e topo_livre
int  le_insere_inicio(ListaEstatica *lista, Atleta atleta); // Retorna 1 se inserido, 0 se lista cheia
int  le_insere_fim(ListaEstatica *lista, Atleta atleta); // Retorna 1 se inserido, 0 se lista cheia
void le_imprime(const ListaEstatica *lista); // Imprime a lista no formato "Posicao | Nome | Tempo | Pace"
int  le_busca(const ListaEstatica *lista, int numero); // Retorna indice do atleta ou -1 se nao encontrado
int  le_remove(ListaEstatica *lista, int numero); // Retorna 1 se removido, 0 se nao encontrado
void le_libera(ListaEstatica *lista); // Para compatibilidade, embora nao seja necessario liberar memoria em lista estatica
 
// Utilitarios para ordenacao e atualizacao de posicoes 
int  le_para_vetor(const ListaEstatica *lista, Atleta *vetor, int max); // Converte a lista para um vetor (usado para ordenacao)
void le_de_vetor(ListaEstatica *lista, const Atleta *vetor, int n); // Reconstrói a lista a partir de um vetor ordenado
void le_atualizar_posicoes(ListaEstatica *lista); // Recalcula posicoes com base na ordem atual
 
#endif