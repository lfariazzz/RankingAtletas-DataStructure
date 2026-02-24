#include "atleta.h"

int main() {
    // 1. Declaração das estruturas da lista estática
    NoEstatico ranking[MAX];
    int inicio, disponivel;

    // 2. Inicialização (Onde você organiza o "armário")
    inicializa_estatica(ranking, &inicio, &disponivel);

    // 3. Criando atletas de teste
    Atleta a1 = {101, "Levi", 3600.5, 4.5}; // Exemplo de dados
    Atleta a2 = {202, "Ana", 3450.2, 4.2};

    // 4. Inserindo no ranking
    insere_estatica(ranking, &inicio, &disponivel, a1);
    insere_estatica(ranking, &inicio, &disponivel, a2);

    // 5. Exibindo o resultado
    imprime_estatica(ranking, inicio);

    return 0;
}
