#ifndef I2C_LCD_LIB_H_
/*Librer�a I2C-LCD desarrollada por Kalun Lau.
Las principales funciones para el LCD fueron basados en una
libreria desarrollada por Sergio Salas para un PIC18F4550.
Las configuraciones del perif�rico I2C1 fueron basados en
un c�digo proporcionado por Alonso S�nchez.
Las funciones de inicializaci�n, env�o de dato y env�o de
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

/*Funci�n para inicializar el perif�rico I2C1*/
void I2C1_INIT(void);
/*Funci�n para escribir un solo dato de un byte a trav�s del
 perif�rico I2C1 donde:
 -direcci�n es la direcci�n del esclavo (sin incluir el
 *  bit R/W)
 -dato es el dato de ocho bits a enviar*/
void I2C1_WRITESINGLEBYTE(unsigned char direccion, unsigned char* dato, unsigned char len);

#endif	/* XC_HEADER_TEMPLATE_H */