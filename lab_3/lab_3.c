/*
 * Nesse exercicio, é gerado um array 'values' com valores aleatorios
 * e o timer 'ct' é executado a cada 3 segundos e acende o led 
 * correspondente. Na task, é lido os botões e armazenado o valor
 * lido no array 'pressed'. Após dez leituras, os valores são 
 * comparados na função 'check_result'. Os valores possiveis no array
 * 'pressed' são 1 para o botão left, 0 para o botão right e 2 para
 * nenhum botão pressionado.
 */

#include "contiki.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "random.h" /* random_init e random_rand */
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(lab3_process, "Lab 3 process");
AUTOSTART_PROCESSES(&lab3_process);
/*---------------------------------------------------------------------------*/
static struct ctimer ct;
static int cnt=0, corretos=0;
static int values[10], pressed[10];

static void setled_callback( void * ptrValor){
    if( (values[cnt])==0)
        leds_on(LEDS_RED);
    else
        leds_on(LEDS_GREEN);
    cnt++;
    ctimer_restart(&ct);
}

static void gen_random_array(){
    int i;
    random_init(123);
    for(i=0; i < 10; i++){
        values[i] = random_rand()%2;//inicializa o array com 0 ou 1
        pressed[i]=2; //2 indica q nenhum botao foi pressionado
    }
}

static void check_result(){
    int i, corretos=0, timeout=0;

    for(i=0; i < 10; i++ ){
        if(pressed[i] == values[i])
            corretos++;
        if(pressed[i]==2)
            timeout++;
    }

    printf("Resultado:\n");
    printf("Corretos: %d\n",corretos);
    printf("Timeout: %d\n", timeout);
}

PROCESS_THREAD(lab3_process, ev, data)
{
    static int i=0;
    PROCESS_BEGIN();
    printf("Lab3\n");
    SENSORS_ACTIVATE(button_sensor);
    gen_random_array();

    ctimer_set(&ct, CLOCK_SECOND *3, setled_callback, NULL);

    while(cnt<10){
        PROCESS_YIELD();
        if(ev == sensors_event){
            if(data == &button_left_sensor)
                pressed[cnt-1] = 1;
            if(data == &button_right_sensor)
                pressed[cnt-1] = 0;
        }
    }

    check_result();

    while(1)
        PROCESS_YIELD();




    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
