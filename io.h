/*
 ============================================================================
 TRABALHO PRÁTICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 MAIN.C - Arquivo principal do programa, contém a chamada de execução da função principal.
 ============================================================================
 */

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "heuristica.h"

#define TAMMAXNOMEARQ 256
#define TAMMAXPALAVRA 40

/* ============================ Funções de Entrada ============================ */
//Carrega as palavras de um arquivo já aberto como leitura inserindo no TipoTexto
//void CarregaPalavrasArq(FILE* ArqEntrada, char* NomeArq, TipoTexto *T);
//Abre um arquivo como leitura. Em caso de erro encerra a execução do programa
FILE * abreArquivoLeitura(char* NomeArq);
int LeInt(FILE *ArqEntrada);


/* ============================ Funções de Saída ============================ */
//Escreve o conteúdo de um TipoTexto num arquivo já aberto
void EscreveResultados(FILE *ArqSaida, LISTA mulher[], LISTA homem[], int n, float satisf_fem, float satisf_masc, float satisf_geral);
void LeituraInput(FILE* ArqEntrada, int n, LISTA mulher[], LISTA homem[]);
//Escreve um int num arquivo já aberto como escrita com uma quebra de linha em seguida
void EscreveInt(FILE* ArqAnaliseRes, int x);
//Escreve os resultados de análise de execução do programa no formato de tabela em LaTeX
void EscreveAnaliseLatex(FILE *ArqAnalise, int numThreads, int n, double tempoexec);
//Escreve os resultados de análise de execução do programa no formato tabular para plotagem com o GnuPlot
void EscreveAnaliseGnuPlot(FILE *ArqAnalise, int numThreads, int n, double tempoexec);
//Limpa o conteúdo de um arquivo
void LimpaArquivo(char *NomeArqSaida);
void MostraDoisVetListas(LISTA *mulher, LISTA *homem, int n);


/* ============================ Funções para Testes e Funções Auxiliares ============================ */
//Verifica se os parâmetros com o nome do arquivo de entrada e saída foram passados corretamente
//Caso encontre algum erro, finaliza a execução do programa
void checaEntrada(int argc, char **argv, char *NomeArqEntrada, char *NomeArqSaida, int *numThreads, bool *Analise);


#endif // IO_H_INCLUDED
