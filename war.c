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

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.


// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char corExercito[10];
    int numeroTropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funções de interface com o usuário:
void exibirMapa(struct Territorio* mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (Exercito %s, Tropa: %d)\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
}

// Funções de lógica principal do jogo:
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Pode colar a lógica de dados/conquista aqui dentro
    // Lembre-se de usar atacante->nome em vez de atacante.nome

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
                defensor->nome, atacante->corExercito;
            
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
        // --- CENÁRIO 2: DEFENSOR VENCE (Faltava isso aqui!) ---
        printf("DEFESA BEM SUCEDIDA! O atacante perdeu 1 tropa.\n");
        if (atacante->numeroTropas > 0) {
            atacante->numeroTropas--;
        }
    }
}
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    int quantidadeTerritorios;
    
    // - Define a cor do jogador e sorteia sua missão secreta.
    // Declaração de vetor para armazenar 5 territórios
    struct Territorio *territorio;

    printf("======================================\n");
    printf("  WAR ESTRUTURADO - CADASTRO INICIAL\n");
    printf("======================================\n");

    // 1. Pergunta o tamanho (Mudança principal do nível novo)
    printf("Quantos territorios deseja cadastrar: ");
    scanf("%d", &quantidadeTerritorios);
    limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.

    territorio = (struct Territorio*) calloc(quantidadeTerritorios, sizeof(struct Territorio));

    if (territorio == NULL) {
    printf("Erro de memoria!\n");
    return 1;
    }

    for (int i = 0; i < quantidadeTerritorios; i++) {
        printf("\n--- Cadastrando Territorio %d ---", i + 1);
        printf("\nNome do Territorio: ");
        fgets(territorio[i].nome, 30, stdin);
        territorio[i].nome[strcspn(territorio[i].nome, "\n")] = '\0'; // Remove o '\n' do final

        printf("Cor do Exercito (ex: Azul, Vermelho): ");
        fgets(territorio[i].corExercito, 10, stdin);
        territorio[i].corExercito[strcspn(territorio[i].corExercito, "\n")] = '\0';

        printf("Numero de Tropas: ");
        scanf("%d", &territorio[i].numeroTropas);
        limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.

        }

        printf("\nCadastro inicial concluido com sucesso!\n");
        // Exibição dos territórios cadastrados
        printf("\n======================================\n");
        printf("   MAPA DO MUNDO - ESTADDO ATUAL \n");
        printf("======================================\n");
        for (int i = 0; i < quantidadeTerritorios; i++) {
            printf("%d. %s (Exercito %s, Tropas: %d\n)", i + 1, territorio[i].nome, territorio[i].corExercito, territorio[i].numeroTropas);
        }

    printf("\nCadastro inicial concluido com sucesso!\n");

    int idAtacante, idDefensor;

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
            // Adicionei aqui a verificação de cor para evitar fogo amigo
                if (strcmp(territorio[idxA].corExercito, territorio[idxD].corExercito) == 0) {
                    printf("ERRO: Voce nao pode atacar seu proprio exercito!\n");
                } else {
                    atacar(&territorio[idxA], &territorio[idxD]);
                }
            } else {
                printf("Jogada invalida!\n");
            }

            printf("\nPressione Enter para continuar...");
            getchar();
        }

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(territorio);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.