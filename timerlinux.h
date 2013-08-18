/*
 ============================================================================
 TRABALHO PRÁTICO 6 - Paralelismo
 Algoritmos e Estruturas de Dados III

 Bruno Maciel Peres
 brunomperes@dcc.ufmg.br

 TIMERLINUX.H - Define as funções e a estrutura do cronômetro de alta precisao utilizada para fins experimentais
 ============================================================================
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <sys/time.h>

typedef struct {
    struct timeval start;
    struct timeval stop;
} stopWatch;

//Inicia o contador de tempo
void startTimer( stopWatch *timer) ;

//Para o contador de tempo
void stopTimer( stopWatch *timer) ;

//Retorna o tempo gasto em milissegundos
double getElapsedTime( stopWatch *timer) ;

#endif // TIMER_H_INCLUDED
