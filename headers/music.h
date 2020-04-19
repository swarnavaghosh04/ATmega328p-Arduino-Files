/*
DEFINE (not necessary):
    BPM - Beats per minute (Default: 120)
    INT_PIN - interrput pin > can be either OCR0A or OCR0B (Default: OCR0A)

SETUP: 
    TCCR0A = 0b01000010;   // Toggle OC0A (PD6) on compare match | Normal port peration for OC0B (PD5) | CTC Mode
    TCCR0B = 0b00000100;   // Prescalaing: ClockIO / 256
    TIMSK0 = 0b00000001;   // Overflow interrupt enabled

    ISR(TIMER0_OVF_vect){
        TIFR0 |= 1; // Clear Overflow
    }

*/
#pragma once

#ifndef BPM
#define BPM 120
#endif

#ifndef INT_PIN
#define INT_PIN OCR0A
#endif

#define FREQUENCY(f) (F_CPU/(512.*f) - 1.)
#define BWIDTH_ms (double)(60000./BPM)
#define DETACH BWIDTH_ms*0.05

#define STOP_SOUND (TCCR0A = 0b10000010)
#define START_SOUND (TCCR0A = 0b01000010)
#define C4 (uint8_t)FREQUENCY(261.63)
#define Cs4 (uint8_t)FREQUENCY(277.18)
#define Df4 Cs4
#define D4 (uint8_t)FREQUENCY(293.66)
#define Ds4 (uint8_t)FREQUENCY(311.13)
#define Ef4 Ds4
#define E4 (uint8_t)FREQUENCY(329.63)
#define Es4 F4
#define Ff4 E4
#define F4 (uint8_t)FREQUENCY(349.23)
#define Fs4 (uint8_t)FREQUENCY(369.99)
#define Gf4 Fs4
#define G4 (uint8_t)FREQUENCY(329)
#define Gs4 (uint8_t)FREQUENCY(415.3)
#define Af4 Gs4
#define A4 (uint8_t)FREQUENCY(440.)
#define As4 (uint8_t)FREQUENCY(466.16)
#define Bf4 As4
#define B4 (uint8_t)FREQUENCY(493.88)
#define Bs4 C5
#define C5 (uint8_t)FREQUENCY(523.25)
#define Cs5 (uint8_t)FREQUENCY(554.37)
#define Df5 Cs5
#define D5 (uint8_t)FREQUENCY(587.33)
#define Ds5 (uint8_t)FREQUENCY(622.25)
#define Ef5 Ds5
#define E5 (uint8_t)FREQUENCY(659.26)
#define Es5 F5
#define Ff5 E5
#define F5 (uint8_t)FREQUENCY(698.46)
#define Fs5 (uint8_t)FREQUENCY(739.99)
#define Gf5 Fs5
#define G5 (uint8_t)FREQUENCY(783.99)
#define Gs5 (uint8_t)FREQUENCY(830.61)
#define Af5 Gs5
#define A5 (uint8_t)FREQUENCY(880.)
#define As5 (uint8_t)FREQUENCY(932.33)
#define Bf5 As5
#define B5 (uint8_t)FREQUENCY(987.77)
#define Bs5 C6
#define Cf6 B5
#define C6 (uint8_t)FREQUENCY(1046.5)
#define Cs6 (uint8_t)FREQUENCY(1108.73)
#define Df6 Cs6
#define D6 (uint8_t)FREQUENCY(1174.66)
#define Ds6 (uint8_t)FREQUENCY(1244.51)
#define Ef6 Ds6

#define NOTE_1 4.
#define NOTE_1_D 6.
#define NOTE_2 2.
#define NOTE_2_D 3
#define NOTE_4 1.
#define NOTE_4_D 1.5
#define NOTE_8 0.5
#define NOTE_8_D 0.75
#define NOTE_16 0.25
#define NOTE_16_D 0.375

#define PLAY_NOTE_N(note, duration) ({\
    INT_PIN = note;\
    _delay_ms(BWIDTH_ms*duration);\
})

#define PLAY_NOTE_D(note, duration) ({\
    INT_PIN = note;\
    _delay_ms(BWIDTH_ms*duration - DETACH);\
    STOP_SOUND();\
    _delay_ms(DETACH);\
    START_SOUND();\
})

#define PLAY_NOTE_S(note, duration) ({\
    INT_PIN = note;\
    _delay_ms(DETACH);\
    STOP_SOUND;\
    _delay_ms(BWIDTH_ms*duration - DETACH);\
    START_SOUND;\
})

/*
Flag:
    <= 0 - Regular note
    = 1 - Detached notes (not staccato)
    >= 2 - Staccato
*/
#define PLAY_NOTE(note, duration, flag)({\
    INT_PIN = note;\
    if(flag <= 0){\
        _delay_ms(BWIDTH_ms*duration);\
    }else if(flag == 1){\
        _delay_ms(BWIDTH_ms*duration - DETACH);\
        STOP_SOUND;\
        _delay_ms(DETACH);\
        START_SOUND;\
    }else{\
        OCR0A = note;\
        _delay_ms(DETACH);\
        STOP_SOUND;\
        _delay_ms(BWIDTH_ms*duration - DETACH);\
        START_SOUND;\
    }\
})
