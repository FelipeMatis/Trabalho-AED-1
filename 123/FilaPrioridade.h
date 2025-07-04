
#ifndef FILA_PRIORIDADE_H
#define FILA_PRIORIDADE_H

typedef struct {
    int id;
    int tipo; // 0 = pouso, 1 = decolagem
    int prioridade; // 1 = alta, 2 = média
    int tempo_chegada;
} Voo;

typedef struct Nodo {
    Voo voo;
    struct Nodo* prox;
} Nodo;

typedef struct {
    Nodo* inicio;
} FilaPrioridade;

void inicializarFila(FilaPrioridade* fila);
void inserirNaFila(FilaPrioridade* fila, Voo v);
int filaVazia(FilaPrioridade* fila);
Voo removerDaFila(FilaPrioridade* fila);
void mostrarFila(FilaPrioridade* fila, int tempoAtual, const char* tipo);
void liberarFila(FilaPrioridade* fila);

#endif
