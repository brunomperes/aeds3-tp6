/*
 ============================================================================
 TRABALHO PR�TICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 LISTA.H - Define as estruturas de dados e cabe�alhos das fun��es relacionadas �s opera��es sobre o TAD lista
 ============================================================================
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

//Guarda o conte�do relativo ao n� (prefer�ncia da pessoa)
typedef struct ITEM {
    int indice; //Indica se � o 1� elemento, 2� elemento. � utilizado para buscas e calcular o �ndice de satisfabilidade
    int valor; //Armazena a id do pretendente
} ITEM;

//Armazena os ponteiros para o pr�ximo e para o n� anterior, al�m de uma estrutura contendo valores relativos � prefer�ncia da pessoa
typedef struct NO {
    ITEM item;
    struct NO *proximo;
    struct NO *anterior;
} NO;

//Define a lista, que representar� uma pessoa
typedef struct LISTA {
    int casado;
    int casado_com_id; //id do pretendente com o qual o interessado casou-se
    NO *inicio;
    NO *fim;
} LISTA;

//Cria uma lista vazia em O(1)
void lista_criar(LISTA *lista);

//Verifica se uma lista est� vazia em O(1)
int lista_vazia(LISTA *lista);

//Apaga uma lista em O(n)
void lista_apagar(LISTA *lista);

//Push_front, O(1). Retorna 0 em caso de erro
int lista_inserir_inicio(LISTA *lista, ITEM *item);

//Remove um n� de uma lista em O(1)
void lista_remover_no(LISTA* lista, NO* no);

//Remove o primeiro elemento de uma lista em O(1)
void lista_remover_inicio(LISTA* lista);

//Push_back, O(1). Retorna 0 em caso de erro
int lista_inserir_fim(LISTA *lista, ITEM *item);

//Busca um valor contido em um n� em O(n)
NO* lista_busca(LISTA* lista, int valor);

//Imprime os valores de n� de uma lista encadeada
void lista_imprimir(LISTA *lista);

#endif // LIST_H_INCLUDED
