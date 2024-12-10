#ifndef I2C_LCD_LIB_H_
/*Librería I2C-LCD desarrollada por Kalun Lau.
Las principales funciones para el LCD fueron basados en una
libreria desarrollada por Sergio Salas para un PIC18F4550.
Las configuraciones del periférico I2C1 fueron basados en
un código proporcionado por Alonso Sánchez.
Las funciones de inicialización, envío de dato y envío de
comando para el LCD se basaron en el trabajo de Vladimir 
Anglas.
Curso de Microcontroladores
Universidad Peruana de Ciencias Aplicadas
Ultima edicion 19/11/2023*/
#define	I2C_LCD_LIB_H_

#define OON  0
#define OFF 1
#define _XTAL_FREQ 32000000UL
#define  PIN_RS    0x01
#define  PIN_RW    0x02
#define  PIN_EN    0x04
#define  BACKLIGHT 0x08

/*Función para inicializar el periférico I2C1*/
void I2C1_INIT(void);
/*Función para escribir un solo dato de un byte a través del
 periférico I2C1 donde:
 -dirección es la dirección del esclavo (sin incluir el
 *  bit R/W)
 -dato es el dato de ocho bits a enviar*/
void I2C1_WRITESINGLEBYTE(unsigned char direccion, unsigned char* dato, unsigned char len);

#endif	/* XC_HEADER_TEMPLATE_H */