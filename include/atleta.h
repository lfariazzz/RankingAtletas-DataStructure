#ifndef ATLETA_H
#define ATLETA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10000 // Capacidade para testes de volume

// --- ESTRUTURAS ---
typedef struct {
    int numeroPeito;
    char nome[50];
    double distanciaKM;
    double tempoSegundos;
    double pace;          // min/km
    int posicao;          // Definida após ordenação
} Atleta;

typedef struct No { // Dinâmica
    Atleta dado;
    struct No *proximo;
} No;

typedef struct { // Estática
    Atleta dado;
    int proximo;
} NoEstatico;

// --- PROTÓTIPOS ---

// atleta.c (Regras de Negócio)
void calcula_pace(Atleta *a);

// lista_estatica.c (Gerenciamento de Vetor)
void inicializa_estatica(NoEstatico lista[], int *inicio, int *disponivel);
int insere_estatica(NoEstatico lista[], int *inicio, int *disponivel, Atleta a);
void imprime_estatica(NoEstatico lista[], int inicio);
int busca_estatica(NoEstatico lista[], int inicio, int numeroPeito);
int remove_estatica(NoEstatico lista[], int *inicio, int *disponivel, int numeroPeito);
void atribui_posicoes_estatica(NoEstatico lista[], int inicio);

// lista_dinamica.c (Gerenciamento de Memória)
void inicializa_dinamica(No **inicio);
int insere_dinamica(No **inicio, Atleta a);
void imprime_dinamica(No *inicio);
int busca_dinamica(No *inicio, int numeroPeito);
int remove_dinamica(No **inicio, int numeroPeito);
void atribui_posicoes_dinamica(No *inicio);
void libera_dinamica(No **inicio);

// sorts.c (TODOS os Algoritmos)
// Critério: Sempre pelo menor Tempo (tempoSegundos)

// Estáticos
void bubble_sort_estatico(NoEstatico lista[], int inicio);
void selection_sort_estatico(NoEstatico lista[], int inicio);
void insertion_sort_estatico(NoEstatico lista[], int inicio);
void quick_sort_estatico(NoEstatico lista[], int inicio, int fim_logico);
void merge_sort_estatico(NoEstatico lista[], int inicio, int fim_logico);

// Dinâmicos
void bubble_sort_dinamico(No *inicio);
void selection_sort_dinamico(No *inicio);
void insertion_sort_dinamico(No *inicio);
void quick_sort_dinamico(No *inicio, No *fim);
void merge_sort_dinamico(No **inicio);

#endif
