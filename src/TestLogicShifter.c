/* ========= TODO ============

Convert data in ADCRes to
string before sending it
in the funtion ISR(ADC_vect)

=========================== */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600UL
#define MYUBRR (F_CPU/BAUD/16UL-1)

uint16_t num;
char dig_buf[5];

void reverseString(const char* c, char* buf, unsigned int size_buf){

    if(c == buf){
        buf += size_buf-1;
        *(buf--) = 0;
        char ctemp;
        while(c < buf){
            ctemp = *c;
            *((char*)c++) = *buf;
            *(buf--) = ctemp;
        }
    }else{
        char* buf2 = buf+size_buf-1;
        *(buf2--) = 0;
        while(*c != 0 && buf2>=buf){
            *buf2 = *c;
            c++;
            buf2--;
        }
    }
    return;
}

void intToString(uint16_t num16, char* buf, unsigned int size_buf){
    int counter = 0;
    if(num16 == 0 && size_buf >= 2){
        *(buf++) = '0';
        *buf = 0;
        return;
    }
    while(counter < (size_buf-1)){
        buf[counter] = (char)(num16%10 + '0');
        num16 /= 10;
        counter++;
    }
    buf[counter--] = 0;
    while(buf[counter] == '0'){
        buf[counter] = 0;
        counter--;
    }
    reverseString(buf, buf, counter+2);
    return;
}


int main(){

    DDRC &= ~(1 << PC2);        // Set PC2 as input
    PORTC &= ~(1 << PORTC2);    // Disable pullup for PC2

    /* ADMUX

        REFS0   REFS1   ADLAR   -       MUX3    MUX2    MUX1    MUX0
        0       1       0       0       0       0       1       0
        
        REFS0..1    : AVCC with external capacitor at AREF pin (Arduino default)
        ADLAR       : Right Adjust data
        MUX3..0     : Connect PC2 to ADC
    */
    ADMUX = 0b01000010;

    /* ADCSRB

        -       ACME    -       -       -       ADTS2   ATDS1   ADTS0
        0       0       0       0       0       0       0       0

        ACME        : Analog Comparator Multiplexor disabled
        ADTS2..0    : Free running Mode

    */
    ADCSRB = 0;


    /* ADCSRA

        ADEN    ADSC    ADATE   ADIF    ADIE    ADPS2   ADPS1   ADPS0
        1       1       1       1       1       0       1       1

        ADEN        : Enable ADC
        ADSC        : Start Conversion
        ADATE       : Enable Auto trigger
        ADIF        : (flag bit) clear bit by writing logic one to it
        ADIE        : Enable interrupt for when ADIF is set ( => when each converstion is complete)
        ADPS2..1    : Divison factor of 8 between system clock and ADC clock

    */
    ADCSRA = 0b11111011;

    
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
    // unsigned char myUBRR = F_CPU/(16*BAUD) - 1;
    UBRR0H = (unsigned char) (MYUBRR>>8);
    UBRR0L = (unsigned char) MYUBRR;

    // Enable USART0 transmitter
    UCSR0B |= (1 << TXEN0);

    // Hello
/*
    const unsigned char* message = "hello";
    unsigned char* outChar = message;
    do{
        while(UCSR0A & (1 << UDRE0)){}
        UDR0 = *outChar;
        outChar++;
    }while(*outChar != 0);*/

    const char* message = "Hello, My name is swarnava ghosh\n";
    const char* outChar;
    for(int i = 0; i < 5; i++){
        outChar = message;
        while(*outChar != 0){
            while(!(UCSR0A & (1 << UDRE0))){}
            UDR0 = *outChar;
            outChar++;
        }
        _delay_ms(2000);
    }


    sei();

    while(1){}

}

ISR(ADC_vect){

    num = ADCL;
    num += (ADCH <<8);

    intToString(num, dig_buf, 5);

    char* outChar = dig_buf;
    while(*outChar != 0){
        while(!(UCSR0A & (1 << UDRE0))){}
        UDR0 = *outChar;
        outChar++;
    }
    while(!(UCSR0A & (1 << UDRE0))){}
    UDR0 = '\n';


}