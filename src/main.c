#include <avr/io.h>
#include <util/delay.h>
#include "init/init.h"
#include "led/led.h"
#include "sevseg/sevseg.h"
#include "uart/uart.h"
#include "buttons/button.h"



// ==================== Function decleration ====================
void ButtonTester();
void SaveButtonWorking(uint8_t pressedButton);



// ==================== Testing variables ====================
uint8_t button0 = 0;
uint8_t button1 = 0;
uint8_t button2 = 0;
uint8_t button3 = 0;
uint8_t button4 = 0;



// ==================== Global variables ====================
// For the leds and buttons
uint8_t leds = 0xFF;
uint8_t buttonInput = 0;





// ==================== Main Code ====================

// Main function
int main(void) {

    // Initialization
    InitPorts();
    InitTimer();

    // Set all the leds to on
    LedOut(leds);

    // Main loop
    while(1) {
    }
}





// ==================== Interrupts ====================

// Timer 1 interrupt routine
ISR(TIMER1_COMPA_vect) {
    // Button tester function
    ButtonTester();
}





// ==================== Button Testing Functions ====================
void ButtonTester() {
    buttonInput = ButtonManager();

    if (buttonInput) {
        
        leds ^= 0xFF;
    } else {
        leds = 0xFF;
    }
    LedOut(leds);
}

// Set the 
void SaveButtonWorking(uint8_t pressedButton) {
    switch (pressedButton)
    {
    case 1:
        button0 = 1;
        break;
    case 2:
        button1 = 1;
        break;
    case 3:
        button2 = 1;
        break;
    case 4:
        button3 = 1;
        break;
    case 5:
        button4 = 1;
        break;
    default:
        break;
    }
}