#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define BPM 100
#include <music.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

ISR(TIMER0_OVF_vect){
    TIFR0 |= 1;
}

int main(){

    DDRD |= (1 << PD6);

    TCCR0A = 0b01000010;
    TCCR0B = 0b00000011;
    TIMSK0 = 0b00000001;
    sei();

    // Monody

    while(1){

        PLAY_NOTE(Gs5, NOTE_8, 0);
        PLAY_NOTE(B5, NOTE_8, 0);
        //---------------------
        PLAY_NOTE(Cs6, NOTE_4, 1);
        PLAY_NOTE(Cs6, NOTE_8, 0);
        PLAY_NOTE(B5, NOTE_8, 0);
        PLAY_NOTE(Gs5, NOTE_4, 0);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        //---------------------
        PLAY_NOTE(Fs5, NOTE_4, 1);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        PLAY_NOTE(Cs5, NOTE_4, 0);
        PLAY_NOTE(B4, NOTE_8, 0);
        PLAY_NOTE(Cs5, NOTE_8, 0);
        //---------------------
        PLAY_NOTE(E5, NOTE_4, 0);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(Gs5, NOTE_8, 0);
        PLAY_NOTE(A5, NOTE_4, 0);
        PLAY_NOTE(Gs5, NOTE_8, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        //--------------------
        PLAY_NOTE(Fs5, NOTE_2_D, 0);   // Phrase
        PLAY_NOTE(Gs5, NOTE_8, 0);
        PLAY_NOTE(B5, NOTE_8, 0);
        //---------------------
        PLAY_NOTE(Cs6, NOTE_8_D, 0);
        PLAY_NOTE(Ds6, NOTE_16, 0);
        PLAY_NOTE(Cs6, NOTE_8, 0);
        PLAY_NOTE(B5, NOTE_8, 0);
        PLAY_NOTE(Gs5, NOTE_4, 0);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        //--------------------
        PLAY_NOTE(Fs5, NOTE_8_D, 0);
        PLAY_NOTE(Gs5, NOTE_16, 0);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        PLAY_NOTE(Cs5, NOTE_4, 0);
        PLAY_NOTE(B4, NOTE_8, 0);
        PLAY_NOTE(Cs5, NOTE_8, 0);
        //--------------------
        PLAY_NOTE(E5, NOTE_4, 0);
        PLAY_NOTE(Fs5, NOTE_8, 0);
        PLAY_NOTE(Gs5, NOTE_8, 0);
        PLAY_NOTE(Fs5, NOTE_4, 0);
        PLAY_NOTE(E5, NOTE_8, 0);
        PLAY_NOTE(Cs5, NOTE_8, 0);
        //--------------------
        PLAY_NOTE(E5, 4, 0);     // Phrase
        STOP_SOUND;
        _delay_ms(BWIDTH_ms*4);
        START_SOUND;
    }

    return 0;

}
