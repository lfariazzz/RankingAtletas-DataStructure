#include "atleta.h"

void inicializa_estatica(NoEstatico lista[], int *inicio, int *disponivel) {
    *inicio = -1; // Ranking começa vazio
    *disponivel = 0; // Primeira gaveta livre é a zero

    // Cria o encadeamento da lista de espaços disponíveis
    for (int i = 0; i < MAX - 1; i++) {
        lista[i].proximo = i + 1;
    }
    lista[MAX - 1].proximo = -1; // Última gaveta não tem próxima
}

int insere_estatica(NoEstatico lista[], int *inicio, int *disponivel, Atleta a) {
    // 1. Verifica se o "armário" está cheio
    if (*disponivel == -1) {
        return 0; // Erro: lista cheia
    }

    // 2. "Pega" a primeira gaveta livre
    int novoIndice = *disponivel;

    // 3. O ponteiro 'disponivel' pula para a próxima gaveta que estava na fila de espera
    *disponivel = lista[novoIndice].proximo;

    // 4. Colocamos os dados do atleta na gaveta que pegamos
    lista[novoIndice].dado = a;

    // 5. Fazemos essa nova gaveta apontar para quem era o antigo 'inicio'
    lista[novoIndice].proximo = *inicio;

    // 6. O novo 'inicio' do ranking agora é essa gaveta
    *inicio = novoIndice;

    return 1; // Sucesso!
}

void imprime_estatica(NoEstatico lista[], int inicio) {
    int i = inicio;
    printf("\n--- RANKING DE ATLETAS (LISTA ESTATICA) ---\n");
    while (i != -1) {
        printf("Numero: %d | Nome: %-15s | Tempo: %.2fs | Pace: %.2f\n",
                lista[i].dado.numeroPeito,
                lista[i].dado.nome,
                lista[i].dado.tempoSegundos,
                lista[i].dado.pace);
        i = lista[i].proximo;
    }
    printf("-------------------------------------------\n");
}
