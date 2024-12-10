/*
 * File:   GUSWATCH
 * Author: Joseph
 *
 * Created on November 16, 2024, 8:33 PM
 */


#include "main.h"
#include "I2C_LCD.h"
#include "SSD1306.h"
#include "Text_Font.h"
#include "LIB_UART.h"
#include "cabecera.h"
uint16_t tramaserial[1122];
uint16_t indice=0;
    ssd1306_t SSD1306;
const uint8_t campal[]={0x10, 0xf0, 0x70, 0x78, 0x60, 0xc6, 0xce, 0xff, 0xce, 0xc6, 0x60, 0x78, 0x78, 0xf0, 0x10, 0x08, 
0x0f, 0x0e, 0x1e, 0x06, 0x63, 0x73, 0xff, 0x73, 0x63, 0x06, 0x1e, 0x0e, 0x0f, 0x08};

void configuro (void) {
    I2C1_INIT();
    OSCCON1 = 0x60;
    OSCFRQ =0x06;
    OSCEN=0x40;
    U1_INIT(16);
    TRISEbits.TRISE0=0;
    ANSELEbits.ANSELE0=0;
    TRISFbits.TRISF1=0;
    ANSELFbits.ANSELF1=0;
    PIE4bits.U1RXIE = 1;
    PIR4bits.U1RXIF = 0;
    INTCON0bits.GIE = 1;

    SSD1306.address = SSD1306_ADDR;
    SSD1306.width= SSD1306_WIDTH ;
    SSD1306.height = SSD1306_HEIGHT;
    SSD1306.color = SSD1306_WHITE;
    SSD1306.contrast = 0xCF;
    SSD1306.left = SSD1306_LEFT;
    SSD1306.right = SSD1306_RIGHT;
    SSD1306.center = SSD1306_CENTER;
    SSD1306_I2C_Init (&SSD1306);
}

void main(){
    configuro();
    while(1){
    SSD1306_I2C_SetFont( &SSD1306, BIG_FONTS);
    SSD1306_I2C_Cadena( &SSD1306,0,0,"GUSWATCH");
    SSD1306_I2C_Animation(&SSD1306,campal,15,16);
    SSD1306_I2C_Update( &SSD1306);

    }
}










void __interrupt(irq(IRQ_U1RX)) U1RX_ISR(void){
    PIR4bits.U1RXIF = 0;
    // Primero guardar el byte recibido
    tramaserial[indice] = U1RXB;
    if(tramaserial[indice] == 0xAA){
        indice = 0;
    } else {
        indice++;
    }
}