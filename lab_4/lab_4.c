/*
 * Esse exercício funciona com uma maquina de estados. Num estado é apresentada a sequencia de leds,
 * no outro estado são lidos os botoes e o outro estado é o estado de erro. No estado que a sequencia
 * de leds é exibida, a cada iteracao, um item eh gerado e adicionado a lista até o maximo de 5 itens.
 * No estado de leitura de botoes, a cada botão lido, é comparado com o valor presente no array values
 * e caso o botao esteja certo, é incrementado um indice i e caso esteja errado, o estado de erro eh
 * setado na variavel de estado. Como pedido, foi criado o arquivo leds_util.c que contem a funcao
 * responsavel por manipular os leds. Foi criado em leds_util.h uma enum com os valores possiveis dos
 * leds, que são led vermelho aceso, led verde aceso, ambos os leds acesos e nenhum led aceso. Quando
 * o usuario vence, os leds piscam tres vezes. Para isso foi utilizado uma funcao de delay basica 
 * presente no contik ao inves de um timer.
 */

#include "contiki.h"
#include "dev/leds.h"
#include "leds_util.h"
#include "dev/button-sensor.h"
#include "random.h" /* random_init e random_rand */
#include "sys/clock.h"
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/

PROCESS(lab4_process, "Lab 4 process");

AUTOSTART_PROCESSES(&lab4_process);
/*---------------------------------------------------------------------------*/
typedef enum{
    st_leds,
    st_buttons,
    st_error,
    st_end
}mstate_t;

static struct ctimer ct;
static int cnt=-1, values[5];
static mstate_t mstate;



static void setled_callback( void * ptrValor){
    static int i=0;
    acende_led(None);
    clock_delay(100000);
    if( (values[i])==Red)
        acende_led(Red);
    else
        acende_led(Green);


    if(i<cnt){
        ctimer_restart(&ct);
        i++;
    }
    else{
        i=0;
        mstate = st_buttons;
    }
}

static void gen_random_item(){
    cnt++;
    values[cnt] = random_rand()%2;//inicializa o array com 0 ou 1

}

PROCESS_THREAD(lab4_process, ev, data)
{
    PROCESS_BEGIN();

    static int i=0;
    mstate = st_leds;

    printf("Lab4\n");
    SENSORS_ACTIVATE(button_sensor);
    random_init(123);


    while(cnt<5){


        if(mstate == st_leds){
            gen_random_item();
            ctimer_set(&ct, CLOCK_SECOND * 1, setled_callback, NULL);
            i = 0;
        }
        PROCESS_YIELD();
        if(mstate == st_buttons){
            if(ev == sensors_event){
                if(data == &button_left_sensor){
                    if(values[i] == Red)
                        i++;
                    else
                        mstate = st_error;
                }
                if(data == &button_right_sensor){
                    if(values[i] == Green)
                        i++;
                    else
                        mstate = st_error;
                }
                if(i == 5)
                    break;

                if(i>cnt)
                    mstate=st_leds;

            }
        }

        if(mstate == st_error){
            break;
        }

    }

    if(mstate != st_error){
        printf("Voce ganhou!\n");
        for(i=0; i < 3; i++){
            acende_led(Both);
            clock_wait(CLOCK_SECOND/2);
            acende_led(None);
            clock_wait(CLOCK_SECOND/2);
        }
    }
    else{
        printf("Voce perdeu!\nFez %d pontos\n", cnt-1);
        acende_led(Both);
    }

    while(1)
        PROCESS_YIELD();




    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
