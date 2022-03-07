#ifndef SIGNAL_H
#define SIGNAL_H


/* Trata sinais recebidos
* INPUTs: Sinal.
* OUTPUTs: Nenhum.
* Pré-condição: Sinal é válido.
* Pós-condição: Sinal devidamente tratado tratado. 
*/
void signalHandler(int signal);

/* Define sinais que serão ignorados
* INPUTs: Nenhum
* OUTPUTs: Nenhum.
* Pré-condição: Nenhuma.
* Pós-condição: Sinais definidos.
*/
void setSignalsIgnore();

/* Define sinais para grupo de vacinados
* INPUTs: Nenhum
* OUTPUTs: Nenhum.
* Pré-condição: Nenhuma.
* Pós-condição: Sinais definidos.
*/
void setSignalsVacinated();

#endif