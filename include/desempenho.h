#ifndef DESEMPENHO_H
#define DESEMPENHO_H

#include "atleta.h"
#include "lista_dinamica.h"
#include "lista_estatica.h"
#include "ordenacao.h"

/* ============================================================
 * MEDICAO DE DESEMPENHO
 * Coleta: tempo de execucao (ns) e numero de comparacoes
 * ============================================================ */

/* Tipos de entrada para os testes */
typedef enum {
    ENTRADA_ALEATORIO   = 0,
    ENTRADA_ORDENADO    = 1,
    ENTRADA_INV_ORDENADO = 2    /* Pior caso para alguns algoritmos */
} TipoEntrada;

/* Resultado de uma unica execucao */
typedef struct {
    long   comparacoes;
    double tempo_ns;    /* Tempo em nanossegundos */
} ResultadoExec;

/* Resultado agregado de N repeticoes */
typedef struct {
    double media_tempo_ns;
    double media_comparacoes;
    double desvio_tempo_ns;
} ResultadoAgregado;

/* ---- API de medicao ---- */

/* Gera vetor de atletas com o tipo de entrada solicitado */
void gerar_entrada(Atleta *v, int n, TipoEntrada tipo);

/*
 * Mede desempenho de um sort sobre VETOR (nao usa estrutura).
 * Executa 'repeticoes' vezes e devolve a media.
 * O vetor e regenerado antes de cada execucao para garantir
 * que o tipo de entrada seja preservado.
 */
ResultadoAgregado medir_sort_vetor(
    long (sort)(Atleta, int, FuncComparacao),
    int n,
    TipoEntrada tipo,
    FuncComparacao cmp,
    int repeticoes
);

/*
 * Mede desempenho de um sort sobre LISTA DINAMICA.
 * Converte lista->vetor, ordena, reconstroi.
 */
ResultadoAgregado medir_sort_lista_dinamica(
    long (sort)(Atleta, int, FuncComparacao),
    int n,
    TipoEntrada tipo,
    FuncComparacao cmp,
    int repeticoes
);

/*
 * Mede desempenho de um sort sobre LISTA ESTATICA.
 */
ResultadoAgregado medir_sort_lista_estatica(
    long (sort)(Atleta, int, FuncComparacao),
    int n,
    TipoEntrada tipo,
    FuncComparacao cmp,
    int repeticoes
);

/* Imprime cabecalho CSV */
void imprimir_cabecalho_csv(void);

/* Imprime uma linha CSV com os resultados */
void imprimir_linha_csv(
    const char *algoritmo,
    const char *estrutura,
    int n,
    const char *tipo_entrada,
    const ResultadoAgregado *res
);

/* Executa toda a bateria de testes e imprime CSV */
void executar_bateria_completa(int repeticoes);

#endif /* DESEMPENHO_H */