#include "mbed.h"

#include "EventDriven.h"
#include "TimerEv.h"

#include "RGBLED.h"

Serial pc(P1_7, P1_6); // tx, rx
DigitalOut led1(P3_1);


// define events
#define LED_TICK      1
#define LED           2
#define GPS_update    3
#define DEBUG         4
#define TEST         5

EventDriven eventloop;
TimerEv timerev(eventloop);

// LED_color_t R = { .R = 1023, .G = 0, .B = 0};
// LED_color_t G = { .R = 0, .G = 1023, .B = 0};
// LED_color_t B = { .R = 0, .G = 0, .B = 1023};
// LED_color_t Y = { .R = 1023, .G = 1023, .B = 0};
// LED_color_t W = { .R = 1023, .G = 1023, .B = 1023};
// LED_color_t OFF = { .R = 0, .G = 0, .B = 0};

LED_Color(OFF, 0, 0, 0);
LED_Color(R, 1023, 0, 0);
LED_Color(G, 0, 250, 0);
LED_Color(B, 0, 0, 700);
LED_Color(Y, 1023, 250, 0);
LED_Color(W, 511, 511, 511);

// LED_state_t colors1[] = {
//     L(R, 75, SWITCH_TO),
//     L(OFF, 50, SWITCH_TO),
//     L(G, 75, SWITCH_TO),
//     L(OFF, 50, SWITCH_TO),
//     L(B, 75, SWITCH_TO),
//     L(OFF, 50, SWITCH_TO),
//     L(Y, 75, SWITCH_TO),
//     L(OFF, 50, SWITCH_TO | LOOP)
// };



LED_state_t colors1[] = {
    L(R, 500, FADE_TO_LINEAR | HEAD),
    L(G, 500, FADE_TO_LINEAR),
    L(B, 500, FADE_TO_LINEAR| LOOP | TAIL)
};

LED_state_t colors2[] = {
    L(R, 200, SWITCH_TO | HEAD),
    L(G, 200, SWITCH_TO),
    L(B, 200, SWITCH_TO| LOOP | TAIL)
};

LED_state_t colors3[] = {
   L(B, 1000, FADE_TO_LINEAR),
   L(OFF, 1000, FADE_TO_LINEAR| LOOP | TAIL)
};

LED_state_t colors4[] = {
    L(R, 100, SWITCH_TO | REVERSE | HEAD),
    L(OFF, 50, SWITCH_TO),
    L(G, 100, SWITCH_TO),
    L(OFF, 50, SWITCH_TO),
    L(B, 100, SWITCH_TO),
    L(OFF, 50, SWITCH_TO | REVERSE | TAIL)
};

LED_state_t colors5[] = {
    L(R, 50, FADE_TO_LINEAR | REVERSE | HEAD),
    L(OFF, 100, FADE_TO_LINEAR),
    L(G, 50, FADE_TO_LINEAR),
    L(OFF, 100, FADE_TO_LINEAR),
    L(B, 50, FADE_TO_LINEAR),
    L(OFF, 100, FADE_TO_LINEAR | REVERSE | TAIL)
};

LED_strip(mod1, colors1);
LED_strip(mod2, colors2);
LED_strip(mod3, colors3);
LED_strip(mod4, colors4);
LED_strip(mod5, colors5);

void LED_OUT(uint16_t R, uint16_t G, uint16_t B);


int16_t _R;
int16_t _G;
int16_t _B;

uint8_t mod = 3;
int main() {

    pc.baud(115200);

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);    // Enable TIMER32_1 clock
    LPC_IOCON->PIO1_4 = (1<<7)| 0x02 ;        // PIO1_4 IS MAT3 output
    LPC_IOCON->R_PIO1_2 = (1<<7) | 0x03;        // PIO1_2 IS MAT1 output
    LPC_IOCON->R_PIO1_1 = (1<<7) | 0x03;        // PIO1_1 IS MAT0 output

    LPC_TMR32B1->PR = 47;

    LPC_TMR32B1->MR0 = 0;
    LPC_TMR32B1->MR1 = 0;
    LPC_TMR32B1->MR3 = 0;

    LPC_TMR32B1->MCR = (1 << 7);
    LPC_TMR32B1->MR2 = 1024;

    LPC_TMR32B1->PWMC = (1<<3)|(1<<1)|(1<<0);

    LPC_TMR32B1->TCR = (1 << 0);

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);    // Enable TIMER32_0 clock
    LPC_TMR32B0->PR = 47;
    LPC_TMR32B0->TCR = 1;

    timerev.add(LED_TICK, 2*1000, 1, 1);  // add repeat LED_TICK timer
    timerev.add(LED, 200*1000, 1, 1);  // add repeat LED timer
    wait_ms(2);
    timerev.add(DEBUG, 500*1000, 1, 1);  // add repeat DEBUG timer

    timerev.add(TEST, 10000*1000, 1, 1);

    while(1) {
        timerev.tick();
        switch(eventloop.get()){

            case LED_TICK:
                switch(mod){
                    case 0:
                        LED_tick(&mod1);
                    break;

                    case 1:
                        LED_tick(&mod2);
                    break;

                    case 2:
                        LED_tick(&mod3);
                    break;

                    case 3:
                        LED_tick(&mod4);
                    break;

                    case 4:
                        LED_tick(&mod5);
                    break;
                }
            break;


            // do every 500 ms
            case DEBUG:
                // pc.printf("Hi~\n");
                pc.printf("[%d]\t%d\t%d\t%d\n", mod, _R & 0x3FF, _G & 0x3FF, _B & 0x3FF);
            break;

            //LED do every 200 ms
            case LED:
                led1 = 1 - led1;
            break;

            case TEST:
                mod = (mod + 1) % 5;
            break;
        }
    }
}

void LED_OUT(int16_t R, int16_t G, int16_t B){
    LPC_TMR32B1->MR1 = 0x3FF - (R & 0x3FF);
    LPC_TMR32B1->MR3 = 0x3FF - (G & 0x3FF);
    LPC_TMR32B1->MR0 = 0x3FF - (B & 0x3FF);

    _R = R;
    _G = G;
    _B = B;
}
