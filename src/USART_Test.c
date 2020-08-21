#include <avr/io.h>
#include <avr/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600UL
#define MYUBRR (F_CPU/BAUD/16UL-1)

int counter = 0;
const char* message = "HELLO\n";

int main(){

    // Set transmitter mode to Synchronous USART
    UCSR0C |= (1 << UMSEL00);
    UCSR0C &= ~(1 << UMSEL01);
    UCSR0A &= ~(1 << U2X0);      // always write 0 for synchronous operations

    // Disable parity
    UCSR0C &= ~((1 << UPM01) | (1 << UPM00));

    // 1 stop bit
    UCSR0C &= (1 << USBS0);

    // 8 bit data transmition 
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UCSR0B &= ~(1 << UCSZ02);

    // Data transmit on Rising edge of clock
    UCSR0C &= ~(1 << UCPOL0);

    // Set Baud Rate
    //unsigned char myUBRR = F_CPU/(16*BAUD) - 1;
    UBRR0H = (unsigned char) (MYUBRR>>8);
    UBRR0L = (unsigned char) MYUBRR;

    // Enable USART0 transmitter
    UCSR0B |= (1 << TXEN0);

    const char message[] = "Hello, This is ATMega328p Arduino\n";
    const char* p_outChar;
    while(1){
        p_outChar = message;
        while(*p_outChar != 0){
            while(!(UCSR0A & (1 << UDRE0))){}
            UDR0 = *(p_outChar++);
        }
        _delay_ms(2000);
    }

    return 0;
}