#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definições e Structs ---
#define MAX_VETOR 10

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Nó para a Lista Encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// --- Variáveis Globais (Vetor) ---
Item vetorMochila[MAX_VETOR];
int qtdVetor = 0;
int vetorEstaOrdenado = 0; // Flag para controlar estado da ordenação

// --- Variáveis Globais (Lista) ---
No* inicioLista = NULL;

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

void lerItem(Item* novoItem) {
    printf("Nome: ");
    limparBuffer();
    fgets(novoItem->nome, 30, stdin);
    removerQuebraLinha(novoItem->nome);
    printf("Tipo: ");
    fgets(novoItem->tipo, 20, stdin);
    removerQuebraLinha(novoItem->tipo);
    printf("Quantidade: ");
    scanf("%d", &novoItem->quantidade);
}

// ==========================================================
// MÓDULO 1: IMPLEMENTAÇÃO COM VETOR (Sequencial)
// ==========================================================

void inserirVetor() {
    if (qtdVetor >= MAX_VETOR) {
        printf("[!] Mochila (Vetor) cheia!\n");
        return;
    }
    lerItem(&vetorMochila[qtdVetor]);
    qtdVetor++;
    vetorEstaOrdenado = 0; // Ao inserir, a lista pode perder a ordem
    printf("[V] Item inserido no vetor.\n");
}

void removerVetor() {
    char nome[30];
    printf("Remover (Vetor) - Nome: ");
    limparBuffer();
    fgets(nome, 30, stdin);
    removerQuebraLinha(nome);

    int pos = -1;
    for (int i = 0; i < qtdVetor; i++) {
        if (strcmp(vetorMochila[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        // Deslocamento (Shift Left)
        for (int i = pos; i < qtdVetor - 1; i++) {
            vetorMochila[i] = vetorMochila[i + 1];
        }
        qtdVetor--;
        printf("[V] Item removido.\n");
    } else {
        printf("[!] Item nao encontrado.\n");
    }
}

void listarVetor() {
    printf("\n--- Mochila (VETOR) ---\n");
    for (int i = 0; i < qtdVetor; i++) {
        printf("[%d] %s (%s) - Qtd: %d\n", i, vetorMochila[i].nome, vetorMochila[i].tipo, vetorMochila[i].quantidade);
    }
}

// Algoritmo Bubble Sort para ordenar por Nome
void ordenarVetor() {
    Item temp;
    for (int i = 0; i < qtdVetor - 1; i++) {
        for (int j = 0; j < qtdVetor - i - 1; j++) {
            if (strcmp(vetorMochila[j].nome, vetorMochila[j + 1].nome) > 0) {
                temp = vetorMochila[j];
                vetorMochila[j] = vetorMochila[j + 1];
                vetorMochila[j + 1] = temp;
            }
        }
    }
    vetorEstaOrdenado = 1;
    printf("[V] Vetor ordenado alfabeticamente!\n");
}

void buscarSequencialVetor() {
    char nome[30];
    printf("Busca Sequencial (Vetor) - Nome: ");
    limparBuffer();
    fgets(nome, 30, stdin);
    removerQuebraLinha(nome);

    int comparacoes = 0;
    int encontrado = 0;

    for (int i = 0; i < qtdVetor; i++) {
        comparacoes++;
        if (strcmp(vetorMochila[i].nome, nome) == 0) {
            printf("[ACHOU] %s na posicao %d.\n", vetorMochila[i].nome, i);
            encontrado = 1;
            break;
        }
    }
    printf(">> Comparacoes realizadas: %d\n", comparacoes);
    if (!encontrado) printf("[!] Nao encontrado.\n");
}

void buscarBinariaVetor() {
    if (!vetorEstaOrdenado) {
        printf("[!] O vetor precisa estar ORDENADO para busca binaria.\n");
        return;
    }

    char nome[30];
    printf("Busca Binaria (Vetor) - Nome: ");
    limparBuffer();
    fgets(nome, 30, stdin);
    removerQuebraLinha(nome);

    int inicio = 0, fim = qtdVetor - 1, meio;
    int comparacoes = 0;
    int encontrado = 0;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;
        
        int res = strcmp(vetorMochila[meio].nome, nome);

        if (res == 0) {
            printf("[ACHOU] %s na posicao %d.\n", vetorMochila[meio].nome, meio);
            encontrado = 1;
            break;
        } else if (res < 0) {
            inicio = meio + 1; // Busca na metade direita
        } else {
            fim = meio - 1;    // Busca na metade esquerda
        }
    }
    printf(">> Comparacoes realizadas (Binaria): %d\n", comparacoes);
    if (!encontrado) printf("[!] Nao encontrado.\n");
}

// ==========================================================
// MÓDULO 2: IMPLEMENTAÇÃO COM LISTA ENCADEADA (Dinâmica)
// ==========================================================

void inserirLista() {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("[!] Erro de memoria.\n");
        return;
    }
    lerItem(&novoNo->dados);
    novoNo->proximo = NULL;

    if (inicioLista == NULL) {
        inicioLista = novoNo;
    } else {
        // Inserir no final para manter ordem de chegada (simil vetor)
        No* atual = inicioLista;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
    printf("[L] Item inserido na lista.\n");
}

void listarLista() {
    printf("\n--- Mochila (LISTA ENCADEADA) ---\n");
    No* atual = inicioLista;
    int i = 0;
    while (atual != NULL) {
        printf("[%d ->] %s (%s) - Qtd: %d\n", i++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
    printf("[NULL]\n");
}

void removerLista() {
    char nome[30];
    printf("Remover (Lista) - Nome: ");
    limparBuffer();
    fgets(nome, 30, stdin);
    removerQuebraLinha(nome);

    No* atual = inicioLista;
    No* anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            // Caso 1: Remover o primeiro
            if (anterior == NULL) {
                inicioLista = atual->proximo;
            } 
            // Caso 2: Remover do meio ou fim
            else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("[L] Item removido da lista.\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("[!] Item nao encontrado na lista.\n");
}

void buscarSequencialLista() {
    char nome[30];
    printf("Busca Sequencial (Lista) - Nome: ");
    limparBuffer();
    fgets(nome, 30, stdin);
    removerQuebraLinha(nome);

    No* atual = inicioLista;
    int comparacoes = 0;
    int encontrado = 0;

    while (atual != NULL) {
        comparacoes++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            printf("[ACHOU] %s na lista.\n", atual->dados.nome);
            encontrado = 1;
            break;
        }
        atual = atual->proximo;
    }
    printf(">> Comparacoes realizadas: %d\n", comparacoes);
    if (!encontrado) printf("[!] Nao encontrado na lista.\n");
}

// ==========================================================
// MENU PRINCIPAL
// ==========================================================
int main() {
    int opcao;
    do {
        printf("\n=== MOCHILA AVENTUREIRO ===\n");
        printf("1. [Vetor] Inserir\n");
        printf("2. [Vetor] Listar\n");
        printf("3. [Vetor] Remover\n");
        printf("4. [Vetor] Ordenar (Bubble Sort)\n");
        printf("5. [Vetor] Busca Sequencial\n");
        printf("6. [Vetor] Busca BINARIA (Requer Ordenacao)\n");
        printf("---------------------------\n");
        printf("7. [Lista] Inserir\n");
        printf("8. [Lista] Listar\n");
        printf("9. [Lista] Remover\n");
        printf("10.[Lista] Busca Sequencial\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: inserirVetor(); break;
            case 2: listarVetor(); break;
            case 3: removerVetor(); break;
            case 4: ordenarVetor(); break;
            case 5: buscarSequencialVetor(); break;
            case 6: buscarBinariaVetor(); break;
            case 7: inserirLista(); break;
            case 8: listarLista(); break;
            case 9: removerLista(); break;
            case 10: buscarSequencialLista(); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while(opcao != 0);

    return 0;
}