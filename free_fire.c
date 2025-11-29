#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definição de constantes
#define MAX_ITENS 10

// --- 1. Definição da Struct ---
// Representa o objeto no inventário
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Variáveis Globais (Simulando o estado da mochila)
Item mochila[MAX_ITENS];
int totalItens = 0; // Contador para saber quantos itens temos atualmente

// --- Funções Auxiliares ---

// Função para limpar o buffer do teclado (evita pular inputs)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove a quebra de linha (\n) que o fgets captura
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// --- Funções Obrigatórias do Desafio ---

// 2. Cadastro de itens
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n[!] A mochila esta cheia! Descarte algo antes.\n");
        return;
    }

    Item novoItem;
    
    printf("\n--- Novo Item (%d/%d) ---\n", totalItens + 1, MAX_ITENS);
    
    printf("Nome do item: ");
    limparBuffer(); // Limpa buffer antes de ler string
    fgets(novoItem.nome, 30, stdin);
    removerQuebraLinha(novoItem.nome);

    printf("Tipo (ex: arma, municao, cura): ");
    fgets(novoItem.tipo, 20, stdin);
    removerQuebraLinha(novoItem.tipo);

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    // Salva no vetor e incrementa o contador
    mochila[totalItens] = novoItem;
    totalItens++;
    
    printf("[+] Item adicionado com sucesso!\n");
}

// 3. Listagem dos itens
void listarItens() {
    printf("\n--- Conteudo da Mochila ---\n");
    if (totalItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    printf("%-5s %-20s %-15s %-10s\n", "ID", "Nome", "Tipo", "Qtd");
    printf("----------------------------------------------------\n");
    
    for (int i = 0; i < totalItens; i++) {
        // Formatação %-20s alinha a string à esquerda com 20 espaços
        printf("%-5d %-20s %-15s %-10d\n", i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// 4. Busca Sequencial
void buscarItem() {
    char nomeBusca[30];
    int encontrado = 0;

    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item: ");
    limparBuffer();
    fgets(nomeBusca, 30, stdin);
    removerQuebraLinha(nomeBusca);

    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 se as strings forem iguais
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[!] Item Localizado:\n");
            printf("Nome: %s | Tipo: %s | Qtd: %d\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            encontrado = 1;
            break; // Para o loop pois já achou
        }
    }

    if (!encontrado) {
        printf("[-] Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

// 5. Remoção de itens
void removerItem() {
    char nomeRemover[30];
    int indiceEncontrado = -1;

    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item para jogar fora: ");
    limparBuffer();
    fgets(nomeRemover, 30, stdin);
    removerQuebraLinha(nomeRemover);

    // 1º Passo: Achar o índice do item
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    // 2º Passo: Se achou, deslocar os itens seguintes para a esquerda
    if (indiceEncontrado != -1) {
        for (int i = indiceEncontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--; // Diminui o tamanho lógico do vetor
        printf("[+] Item '%s' removido da mochila.\n", nomeRemover);
    } else {
        printf("[-] Item nao encontrado.\n");
    }
}

// --- Função Principal ---
int main() {
    int opcao;

    do {
        // Menu de Usabilidade
        printf("\n=== SISTEMA DE SOBREVIVENCIA: MOCHILA ===\n");
        printf("1. Coletar Item (Inserir)\n");
        printf("2. Verificar Mochila (Listar)\n");
        printf("3. Procurar Recurso (Buscar)\n");
        printf("4. Descartar Item (Remover)\n");
        printf("0. Sair\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItem();
                listarItens(); // Requisito: listar após operação
                break;
            case 2:
                listarItens();
                break;
            case 3:
                buscarItem();
                break;
            case 4:
                removerItem();
                listarItens(); // Requisito: listar após operação
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}