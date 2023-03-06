#include <avr/io.h>
#include <util/delay.h>
#include "init/init.h"
#include "led/led.h"
#include "sevseg/sevseg.h"
#include "uart/uart.h"
#include "buttons/button.h"
#include "adc/adc.h"
#include "lcd/lcd.h"


// ==================== Function decleration ====================
void ButtonTester();                                // Tests the buttons and blinks the leds if a button is pressed
void SaveButtonWorking(uint8_t pressedButton);      // Saves the button that is pressed
void GetTemperature();                              // Starts the ADC conversion
void AdcResultReady();                              // Gets the ADC result and converts it to temperature
uint8_t GetMatrixNumber();                          // Gets the number from the matrix keyboard
void RGB_LED(uint8_t red, uint8_t green, uint8_t blue);

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
uint16_t sevSegCounter = 0;
uint8_t sevSegToggle = 0;

// For the ADC and temperature
uint16_t temperature = 0;
uint16_t AdcResult = 0;

// For the matrix keyboard		    0   1   2   3   4   5   6   7   8   9   *   #
unsigned char MatrixManager[12] = {69, 14, 13, 11, 22, 21, 19, 38, 37, 35, 70, 67};
uint8_t SizeOfMatrixManager = sizeof(MatrixManager) / sizeof(MatrixManager[0]);
uint8_t MatrixInput = 12;		// This will be the button input from the matrix keyboard
uint8_t matrixPressed = 0;

// For the RGB led
uint8_t rgbled = 0;
uint8_t rgbCounter = 0;

// For the UART
uint8_t uartCounter = 0;
char uartToSend = '0';
// ==================== Main Code ====================

// Main function
int main(void) {

    // Initialization
    InitPorts();
    InitTimer();
    ADC_Init(0x00, 0x07);
    ADC_EnableInterrupts();
    LCD_Initialization();
    UsartInit(MYUBRR);

    // Set all the leds to on
    LedOut(leds);

    // LCD test
    LCD_SendStringToLine("ABCDEFGHIJKLMNOP", 0);
    LCD_SendStringToLine("abcdefghijklmnop", 1);
    LCD_SendStringToLine("1234567890123456", 2);
    LCD_SendStringToLine("1234567890123456", 3);

    // Main loop
    while(1) {
    }
}



// ==================== Interrupts ====================

// Timer 0 interrupt routine
ISR(TIMER0_OVF_vect) {
    // Matrix keyboard
    int matrixInput = GetMatrixNumber();
    if (matrixInput != 12) {
        if (!matrixPressed) {
            matrixPressed = 1;
            LCD_SetCursorPos(0, 0);
            LCD_SendData(matrixInput + '0');
        }
    } else {
        matrixPressed = 0;
    }
}

// Timer 1 interrupt routine
ISR(TIMER1_COMPA_vect) {
    sei();

    // Button tester function
    ButtonTester();
    
    if (rgbCounter >= 10) {
        rgbCounter = 0;

                // RGB led
        switch (rgbled) {
        case 0:
            RGB_LED(1, 0, 0);
            rgbled++;
            break;
        case 1:
            RGB_LED(0, 1, 0);
            rgbled++;
            break;
        case 2:
            RGB_LED(0, 0, 1);
            rgbled = 0;
            break;
        default:
            break;
        }


        // UART send OK message
        UsartClearLine();
        UsartTransmitString("\rOK");
        uartToSend = uartCounter + '0';
        UsartTransmitString(&uartToSend);
        UsartTransmitString("\r");

        uartCounter++;
        if (uartCounter >= 10) {
            uartCounter = 0;
        }

    } else {
        rgbCounter++;
    }
}

// Timer 2 interrupt routine
ISR(TIMER2_OVF_vect) {
    // 7 segment display

    if (sevSegToggle) {
        SevsegOut(sevenSeg);
    } else {
        GetTemperature();
        SevsegOut(temperature);
    }

    if (sevSegCounter >= 1500) {
        sevSegToggle = !sevSegToggle;
        sevSegCounter = 0;
    } else {
        sevSegCounter++;
    }
}

// ADC interrupt routine
ISR(ADC_vect) {
    AdcResultReady();
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



// ==================== Temperature Sensor ====================

void GetTemperature() {
    if (!ADC_IsInConversion()) {
        ADC_ConvStart(0x00);
    }
}

void AdcResultReady() {
	AdcResult = ADC;
	AdcResult = AdcResult >> 2;
	AdcResult = AdcResult/10;
	temperature = AdcResult >> 2;
	ADCSRA &= ~(1<<ADIF);
}



// ==================== Matrix Keyboard ====================

uint8_t GetMatrixNumber() {
	uint8_t sor = 0x08;		// Elso sor cimzese
	uint8_t olvas = 0;
	uint8_t i = 0;

	while(1){
		PORTC &= 0x80;
		PORTC |= sor;
		_delay_us(10);
		olvas = PINC & 0x7F;
		
		i = 0;
		while (i < SizeOfMatrixManager) {
			if (MatrixManager[i] == olvas) {
				return i;
			}
			i++;
		}
		
		if (sor == 0x40) {
			break;
		} else {
			sor = sor << 1;
		}
	}
	
	// If there is no button pressed
	return 12;
}



// RGB LED
void RGB_LED(uint8_t red, uint8_t green, uint8_t blue) {
	if (red) {
		PORTC |= 0x80;
	} else {
		PORTC &= ~0x80;
	}
	
	if (blue) {
		PORTE |= 0x08;
	} else {
		PORTE &= ~0x08;
	}
	
	if (green) {
		PORTE |= 0x04;
	} else {
		PORTE &= ~0x04;
	}
}