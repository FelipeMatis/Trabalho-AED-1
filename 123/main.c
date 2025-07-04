#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

typedef struct {
    int id;
    char nome[100];
    int idade;
    int prioridade; // 1 = prioritario, 2 = comum
    int tempo_chegada;
} Cliente;

typedef struct Nodo {
    Cliente cliente;
    struct Nodo* prox;
} Nodo;

typedef struct {
    Nodo* inicio;
} FilaPrioridade;

void inicializarFila(FilaPrioridade* fila) {
    fila->inicio = NULL;
}

void inserirNaFila(FilaPrioridade* fila, Cliente c) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    novo->cliente = c;
    novo->prox = NULL;

    if (fila->inicio == NULL || c.prioridade < fila->inicio->cliente.prioridade) {
        novo->prox = fila->inicio;
        fila->inicio = novo;
    } else {
        Nodo* atual = fila->inicio;
        while (atual->prox != NULL && atual->prox->cliente.prioridade <= c.prioridade)
            atual = atual->prox;

        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

int filaVazia(FilaPrioridade* fila) {
    return fila->inicio == NULL;
}

Cliente removerDaFila(FilaPrioridade* fila) {
    Nodo* temp = fila->inicio;
    Cliente c = temp->cliente;
    fila->inicio = temp->prox;
    free(temp);
    return c;
}

void mostrarFila(FilaPrioridade* fila, int tempoAtual, const char* titulo) {
    printf("\n--- %s ---\n", titulo);
    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }
    Nodo* atual = fila->inicio;
    while (atual != NULL) {
        Cliente c = atual->cliente;
        printf("Cliente %d - %s | Idade: %d | Prioridade: %s | Espera: %d segundos\n",
               c.id,
               c.nome,
               c.idade,
               (c.prioridade == 1 ? "Prioritario" : "Comum"),
               tempoAtual - c.tempo_chegada);
        atual = atual->prox;
    }
}

void liberarFila(FilaPrioridade* fila) {
    while (!filaVazia(fila)) {
        removerDaFila(fila);
    }
}

#endif

// =================== atendimento.c ===================

#define PRIORITARIO 1
#define COMUM 2

int main() {
    FilaPrioridade fila;
    inicializarFila(&fila);

    int opcao;
    int id_global = 1;

    do {
        printf("\n==== Menu de Atendimento ====\n");
        printf("1. Adicionar cliente\n");
        printf("2. Atender cliente\n");
        printf("3. Mostrar fila\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // consumir '\n'

        if (opcao == 1) {
            Cliente cliente;
            cliente.id = id_global++;
            cliente.tempo_chegada = time(NULL);

            printf("Nome do cliente: ");
            fgets(cliente.nome, sizeof(cliente.nome), stdin);
            cliente.nome[strcspn(cliente.nome, "\n")] = 0; // remove '\n'

            printf("Idade: ");
            scanf("%d", &cliente.idade);
            getchar(); // consumir '\n'

            char resposta[10];
            int ehPrioritario = 0;

            if (cliente.idade >= 60) ehPrioritario = 1;
            else {
                printf("Gestante? (s/n): ");
                fgets(resposta, sizeof(resposta), stdin);
                if (resposta[0] == 's' || resposta[0] == 'S') ehPrioritario = 1;

                if (!ehPrioritario) {
                    printf("Deficiencia fisica ou mental? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    if (resposta[0] == 's' || resposta[0] == 'S') ehPrioritario = 1;
                }
            }

            cliente.prioridade = ehPrioritario ? PRIORITARIO : COMUM;
            inserirNaFila(&fila, cliente);

            printf("Cliente %s adicionado com prioridade %s.\n",
                   cliente.nome,
                   cliente.prioridade == PRIORITARIO ? "Prioritario" : "Comum");

        } else if (opcao == 2) {
            if (filaVazia(&fila)) {
                printf("Fila vazia.\n");
            } else {
                Cliente cliente = removerDaFila(&fila);
                printf("Atendendo cliente %d - %s (Prioridade: %s).\n",
                       cliente.id,
                       cliente.nome,
                       cliente.prioridade == PRIORITARIO ? "Prioritario" : "Comum");
            }

        } else if (opcao == 3) {
            mostrarFila(&fila, time(NULL), "Fila de Atendimento");

        } else if (opcao == 4) {
            printf("Encerrando...\n");
        } else {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 4);

    liberarFila(&fila);
    return 0;
}
