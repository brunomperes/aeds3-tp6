/*
 ============================================================================
 TRABALHO PRÁTICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 IO.C - Realiza a comunicação do programa com o ambiente, lendo o arquivo de entrada, armazenando as informações lidas na memória e escrevendo nos arquivos de saída.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getopt()
#include <string.h>
#include <stdbool.h>

#include "io.h"
#include "lista.h"

//Preenche os parâmetros para execução do programa e verifica se os parâmetros necessários foram passados ao programa
void checaEntrada(int argc, char **argv, char *NomeArqEntrada, char *NomeArqSaida, int *numThreads, bool *Analise) {

    int opcao, checaEntrada=0, checaSaida=0;

    *Analise = false;

    char *temp1 = NULL;
    char *temp2 = NULL;

    strncpy(NomeArqEntrada, " ", 2);
    NomeArqEntrada[2] = '\0';
    strncpy(NomeArqSaida, " ", 2);
    NomeArqSaida[2] = '\0';

    while( (opcao = getopt(argc, argv, "i:o:a")) != -1 ) {
        switch( opcao ) {
        case 'i':
            checaEntrada=1 ;
            temp1 = optarg ;
            break ;
        case 'o':
            checaSaida=1 ;
            temp2 = optarg ;
            break ;
        case 'a':
            *Analise=true;
            break ;
        case '?':
            break ;
        }
    }
    *numThreads = atoi(argv[5]);

    //Verifica se os parâmetros necessários foram preenchidos
    if (checaEntrada == 0 || checaSaida == 0) {
        puts ("\n[ERRO] Verifique se os parametros de nome de arquivo de entrada e de saida foram passados corretamente!\n");
        exit(1);
    }
    if (NomeArqEntrada == NULL || NomeArqSaida == NULL) {
        puts ("\n[ERRO] Verifique se os parametros de nome de arquivo de entrada e de saida foram passados corretamente!\n");
        exit(1);
    }

    strncpy(NomeArqEntrada, temp1, strlen(temp1));
    NomeArqEntrada[strlen(temp1)] = '\0';
    strncpy(NomeArqSaida, temp2, strlen(temp2));
    NomeArqSaida[strlen(temp2)] = '\0';
}

//Lê um número inteiro de um arquivo aberto como leitura
int LeInt(FILE *ArqEntrada) {
    int x;

    //Leitura efetiva do arquivo
    fscanf(ArqEntrada, "%d", &x); //Escreve na variável x o conteúdo lido do arquivo

    return x;
}

//Abre um arquivo como leitura. Em caso de erro encerra a execução do programa
FILE * abreArquivoLeitura(char* NomeArq) {
    FILE *ArqEntrada = fopen(NomeArq, "r");
    if (ArqEntrada == NULL) {
        printf("\n[ERRO] Problema ao abrir o arquivo %s\nVerifique se o arquivo passado como argumento realmente existe\n\n", NomeArq);
        exit(1);
    }
    return ArqEntrada;
}

void CarregaVetLista(FILE* ArqEntrada, int n, LISTA *l) {

    int i,j; //i=contador de ids e j=contador de linhas
    ITEM novo_no;

    //Lê as ids de todos os homens
    for (j=1; j<=n; j++) {
        for (i=1; i<=n; i++) {
            //Preencher o novo nó com valores iniciais
            novo_no.valor = LeInt(ArqEntrada);
            novo_no.indice = i; //id atual do homem e índice dentro da lista
            if(!(lista_inserir_fim(&l[j], &novo_no))) exit(1); //Escreve na lista 'mulher' as ids de preferência de cada mulher, finaliza o programa em caso de erro
        }
    }
}

//Limpa o conteúdo de um arquivo
void LimpaArquivo(char *NomeArqSaida) {
    FILE * ArqSaida = fopen (NomeArqSaida,"w");
    fclose (ArqSaida);
}

//Escreve um int num arquivo já aberto como escrita com uma quebra de linha em seguida
void EscreveInt(FILE* ArqAnaliseRes, int x) {
    fprintf(ArqAnaliseRes, "%d ", x);
}

//Escreve o tempo de execução do programa num arquivo já aberto, finalizando uma tabela no formato LaTeX
void EscreveTempoExecLaTex(FILE *ArqAnaliseRes, double tempogasto) {
    fprintf(ArqAnaliseRes, "%lf\t", tempogasto);
    fprintf(ArqAnaliseRes, "\\\\ \\hline\n");
}

//Escreve os resultados de análise do programa no formato de tabela em LaTeX
void EscreveAnaliseLatex(FILE *ArqAnalise, int numThreads, int n, double tempoexec) {
    fprintf(ArqAnalise, "%d\t&", numThreads);
    fprintf(ArqAnalise, "%d\t&", n);
    EscreveTempoExecLaTex(ArqAnalise, tempoexec);
}

//Escreve os resultados de análise do programa no formato de tabela em LaTeX
void EscreveAnaliseGnuPlot(FILE *ArqAnalise, int numThreads, int n, double tempoexec) {
    fprintf(ArqAnalise, "%d\t", numThreads);
    fprintf(ArqAnalise, "%d\t", n);
    EscreveTempoExecLaTex(ArqAnalise, tempoexec);
}

//Faz a leitura das ids de preferância dos homens e das mulheres do arquivo de entrada
void LeituraInput(FILE* ArqEntrada, int n, LISTA mulher[], LISTA homem[]) {
    CarregaVetLista(ArqEntrada, n, homem);
    CarregaVetLista(ArqEntrada, n, mulher);
}

void EscreveAlocacao(FILE * ArqSaida, LISTA * l, int n) {
    int i;
    for (i=1; i<=n; i++) {
        fprintf (ArqSaida, "%d %d\n", i, l[i].casado_com_id); //Escreve a id de homem e da mulher cônjuges separados por um espaço simples
    }
}

void EscreveMetricas(FILE *ArqSaida, float satisf_geral, float satisf_masc, float satisf_fem) {
    fprintf (ArqSaida, "%.3f\n", satisf_geral); //Satisfabilidade Geral
    fprintf (ArqSaida, "%.3f\n", satisf_masc); //Satisfabilidade Masculina
    fprintf (ArqSaida, "%.3f\n", satisf_fem); //Satisfabilidade Feminina
}

//Escreve no arquivo output.txt os casais formados por suas ids, a satisfabilidade geral, satisfabilidade masculina e satisfabilidade feminina separados por uma quebra de linha
void EscreveResultados(FILE *ArqSaida, LISTA mulher[], LISTA homem[], int n, float satisf_fem, float satisf_masc, float satisf_geral) {
    EscreveAlocacao(ArqSaida, homem, n);
    EscreveMetricas(ArqSaida, satisf_geral, satisf_masc, satisf_fem);
}

//Imprime o conteúdo de duas listas de tamanho n
void MostraDoisVetListas(LISTA *mulher, LISTA *homem, int n) {
    int i;
    printf("Lista de mulheres\n");
    for (i=1; i<=n; i++) { //Lê as ids de todas as mulheres
        printf ("[%d] ",i);
        lista_imprimir(&mulher[i]);
    }
    printf("\nLista de homens\n");
    for (i=1; i<=n; i++) { //Lê as ids de todas as mulheres
        printf ("[%d] ",i);
        lista_imprimir(&homem[i]);
    }
    printf("\n");
}
