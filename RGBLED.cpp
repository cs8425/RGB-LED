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
LED_Color(G, 0, 1023, 0);
LED_Color(B, 0, 0, 1023);
LED_Color(Y, 1023, 1023, 0);
LED_Color(W, 1023, 1023, 1023);

// LED_state_t colors1[] = {
//     { .color = &R, .time = 500, .next = SWITCH_TO},
//     { .color = &OFF, .time = 300, .next = SWITCH_TO | LOOP}
// };
LED_state_t colors1[] = {
    L(R, 500, SWITCH_TO),
    L(OFF, 300, SWITCH_TO | LOOP)
};

// LED_strip_t mod1_var = { .states = mod1, .idx = 0, .timeout = 0};
LED_strip(mod1, colors1);

void LED_PWM(uint16_t R, uint16_t G, uint16_t B);

int main() {
    pc.baud(115200);

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);    // Enable TIMER32_1 clock
    LPC_IOCON->PIO1_4 = (1<<7)| 0x02 ;        // PIO1_4 IS MAT3 output
    LPC_IOCON->R_PIO1_2 = (1<<7) | 0x03;        // PIO1_2 IS MAT1 output
    LPC_IOCON->R_PIO1_1 = (1<<7) | 0x03;        // PIO1_1 IS MAT0 output

    LPC_TMR32B1->PR = 47;

    LPC_TMR32B1->MR0 = 1023;
    LPC_TMR32B1->MR1 = 500;
    LPC_TMR32B1->MR3 = 1;

    LPC_TMR32B1->MCR = (1 << 7);
    LPC_TMR32B1->MR2 = 1024;

    LPC_TMR32B1->PWMC = (1<<3)|(1<<1)|(1<<0);

    LPC_TMR32B1->TCR = (1 << 0);

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);    // Enable TIMER32_0 clock
    LPC_TMR32B0->PR = 47;
    LPC_TMR32B0->TCR = 1;

    OutputPtr = &LED_PWM;

    timerev.add(LED_TICK, 8*1000, 1, 1);  // add repeat LED_TICK timer
    timerev.add(LED, 200*1000, 1, 1);  // add repeat LED timer
    wait_ms(2);
    timerev.add(DEBUG, 500*1000, 1, 1);  // add repeat DEBUG timer

    while(1) {
        timerev.tick();
        switch(eventloop.get()){

            case LED_TICK:
                LED_tick(&mod1);
                // pc.printf("%d\t%d\t%d\n", ch1, ch2, ch3);
            break;


            // do every 500 ms
            case DEBUG:
                pc.printf("Hi~\n");
                // pc.printf("%d\t%d\t%d\n", ch1, ch2, ch3);
            break;

            //LED do every 200 ms
            case LED:
                led1 = 1 - led1;
            break;

        }
    }
}

void LED_PWM(uint16_t R, uint16_t G, uint16_t B){
    LPC_TMR32B1->MR0 = R & 0x400;
    LPC_TMR32B1->MR1 = G & 0x400;
    LPC_TMR32B1->MR3 = B & 0x400;
}
