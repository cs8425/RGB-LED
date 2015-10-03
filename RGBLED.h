#ifndef __RGBLED_H
#define __RGBLED_H

#define HEAD            (0x01 << 7)
#define TAIL            (0x01 << 6)

#define STOP            (0x01 << 5)
#define LOOP            (0x01 << 4)
#define REVERSE         (0x01 << 3)

#define SWITCH_TO       (0x01 << 0)
#define FADE_TO_LINEAR  (0x01 << 1)

#define LED_Color(X, _R, _G, _B) LED_color_t X = { .R = _R, .G = _G, .B = _B }
#define LED_strip(X, data) LED_strip_t X = { .states = data, .idx = 0, .timeout = 1, .dir = 0 }
#define L(C, T, N) {  .color = &C, .time = T, .next = N }


typedef struct LED_color_s
{
    uint16_t R;
    uint16_t G;
    uint16_t B;
} LED_color_t;

typedef struct LED_state_s
{
    LED_color_t *color;
    uint16_t time;
    /*uint8_t div;*/
    uint8_t next;
} LED_state_t;

typedef struct LED_strip_s
{
    LED_state_t *states;
    uint16_t idx;
    uint16_t timeout;
    uint8_t dir;
} LED_strip_t;

// void (*OutputPtr)(uint16_t, uint16_t, uint16_t);
extern void LED_OUT(uint16_t, uint16_t, uint16_t);

void LED_tick(LED_strip_t *strip){
    uint16_t idx = strip->idx;

    LED_state_t *curr = (strip->states + idx);
    uint8_t nextStatus = curr->next;
    // uint16_t time = curr->time;

    // LED_state_t *next = (strip->states + idx + 1);

    // printf("\t[curr color]\t%03x\t%03x\t%03x\n", curr->color->R, curr->color->G, curr->color->B);
    // LED_OUT(curr->color->R, curr->color->G, curr->color->B);

    strip->timeout = strip->timeout - 1;
    if(strip->timeout == 0){
        // strip->timeout = 0;
        // printf("\t[status]\t%d\t%d\t%d\n", idx, strip->timeout, strip->dir);
        if(nextStatus & LOOP){
            strip->idx = 0;
        }else{
            if(nextStatus & REVERSE){
                if(nextStatus & TAIL){
                    strip->dir = 1;
                    strip->idx--;
                }else{
                    strip->dir = 0;
                    strip->idx++;
                }
            }else{
                if(nextStatus & (HEAD | STOP)){
                    if(strip->dir == 0){
                        strip->idx++;
                    }
                }else{
                    if(nextStatus & (TAIL | STOP)){

                    }else{
                        if(strip->dir == 0){
                            strip->idx++;
                        }else{
                            strip->idx--;
                        }
                    }
                }
            }
        }
        LED_state_t *next = (strip->states + idx);
        strip->timeout = next->time;

        // printf("\t[status]\t%d\t%d\n", idx, strip->timeout);
        // printf("\t[next color]\t%03x\t%03x\t%03x\n", next->color->R, next->color->G, next->color->B);
        // // OutputPtr(next->color->R, next->color->G, next->color->B);
        LED_OUT(next->color->R, next->color->G, next->color->B);
        // LED_OUT(curr->color->R, curr->color->G, curr->color->B);
    }

}

#endif
