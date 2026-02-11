// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões estratégicas para cada jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de missões e tamanho máximo de strings, facilitando a manutenção.
#define TOTAL_MISSOES 5
#define MAX_MISSAO_LEN 100

// --- Vetor de Missões Estratégicas ---
// Contém as descrições pré-definidas de missões que podem ser sorteadas para cada jogador.
char* missoes[TOTAL_MISSOES] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor Vermelho",
    "Dominar pelo menos 4 territorios",
    "Conquistar todos os territorios do mapa",
    "Ter mais de 10 tropas em um unico territorio"
};

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char corExercito[10];
    int numeroTropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
void exibirMapa(struct Territorio* mapa, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, const char* corJogador);
void exibirMissao(const char* missao);
void liberarMemoria(struct Territorio* mapa, char** missoesJogadores, char** coresJogadores, int numJogadores);

// --- Implementação das Funções ---

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin),
// evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa.
// Recebe o mapa por ponteiro para leitura dos dados.
void exibirMapa(struct Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (Exercito %s, Tropa: %d)\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
}

// atacar():
// Executa a lógica de uma batalha entre dois territórios usando rand() para simular dados (1 a 6).
// Se o atacante vencer, o defensor perde uma tropa. Se o território for conquistado,
// transfere a cor e metade das tropas. Caso contrário, o atacante perde uma tropa.
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATACANTE! O defensor perdeu %d tropas.\n", 1);
        defensor->numeroTropas--;

        if (defensor->numeroTropas <= 0) {
            printf("CONQUISTA! O territorio %s foi conquistado pelo exercito %s!\n", defensor->nome, atacante->corExercito);

            // 1. TRANSFERE A COR (Isso muda o dono!)
            strcpy(defensor->corExercito, atacante->corExercito);

            // 2. TRANSFERE METADE DAS TROPAS
            int tropasMovidas = atacante->numeroTropas / 2;

            // Garante que mova pelo menos 1 soldado
            if (tropasMovidas < 1 && atacante->numeroTropas > 1) tropasMovidas = 1;

           defensor->numeroTropas = tropasMovidas;
           atacante->numeroTropas -= tropasMovidas;

           printf("%d tropas foram movidas para o novo territorio.\n", tropasMovidas);
        }
    } else {
        // --- CENÁRIO 2: DEFENSOR VENCE ---
        printf("DEFESA BEM SUCEDIDA! O atacante perdeu 1 tropa.\n");
        if (atacante->numeroTropas > 0) {
            atacante->numeroTropas--;
        }
    }
}

// atribuirMissao():
// Sorteia uma missão aleatória do vetor de missões e copia para a variável de missão do jogador.
// Recebe o destino por ponteiro (passagem por referência) para atribuição.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// verificarMissao():
// Avalia se a missão do jogador foi cumprida com base no estado atual do mapa.
// Recebe a missão e a cor do jogador por ponteiro para verificação.
// Retorna 1 se a missão foi cumprida, 0 caso contrário.
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, const char* corJogador) {
    // Missão: "Conquistar 3 territorios seguidos"
    // Verifica se o jogador possui pelo menos 3 territórios consecutivos
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                consecutivos++;
                if (consecutivos >= 3) return 1;
            } else {
                consecutivos = 0;
            }
        }
        return 0;
    }

    // Missão: "Eliminar todas as tropas da cor Vermelho"
    // Verifica se não existe nenhum território com a cor "Vermelho"
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelho") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, "Vermelho") == 0) {
                return 0;
            }
        }
        return 1;
    }

    // Missão: "Dominar pelo menos 4 territorios"
    // Verifica se o jogador possui pelo menos 4 territórios
    if (strcmp(missao, "Dominar pelo menos 4 territorios") == 0) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) {
                count++;
            }
        }
        return count >= 4;
    }

    // Missão: "Conquistar todos os territorios do mapa"
    // Verifica se todos os territórios pertencem ao jogador
    if (strcmp(missao, "Conquistar todos os territorios do mapa") == 0) {
        if (tamanho == 0) return 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) != 0) {
                return 0;
            }
        }
        return 1;
    }

    // Missão: "Ter mais de 10 tropas em um unico territorio"
    // Verifica se algum território do jogador tem mais de 10 tropas
    if (strcmp(missao, "Ter mais de 10 tropas em um unico territorio") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0 && mapa[i].numeroTropas > 10) {
                return 1;
            }
        }
        return 0;
    }

    return 0;
}

// exibirMissao():
// Exibe a descrição da missão do jogador. Recebe a missão por valor (const) para apenas leitura.
void exibirMissao(const char* missao) {
    printf("\n*** SUA MISSAO SECRETA: %s ***\n", missao);
}

// liberarMemoria():
// Libera toda a memória alocada dinamicamente (territórios, missões e cores dos jogadores).
// Evita vazamentos de memória ao final da execução.
void liberarMemoria(struct Territorio* mapa, char** missoesJogadores, char** coresJogadores, int numJogadores) {
    // Libera a memória das missões e cores de cada jogador
    for (int i = 0; i < numJogadores; i++) {
        free(missoesJogadores[i]);
        free(coresJogadores[i]);
    }
    // Libera os vetores de ponteiros
    free(missoesJogadores);
    free(coresJogadores);
    // Libera a memória do mapa de territórios
    free(mapa);
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    int quantidadeTerritorios;
    struct Territorio *territorio;

    printf("======================================\n");
    printf("  WAR ESTRUTURADO - CADASTRO INICIAL\n");
    printf("======================================\n");

    // Pergunta quantos territórios cadastrar
    printf("Quantos territorios deseja cadastrar: ");
    scanf("%d", &quantidadeTerritorios);
    limparBufferEntrada();

    // Aloca memória para os territórios usando calloc
    territorio = (struct Territorio*) calloc(quantidadeTerritorios, sizeof(struct Territorio));

    if (territorio == NULL) {
        printf("Erro de memoria!\n");
        return 1;
    }

    // Cadastro dos territórios
    for (int i = 0; i < quantidadeTerritorios; i++) {
        printf("\n--- Cadastrando Territorio %d ---", i + 1);
        printf("\nNome do Territorio: ");
        fgets(territorio[i].nome, 30, stdin);
        territorio[i].nome[strcspn(territorio[i].nome, "\n")] = '\0';

        printf("Cor do Exercito (ex: Azul, Vermelho): ");
        fgets(territorio[i].corExercito, 10, stdin);
        territorio[i].corExercito[strcspn(territorio[i].corExercito, "\n")] = '\0';

        printf("Numero de Tropas: ");
        scanf("%d", &territorio[i].numeroTropas);
        limparBufferEntrada();
    }

    printf("\nCadastro inicial concluido com sucesso!\n");

    // Exibição dos territórios cadastrados
    printf("\n======================================\n");
    printf("   MAPA DO MUNDO - ESTADO ATUAL \n");
    printf("======================================\n");
    for (int i = 0; i < quantidadeTerritorios; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, territorio[i].nome, territorio[i].corExercito, territorio[i].numeroTropas);
    }

    // --- Sistema de Missões Estratégicas ---
    // Pergunta quantos jogadores participarão e aloca missão para cada um
    int numJogadores;
    printf("\nQuantos jogadores participam? ");
    scanf("%d", &numJogadores);
    limparBufferEntrada();

    // Aloca vetor de ponteiros para as missões dos jogadores
    char** missoesJogadores = (char**) malloc(numJogadores * sizeof(char*));
    if (missoesJogadores == NULL) {
        printf("Erro de memoria ao alocar missoes!\n");
        free(territorio);
        return 1;
    }

    // Aloca vetor de ponteiros para as cores dos jogadores
    char** coresJogadores = (char**) malloc(numJogadores * sizeof(char*));
    if (coresJogadores == NULL) {
        printf("Erro de memoria ao alocar cores!\n");
        free(missoesJogadores);
        free(territorio);
        return 1;
    }

    // Aloca memória para a missão e cor de cada jogador e atribui missão sorteada
    for (int i = 0; i < numJogadores; i++) {
        missoesJogadores[i] = (char*) malloc(MAX_MISSAO_LEN * sizeof(char));
        coresJogadores[i] = (char*) malloc(10 * sizeof(char));
        if (missoesJogadores[i] == NULL || coresJogadores[i] == NULL) {
            printf("Erro de memoria ao alocar dados do jogador %d!\n", i + 1);
            // Libera memória já alocada antes de sair
            for (int j = 0; j <= i; j++) {
                free(missoesJogadores[j]);
                free(coresJogadores[j]);
            }
            free(missoesJogadores);
            free(coresJogadores);
            free(territorio);
            return 1;
        }
        printf("Cor do exercito do Jogador %d: ", i + 1);
        fgets(coresJogadores[i], 10, stdin);
        coresJogadores[i][strcspn(coresJogadores[i], "\n")] = '\0';
        atribuirMissao(missoesJogadores[i], missoes, TOTAL_MISSOES);
    }

    // Exibe a missão de cada jogador apenas uma vez (no início do jogo)
    printf("\n======================================\n");
    printf("   MISSOES SECRETAS DOS JOGADORES\n");
    printf("======================================\n");
    for (int i = 0; i < numJogadores; i++) {
        printf("Jogador %d:", i + 1);
        exibirMissao(missoesJogadores[i]);
    }

    // --- Laço Principal do Jogo (Game Loop) ---
    // Roda em loop até o jogador sair (opção 0) ou algum jogador cumprir sua missão.
    int idAtacante, idDefensor;
    int vencedor = 0;

    while (1) {
        // Exibição dos territórios cadastrados
        printf("\n======================================\n");
        printf("   MAPA DO MUNDO - ESTADO ATUAL \n");
        printf("======================================\n");

        exibirMapa(territorio, quantidadeTerritorios);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", quantidadeTerritorios);
        scanf("%d", &idAtacante);
        limparBufferEntrada();

        if (idAtacante == 0) {
            printf("Jogo encerrado e memoria liberada. Ate a proxima!\n");
            break;
        }

        printf("Escolha o territorio defensor (1 a %d): ", quantidadeTerritorios);
        scanf("%d", &idDefensor);
        limparBufferEntrada();

        int idxA = idAtacante - 1;
        int idxD = idDefensor - 1;

        if (idxA >= 0 && idxA < quantidadeTerritorios && idxD >= 0 && idxD < quantidadeTerritorios && idxA != idxD) {
            // Verificação de cor para evitar fogo amigo
            if (strcmp(territorio[idxA].corExercito, territorio[idxD].corExercito) == 0) {
                printf("ERRO: Voce nao pode atacar seu proprio exercito!\n");
            } else {
                atacar(&territorio[idxA], &territorio[idxD]);
            }
        } else {
            printf("Jogada invalida!\n");
        }

        // Verificação silenciosa de missões ao final de cada turno
        for (int i = 0; i < numJogadores; i++) {
            if (verificarMissao(missoesJogadores[i], territorio, quantidadeTerritorios, coresJogadores[i])) {
                printf("\n======================================\n");
                printf("   VITORIA! JOGADOR %d VENCEU!\n", i + 1);
                printf("   Missao cumprida: %s\n", missoesJogadores[i]);
                printf("======================================\n");
                vencedor = 1;
                break;
            }
        }

        if (vencedor) break;

        printf("\nPressione Enter para continuar...");
        getchar();
    }

    // 3. Limpeza:
    // Ao final do jogo, libera toda a memória alocada dinamicamente para evitar vazamentos.
    liberarMemoria(territorio, missoesJogadores, coresJogadores, numJogadores);

    return 0;
}