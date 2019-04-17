/*
 * Micro-controller configuration
 */
#include <pic18f4550.h>
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF

/*
 * Port Definitions
 */
#define LCD_DATA PORTB
#define en PORTDbits.RD2
#define rs PORTDbits.RD0
#define RAIN PORTCbits.RC0

/*
 * Function definitions
 */
void delay (unsigned int);
void delayMicroseconds (unsigned int);
void init_LCD(void);
void tx_cmd(unsigned char);
void tx_byte(unsigned char);
void tx_str(unsigned char *);

void main()
{
    char msg1[] = "Unique Circuits";
    
    /*
     * GPIO direction configuration
     */
    TRISD = 0x00;
    TRISB = 0x00;
    TRISCbits.RC0=1;
    
    init_LCD();                                             // Initialize LCD in 5*7 matrix
    tx_str(msg1);                   
    while(1) {
        tx_cmd(0xc0);                                       // Set cursor to (0,2))
        if(RAIN == 0) {                                     // rain is detected when output of rain sensor is low
            tx_str("Rain Detected");
        } else {
            tx_str("                ");
        }
            delay(100);
    }
    return;
}

/*
 * Initialize sequence for LCD
 */
void init_LCD(void)
{
    tx_cmd(0x38);                                           // LCD in 5*7 matrix in 8  it mode
    delay(15);
    tx_cmd(0x01);                                           // Clear LCD
    delay(15);
    tx_cmd(0x0C);                                           // Display ON cursor OFF
    delay(15);
    tx_cmd(0x80);                                           // set cursor to )0,1)
    delay(15);
    return;
}

/*
 * Function for transmitting command to LCD
 */
void tx_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;                                         // Write data to port
    rs = 0;                                                 // select command register
    en = 1;                                                 // enable strobe signal
    delayMicroseconds(5);
    en = 0;
    return;
}

/*
 * Function for transmitting byte to LCD
 */
void tx_byte(unsigned char data)
{
    LCD_DATA = data;                                        // Write data to port
    rs = 1;                                                 // select data register
    en = 1;                                                 // enable strobe signal
    delayMicroseconds(5);
    en = 0;
    return;
}

/*
 * Function for transmitting a string to LCD
 */
void tx_str(unsigned char *str)
{
    int i = 0;
    while (str[i] != 0)                                     // loop till null character
    {
        tx_byte(str[i]);
        i++;
    }
    return;
}

/*
 * TImer0 based delay functions
 */
void delay (unsigned int ms)
{
    T0CON = 0x08;                                           // Timer0, OFF, 16 bit without pre-scalar
    while(ms--){                                            // Loop till required delay is generated
        TMR0H = 0xEC;                                       // Value loaded as per delay of 1mS
        TMR0L = 0x78;
        
        T0CONbits.TMR0ON = 1;                               // Turn on Timer0
        while(!INTCONbits.TMR0IF);                          // Checking for timer overflow flag
        INTCONbits.TMR0IF = 0;                              // Turn Off timer
        T0CONbits.TMR0ON=0;                                 // Clear overflow flag
    }
    return;
}
void delayMicroseconds(unsigned int us){
    T0CON = 0x08;                                           // Timer0, OFF, 16 bit without pre-scalar
    while(us--){                                            // Loop till required delay is generated
        TMR0H = 0xff;                                       // Value loaded as per delay of 1uS
        TMR0L = 0xfa;
        
        T0CONbits.TMR0ON = 1;                               // Turn on Timer0
        while(!INTCONbits.TMR0IF);                          // Checking for timer overflow flag
        INTCONbits.TMR0IF = 0;                              // Turn Off timer
        T0CONbits.TMR0ON=0;                                 // Clear overflow flag
    }
    return;
}
