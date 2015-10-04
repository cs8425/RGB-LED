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
#define LED_strip(X, data) LED_strip_t X = { .states = data, .idx = 0, .nidx = 0, .timeout = 1, .dir = 0, .step = { .R = 0, .G = 0, .B = 0 }, .out = { .R = 0, .G = 0, .B = 0 } }
#define L(C, T, N) {  .color = &C, .time = T, .next = N }


typedef struct LED_color_s
{
    int16_t R;
    int16_t G;
    int16_t B;
} LED_color_t;

// typedef struct LED_color_step_s
// {
//     int16_t R;
//     int16_t G;
//     int16_t B;
// } LED_color_step_t;

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
    uint16_t nidx;
    uint16_t timeout;
    uint8_t dir;
    LED_color_t step;
    LED_color_t out;
} LED_strip_t;

extern void LED_OUT(int16_t, int16_t, int16_t);

uint16_t find_next_idx(uint16_t idx, uint8_t nextStatus, uint8_t dir, uint8_t* ndir){
    *ndir = dir;
    if(nextStatus & LOOP){
        idx = 0;
    }else{
        if(nextStatus & REVERSE){
            if(nextStatus & TAIL){
                *ndir = 1;
                idx--;
            }else{
                *ndir = 0;
                idx++;
            }
        }else{
            if(nextStatus & (HEAD | STOP)){
                if(dir == 0){
                    idx++;
                }
            }else{
                if(nextStatus & (TAIL | STOP)){

                }else{
                    if(dir == 0){
                        idx++;
                    }else{
                        idx--;
                    }
                }
            }
        }
    }
    return idx;
}

void LED_tick(LED_strip_t *strip){
    uint16_t idx = strip->idx;
    LED_state_t *curr = (strip->states + idx);
    uint8_t nextStatus = curr->next;

    uint8_t ndir = 0;
    // uint16_t nidx = find_next_idx(idx, nextStatus, strip->dir, &ndir);
    uint16_t nidx = strip->nidx;
    LED_state_t *next = (strip->states + nidx);

    if(nextStatus & FADE_TO_LINEAR){
        strip->out.R += strip->step.R;
        strip->out.G += strip->step.G;
        strip->out.B += strip->step.B;
        // LED_OUT(strip->out.R, strip->out.G, strip->out.B);
        // printf("\t[step]\t%d\t%d\t%d\n", strip->step.R, strip->step.G, strip->step.B);
    }
    LED_OUT(strip->out.R, strip->out.G, strip->out.B);

    // printf("\t[curr color]\t%03x\t%03x\t%03x\n", curr->color->R, curr->color->G, curr->color->B);
    // LED_OUT(curr->color->R, curr->color->G, curr->color->B);

    strip->timeout = strip->timeout - 1;
    if(strip->timeout == 0){
        strip->nidx = find_next_idx(idx, nextStatus, strip->dir, &ndir);
        // printf("\t[status]\t%d\t%d\t%d\t%d\n", idx, nidx, strip->timeout, strip->dir);
        // printf("\t[status]\t%d\t%d\t%d\n", idx, nidx, nextStatus & FADE_TO_LINEAR);

        // strip->timeout = next->time;
        strip->timeout = curr->time;

        // printf("\t[status]\t%d\t%d\n", idx, strip->timeout);
        // printf("\t[next color]\t%03x\t%03x\t%03x\n", next->color->R, next->color->G, next->color->B);
        // LED_OUT(next->color->R, next->color->G, next->color->B);
        LED_OUT(curr->color->R, curr->color->G, curr->color->B);
        strip->out.R = curr->color->R;
        strip->out.G = curr->color->G;
        strip->out.B = curr->color->B;

        strip->idx = nidx;
        strip->dir = ndir;
        nextStatus = next->next;
        if(nextStatus & FADE_TO_LINEAR){
            strip->step.R = (next->color->R - curr->color->R);
            if(strip->step.R) strip->step.R = strip->step.R / next->time + 1;

            strip->step.G = (next->color->G - curr->color->G);
            if(strip->step.G) strip->step.G = strip->step.G / next->time + 1;

            strip->step.B = (next->color->B - curr->color->B);
            if(strip->step.B) strip->step.B = strip->step.B / next->time + 1;

            // printf("\t[step]\t%d\t%d\t%d\n", strip->step.R, strip->step.G, strip->step.B);
        }
    }

}

#endif
