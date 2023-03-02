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
uint8_t boardLeds = 0;      // 0-7 leds

uint8_t button0 = 0;        // right most button
uint8_t button1 = 0;        // 2nd right most button
uint8_t button2 = 0;        // center button
uint8_t button3 = 0;        // 2nd left most button
uint8_t button4 = 0;        // left most button

uint8_t sevSegWorking = 0;  // 7 segment display




// ==================== Global variables ====================

// For the leds and buttons
uint8_t leds = 0xFF;
uint8_t buttonInput = 0;

// For the 7 segment display
uint16_t sevenSeg = 8888;



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

// Timer 0 interrupt routine
ISR(TIMER0_OVF_vect) {
    // 7 segment display
    //SevsegOut(sevenSeg);
}

// Timer 1 interrupt routine
ISR(TIMER1_COMPA_vect) {
    // Button tester function
    ButtonTester();
    
}

// Timer 2 interrupt routine
ISR(TIMER2_OVF_vect) {
    // 7 segment display
    SevsegOut(sevenSeg);
}



// ==================== Button Testing Functions ====================

// Blink the leds if a button is pressed
void ButtonTester() {
    buttonInput = ButtonManager();

    if (buttonInput) {
        SaveButtonWorking(buttonInput);     
        leds ^= 0xFF;
    } else {
        leds = 0xFF;
    }
    LedOut(leds);
}

// Saves the pressed button
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