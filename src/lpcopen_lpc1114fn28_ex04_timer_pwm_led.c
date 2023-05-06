/*
===============================================================================
 Name        : lpcopen_lpc1114fn28_ex04_timer_pwm_led.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "pwm.h"

// TODO: insert other definitions and declarations here
uint32_t pwm_match_counter;

void TIMER32_1_IRQHandler(void)
{
    if (Chip_TIMER_MatchPending(LPC_TIMER32_1, PWM_MR2)) {
        Chip_TIMER_ClearMatch(LPC_TIMER32_1, PWM_MR2);
        Chip_TIMER_SetMatch(LPC_TIMER32_1, PWM_MR0, DutyCycle0);
        pwm_match_counter++;
    }
}


int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    uint32_t pwmDutyCycle = 50;
    uint8_t fadeIn = 1;

    DutyCycle0 = 0;
    DutyCycle1 = 0;
    pwm_match_counter = 0;

    IOCON_Config_Request();
    PWM_Config_Request(LPC_TIMER32_1);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        if (pwm_match_counter >= 0x0200) {
        	pwm_match_counter = 0;
        	if(fadeIn){  // Fade in
        		pwmDutyCycle++;
        		if(pwmDutyCycle == 256){
        			fadeIn = 0;
        		}
        	} else {  // Fade out
        		pwmDutyCycle--;
        		if(pwmDutyCycle == 1){
        			fadeIn = 1;
        		}
        	}
        	PWM_Set_DutyCycle(PWM_MR0, pwmDutyCycle);
        }
		  i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
