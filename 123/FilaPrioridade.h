#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

#include <time.h>

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
    struct NoFila* proximo;
} NoFila;

typedef struct FilaPrioridade {
    NoFila* inicio;
    NoFila* fim;
} FilaPrioridade;

// Funções da fila de prioridade
void inicializarFila(FilaPrioridade* fila);
int filaVazia(FilaPrioridade* fila);
void inserirNaFila(FilaPrioridade* fila, Cliente novoCliente);
Cliente removerDaFila(FilaPrioridade* fila);
void mostrarFila(FilaPrioridade* fila, time_t tempoAtual, const char* titulo);
void liberarFila(FilaPrioridade* fila);

#endif
