#ifndef ATLETA_H
#define ATLETA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 64
#define MAX 10010

typedef enum {
    ATIVO    = 0,   // Atleta correndo
    FINALIZADO = 1, // Atleta terminou a corrida
    DNF      = 2    // Atleta não finalizou
} StatusAtleta;

// Estrutura do Atleta
typedef struct {
    int numero;
    char nome[MAX_NOME];
    float tempoSegundos;
    float pace; 
    float distanciaKM;
    int posicao;
    StatusAtleta status;
} Atleta;

// Estrutura para seleção de critério de ordenação
typedef enum {
    CRITERIO_POSICAO  = 0,
    CRITERIO_PACE     = 1,
    CRITERIO_TEMPO    = 2,
    CRITERIO_NOME     = 3,
    CRITERIO_NUMERO   = 4,
    CRITERIO_DISTANCIA= 5
} CriterioOrdenacao;

// Funcao de comparacao generica
typedef int (*FuncComparacao)(const Atleta *a, const Atleta *b);
 
// Retorna funcao de comparacao para o criterio dado
FuncComparacao obter_comparador(CriterioOrdenacao criterio);
 
// Comparadores individuais
int cmp_posicao  (const Atleta *a, const Atleta *b);
int cmp_pace     (const Atleta *a, const Atleta *b);
int cmp_tempo    (const Atleta *a, const Atleta *b);
int cmp_nome     (const Atleta *a, const Atleta *b);
int cmp_numero   (const Atleta *a, const Atleta *b);
int cmp_distancia(const Atleta *a, const Atleta *b);
 
// Utilitarios
void imprimir_atleta(const Atleta *a);
void gerar_atleta_aleatorio(Atleta *a, int numero);
 
#endif 