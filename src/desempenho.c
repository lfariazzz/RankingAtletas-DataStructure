#define _POSIX_C_SOURCE 200809L
#include "desempenho.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ============================================================
 * GERACAO DE ENTRADAS DE TESTE
 * ============================================================ */

/* Comparador padrao para gerar entrada ordenada/invertida */
static int cmp_tempo_padrao(const Atleta *a, const Atleta *b) {
    if (a->tempo_segundos < b->tempo_segundos) return -1;
    if (a->tempo_segundos > b->tempo_segundos) return  1;
    return 0;
}

static int cmp_tempo_inv(const Atleta *a, const Atleta *b) {
    return -cmp_tempo_padrao(a, b);
}

void gerar_entrada(Atleta *v, int n, TipoEntrada tipo) {
    /* Sempre começa com dados aleatorios */
    for (int i = 0; i < n; i++)
        gerar_atleta_aleatorio(&v[i], i + 1);

    if (tipo == ENTRADA_ORDENADO) {
        /* Ordena com merge sort para criar entrada ja ordenada */
        Atleta *aux = malloc(n * sizeof(Atleta));
        if (aux) {
            _merge_sort_rec(v, aux, 0, n - 1, cmp_tempo_padrao);
            free(aux);
        }
    } else if (tipo == ENTRADA_INV_ORDENADO) {
        /* Ordena inversamente — pior caso para Bubble/Insertion/Selection */
        Atleta *aux = malloc(n * sizeof(Atleta));
        if (aux) {
            _merge_sort_rec(v, aux, 0, n - 1, cmp_tempo_inv);
            free(aux);
        }
    }
    /* ENTRADA_ALEATORIO: ja esta aleatorio */
}

/* ============================================================
 * MEDICAO DE TEMPO (nanosegundos via clock_gettime)
 * ============================================================ */

static double agora_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}

/* ============================================================
 * MEDICAO SOBRE VETOR
 * ============================================================ */

ResultadoAgregado medir_sort_vetor(
    long (sort)(Atleta, int, FuncComparacao),
    int n,
    TipoEntrada tipo,
    FuncComparacao cmp,
    int repeticoes)
{
    Atleta *v    = malloc(n * sizeof(Atleta));
    Atleta *copia = malloc(n * sizeof(Atleta));

    double soma_tempo = 0.0;
    double soma_sq    = 0.0;
    double soma_cmp   = 0.0;

    /* Gera entrada base uma vez */
    gerar_entrada(v, n, tipo);

    for (int r = 0; r < repeticoes; r++) {
        /* Restaura vetor original antes de cada execucao */
        memcpy(copia, v, n * sizeof(Atleta));

        double t0  = agora_ns();
        long   comp = sort(copia, n, cmp);
        double t1  = agora_ns();

        double dt = t1 - t0;
        soma_tempo += dt;
        soma_sq    += dt * dt;
        soma_cmp   += (double)comp;
    }

    free(v);
    free(copia);

    ResultadoAgregado res;
    res.media_tempo_ns    = soma_tempo / repeticoes;
    res.media_comparacoes = soma_cmp   / repeticoes;

    double variancia = (soma_sq / repeticoes) -
                       (res.media_tempo_ns * res.media_tempo_ns);
    res.desvio_tempo_ns = (variancia > 0) ? sqrt(variancia) : 0.0;

    return res;
}

/* ============================================================
 * MEDICAO SOBRE LISTA DINAMICA
 * ============================================================ */

ResultadoAgregado medir_sort_lista_dinamica(
    long (sort)(Atleta, int, FuncComparacao),
    int n,
    TipoEntrada tipo,
    FuncComparacao cmp,
    int repeticoes)
{
    Atleta *v    = malloc(n * sizeof(Atleta));
    Atleta *buf  = malloc(n * sizeof(Atleta));
    ListaDinamica lista;

    double soma_tempo = 0.0;
    double soma_sq    = 0.0;
    double soma_cmp   = 0.0;

    gerar_entrada(v, n, tipo);

    for (int r = 0; r < repeticoes; r++) {
        /* Reconstroi a lista antes de cada medicao */
        ld_inicializa(&lista);
        for (int i = 0; i < n; i++)
            ld_insere_fim(&lista, v[i]);

        /* Exporta para buffer, ordena, reconstroi */
        int qtd = ld_para_vetor(&lista, buf, n);

        double t0   = agora_ns();
        long   comp = sort(buf, qtd, cmp);
        double t1   = agora_ns();

        ld_de_vetor(&lista, buf, qtd);

        double dt = t1 - t0;
        soma_tempo += dt;
        soma_sq    += dt * dt;
        soma_cmp   += (double)comp;

        ld_libera(&lista);
    }

    free(v);
    free(buf);

    ResultadoAgregado res;
    res.media_tempo_ns    = soma_tempo / repeticoes;
    res.media_comparacoes = soma_cmp   / repeticoes;

    double variancia = (soma_sq / repeticoes) -
                       (res.media_tempo_ns * res.media_tempo_ns);
    res.desvio_tempo_ns = (variancia > 0) ? sqrt(variancia) : 0.0;

    return res;
}