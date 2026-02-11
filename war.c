// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

/// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[TAM_NOME];
    char corExercito[TAM_COR];
    int numeroTropas;
};
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
struct Territorio* alocarMapa(int quantidade);
void inicializarTerritorios(struct Territorio* mapa, int quantidade);
void liberarMemoria(struct Territorio* mapa);

// Funções de interface com o usuário:
int exibirMenuPrincipal();
void exibirMapa(const struct Territorio* mapa, int quantidade);
void exibirMissao(int idMissao);

// Funções de lógica principal do jogo:
void faseDeAtaque(struct Territorio* mapa, int quantidade);
void simularAtaque(struct Territorio* atacante, struct Territorio* defensor);
int sortearMissao();
int verificarVitoria(int idMissao, const struct Territorio* mapa, int quantidade, const char* corJogador);
int verificarDerrota(const struct Territorio* mapa, int quantidade, const char* corJogador);

// Função utilitária:
void limparBufferEntrada();


// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    int quantidadeTerritorios;
    struct Territorio *mapa;
    int idMissao;
    char corJogador[TAM_COR];

    printf("======================================\n");
    printf("  WAR ESTRUTURADO - CADASTRO INICIAL\n");
    printf("======================================\n");

    // 1. Pergunta o tamanho (Mudança principal do nível novo)
    printf("Quantos territorios deseja cadastrar: ");
    scanf("%d", &quantidadeTerritorios);
    limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    mapa = alocarMapa(quantidadeTerritorios);
    if (mapa == NULL) {
        printf("Erro critico de memoria! Encerrando.\n");
        return 1;
    }

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa, quantidadeTerritorios);

    int indiceSorteio = rand() % quantidadeTerritorios;
    strcpy(corJogador, mapa[indiceSorteio].corExercito);

    // - Define a cor do jogador e sorteia sua missão secreta.
    idMissao = sortearMissao();
    printf("\n--- SUA MISSAO (Exercito %s) ---\n", corJogador);
    exibirMissao(idMissao);
    printf("\nPressione Enter para continuar...");
    getchar();

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    int rodando = 1;
    int opcao;
    do {
        // - A cada iteração, exibe o mapa, a missão e o menu de ações.
        exibirMapa(mapa, quantidadeTerritorios);

        printf("\n--- SUA MISSAO (Exercito %s) ---\n", corJogador);
        exibirMissao(idMissao);
        
        opcao = exibirMenuPrincipal();

        // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
        switch (opcao){
            // - Opção 1: Inicia a fase de ataque.
            case 1:
                faseDeAtaque(mapa, quantidadeTerritorios);
                if (verificarVitoria(idMissao, mapa, quantidadeTerritorios, corJogador)) {
                    printf("\n******************************************\n");
                    printf("      PARABENS! MISSAO CUMPRIDA!          \n");
                    printf("******************************************\n");
                    rodando = 0; // Encerra o jogo auomaticamente

                }
                break;
            //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
            case 2:
                if (verificarVitoria(idMissao, mapa, quantidadeTerritorios, corJogador)) {
                    printf("\n******************************************\n");
                    printf("      PARABENS! MISSAO CUMPRIDA!          \n");
                    printf("******************************************\n");
                    rodando = 0;
                } else {
                    printf("\nAinda nao cumpriu a missao. Continue a luta!\n");
                }
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
             //   - Opção 0: Encerra o jogo.
             case 0:
                printf("\nEncerra o jogo...");
                rodando = 0;
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
            // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        }
            if (rodando && opcao != 0){
                if (verificarDerrota(mapa, quantidadeTerritorios, corJogador)) {
                    printf("\n##########################################\n");
                    printf("           FIM DE JOGO: DERROTA!          \n");
                    printf(" Voce nao tem mais tropas suficientes para\n");
                    printf(" atacar (todos com 1) ou foi eliminado.   \n");
                    printf("##########################################\n");
                    
                    printf("\nPressione Enter para sair...");
                    getchar();
                    rodando = 0;
                }
            }
    }  while (rodando);
    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
struct Territorio* alocarMapa(int quantidade) {
    return (struct Territorio*) calloc(quantidade, sizeof(struct Territorio));
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(struct Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Cadastrando Territorio %d ---", i + 1);

        printf("\nNome do Territorio: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Remove o '\n' do final

        printf("Cor do Exercito (ex: Azul, Vermelho): ");
        fgets(mapa[i].corExercito, 10, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].numeroTropas);
        limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.
    }
    printf("\nCadastro inicial concluido com sucesso!\n");
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(struct Territorio* mapa) {
    if (mapa != NULL ) {
        free(mapa);
        printf("Memoria liberada com sucesso!\n");
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
int exibirMenuPrincipal() {
    int opcao;
    printf("\n--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("Escolha sua acao: ");
    scanf("%d", &opcao);
    limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.
    return opcao;
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const struct Territorio* mapa, int quantidade) {
    printf("============== MAPA DO MUNDO ==============\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %-15s (Exercito %-10s, Tropa: %d)\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
    printf("======================================\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int idMissao){
    if (idMissao == 1) {
        printf("\nConquistar 3 territorios.\n");
    } else if (idMissao == 2) {
        printf("\nEliminar o Exercito verde\n");
    } else if (idMissao == 3) {
        printf("\nEliminar o Exercito vermelho\n");
    } 
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(struct Territorio* mapa, int quantidade) {
    int idAtacante, idDefensor;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o territorio atacante (1 a %d): ", quantidade);
    scanf("%d", &idAtacante);
    limparBufferEntrada();

    printf("Escolha o territorio defensor (1 a %d): ", quantidade);
    scanf("%d", &idDefensor);
    limparBufferEntrada();

    int idxA = idAtacante - 1;
    int idxD = idDefensor - 1;

    if (idxA >= 0 && idxA < quantidade && idxD >= 0 && idxD < quantidade && idxA != idxD) {
    // Adicionei aqui a verificação de cor para evitar fogo amigo
        if (strcmp(mapa[idxA].corExercito, mapa[idxD].corExercito) == 0) {
            printf("ERRO: Voce nao pode atacar seu proprio exercito!\n");
        } else if (mapa[idxA].numeroTropas <= 1) {
            printf("\nERRO: Territorio sem tropas suficientes para atacar!\n");
            printf("Necessario no minimo 2 tropas (1 fica, as outras atacam).\n");
        } else {
            simularAtaque(&mapa[idxA], &mapa[idxD]);
        }
    } else {
        printf("Jogada invalida!\n");
    }
    printf("\nPressione Enter para continuar...");
    getchar();
}
    

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(struct Territorio* atacante, struct Territorio* defensor) {
    // Simula a rolagem de dados para atacante e defensor (1 a 6)

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
    } else if (dadoAtacante < dadoDefensor) {
        // --- CENÁRIO 2: DEFENSOR VENCE (Faltava isso aqui!) ---
        printf("DEFESA BEM SUCEDIDA! O atacante perdeu 1 tropa.\n");
        if (atacante->numeroTropas > 0) {
            atacante->numeroTropas--;
        }
    } else {
        printf("EMPATE! Nenhuma alteracao.\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return (rand()% 3) + 1;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(int idMissao, const struct Territorio* mapa, int quantidade, const char* corJogador) {

    if (idMissao == 1) {
        int contar = 0;
        for (int i = 0; i < quantidade; i++) {
            if (strcmp(mapa[i].corExercito, corJogador) == 0) contar++;
        }
        return (contar >= 3);
    } else if (idMissao == 2) {
        for (int i = 0; i < quantidade; i++) {
            if (strcmp(mapa[i].corExercito, "Verde") == 0) return 0;
        }
        return 1;
    } else if (idMissao == 3) {
        for (int i = 0; i < quantidade; i++) {
            if (strcmp(mapa[i].corExercito, "Vermelho") == 0) return 0;
        }
        return 1;
    }
    return 0;
}

int verificarDerrota(const struct Territorio* mapa, int quantidade, const char* corJogador){
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mapa[i].corExercito, corJogador) == 0) {
            if (mapa[i].numeroTropas >= 2) {
                return 0;
            }
        }
    }
    return 1;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}