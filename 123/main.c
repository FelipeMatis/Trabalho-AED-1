#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "FilaPrioridade.h"

int id_global = 1;

void inicializarFila(FilaPrioridade* fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

void inserirNaFila(FilaPrioridade* fila, Cliente novoCliente) {
    NoFila* novoNo = (NoFila*)malloc(sizeof(NoFila));
    if (novoNo == NULL) {
        fprintf(stderr, "Erro: Falha na alocacao de memoria para novo cliente.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->cliente = novoCliente;
    novoNo->proximo = NULL;

    if (filaVazia(fila) || (novoCliente.prioridade == PRIORITARIO && fila->inicio->cliente.prioridade == COMUM)) {
        novoNo->proximo = fila->inicio;
        fila->inicio = novoNo;
        if (fila->fim == NULL) {
            fila->fim = novoNo;
        }
    } else {
        NoFila* atual = fila->inicio;
        NoFila* anterior = NULL;

        while (atual != NULL &&
               (atual->cliente.prioridade == PRIORITARIO ||
                (novoCliente.prioridade == COMUM && atual->cliente.prioridade == COMUM &&
                 atual->cliente.tempo_chegada <= novoCliente.tempo_chegada)))
        {
            anterior = atual;
            atual = atual->proximo;
        }

        if (anterior == NULL) {
            novoNo->proximo = fila->inicio;
            fila->inicio = novoNo;
        } else {
            novoNo->proximo = atual;
            anterior->proximo = novoNo;
        }
        
        if (atual == NULL) {
            fila->fim = novoNo;
        }
    }
    printf("Cliente %s (ID: %d) adicionado a fila com sucesso.\n", novoCliente.nome, novoCliente.id);
}

Cliente removerDaFila(FilaPrioridade* fila) {
    if (filaVazia(fila)) {
        fprintf(stderr, "Erro: Tentativa de remover cliente de fila vazia.\n");
        exit(EXIT_FAILURE);
    }
    NoFila* noRemover = fila->inicio;
    Cliente clienteAtendido = noRemover->cliente;

    fila->inicio = fila->inicio->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(noRemover);
    return clienteAtendido;
}

void mostrarFila(FilaPrioridade* fila, time_t tempoAtual, const char* titulo) {
    printf("\n--- %s ---\n", titulo);
    if (filaVazia(fila)) {
        printf("A fila esta vazia.\n");
        return;
    }

    NoFila* atual = fila->inicio;
    printf("ID\tNome\tIdade\tSexo\tPrioridade\tTempo de Espera\n");
    printf("--\t----\t-----\t----\t----------\t----------------\n");
    while (atual != NULL) {
        double tempo_espera_segundos = difftime(tempoAtual, atual->cliente.tempo_chegada);
        int minutos = (int)(tempo_espera_segundos / 60);
        int segundos = (int)tempo_espera_segundos % 60;

        printf("%d\t%s\t%d\t%c\t%s\t\t%02d:%02d\n",
               atual->cliente.id,
               atual->cliente.nome,
               atual->cliente.idade,
               atual->cliente.sexo,
               atual->cliente.prioridade == PRIORITARIO ? "Prioritario" : "Comum",
               minutos, segundos);
        atual = atual->proximo;
    }
    printf("--------------------------------------------\n");
}

void liberarFila(FilaPrioridade* fila) {
    NoFila* atual = fila->inicio;
    while (atual != NULL) {
        NoFila* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    printf("Memoria da fila liberada.\n");
}

int filaVazia(FilaPrioridade* fila) {
    return fila->inicio == NULL;
}

int main() {
    FilaPrioridade fila;
    inicializarFila(&fila);

    int opcao;

    do {
        printf("\n--- Sistema de Atendimento ---\n");
        printf("1. Adicionar cliente\n");
        printf("2. Atender proximo cliente\n");
        printf("3. Mostrar fila\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor, digite um numero.\n");
            while (getchar() != '\n'); 
            continue;
        }
        getchar();

        if (opcao == 1) {
            Cliente cliente;
            cliente.id = id_global++;
            cliente.tempo_chegada = time(NULL);

            printf("Nome do cliente: ");
            fgets(cliente.nome, sizeof(cliente.nome), stdin);
            cliente.nome[strcspn(cliente.nome, "\n")] = 0;

            printf("Idade: ");
            if (scanf("%d", &cliente.idade) != 1 || cliente.idade < 0) {
                printf("Idade invalida. Por favor, digite um numero positivo.\n");
                while (getchar() != '\n');
                continue;
            }
            getchar();

            printf("Sexo (M/F): ");
            char sexo_resposta[10];
            fgets(sexo_resposta, sizeof(sexo_resposta), stdin);
            cliente.sexo = toupper(sexo_resposta[0]);
            if (cliente.sexo != 'M' && cliente.sexo != 'F') {
                printf("Sexo invalido. Digite 'M' ou 'F'.\n");
                continue;
            }

            int ehPrioritario = 0;
            char resposta[10];

            if (cliente.idade >= 60) {
                ehPrioritario = 1;
            } else {
                if (cliente.sexo == 'F' && cliente.idade >= 13 && cliente.idade <= 60) {
                    printf("Gestante? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    if (tolower(resposta[0]) == 's') {
                        ehPrioritario = 1;
                    }
                }

                if (!ehPrioritario) {
                    printf("Deficiencia fisica ou mental? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    if (tolower(resposta[0]) == 's') {
                        ehPrioritario = 1;
                    }
                }
            }

            cliente.prioridade = ehPrioritario ? PRIORITARIO : COMUM;
            inserirNaFila(&fila, cliente);

        } else if (opcao == 2) {
            if (filaVazia(&fila)) {
                printf("Fila vazia. Nenhum cliente para atender.\n");
            } else {
                Cliente clienteAtendido = removerDaFila(&fila);
                printf("Atendendo cliente %d - %s (Prioridade: %s).\n",
                       clienteAtendido.id,
                       clienteAtendido.nome,
                       clienteAtendido.prioridade == PRIORITARIO ? "Prioritario" : "Comum");
            }

        } else if (opcao == 3) {
            time_t tempoAtual = time(NULL);
            mostrarFila(&fila, tempoAtual, "Fila de Atendimento Atual");

        } else if (opcao == 4) {
            printf("Encerrando o sistema de atendimento.\n");

        } else {
            printf("Opcao invalida. Por favor, tente novamente.\n");
        }

    } while (opcao != 4);

    liberarFila(&fila);
    return 0;
}