#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "atleta.h"
#include "lista_dinamica.h"
#include "lista_estatica.h"
#include "ordenacao.h"
#include "desempenho.h"

// Função de demonstração do sistema de ranking

static void demo_ranking(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║    SISTEMA DE RANKING - CORRIDA DE RUA 10K          ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");

    srand(42); // Semente fixa para resultados reproduzíveis

    ListaDinamica lista;
    ld_inicializa(&lista);

    // Gerar e inserir atletas aleatorios
    printf(">> Inscrevendo 10 atletas...\n");
    for (int i = 1; i <= 10; i++) {
        Atleta a;
        gerar_atleta_aleatorio(&a, i);
        a.status = ATIVO; // Todos começam como ativos
        ld_insere_fim(&lista, a);
    }

    // Ordena por pace e exibe ranking inicial
    Atleta buf[20];
    int n = ld_para_vetor(&lista, buf, 20);
    merge_sort(buf, n, cmp_pace);
    ld_de_vetor(&lista, buf, n);
    ld_atualizar_posicoes(&lista);

    printf("\n--- RANKING INICIAL (ordenado por Pace) ---\n");
    ld_imprime(&lista);

    // Simula chegada de novo atleta e saida por DNF
    printf("\n>> Novo atleta inscrito no dorsal #999...\n");
    Atleta novo;
    gerar_atleta_aleatorio(&novo, 999);
    novo.status = ATIVO;
    ld_insere_fim(&lista, novo);

    // Reordena por pace apos chegada do novo atleta
    printf(">> Atleta dorsal #3 abandona a prova (DNF)...\n");
    No *alvo = ld_busca(&lista, 3);
    if (alvo) {
        alvo->dado.status = DNF;
        printf("   Status atualizado: ");
        imprimir_atleta(&alvo->dado);
    }
    ld_remove(&lista, 3);

    // Reordena por pace apos mudancas
    n = ld_para_vetor(&lista, buf, 20);
    merge_sort(buf, n, cmp_pace);
    ld_de_vetor(&lista, buf, n);
    ld_atualizar_posicoes(&lista);

    printf("\n--- RANKING ATUALIZADO (por Pace, apos mudancas) ---\n");
    ld_imprime(&lista);

    // Reordena por numero do dorsal
    n = ld_para_vetor(&lista, buf, 20);
    insertion_sort(buf, n, cmp_numero);
    ld_de_vetor(&lista, buf, n);

    printf("\n--- LISTAGEM POR NUMERO DO DORSAL ---\n");
    ld_imprime(&lista);

    // Reordena por nome
    n = ld_para_vetor(&lista, buf, 20);
    quick_sort(buf, n, cmp_nome);
    ld_de_vetor(&lista, buf, n);

    printf("\n--- LISTAGEM ALFABETICA (por Nome) ---\n");
    ld_imprime(&lista);

    ld_libera(&lista);

    // Agora repetimos o mesmo processo usando a lista estatica para demonstrar a versatilidade do sistema
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║  MESMA OPERACAO COM LISTA ENCADEADA ESTATICA    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n\n");

    srand(42);
    ListaEstatica le;
    le_inicializa(&le);

    for (int i = 1; i <= 10; i++) {
        Atleta a;
        gerar_atleta_aleatorio(&a, i);
        a.status = ATIVO;
        le_insere_fim(&le, a);
    }

    Atleta buf2[20];
    n = le_para_vetor(&le, buf2, 20);
    merge_sort(buf2, n, cmp_pace);
    le_de_vetor(&le, buf2, n);
    le_atualizar_posicoes(&le);

    printf("--- RANKING INICIAL - Lista Estatica (por Pace) ---\n");
    le_imprime(&le);

    printf("\n>> Removendo dorsal #3 (DNF) da lista estatica...\n");
    le_remove(&le, 3);

    n = le_para_vetor(&le, buf2, 20);
    merge_sort(buf2, n, cmp_pace);
    le_de_vetor(&le, buf2, n);
    le_atualizar_posicoes(&le);

    printf("\n--- RANKING ATUALIZADO - Lista Estatica ---\n");
    le_imprime(&le);

    le_libera(&le);
}

// Ponto de entrada do programa

int main(int argc, char *argv[]) {
    // Semente aleatoria para gerar atletas diferentes a cada execucao
    srand((unsigned int)time(NULL));

    if (argc > 1 && strcmp(argv[1], "--benchmark") == 0) {
        // Modo benchmark: executa bateria completa e gera CSV
        int repeticoes = 100;
        if (argc > 2) repeticoes = atoi(argv[2]);
        fprintf(stderr, "[INFO] Iniciando bateria com %d repeticoes...\n",
                repeticoes);
        executar_bateria_completa(repeticoes);
    } else {
        // Modo demonstracao: mostra o sistema de ranking em acao
        demo_ranking();

        printf("\n\n");
        printf("Para gerar dados de benchmark (CSV), execute:\n");
        printf("  ./ranking --benchmark 100\n\n");
    }

    return 0;
}