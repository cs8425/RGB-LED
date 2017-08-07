# RGB-LED
A simple yet powerful library for LED sequential in C.

## Setup

1. define colors
``` c
LED_Color(OFF, 0, 0, 0);
LED_Color(R, 0xFF, 0, 0);
LED_Color(G, 0, 0xFF, 0);
LED_Color(B, 0, 0, 0xFF);
LED_Color(Y, 0xFF, 0xFF, 0);
LED_Color(W, 0xFF, 0xFF, 0xFF);
```

2. define color sequential
``` c
LED_state_t colors5[] = {
	L(R, 300, FADE_TO_LINEAR | REVERSE | HEAD),
	L(OFF, 100, FADE_TO_LINEAR),
	L(G, 300, FADE_TO_LINEAR),
	L(OFF, 100, FADE_TO_LINEAR),
	L(B, 300, FADE_TO_LINEAR | REVERSE | TAIL)
};
```

3. define running mode
``` c
LED_strip(mod1, colors1);
LED_strip(mod2, colors2);
LED_strip(mod3, colors3);
LED_strip(mod4, colors4);
LED_strip(mod5, colors5);
```

4. implement `void LED_OUT(int16_t R, int16_t G, int16_t B)` which do real output
``` c
void LED_OUT(int16_t R, int16_t G, int16_t B) {
	LPC_TMR32B1->MR1 = 0x3FF - R_map[(R & 0x3FF) >> 2];
	LPC_TMR32B1->MR3 = 0x3FF - G_map[(G & 0x3FF) >> 2];
	LPC_TMR32B1->MR0 = 0x3FF - B_map[(B & 0x3FF) >> 2];
}
```

## Usage
more can see `RGBLED.cpp`, base on mbed and [EventDriven](https://github.com/cs8425/EventDriven).

``` cpp
#include "RGBLED.h"

// colors
LED_Color(OFF, 0, 0, 0);
LED_Color(R, 0xFF, 0, 0);
LED_Color(G, 0, 0xFF, 0);
LED_Color(B, 0, 0, 0xFF);
LED_Color(Y, 0xFF, 0xFF, 0);
LED_Color(W, 0xFF, 0xFF, 0xFF);

// color sequential
LED_state_t colors1[] = {
   L(W, 250, SWITCH_TO),
   L(OFF, 25, SWITCH_TO),
   L(Y, 250, SWITCH_TO),
   L(OFF, 25, SWITCH_TO| LOOP | TAIL) // will go to first color
};

// running mode
LED_strip(mod1, colors1);

int main(){

	while(1){
		LED_tick(&mod1); // update state
		sleep(2); // sleep 2 ms
	}

	return 0;
}


```


## API

[TOC]

### void LED_tick(&Mode_Name);
update LED.

### LED_Color(Color_NAME, R, G, B);
define Color with Name and RGB value.

### LED_state_t SEQ[] = {....}
define color sequential. Array of `L(Color_Name, tick, TYPE)`.

### L(Color_Name, tick, TYPE)
show what color, how many tick, and TYPE.
TYPE:
- `HEAD` : Start Mark (Must have if using `REVERSE`)
- `TAIL` : End Mark (Must have if using `REVERSE`)
- `STOP` : stop here forever
- `LOOP` : go 1st color
- `REVERSE` : go another way
- `SWITCH_TO` : switch to next color, no fading
- `FADE_TO_LINEAR` : linear fade to next color

### LED_strip(Mode_Name, LED_state_t seq);
define running mode with color sequential.

### LED_strip_rest(&Mode_Name);
reset running mode to initial.

