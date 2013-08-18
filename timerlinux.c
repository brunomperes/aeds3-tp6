// Timer de alta resolução para Linux e Mac
// No código: rodar startTimer antes do trecho a cronometrar,
// stopTimer depois, e obter o número
// de milissegundos com a funcao getElapsedTime.
// Todas requerem o mesmo parametro do tipo “stopWatch”.

#include <stdlib.h>
#include "timerlinux.h"

void startTimer( stopWatch *timer) {
    gettimeofday(&timer->start, NULL);
}

void stopTimer( stopWatch *timer) {
    gettimeofday(&timer->stop, NULL);
}

double getElapsedTime( stopWatch *timer) { // in seconds
    double elapsed_time;
    elapsed_time = (timer->stop.tv_sec - timer->start.tv_sec); //sec
    elapsed_time += (timer->stop.tv_usec - timer->start.tv_usec) / 1000000.0; //microsseconds to seconds
    return elapsed_time;
}
