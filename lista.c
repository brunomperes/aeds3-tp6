/*
 ============================================================================
 TRABALHO PR�TICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 LISTA.C - Define as fun��es que operam sobre o TAD lista adequado �s necessidades do algoritmo
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista.h"

//Cria uma lista vazia em O(1)
void lista_criar(LISTA *lista) {
    lista->inicio = NULL;
    lista->casado = false;
    lista->casado_com_id = 0;
    lista->fim = NULL;
}

//Verifica se uma lista est� vazia em O(1)
int lista_vazia(LISTA *lista) {
    if (lista->inicio == NULL && lista->fim==NULL) return true;
    else return false;
}

//Apaga uma lista em O(n)
void lista_apagar(LISTA *lista) {
    if (!lista_vazia(lista)) {
        NO *paux = lista->inicio;

        while (paux != NULL) {
            NO *prem = paux;
            paux = paux->proximo;
            free(prem);
        }
    }

    lista->inicio = NULL;
    lista->fim = NULL;

}

//Push_front, O(1). Retorna 0 em caso de erro
int lista_inserir_inicio(LISTA *lista, ITEM *item) {
    NO *pnovo = (NO *)malloc(sizeof(NO)); //crio um novo n�

    if (pnovo != NULL) { //verifica se existe mem�ria dispon�vel
        //preenche o os dados
        pnovo->item = *item;
        pnovo->proximo = lista->inicio;
        pnovo->anterior = NULL;
        if (lista->inicio != NULL) {
            lista->inicio->anterior = pnovo;
        } else {
            lista->fim = pnovo; //ajusta ponteiro para fim
        }

        lista->inicio = pnovo; //ajusta ponteiro in�cio

        return 1;

    } else {
        //Em caso de erro
        return 0;
    }
}

//Push_back, O(1). Retorna 0 em caso de erro
int lista_inserir_fim(LISTA *lista, ITEM *item) {
    NO *pnovo = (NO *)malloc(sizeof(NO)); //Cria um novo n�

    if (pnovo != NULL) { //Verifica se existe mem�ria dispon�vel
        //Preenche os dados do n�
        pnovo->item = *item;
        pnovo->proximo = NULL;
        pnovo->anterior = lista->fim;

        if (lista->fim != NULL) {
            lista->fim->proximo = pnovo;
        } else {
            lista->inicio = pnovo; //Ajusta ponteiro para in�cio
        }

        lista->fim = pnovo; //Ajusta ponteiro fim

        return 1;

    } else {
        //Em caso de erro
        return 0;
    }
}

//Busca um valor contido em um n� em O(n)
NO* lista_busca(LISTA* lista, int valor_procurado) {
    NO* cursor = lista->inicio;
    while (cursor->item.valor != valor_procurado && cursor != NULL) {
        cursor = cursor->proximo;
    }
    return cursor;
}

void lista_remover_inicio(LISTA* lista) {
    lista_remover_no(lista, lista->inicio);
}

void lista_remover_no(LISTA* lista, NO* no) {
    NO* aux;
    NO* ant;
    aux = no;
    if (aux == NULL) return;

    if (lista->inicio == lista->fim) { //lista com um elemento
        lista->inicio = NULL;
        lista->fim = NULL;
    } else { //lista com mais de um elemento
        ant = aux->anterior; //aux = aluno de menor nota
        if (ant != NULL) //Verifica se � n� a ser removido n�o � o primeiro elemento
            ant->proximo = aux->proximo;
        else lista->inicio = aux->proximo;

        ant = aux->proximo;
        if (ant != NULL) //Verifica se � n� a ser removido n�o � o �ltimo elemento
            ant->anterior = aux->anterior;
        else lista->fim = aux->anterior;
    }

    free(aux); //removo o n� da mem�ria

    return;
}

//Imprime os valores de n� de uma lista encadeada
void lista_imprimir(LISTA *lista) {
    NO* aux;
    aux = lista->inicio;
    if (!lista_vazia(lista)) {
        printf ("%d ", aux->item.valor); //Mostra o 1� valor
        aux = aux->proximo ;
        while (aux != NULL ) {
            printf ("%d ", aux->item.valor);
            if (aux->proximo != NULL) aux = aux->proximo ;
            else break;
        }
        printf("\n");
    } else printf("Lista vazia\n");
}
