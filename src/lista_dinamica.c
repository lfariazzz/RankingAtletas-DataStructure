#include "atleta.h"

// 1. Inicializa: Apenas aponta o início para o vazio (NULL)
void inicializa_dinamica(No **inicio) {
    *inicio = NULL;
}

// 2. Insere: Pede memória ao sistema e coloca o novo nó no topo
int insere_dinamica(No **inicio, Atleta a) {
    // Aloca espaço para um No na memória RAM
    No *novo = (No *) malloc(sizeof(No));

    // Verifica se o computador conseguiu dar a memória
    if (novo == NULL) return 0;

    novo->dado = a;

    // O novo nó aponta para quem era o antigo primeiro
    novo->proximo = *inicio;

    // O ponteiro de início agora aponta para este novo nó
    *inicio = novo;

    return 1;
}

// 3. Busca: Percorre a RAM até achar o número de peito
int busca_dinamica(No *inicio, int numeroPeito) {
    No *aux = inicio;
    while (aux != NULL) {
        if (aux->dado.numeroPeito == numeroPeito) {
            return 1; // Encontrou
        }
        aux = aux->proximo;
    }
    return 0; // Não encontrou
}

// 4. Remove: Desconecta o nó e usa o FREE para devolver a memória
int remove_dinamica(No **inicio, int numeroPeito) {
    No *atual = *inicio;
    No *anterior = NULL;

    while (atual != NULL && atual->dado.numeroPeito != numeroPeito) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) return 0; // Atleta não está na lista

    if (anterior == NULL) {
        // Remoção do primeiro da lista
        *inicio = atual->proximo;
    } else {
        // Pula o nó que será removido
        anterior->proximo = atual->proximo;
    }

    free(atual); // Devolve o espaço para a RAM (o "lixeiro" do C)
    return 1;
}

// 5. Atribui Posições: Essencial para o ranking final da UFCA
void atribui_posicoes_dinamica(No *inicio) {
    No *aux = inicio;
    int p = 1;
    while (aux != NULL) {
        aux->dado.posicao = p++;
        aux = aux->proximo;
    }
}

// 6. Imprime: Gera o relatório para o console
void imprime_dinamica(No *inicio) {
    No *aux = inicio;
    printf("\n--- RANKING OFICIAL (DINAMICO) ---\n");
    if (inicio == NULL) printf("Lista vazia.\n");

    while (aux != NULL) {
        printf("%dº | %-15s | Pace: %.2f | Tempo: %.1fs\n",
               aux->dado.posicao,
               aux->dado.nome,
               aux->dado.pace,
               aux->dado.tempoSegundos);
        aux = aux->proximo;
    }
}

// 7. Libera: Limpa a RAM inteira antes de fechar o programa
void libera_dinamica(No **inicio) {
    No *aux;
    while (*inicio != NULL) {
        aux = *inicio;
        *inicio = (*inicio)->proximo;
        free(aux);
    }
    *inicio = NULL;
}
