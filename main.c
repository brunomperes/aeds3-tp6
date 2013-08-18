/*
 ============================================================================
 TRABALHO PRÁTICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 MAIN.C - Arquivo principal do programa, contém a chamada de execução da função principal.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "heuristica.h"
#include "io.h"
#include "timerlinux.h"

int main(int argc, char *argv[]) {

    //Nomes dos arquivos
    char NomeArqEntrada[TAMMAXNOMEARQ];
    char NomeArqSaida[TAMMAXNOMEARQ];

    //Status se o programa vai gerar dados para análise ou não
    bool Analise;

    int numThreads;

    //Verifica se os parâmetros necessários foram passados para o programa
    checaEntrada(argc, argv, NomeArqEntrada, NomeArqSaida, &numThreads, &Analise);

    FILE *ArqEntrada = abreArquivoLeitura(NomeArqEntrada);
    FILE *ArqSaida = fopen(NomeArqSaida, "w");

    SMP(ArqEntrada, numThreads, ArqSaida, Analise);

    fclose(ArqEntrada);
    fclose(ArqSaida);

    return EXIT_SUCCESS;
}
