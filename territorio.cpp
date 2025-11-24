#include <stdio.h>
#include <string.h>

/*
    Estrutura Territorio:
    - Armazena informações relacionadas a um território:
      nome, cor do exército e quantidade de tropas.
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor que armazenará os 5 territórios
    Territorio territorios[5];

    printf("=== Cadastro de Territorios ===\n");

    // Cadastro dos territórios
    for (int i = 0; i < 5; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

        // Entrada do nome
        printf("Digite o nome do territorio: ");
        scanf("%29s", territorios[i].nome);

        // Entrada da cor do exército
        printf("Digite a cor do exercito: ");
        scanf("%9s", territorios[i].cor);

        // Entrada da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\n\n=== Territorios Cadastrados ===\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}

