#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define PRIORITARIO 1
#define COMUM       0
#define MAX_NOME 50

typedef struct Cliente {
    int id;
    time_t tempo_chegada;
    char nome[MAX_NOME];
    int idade;
    char sexo;
    int prioridade;
} Cliente;

typedef struct NoFila {
    Cliente cliente;
    struct NoFila *proximo;
} NoFila;

typedef struct Fila {
    NoFila *inicio;
    NoFila *fim;
} Fila;

int id_global = 1;

void inicializarFila(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

int filaVazia(Fila *fila) {
    return (fila->inicio == NULL);
}

void inserirNaFila(Fila *fila, Cliente novoCliente) {
    NoFila *novoNo = (NoFila *)malloc(sizeof(NoFila));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(EXIT_FAILURE);
    }
    novoNo->cliente = novoCliente;
    novoNo->proximo = NULL;

    if (filaVazia(fila)) {
        fila->inicio = novoNo;
        fila->fim = novoNo;
        printf("Cliente '%s' (ID: %d) adicionado a fila.\n", novoCliente.nome, novoCliente.id);
        return;
    }

    NoFila *atual = fila->inicio;
    NoFila *anterior = NULL;

    while (atual != NULL) {
        if (novoCliente.prioridade == PRIORITARIO && atual->cliente.prioridade == COMUM) {
            break;
        }
        if (novoCliente.prioridade == atual->cliente.prioridade && novoCliente.tempo_chegada < atual->cliente.tempo_chegada) {
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL) {
        novoNo->proximo = fila->inicio;
        fila->inicio = novoNo;
    } else {
        novoNo->proximo = atual;
        anterior->proximo = novoNo;
        if (atual == NULL) {
            fila->fim = novoNo;
        }
    }
    printf("Cliente '%s' (ID: %d) adicionado a fila.\n", novoCliente.nome, novoCliente.id);
}

Cliente removerDaFila(Fila *fila) {
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia.\n");
        Cliente clienteVazio = {0};
        return clienteVazio;
    }

    NoFila *noRemovido = fila->inicio;
    Cliente clienteAtendido = noRemovido->cliente;
    fila->inicio = fila->inicio->proximo;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(noRemovido);
    return clienteAtendido;
}

void mostrarFila(Fila *fila, time_t tempo_atual, const char *titulo) {
    if (filaVazia(fila)) {
        printf("%s: Fila vazia.\n", titulo);
        return;
    }

    printf("\n--- %s ---\n", titulo);
    NoFila *atual = fila->inicio;
    int i = 1;
    while (atual != NULL) {
        double tempo_na_fila = difftime(tempo_atual, atual->cliente.tempo_chegada);
        printf("%d. ID: %d | Nome: %s | Idade: %d | Sexo: %c | Prioridade: %s | Tempo na fila: %.0f segundos\n",
               i++,
               atual->cliente.id,
               atual->cliente.nome,
               atual->cliente.idade,
               atual->cliente.sexo,
               atual->cliente.prioridade == PRIORITARIO ? "Prioritario" : "Comum",
               tempo_na_fila);
        atual = atual->proximo;
    }
    printf("------------------------\n\n");
}

void liberarFila(Fila *fila) {
    NoFila *atual = fila->inicio;
    while (atual != NULL) {
        NoFila *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
}

int main() {
    Fila fila;
    inicializarFila(&fila);

    int opcao;

    do {
        printf("\n--- Sistema de Atendimento ---\n");
        printf("1. Adicionar cliente\n");
        printf("2. Atender proximo cliente\n");
        printf("3. Mostrar fila\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            Cliente cliente;
            cliente.id = id_global++;
            cliente.tempo_chegada = time(NULL);

            printf("Nome do cliente: ");
            fgets(cliente.nome, sizeof(cliente.nome), stdin);
            cliente.nome[strcspn(cliente.nome, "\n")] = 0;

            printf("Idade: ");
            scanf("%d", &cliente.idade);
            getchar();

            printf("Sexo (M/F): ");
            char sexo_resposta[10];
            fgets(sexo_resposta, sizeof(sexo_resposta), stdin);
            cliente.sexo = toupper(sexo_resposta[0]);

            char resposta[10];
            int ehPrioritario = 0;

            if (cliente.idade >= 60) {
                ehPrioritario = 1;
            } else {
                if (cliente.sexo == 'F') {
                    if(cliente.idade >= 13 && cliente.idade <= 60){
                        printf("Gestante? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    if (resposta[0] == 's' || resposta[0] == 'S') {
                        ehPrioritario = 1;
                    }
                    }
                }

                if (!ehPrioritario) {
                    printf("Deficiencia fisica ou mental? (s/n): ");
                    fgets(resposta, sizeof(resposta), stdin);
                    if (resposta[0] == 's' || resposta[0] == 'S') {
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
            mostrarFila(&fila, time(NULL), "Fila de Atendimento");

        } else if (opcao == 4) {
            printf("Encerrando o sistema de atendimento.\n");
        } else {
            printf("Opcao invalida. Por favor, tente novamente.\n");
        }

    } while (opcao != 4);

    liberarFila(&fila);
    return 0;
}
