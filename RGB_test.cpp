#include <iostream>
#include <stdio.h>
#include <stdint.h>

using namespace std;

#include "RGBLED.h"


LED_Color(OFF, 0, 0, 0);
LED_Color(R, 1023, 0, 0);
LED_Color(G, 0, 1023, 0);
LED_Color(B, 0, 0, 1023);
LED_Color(Y, 1023, 1023, 0);
LED_Color(W, 1023, 1023, 1023);

LED_state_t colors1[] = {
    L(R, 3, SWITCH_TO),
    L(OFF, 2, SWITCH_TO | LOOP)
};

LED_state_t colors2[] = {
    L(R, 4, SWITCH_TO | REVERSE | HEAD),
    L(G, 3, SWITCH_TO),
    L(B, 2, SWITCH_TO),
    L(OFF, 1, SWITCH_TO | REVERSE | TAIL)
};

// LED_state_t colors1[] = {
//     L(R, 3, SWITCH_TO | STOP | HEAD),
//     L(G, 2, SWITCH_TO),
//     L(B, 1, SWITCH_TO | REVERSE | TAIL),
// };

LED_strip(mod1, colors1);
LED_strip(mod2, colors2);

void LED_PWM(uint16_t R, uint16_t G, uint16_t B);

uint16_t _R;
uint16_t _G;
uint16_t _B;

int main(int argc, char *argv[]) {
    int i;

    for(i=0; i<30; i++){
        printf("[%03d]\n", i);
        // LED_tick(&mod1);
        if(i < 15){
            LED_tick(&mod1);
        }else{
            LED_tick(&mod2);
        }
        printf("\t[curr color]\t%03x\t%03x\t%03x\n", _R, _G, _B);
    }
}

void LED_PWM(uint16_t R, uint16_t G, uint16_t B){
    printf("\t[O]\t%03x\t%03x\t%03x\n", R & 0x3FF, G & 0x3FF, B & 0x3FF);
}

void LED_OUT(uint16_t R, uint16_t G, uint16_t B){
    // printf("\t[O]\t%03x\t%03x\t%03x\n", R & 0x3FF, G & 0x3FF, B & 0x3FF);
    _R = R;
    _G = G;
    _B = B;
}
