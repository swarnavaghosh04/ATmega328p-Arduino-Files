#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Output pins
    // SRCLK   Pin 5 (PD5)
    // RCLK    Pin 6 (PD6)
    // SER     Pin 7 (PD7)

// Input pins
    // pin 2 (PD2 | INT0) as input with rising edge interrept
    // pin 3 (PD3) as SER input

ISR(INT0_vect){

    // Figure out the value at PD3 for SER input

    if(PIND & (1 << PD3)){            // if PD3 is high
        PORTD &= ~(1 << PD7);         // Set SER output to low (oppostie due to internal pull-up resistsor)
    } else{                           // else
        PORTD |= (1 << PD7);          // Set SER output to high
    }

    // SRCLK (PD5) and RCLK (PD6) high

    PORTD |= (1 << PD5);
    _delay_ms(3);
    PORTD |= (1 << PD6);
    _delay_ms(3);

    // SRCLK (PD5) and RCLK (PD6) low

    PORTD &= ~(1 << PD5);
    _delay_ms(3);
    PORTD &= ~(1 << PD6);
    _delay_ms(3);

}

int main(){

    // Configure ports

    DDRD &= ~(1 << PD2 | 1 << PD3);                  // set PD2/3 as input
    DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);    // set PD5/6/7 as outputs

    PORTD &= ~(1 << PD2);                            // PD2 pull-up resistors off
    PORTD |= (1 << PD3);                             // PD3 pull-uo resistor on
    PORTD &= ~(1 << PD5 | 1 << PD6 | 1 << PD7);      // PD5/6/7 output to 0

    // Configure interrput on PD2 (INT0)

    EICRA = 0b00000011;        // Generate interrupt request on rising edge
    EIMSK = 0b00000001;        // Activate INT0 interrupt requests

    sei();         // Enable interrupts

    while(1){}


    return 0;
}