#include "atleta.h"

int main() {
    // 1. SETUP: Inicializando os dois motores
    NoEstatico listaE[MAX];
    int iniE, dispE;
    inicializa_estatica(listaE, &iniE, &dispE);

    No *listaD;
    inicializa_dinamica(&listaD);

    // 2. CRIAÇÃO: Dados dos Atletas (mesma distância para teste)
    Atleta a1 = {10, "Levi", 5.0, 1200.0};  // Pace 4.0
    Atleta a2 = {20, "Ana", 5.0, 1140.0};   // Pace 3.8
    Atleta a3 = {30, "Neto", 5.0, 1320.0};  // Pace 4.4

    // 3. CÁLCULO: Aplicando a regra de negócio do atleta.c
    calcula_pace(&a1);
    calcula_pace(&a2);
    calcula_pace(&a3);

    // 4. INSERÇÃO: Alimentando as duas estruturas
    // Testando a Estática
    insere_estatica(listaE, &iniE, &dispE, a1);
    insere_estatica(listaE, &iniE, &dispE, a2);
    insere_estatica(listaE, &iniE, &dispE, a3);

    // Testando a Dinâmica
    insere_dinamica(&listaD, a1);
    insere_dinamica(&listaD, a2);
    insere_dinamica(&listaD, a3);

    // 5. RANKING: Atribuindo posições (simulando pós-ordenação)
    atribui_posicoes_estatica(listaE, iniE);
    atribui_posicoes_dinamica(listaD);

    // 6. EXIBIÇÃO: Comparando os resultados no console
    printf("===========================================");
    imprime_estatica(listaE, iniE);
    printf("===========================================");
    imprime_dinamica(listaD);
    printf("===========================================");

    // 7. LIMPEZA: Devolvendo memória RAM (essencial na dinâmica!)
    libera_dinamica(&listaD);
    printf("\n[SISTEMA]: Memoria RAM liberada com sucesso.\n");

    return 0;
}
