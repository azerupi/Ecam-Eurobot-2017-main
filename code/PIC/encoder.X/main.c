/*
 * File:   main.c
 * Author: Mathieu David
 *
 * Created on March 20, 2017, 7:02 PM
 */


#include <xc.h>

// ----------------------
// Hardware Configuration
// ----------------------
// Datasheet p. 268

// OSC frequency for delay functions
#define _XTAL_FREQ 16000000 // 16MHz

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (clock frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switchover bit (Internal External Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled)
// BORV = No Setting

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDPS = 32768     // Watchdog Timer Postscale Select bits (1:32768)
#pragma config WINEN = OFF      // Watchdog Timer Window Enable bit (WDT window disabled)

// CONFIG3L
#pragma config PWMPIN = OFF     // PWM output pins Reset state control (PWM outputs disabled upon Reset (default))
#pragma config LPOL = HIGH      // Low-Side Transistors Polarity (PWM0, 2, 4 and 6 are active-high)
#pragma config HPOL = HIGH      // High-Side Transistors Polarity (PWM1, 3, 5 and 7 are active-high)
#pragma config T1OSCMX = ON     // Timer1 Oscillator MUX (Low-power Timer1 operation when microcontroller is in Sleep mode)

// CONFIG3H
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-000FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (001000-001FFF) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-000FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (001000-001FFF) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-000FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (001000-001FFF) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from table reads executed in other blocks)




// -----------------------
// Function Prototypes
// -----------------------
void interrupt low_priority low_isr(void);
void interrupt high_isr(void);

void flash_leds(char n);




// -----------------------
// State variables
// -----------------------
int rotation = 0;
int velocity = 0;

void main() {
    // Interrupts configuration
    RCONbits.IPEN = 1; // Enable interrupt priority
    INTCONbits.GIEH = 1; // Enable high priority interrupts
    INTCONbits.GIEL = 1; // Enable low priority interrupts
    
    // SPI configuration
    SSPCONbits.WCOL = 0; // No collision
    SSPCONbits.SSPOV = 0; // No overflow
    SSPCONbits.SSPEN = 1; // configures SCK, SDO and SDI as serial port pins
    SSPCONbits.CKP = 0; // SPI set idle state for clock at low level
    SSPCONbits.SSPM = 0b0100; // SPI Slave mode, clock = SCK pin. SS pin control enabled. 
    SSPSTATbits.CKE = 1; // SPI clock edge select, data transmitted on rising edge of SCK
    TRISCbits.TRISC4 = 1; // SDI must have TRISC<4> set
    TRISCbits.TRISC5 = 0; // SDO must have TRISC<5> cleared
    TRISCbits.TRISC3 = 1; // SCK (Slave mode) must have TRISC<3> set
    // TRISAbits.TRISA5 = 1; DOES NOT EXIST??? 
    TRISA = TRISA | 0b00100000; // SS must have TRISA<5> set (workaround)
    
    PIR1bits.SSPIF = 0; // Set the SPI interruption flag to zero (needed in setup?)

    // QEI module configuration
    QEICONbits.VELM = 1; // Velocity mode enabled
    QEICONbits.ERROR = 0; // Disable overflow / underflow error
    QEICONbits.QEIM = 0b001; // MODE: Two clocks per QEA pulse, INDX resets POSCNT.

    // QEI Interrupts
    PIE3bits.IC2QEIE = 1; // Enable QEI interrupts
    IPR3bits.IC2QEIP = 0; // Set QEI interrupts to low priority

    // Status LEDs configuration
    TRISBbits.RB1 = 0; // Set RB1 as output for the green led
    TRISBbits.RB2 = 0; // Set RB2 as output for the red led

    // Button configuration
    TRISBbits.RB0 = 1; // Set RB0 as the input for the button
    INTCONbits.RBIE = 1; // Enable interrupts on the RB ports
    INTCON2bits.RBIP = 0; // Set RB interrupts to low priority

    // Flash LEDs 3 times to show the controller is working
    flash_leds(3, 400);

    // -----------------------
    // Main routine
    // -----------------------
    while (1) {

    }
}

// -----------------------
// Interrupt routines
// -----------------------

void interrupt high_isr(void) {

}

void interrupt low_priority low_isr(void) {
    if (IC2QEIF) {
        if (QEICONbits.UPDOWN) {
            rotation++;
        } else {
            rotation--;
        }

        IC2QEIF = 0;
    } else if (INTCONbits.RBIF) {
        
    }
}



// Function to make the LEDs blink

void flash_leds(char n, int period) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            __delay_ms(period/2);
        }
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        __delay_ms(period/2);
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 0;
    }
}