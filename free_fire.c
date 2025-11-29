#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Configurações e Structs ---
#define MAX_COMPONENTES 20

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade; // 1 (Baixa) a 10 (Alta)
} Componente;

// Variáveis Globais
Componente torre[MAX_COMPONENTES];
int totalComponentes = 0;
int ordenadoPorNome = 0; // Flag de segurança para a busca binária

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

// --- 1. Entrada de Dados ---
void cadastrarComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("[!] Limite de componentes atingido!\n");
        return;
    }

    Componente novo;
    printf("\n--- Novo Componente (%d/%d) ---\n", totalComponentes + 1, MAX_COMPONENTES);
    
    printf("Nome (ex: Antena 5G): ");
    limparBuffer();
    fgets(novo.nome, 30, stdin);
    removerQuebraLinha(novo.nome);

    printf("Tipo (ex: Hardware, Software): ");
    fgets(novo.tipo, 20, stdin);
    removerQuebraLinha(novo.tipo);

    printf("Prioridade (1-10): ");
    scanf("%d", &novo.prioridade);

    torre[totalComponentes] = novo;
    totalComponentes++;
    ordenadoPorNome = 0; // Ao inserir, perdemos a garantia de ordenação
    printf("[+] Componente registrado.\n");
}

void mostrarComponentes() {
    printf("\n%-30s | %-20s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < totalComponentes; i++) {
        printf("%-30s | %-20s | %d\n", torre[i].nome, torre[i].tipo, torre[i].prioridade);
    }
    printf("Total: %d itens.\n", totalComponentes);
}

// --- 2. Algoritmos de Ordenação ---

// BUBBLE SORT - Ordena por NOME (Necessário para Busca Binária)
void bubbleSortNome(long *comparacoes) {
    Componente temp;
    for (int i = 0; i < totalComponentes - 1; i++) {
        for (int j = 0; j < totalComponentes - i - 1; j++) {
            (*comparacoes)++; // Contando a comparação
            if (strcmp(torre[j].nome, torre[j + 1].nome) > 0) {
                temp = torre[j];
                torre[j] = torre[j + 1];
                torre[j + 1] = temp;
            }
        }
    }
    ordenadoPorNome = 1; // Habilita busca binária
}

// INSERTION SORT - Ordena por TIPO
void insertionSortTipo(long *comparacoes) {
    Componente chave;
    int j;
    for (int i = 1; i < totalComponentes; i++) {
        chave = torre[i];
        j = i - 1;

        // Move os elementos que são maiores que a chave
        while (j >= 0) {
            (*comparacoes)++; // Conta a comparação do loop
            if (strcmp(torre[j].tipo, chave.tipo) > 0) {
                torre[j + 1] = torre[j];
                j = j - 1;
            } else {
                break; 
            }
        }
        torre[j + 1] = chave;
    }
    ordenadoPorNome = 0;
}

// SELECTION SORT - Ordena por PRIORIDADE (Decrescente: Maior para Menor)
void selectionSortPrioridade(long *comparacoes) {
    int maxIdx;
    Componente temp;
    
    for (int i = 0; i < totalComponentes - 1; i++) {
        maxIdx = i;
        for (int j = i + 1; j < totalComponentes; j++) {
            (*comparacoes)++;
            // Queremos os de maior prioridade (10) no topo
            if (torre[j].prioridade > torre[maxIdx].prioridade) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            temp = torre[i];
            torre[i] = torre[maxIdx];
            torre[maxIdx] = temp;
        }
    }
    ordenadoPorNome = 0;
}

// --- 3. Busca Binária ---
void buscaBinaria() {
    if (!ordenadoPorNome) {
        printf("\n[!] ALERTA: A lista precisa ser ordenada por NOME antes (Opcao 2).\n");
        return;
    }

    char alvo[30];
    printf("\nDigite o nome do componente chave: ");
    limparBuffer();
    fgets(alvo, 30, stdin);
    removerQuebraLinha(alvo);

    int inicio = 0;
    int fim = totalComponentes - 1;
    int meio;
    int iteracoes = 0;
    int encontrado = 0;

    clock_t t_inicio = clock(); // Medição de tempo

    while (inicio <= fim) {
        iteracoes++;
        meio = (inicio + fim) / 2;
        int res = strcmp(torre[meio].nome, alvo);

        if (res == 0) {
            encontrado = 1;
            break;
        } else if (res < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    clock_t t_fim = clock();
    double tempo_gasto = ((double)(t_fim - t_inicio)) / CLOCKS_PER_SEC;

    if (encontrado) {
        printf("\n[SUCESSO] Componente '%s' encontrado na posicao %d.\n", torre[meio].nome, meio + 1);
        printf("Prioridade: %d | Tipo: %s\n", torre[meio].prioridade, torre[meio].tipo);
    } else {
        printf("\n[FALHA] Componente nao localizado.\n");
    }
    printf(">> Metricas: %d comparacoes | Tempo CPU: %f ms\n", iteracoes, tempo_gasto * 1000);
}

// --- Função Wrapper para Medir Desempenho de Ordenação ---
void executarOrdenacao(int tipo) {
    if (totalComponentes < 2) {
        printf("[!] Adicione pelo menos 2 componentes para ordenar.\n");
        return;
    }

    long comparacoes = 0;
    clock_t inicio = clock();

    switch (tipo) {
        case 1: // Bubble - Nome
            printf("\nExecutando Bubble Sort (Por Nome)...\n");
            bubbleSortNome(&comparacoes);
            break;
        case 2: // Insertion - Tipo
            printf("\nExecutando Insertion Sort (Por Tipo)...\n");
            insertionSortTipo(&comparacoes);
            break;
        case 3: // Selection - Prioridade
            printf("\nExecutando Selection Sort (Por Prioridade Decrescente)...\n");
            selectionSortPrioridade(&comparacoes);
            break;
    }

    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000;

    printf(">> Ordenacao concluida!\n");
    printf(">> Comparacoes logicas realizadas: %ld\n", comparacoes);
    printf(">> Tempo de execucao: %.4f ms\n", tempo_ms);
    
    mostrarComponentes();
}

// --- Main ---
int main() {
    int opcao;

    // Pré-cadastro para facilitar testes (Opcional)
    /*
    strcpy(torre[0].nome, "Gerador"); strcpy(torre[0].tipo, "Energia"); torre[0].prioridade = 10;
    strcpy(torre[1].nome, "Cabo Rede"); strcpy(torre[1].tipo, "Conexao"); torre[1].prioridade = 5;
    strcpy(torre[2].nome, "Antena"); strcpy(torre[2].tipo, "Comunicacao"); torre[2].prioridade = 8;
    totalComponentes = 3;
    */

    do {
        printf("\n=== TORRE DE RESGATE: MODULO MESTRE ===\n");
        printf("1. Cadastrar Componente\n");
        printf("2. Ordenar por NOME (Bubble Sort)\n");
        printf("3. Ordenar por TIPO (Insertion Sort)\n");
        printf("4. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5. Buscar Componente Chave (Binaria)\n");
        printf("6. Listar Tudo\n");
        printf("0. Sair\n");
        printf("Escolha estrategica: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarComponente(); break;
            case 2: executarOrdenacao(1); break;
            case 3: executarOrdenacao(2); break;
            case 4: executarOrdenacao(3); break;
            case 5: buscaBinaria(); break;
            case 6: mostrarComponentes(); break;
            case 0: printf("Encerrando sistema da torre...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}