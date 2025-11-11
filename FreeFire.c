#include <stdio.h>
#include <string.h>

// --- Definição da Estrutura (struct) ---
#define MAX_ITENS 10
#define MAX_NOME 50
#define MAX_TIPO 20

// Estrutura para representar um item do inventário
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    // O campo 'ativo' é usado para marcar se a posição
    // do array está ocupada por um item real ou não.
    int ativo; 
} Item;

// --- Variáveis Globais para o Inventário ---
Item mochila[MAX_ITENS]; // Vetor estático para até 10 itens

// --- Protótipos das Funções ---
void inicializarMochila();
void exibirMenu();
void adicionarItem();
void removerItem();
void listarItens();

// --- Função Principal ---
int main() {
    int escolha;
    inicializarMochila();

    printf("=== Sistema de Inventário Básico ===\n");

    // Loop do-while para manter o menu interativo
    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        // Leitura da escolha do usuário. 
        // Usamos um '&' para passar o endereço da variável 'escolha'.
        if (scanf("%d", &escolha) != 1) {
            // Limpa o buffer de entrada em caso de erro (ex: usuário digitou texto)
            while (getchar() != '\n');
            escolha = 0; // Define como opção inválida
        }

        printf("------------------------------------\n");

        // Switch para processar a escolha
        switch (escolha) {
            case 1:
                adicionarItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                printf("Saindo do programa. Até mais!\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        printf("------------------------------------\n\n");

    } while (escolha != 4);

    return 0;
}

// --- Implementação das Funções ---

/**
 * Inicializa a mochila, marcando todas as posições como inativas.
 * Isso garante que saibamos quais slots estão vazios.
 */
void inicializarMochila() {
    int i;
    for (i = 0; i < MAX_ITENS; i++) {
        mochila[i].ativo = 0;
    }
}

/**
 * Exibe o menu de opções para o usuário.
 */
void exibirMenu() {
    printf("--- Menu ---\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Sair\n");
}

/**
 * Adiciona um novo item na primeira posição vazia disponível do array.
 */
void adicionarItem() {
    int i;
    int slot_encontrado = -1;

    // 1. Encontrar o primeiro slot vazio
    for (i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 0) {
            slot_encontrado = i;
            break;
        }
    }

    if (slot_encontrado != -1) {
        // 2. Coletar os dados do novo item
        printf("--- Adicionar Novo Item ---\n");
        
        // Limpa o buffer de entrada (necessário após scanf de inteiros/opções)
        while (getchar() != '\n'); 

        printf("Nome do Item (max %d): ", MAX_NOME - 1);
        // fgets é mais seguro que scanf para strings, 
        // pois evita overflow e lê espaços.
        if (fgets(mochila[slot_encontrado].nome, MAX_NOME, stdin) == NULL) return;
        // Remove a quebra de linha ('\n') adicionada pelo fgets
        mochila[slot_encontrado].nome[strcspn(mochila[slot_encontrado].nome, "\n")] = '\0'; 

        printf("Tipo do Item (max %d): ", MAX_TIPO - 1);
        if (fgets(mochila[slot_encontrado].tipo, MAX_TIPO, stdin) == NULL) return;
        mochila[slot_encontrado].tipo[strcspn(mochila[slot_encontrado].tipo, "\n")] = '\0';
        
        printf("Quantidade: ");
        if (scanf("%d", &mochila[slot_encontrado].quantidade) != 1) {
             printf("Erro: Quantidade inválida.\n");
             // Marca o slot como inativo novamente
             mochila[slot_encontrado].ativo = 0; 
             return;
        }

        // 3. Ativar o slot com os novos dados
        mochila[slot_encontrado].ativo = 1;

        printf("\nItem '%s' adicionado com sucesso!\n", mochila[slot_encontrado].nome);

    } else {
        printf("Mochila cheia! Não é possível adicionar mais itens (Máx: %d).\n", MAX_ITENS);
    }
}

/**
 * Remove um item pelo nome, marcando-o como inativo no array.
 */
void removerItem() {
    char nome_remover[MAX_NOME];
    int i;
    int encontrado = 0;

    // Limpa o buffer de entrada
    while (getchar() != '\n'); 

    printf("--- Remover Item ---\n");
    printf("Digite o nome do item a remover: ");
    if (fgets(nome_remover, MAX_NOME, stdin) == NULL) return;
    nome_remover[strcspn(nome_remover, "\n")] = '\0'; // Remove o '\n'

    // 1. Buscar o item pelo nome
    for (i = 0; i < MAX_ITENS; i++) {
        // Verifica se o slot está ativo e se os nomes são iguais (ignorando maiúsculas/minúsculas)
        if (mochila[i].ativo == 1 && strcasecmp(mochila[i].nome, nome_remover) == 0) {
            // 2. Remover: basta marcar o slot como inativo
            mochila[i].ativo = 0;
            printf("\nItem '%s' removido com sucesso!\n", mochila[i].nome);
            encontrado = 1;
            break; // Sai do loop após encontrar e remover o primeiro item
        }
    }

    if (!encontrado) {
        printf("\nErro: Item '%s' não encontrado na mochila.\n", nome_remover);
    }
}

/**
 * Lista todos os itens ativos em formato de tabela.
 */
void listarItens() {
    int i;
    int itens_ativos = 0;

    printf("--- Inventário Atual ---\n");
    printf("+---------------------------------------------------+\n");
    printf("| NOME               | TIPO          | QUANTIDADE  |\n");
    printf("+--------------------+---------------+-------------+\n");

    // 1. Iterar por todo o array
    for (i = 0; i < MAX_ITENS; i++) {
        // 2. Exibir apenas os itens ativos
        if (mochila[i].ativo == 1) {
            printf("| %-18s | %-13s | %11d |\n", 
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            itens_ativos++;
        }
    }
    
    printf("+---------------------------------------------------+\n");

    if (itens_ativos == 0) {
        printf("|              A mochila está vazia.                |\n");
        printf("+---------------------------------------------------+\n");
    } else {
        printf("Total de %d item(s) diferente(s).\n", itens_ativos);
    }
}
