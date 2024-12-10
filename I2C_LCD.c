//Librería I2C-LCD desarrollada por Kalun Lau
//Las principales funciones para el LCD fueron basados en una libreria
//desarrollada por Sergio Salas para un PIC18F4550
//Las configuraciones del periférico I2C1 fueron basados en un código
//proporcionado por Alonso Sánchez
//Las funciones de inicialización, envío de dato y envío de comando para el LCD
//se basaron en el trabajo de Vladimir Anglas
//Curso de Microcontroladores
//Universidad Peruana de Ciencias Aplicadas
//Ultima edicion 19/11/2023

#include <xc.h>
#include "I2C_LCD.h"
#include <string.h>

void I2C1_INIT(void){
    //configuracion del I2C
    TRISCbits.TRISC3 = 0; // outputs
    TRISCbits.TRISC4 = 0;
    ANSELCbits.ANSELC3 = 0; // digitales
    ANSELCbits.ANSELC4 = 0;
    ODCONCbits.ODCC3 = 1; // open drain
    ODCONCbits.ODCC4 = 1;
    // COnfiguracion de PPS para I2C (outputs en RC3, RC4)    
    RC3PPS = 0x37; // I2C1 SCL
    RC4PPS = 0x38; // I2C1 SDA
    // Configuracion de puerto C para i2c    
    RC3I2Cbits.PU = 2; // RC3I2C, PU = "10". 10x current of standard weak pull-up
    RC3I2Cbits.TH = 1; // RC3I2C, TH = "01". I2C-specific input thresholds
    RC4I2Cbits.PU = 2; // RC4I2C, PU = "10". 10x current of standard weak pull-up
    RC4I2Cbits.TH = 1; // RC4I2C, TH = "01". I2C-specific input thresholds    
    I2C1CON0bits.EN = 1; // habilitar modulo I2C
    I2C1CLKbits.CLK = 3; // MFINTOSC (500 kHz), lo cual genera un SCL a 100 KHz por defecto
    I2C1CON0bits.MODE = 0x4;            //I2C host mode 7bit addressing
    I2C1CON2bits.ABD = 0;               //I2C con registros independientes para Addr y Data    
}

void I2C1_WRITESINGLEBYTE(unsigned char direccion, unsigned char* dato, unsigned char len){
    unsigned char buffer=1;
    I2C1CNT = len;
    I2C1ADB1 = (direccion << 1) & 0xFE;     //slave addr + 0 (write)
    I2C1TXB = dato[0];                     //colocamos el dato a enviar
    I2C1CON0bits.S = 1; // START condition
    while(I2C1CON0bits.S);
    while(I2C1STAT0bits.MMA)
    {
        if(I2C1CON0bits.MDR)
        {
            if(I2C1STAT1bits.TXBE)
            {
                if(buffer>=len)
                {
                    I2C1TXB=0x00;
                }
                else{
                I2C1TXB=dato[buffer];
                buffer++;
                }
            }
        }
    }
    while(I2C1STAT1bits.TXBE == 0); // espera a que el buffer TX este vacio
    while(I2C1CON1bits.ACKSTAT == 1); // Espera a recibir la rpta del cliente (ACK))
    while(I2C1PIRbits.PCIF == 0); //Espera a terminar STOP condition
}
