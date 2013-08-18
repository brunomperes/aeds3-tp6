/*
 ============================================================================
 TRABALHO PRÁTICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 HEURISITCA.C - Define as operações que realizarão o método heurístico e avaliarão a qualidade dos resultados através do índice de satisfabilidade
 ============================================================================
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "heuristica.h"
#include "lista.h"
#include "io.h"
#include "timerlinux.h"

#define NOMEARQLATEX "Analise-latex.txt"
#define NOMEARQGNUPLOT "Analise-gnuplot.txt"

GlobalVars global;
pthread_mutex_t mutex_global;
pthread_mutex_t* mutex_mulheres;
pthread_mutex_t* mutex_homens;

void *casa_homem_SEQ(void *t);
void men_porpose_algorithm_SEQ(int n);

LISTA * AlocaVetLista(int n) {
    LISTA * mulher;
    int i;
    mulher = (LISTA *)malloc((n+1) * sizeof (LISTA)); //Aloca um vetor de listas para guardar as ids de preferências de n mulheres
    if (mulher == NULL) exit (1); //Finaliza o programa com status de erro
    for (i = 0; i <= n; i++) {
        lista_criar(&mulher[i]);
    }
    return mulher;
}

void DesalocaVetLista(LISTA *l, int n) {
    int i;
    for (i=0; i<=n; i++)
        lista_apagar(&l[i]);
    lista_apagar(l);
}

void CalculaMetricas(float *satisf_masc, float *satisf_fem, float *satisf_geral, int n) {
    *satisf_masc = calcula_satisfabilidade(global.homem, n);
    *satisf_fem = calcula_satisfabilidade(global.mulher, n);
    *satisf_geral = calcula_satisfabilidade_geral(*satisf_masc, *satisf_fem);
}

//Mostra os casais formados até agora
//Omite aqueles que não estão casados
void mostracasais(LISTA* mulher, LISTA* homem, int n) {
    int i;
    for (i=1; i<=n; i++)
        printf ("mulher %d casa com homem %d\n", i, mulher[i].casado_com_id);
    printf ("\n");
}

void SMP(FILE *ArqEntrada, int numThreads, FILE *ArqSaida, bool Analise) {

    int j,n;

    //Le a quantidade de instâncias do arquivo de entrada
    int instancias = LeInt(ArqEntrada);

    float satisf_masc;
    float satisf_fem;
    float satisf_geral;

    FILE *ArqAnaliseLatex = fopen(NOMEARQLATEX,"a");
    FILE *ArqAnaliseGnuPlot = fopen(NOMEARQGNUPLOT,"a");
    float tempoexec;
    stopWatch cronometro;

    //Executa 'instancias' vezes
    for (j=0; j<instancias; j++) {

        n = LeInt(ArqEntrada); //Lê o número n de indíviduos no conjunto

        //Criar vetor de listas alocadas dinamicamente
        global.mulher = AlocaVetLista(n);
        global.homem = AlocaVetLista(n);

        //Povoar as listas de homens e de mulheres
        LeituraInput(ArqEntrada, n, global.mulher, global.homem);

        if (Analise) startTimer(&cronometro);

        //Heurística
        men_porpose_algorithm(n, numThreads);

        //Calcular Satisfabilidades
        CalculaMetricas(&satisf_masc, &satisf_fem, &satisf_geral, n);

        if (Analise) stopTimer(&cronometro);
        if (Analise) tempoexec = getElapsedTime(&cronometro);

        if (Analise) EscreveAnaliseLatex(ArqAnaliseLatex, numThreads, n, tempoexec);
        if (Analise) EscreveAnaliseGnuPlot(ArqAnaliseGnuPlot, numThreads, n, tempoexec);

        //Escreve os resultados no arquivo
        EscreveResultados(ArqSaida, global.mulher, global.homem, n, satisf_fem, satisf_masc, satisf_geral);

        //Liberação da memória alocada para as listas de homem e de mulher
        DesalocaVetLista(global.homem, n);
        DesalocaVetLista(global.mulher, n);
        free(global.homem);
        free(global.mulher);
    }

    fclose (ArqAnaliseLatex);
    fclose (ArqAnaliseGnuPlot);
}

//Calcula a satisfabilidade de uma lista após o algoritmo de casamento
float calcula_satisfabilidade(LISTA lista[], int n) {
    int i;
    NO* cursor;
    float satisf=0;
    for (i=1; i<=n; i++) {
        cursor = lista[i].inicio;
        while (cursor->item.valor != lista[i].casado_com_id) { //Continua incrementando somente se não estiver na primeira posição
            cursor = cursor->proximo;
        }
        satisf+=cursor->item.indice;
    }
    return satisf /= n;
}

//Calcula a satisfabilidade após o algoritmo de casamento
float calcula_satisfabilidade_geral(float satisf_fem, float satisf_masc) {
    return (satisf_fem+satisf_masc)/2;
}

//Perorre a lista de preferência da mulher à procura das ids dos pretendentes conflitantes, retorna o que for mais interessante para a mulher
//O(n) pois percorre as prioridades da mulher
NO* desempata(LISTA* mulher, int id_homem1, int id_homem2) {
    NO* cursor;
    cursor = mulher->inicio; //Recebe a ordem de pretendentes que o interessado quer casar

    while (cursor != NULL) { //Percorre todos os pretendentes do interessado na lista de preferência

        if (cursor->item.valor == id_homem1) {
            return cursor;
        } else if(cursor->item.valor == id_homem2) {
            return cursor;
        }
        cursor = cursor->proximo;
    }

    return cursor;
}

//Retorna 1 se a pessoa estiver casada e 0 se não
int casado(LISTA* pretendente) {
    return pretendente->casado;
}

//Altera o estado de casado dentre dois casados para CASADOS
void casa(LISTA* pessoa1, int id_pessoa1, LISTA* pessoa2, int id_pessoa2) {
    pessoa1->casado_com_id = id_pessoa2;
    pessoa2->casado_com_id = id_pessoa1;
    pessoa1->casado = true;
    pessoa2->casado = true;
}

//Altera o estado de casado dentre dois casados para NÃO casados
void descasa(LISTA* pessoa1, LISTA* pessoa2) {
    pessoa1->casado_com_id = 0;
    pessoa2->casado_com_id = 0;
    pessoa1->casado = false;
    pessoa2->casado = false;
}

//Recebe a lista de preferencias de um homem e casa esse homem, respeitando critério de desempate da heurística
void *casa_homem(void *t) {

    //Pegar a id da thread
    int i = (int) t;

    int id_homem = i; //Função tenta casar o homem atual
    pthread_mutex_lock (&mutex_homens[id_homem]);
    NO* cursor = global.homem[i].inicio; //Percorrerá a lista de preferência do homem
    int id_mulher = cursor->item.valor;
    int id_homem_ja_casado; //Id do homem que disputará com o homem id_homem caso a mulher já esteja casada

    //O(n²)
    while (global.homem[i].casado == false && cursor != NULL) {

        //Verifica se a mulher está casada
        pthread_mutex_lock (&mutex_mulheres[id_mulher]);
        if (casado(&global.mulher[id_mulher])) {

            //Se ela está casada, ela escolhe o de sua maior preferência, nomeado como 'melhor_opcao'
            NO* melhor_opcao;
            id_homem_ja_casado = global.mulher[id_mulher].casado_com_id;

            pthread_mutex_lock (&mutex_homens[id_homem_ja_casado]);

            melhor_opcao = desempata(&global.mulher[id_mulher], id_homem_ja_casado, id_homem);

            if (melhor_opcao->item.valor != global.mulher[id_mulher].casado_com_id) {
                //Se a melhor opção NÃO for com quem ela está casada atualmente, casa com a melhor opção

                //Descasa a mulher 'id_mulher' com o homem 'id_homem'
                descasa(&global.mulher[id_mulher], &global.homem[global.mulher[id_mulher].casado_com_id]); //Descasa o homem com quem a mulher está atualmente casada
                pthread_mutex_unlock (&mutex_homens[id_homem_ja_casado]);
                //Casa homem 'melhor_opcao->item.valor' com mulher 'id_mulher'
                casa(&global.mulher[id_mulher], id_mulher, &global.homem[melhor_opcao->item.valor], melhor_opcao->item.valor); //Casa a melhor opção

            } else {
                //Caso a melhor opção seja com quem ela está casada atualmente, passa para a próxima opção na lista de preferências do homem
                pthread_mutex_unlock (&mutex_homens[id_homem_ja_casado]);
                pthread_mutex_unlock (&mutex_mulheres[id_mulher]);

                cursor = cursor->proximo;

                id_mulher = cursor->item.valor;

                lista_remover_no(&global.homem[id_homem],cursor->anterior);
            }

        } else {
            //Caso a mulher não esteja casada, casa ela com o homem 'i', tal que i = id_homem
            casa(&global.mulher[id_mulher], id_mulher, &global.homem[id_homem], id_homem);
            pthread_mutex_lock (&mutex_global);
            global.qnt_solteiros--;
            pthread_mutex_unlock (&mutex_global);
        }
        pthread_mutex_unlock (&mutex_mulheres[id_mulher]);
    }
    pthread_mutex_unlock (&mutex_homens[id_homem]);
    pthread_exit((void *)NULL);
}

//Casa dois grupos através do algoritmo de Gale-Stanley, onde os homens propõem e o critério de desempate de conflito é a preferência da mulher
void men_porpose_algorithm(int n, int numThreads) {

    int i;

    pthread_t threads[numThreads];
    int rc;
    int t; //indice da thread
    pthread_mutex_init(&mutex_global, NULL);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    mutex_mulheres = (pthread_mutex_t*)malloc((n+1) * sizeof (pthread_mutex_t));
    mutex_homens = (pthread_mutex_t*)malloc((n+1) * sizeof (pthread_mutex_t));

    if (mutex_mulheres == NULL || mutex_homens == NULL) exit(1);

    for (i=0; i<=n; i++) {
        pthread_mutex_init(&mutex_mulheres[i], NULL);
        pthread_mutex_init(&mutex_homens[i], NULL);
    }

    global.numThreads = numThreads;
    global.qnt_solteiros = n;

    t=0;
    i=1;
    //Enquanto houverem solteiros, percorrer todo o vetor de homens
    pthread_mutex_lock (&mutex_global);
    while (global.qnt_solteiros > 0) {
        pthread_mutex_unlock (&mutex_global);

        if (i > n) i = 1;

        pthread_mutex_lock (&mutex_homens[i]);
        if (!casado(&global.homem[i])) {
            pthread_mutex_unlock (&mutex_homens[i]);
            rc = pthread_create(&threads[t], &attr, casa_homem, (void *) i);
            t++;
            if (rc) {
                printf("[ERROR] return code from pthread_create() is %d: %s\n\n", rc, strerror(rc));
                exit(-1);
            }
        } else pthread_mutex_unlock (&mutex_homens[i]);

        if (t >= global.numThreads-1) {
            for ( t--; t>=0; t--) {
                rc = pthread_join(threads[t], NULL);
                if (rc) {
                    printf("[ERROR] return code from pthread_join() is %d: %s\n\n", rc, strerror(rc));
                    exit(-1);
                }
            }
            t=0;
        }
        i++;
        pthread_mutex_lock (&mutex_global);
    }

    //Join final, para caso não tenha entrado na condição do join acima (caso: nº homens % threads != 0)
    pthread_mutex_unlock (&mutex_global);
    for ( t--; t>=0; t--) {
        rc = pthread_join(threads[t], NULL);
        if (rc) {
            printf("[ERROR] return code from pthread_join() is %d: %s\n\n", rc, strerror(rc));
            exit(-1);
        }
    }

    //Liberação da memória alocada
    pthread_mutex_destroy(&mutex_global);
    pthread_attr_destroy(&attr);
    for (i=0; i<=n; i++) {
        pthread_mutex_destroy(&mutex_mulheres[i]);
        pthread_mutex_destroy(&mutex_homens[i]);
    }
    free(mutex_mulheres);
    free(mutex_homens);

    return;
}

void men_porpose_algorithm_SEQ(int n) {
    int i;

    global.qnt_solteiros = n;

    //Enquanto houverem solteiros, percorrer todo o vetor de homens
    i=1;
    while (global.qnt_solteiros > 0) {
        if (i > n) i = 1;
        if (!casado(&global.homem[i]))
            casa_homem_SEQ((void *)i);
        i++;
    }
    return;
}

void *casa_homem_SEQ(void *t) {

    int i = (int) t;

    int id_homem = i; //Função tenta casar o homem atual
    NO* cursor = global.homem[i].inicio; //Percorrerá a lista de preferência do homem
    int id_mulher = cursor->item.valor;
    int id_homem_ja_casado; //Id do homem que disputará com o homem id_homem caso a mulher já esteja casada

    //O(n²)
    while (global.homem[i].casado == false && cursor != NULL) {

        //Verifica se a mulher está casada
        if (casado(&global.mulher[id_mulher])) {

            //Se ela está casada, ela escolhe o de sua maior preferência, nomeado como 'melhor_opcao'
            NO* melhor_opcao;
            id_homem_ja_casado = global.mulher[id_mulher].casado_com_id;
            melhor_opcao = desempata(&global.mulher[id_mulher], id_homem_ja_casado, id_homem);

            if (melhor_opcao->item.valor != global.mulher[id_mulher].casado_com_id) {
                //Se a melhor opção NÃO for com quem ela está casada atualmente, casa com a melhor opção

                descasa(&global.mulher[id_mulher], &global.homem[global.mulher[id_mulher].casado_com_id]); //Descasa o homem com quem a mulher está atualmente casada
                //Casa homem 'melhor_opcao->item.valor' com mulher 'id_mulher'
                casa(&global.mulher[id_mulher], id_mulher, &global.homem[melhor_opcao->item.valor], melhor_opcao->item.valor); //Casa a melhor opção

            } else {
                //Caso a melhor opção seja com quem ela está casada atualmente, passa para a próxima opção na lista de preferências do homem
                cursor = cursor->proximo;
                id_mulher = cursor->item.valor;
                lista_remover_no(&global.homem[id_homem],cursor->anterior);
            }

        } else {
            //Caso a mulher não esteja casada, casa ela com o homem 'i', tal que i = id_homem
            casa(&global.mulher[id_mulher], id_mulher, &global.homem[id_homem], id_homem);
            global.qnt_solteiros--;
        }
    }
    return NULL;
}
