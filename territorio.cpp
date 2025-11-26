#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos das funções
Territorio* alocarTerritorios(int num);
void cadastrarTerritorios(Territorio* mapa, int num);
void exibirTerritorios(Territorio* mapa, int num);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
int rolarDado();

int main() {
    int numTerritorios;
    Territorio* mapa = NULL;
    int escolhaAtacante, escolhaDefensor;

    // 1. Inicializa o gerador de números aleatórios para a simulação de dados
    srand(time(NULL));

    printf("Bem-vindo ao simulador de batalha War!\n");

    // 2. Alocação dinâmica de territórios
    printf("Informe o numero total de territorios a serem cadastrados: ");
    if (scanf("%d", &numTerritorios) != 1 || numTerritorios <= 0) {
        printf("Numero invalido. Saindo do programa.\n");
        return 1;
    }

    mapa = alocarTerritorios(numTerritorios);
    if (mapa == NULL) {
        printf("Falha na alocacao de memoria. Saindo do programa.\n");
        return 1;
    }

    // 3. Cadastro inicial dos territórios
    cadastrarTerritorios(mapa, numTerritorios);

    // Loop principal do jogo/simulação
    while (1) {
        exibirTerritorios(mapa, numTerritorios);

        printf("\n--- SIMULACAO DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", numTerritorios);
        scanf("%d", &escolhaAtacante);

        if (escolhaAtacante == 0) {
            break; // Sair do loop principal
        }

        // Validação básica do índice do atacante
        if (escolhaAtacante < 1 || escolhaAtacante > numTerritorios) {
            printf("Escolha invalida para o atacante.\n");
            continue;
        }

        printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
        scanf("%d", &escolhaDefensor);

        // Validação básica do índice do defensor
        if (escolhaDefensor < 1 || escolhaDefensor > numTerritorios) {
            printf("Escolha invalida para o defensor.\n");
            continue;
        }

        // Ajusta para índices baseados em zero para acesso ao array
        int idxAtacante = escolhaAtacante - 1;
        int idxDefensor = escolhaDefensor - 1;

        // Requisito: Validar para não atacar o próprio território
        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Erro: Voce nao pode atacar um territorio da sua propria cor (%s).\n", mapa[idxAtacante].cor);
            continue;
        }

        // Requisito: O atacante precisa ter pelo menos 2 tropas para atacar (1 fica no território, 1 ataca)
        if (mapa[idxAtacante].tropas < 2) {
            printf("Erro: O territorio atacante precisa ter no minimo 2 tropas para iniciar um ataque.\n");
            continue;
        }
        
        // 4. Executa a função de ataque usando ponteiros
        // Passamos os endereços dos territórios específicos no array
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
    }

    // 5. Libera a memória alocada dinamicamente
    liberarMemoria(mapa);
    printf("Memoria liberada. Fim do programa.\n");

    return 0;
}

/**
 * Aloca dinamicamente memória para um vetor de territórios.
 * @param num O número de territórios a serem alocados.
 * @return Um ponteiro para o primeiro elemento do vetor alocado, ou NULL se falhar.
 */
Territorio* alocarTerritorios(int num) {
    // Uso de calloc para inicializar a memória com zeros
    Territorio* mapa = (Territorio*)calloc(num, sizeof(Territorio));
    if (mapa == NULL) {
        perror("calloc falhou");
    }
    return mapa;
}

/**
 * Permite ao usuário cadastrar nome, cor e tropas iniciais de cada território.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param num O número total de territórios.
 */
void cadastrarTerritorios(Territorio* mapa, int num) {
    printf("\n--- CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < num; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: ");
        scanf("%s", (mapa + i)->nome); // Acesso via ponteiro
        printf("Cor do exercito (ex: Vermelho): ");
        scanf("%s", (mapa + i)->cor);
        printf("Numero de tropas: ");
        scanf("%d", &(mapa + i)->tropas);
        printf("\n");
    }
}

/**
 * Exibe o estado atual de todos os territórios.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param num O número total de territórios.
 */
void exibirTerritorios(Territorio* mapa, int num) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    for (int i = 0; i < num; i++) {
        // Acesso aos campos da struct usando notação de ponteiro ->
        printf("[%d] %s | Cor: %s | Tropas: %d\n", 
               i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }
    printf("-----------------------------\n");
}

/**
 * Simula a rolagem de um dado de 6 lados.
 * @return Um número aleatório entre 1 e 6.
 */
int rolarDado() {
    return (rand() % 6) + 1;
}

/**
 * Simula um ataque entre dois territórios usando rolagem de dados.
 * Atualiza as tropas e o controle do território defensor se necessário.
 * 
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("\n--- RESULTADO DA BATALHA EM %s ---\n", defensor->nome);
    printf("%s (Atacante) rolou %d | %s (Defensor) rolou %d\n", 
           atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence: transfere cor e metade das tropas para o defensor
        printf("O atacante %s venceu a batalha!\n", atacante->nome);

        // Calcula metade das tropas a serem movidas (pelo menos 1 tropa deve permanecer no território atacante)
        int tropasMovidas = atacante->tropas / 2; 
        if (tropasMovidas >= atacante->tropas) { // Garante que pelo menos 1 fique para trás
            tropasMovidas = atacante->tropas - 1;
        }

        // Requisito: mudar de dono (cor)
        strcpy(defensor->cor, atacante->cor);
        // Requisito: atualizar tropas do defensor com as tropas movidas do atacante
        defensor->tropas = tropasMovidas;
        // Requisito: remover tropas do atacante
        atacante->tropas -= tropasMovidas;

        printf("%s agora controla %s com %d tropas.\n", atacante->cor, defensor->nome, defensor->tropas);
        
    } else {
        // Defensor vence ou empata: atacante perde uma tropa
        printf("O defensor %s resistiu ao ataque!\n", defensor->nome);
        atacante->tropas -= 1;
        if (atacante->tropas < 1) {
            // Em um jogo real, o território ficaria vulnerável ou vazio.
            printf("%s ficou com 0 tropas restantes e esta desprotegido.\n", atacante->nome);
        } else {
            printf("%s perdeu 1 tropa e agora tem %d tropas.\n", atacante->nome, atacante->tropas);
        }
    }
}

/**
 * Libera a memória alocada dinamicamente para o mapa de territórios.
 * @param mapa Ponteiro para o início do vetor de territórios.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        mapa = NULL; // Boa prática para evitar ponteiros pendentes (dangling pointers)
    }
}


