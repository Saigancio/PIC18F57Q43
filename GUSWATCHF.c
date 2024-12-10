/*
 * File:   GUSWATCHF.c
 * Author: Joseph
 *
 * Created on 30 de noviembre de 2024, 05:54 PM
 */

//Proyecto Final hecho para el curso de Taller de Microcontroladores 2024-2 del Ing. Kalun
//Creditos a la libreria de Wells (SSD1306) que ha sido modificada, a�adiendo la funcion "Animation" y la correcci�n de bugs.
//Se ha usado c�digos del repositorio del Ing. Kalun (tocache)
//Tambien a�adir el uso de la IA Claude para la correcci�n de bugs menores y dem�s
//Este proyecto se actualizar� constantemente usando git con la finalidad de explorar las capacidades del PIC18F57Q43

#include <stdio.h>
#include "main.h"
#include "I2C_LCD.h"
#include "SSD1306.h"
#include "Text_Font.h"
#include "LIB_UART.h"
#include "cabecera.h"
uint16_t tramaserial[1200];
uint16_t indice=0;
ssd1306_t SSD1306;
unsigned char pantalla=0;
unsigned char temperatura;
unsigned char horas = 0;
unsigned char minutos = 0;
unsigned char segundos = 0;
unsigned char ticks = 0;
const char* semanat = "Lorem";
const char* mes = "Ipsum";
unsigned char dia = 1;
    unsigned char alahor=0;
    unsigned char alamin=0;
    unsigned char alaseg=0;
const uint8_t chatgpt[]={0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xf0, 0x3c, 0x0c, 0x06, 0x87, 0x83, 0xc3, 0xc3, 0x63, 
0x73, 0x36, 0x3e, 0x1c, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x38, 0x30, 0xf0, 0xc0, 0x80, 0x00, 0x00, 
0xf8, 0xfe, 0x07, 0x03, 0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xb5, 0x11, 0x18, 0x08, 
0x0c, 0x1e, 0x16, 0xb3, 0xe3, 0x63, 0xc7, 0x86, 0x8c, 0x1c, 0x18, 0x38, 0x7f, 0xff, 0xc0, 0x00, 
0x00, 0x03, 0xff, 0xfe, 0x1c, 0x18, 0x38, 0x31, 0x61, 0xe3, 0xc6, 0xc7, 0xce, 0x68, 0x78, 0x30, 
0x10, 0x18, 0x88, 0xad, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x80, 0xc0, 0xe0, 0x7f, 0x1f, 
0x00, 0x00, 0x01, 0x03, 0x07, 0x0c, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x38, 0x38, 0x7c, 0x6c, 0xce, 
0xc6, 0xc3, 0xc3, 0xc1, 0xe1, 0x60, 0x30, 0x3c, 0x0f, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00};
const uint8_t campal[]={0x38, 0x06, 0xf1, 0x0c, 0x02, 0x03, 0x01, 0x01, 0x01, 0x03, 0x02, 0x0c, 0xfb, 0x06, 0x38, 0x00, 
0x18, 0x27, 0x20, 0x20, 0x60, 0xa0, 0xa0, 0xa0, 0x60, 0x20, 0x20, 0x27, 0x18, 0x00};
const uint8_t cronometro[]={0x00, 0xc0, 0x60, 0x30, 0x18, 0x08, 0x0d, 0x0f, 0x0d, 0x08, 0x18, 0x38, 0x6c, 0xc0, 0x00, 0x00, 
0x1f, 0x3a, 0x70, 0x40, 0xc0, 0xc0, 0xc3, 0xc0, 0xc0, 0x40, 0x70, 0x3a, 0x1f, 0x00};
void configuro (void) {
    I2C1_INIT();
    OSCCON1 = 0x60;
    OSCFRQ =0x06;
    OSCEN=0x40;
    U1_INIT(16);
    TRISEbits.TRISE0=0;
    ANSELEbits.ANSELE0=0;
    TRISFbits.TRISF1=1;
    ANSELFbits.ANSELF1=0;
    PIE4bits.U1RXIE = 1;
    PIR4bits.U1RXIF = 0;
    INTCON0bits.GIE = 1;
    //conf del Timer1 y del CCP1
    T1CLK = 0x01;
    T1CON = 0x33;
    CCP1CON = 0x81;     //modo comparador con rst a TMR1
    CCPR1H = 0xF4;
    CCPR1L = 0x24;      //valor de ref de comp 62500
    //conf de la interrupcion
    PIE3bits.CCP1IE = 1;    //habilito CCP1IE
    PIR3bits.CCP1IF = 0;

    SSD1306.address = SSD1306_ADDR;
    SSD1306.width= SSD1306_WIDTH ;
    SSD1306.height = SSD1306_HEIGHT;
    SSD1306.color = SSD1306_WHITE;
    SSD1306.contrast = 0xCF;
    SSD1306.left = SSD1306_LEFT;
    SSD1306.right = SSD1306_RIGHT;
    SSD1306.center = SSD1306_CENTER;
    SSD1306_I2C_Init (&SSD1306);
    TRISEbits.TRISE1=1;    // Configurar como entrada
    ANSELEbits.ANSELE1=0;  // Modo digital
    WPUEbits.WPUE1 = 1;    // Pull-up habilitado
    TRISBbits.TRISB4=1;    // Configurar como entrada
    ANSELBbits.ANSELB4=0;  // Modo digital
    WPUBbits.WPUB4 = 1;    // Pull-up habilitado
    TRISEbits.TRISE0=1;    // Configurar como entrada
    ANSELEbits.ANSELE0=0;  // Modo digital
    WPUEbits.WPUE0 = 1;    // Pull-up habilitado
}

float conversor_temperatura(void){
    int centena, decena, unidad, decima, centesima;
    centena=tramaserial[13]-0x30;
    decena=tramaserial[14]-0x30;
    unidad=tramaserial[15]-0x30;
    decima=tramaserial[16]-0x30;
    centesima=tramaserial[17]-0x30;
    float kelvin;
    kelvin=centena*100+decena*10+unidad+decima*0.1+centesima*0.01;
    float celsius;
    celsius=kelvin-273.15;
    return celsius;
}

int conversor_milibar(void){
    int milb,cenb,decb,unib;
    milb=tramaserial[31]-0x30;
    cenb=tramaserial[32]-0x30;
    decb=tramaserial[33]-0x30;
    unib=tramaserial[34]-0x30;
    int milibar;
    milibar=milb*1000+cenb*100+decb*10+unib*1;
    return milibar;
}

int conversor_humedad(void){
    int humd,humu;
    humd=tramaserial[47]-0x30;
    humu=tramaserial[48]-0x30;
    int humedad;
    humedad=humd*10+humu;
    return humedad;
}

void sincronizador(void) {
    uint16_t i;
    uint16_t suma_semana = 0;
    uint16_t suma_mes = 0;
    bool hora_encontrada = false;
    bool dia_encontrado = false;
    bool num_encontrado = false;
    bool mes_encontrado = false;
    // Primero buscamos la hora
    for(i = 0; i < indice; i++) {
        // Buscar patr�n de a�o
        if(tramaserial[i] == 'Y' && 
           tramaserial[i+1] == 'e' && 
           tramaserial[i+2] == 'a' &&
           tramaserial[i+3] == 'r' &&
           tramaserial[i+4] == ':' &&
           tramaserial[i+5] == ' '){
            horas = (tramaserial[i+46] - 0x30) * 10 + (tramaserial[i+47] - 0x30);
            minutos = (tramaserial[i+58] - 0x30) * 10 + (tramaserial[i+59] - 0x30);
            segundos = (tramaserial[i+70] - 0x30) * 10 + (tramaserial[i+71] - 0x30);
            hora_encontrada = true;
        }
        
        // Buscar patr�n de d�a de la semana
        if(tramaserial[i] == 'D' && 
           tramaserial[i+1] == 'a' && 
           tramaserial[i+2] == 'y' &&
           tramaserial[i+3] == ' ' &&
           tramaserial[i+4] == 'o' &&
           tramaserial[i+5] == 'f' &&
           tramaserial[i+6] == ' ' &&
           tramaserial[i+7] == 'w' &&
           tramaserial[i+8] == 'e' &&
           tramaserial[i+9] == 'e' &&
           tramaserial[i+10] == 'k' &&
           tramaserial[i+11] == ':') {
            
            suma_semana = tramaserial[i+13] +   // Primera letra
                         tramaserial[i+14] +     // Segunda letra
                         tramaserial[i+15];      // Tercera letra
            
            switch(suma_semana) {
                case 0x12A:  // Mon
                    semanat = "Lun";
                    break;
                case 0x12E:  // Tue
                    semanat = "Mar";
                    break;
                case 0x120:  // Wed
                    semanat = "Mie";
                    break;
                case 0x131:  // Thu
                    semanat = "Jue";
                    break;
                case 0x121:  // Fri
                    semanat = "Vie";
                    break;
                case 0x128:  // Sat
                    semanat = "Sab";
                    break;
                case 0x136:  // Sun
                    semanat = "Dom";
                    break;
                default:
                    semanat = "Error";
                    break;
            }
            dia_encontrado = true;
        }
        if(tramaserial[i] == 'Y' && 
           tramaserial[i+1] == 'e' && 
           tramaserial[i+2] == 'a' &&
           tramaserial[i+3] == 'r' &&
           tramaserial[i+4] == ':' &&
           tramaserial[i+5] == ' '){
            horas = (tramaserial[i+45] - 0x30) * 10 + (tramaserial[i+46] - 0x30);
            minutos = (tramaserial[i+57] - 0x30) * 10 + (tramaserial[i+58] - 0x30);
            segundos = (tramaserial[i+69] - 0x30) * 10 + (tramaserial[i+70] - 0x30);
            hora_encontrada = true;
        }
        
        // Buscar patr�n de d�a de la semana
        if(tramaserial[i] == 'D' && 
                tramaserial[i+1] == 'a' && 
                tramaserial[i+2] == 'y' &&
                tramaserial[i+3] == ' ' &&
                tramaserial[i+4] == 'o' &&
                tramaserial[i+5] == 'f' &&
                tramaserial[i+6] == ' ' &&
                tramaserial[i+7] == 'M' &&
                tramaserial[i+8] == 'o' &&
                tramaserial[i+9] == 'n' &&
                tramaserial[i+10] == 't' &&
                tramaserial[i+11] == 'h' &&
                tramaserial[i+12] == ':' &&
                tramaserial[i+13] == ' ') 
        {
            dia = (tramaserial[i+14]-0x30)*10 + (tramaserial[i+15]-0x30);
            num_encontrado = true;
        }
        
        if(tramaserial[i]=='L' &&
                tramaserial[i+1] == 'M' && 
           tramaserial[i+2] == 'o' && 
           tramaserial[i+3] == 'n' &&
           tramaserial[i+4] == 't' &&
           tramaserial[i+5] == 'h' &&
           tramaserial[i+6] == ':') {
            
            suma_mes = tramaserial[i+8]+tramaserial[i+9]+tramaserial[i+10];
            switch(suma_mes) {
                case 0x119: mes = "Enero"; break;    // Jan
                case 0x10D: mes = "Febrero"; break; // Feb
                case 0x120: mes = "Marzo"; break;   // Mar
                case 0x123: mes = "Abril"; break;   // Apr
                case 0x127: mes = "Mayo"; break;    // May
                case 0x12D: mes = "Junio"; break;   // Jun
                case 0x12B: mes = "Julio"; break;   // Jul
                case 0x11D: mes = "Agosto"; break;  // Aug
                case 0x128: mes = "Septiembre"; break; // Sep
                case 0x126: mes = "Octubre"; break; // Oct
                case 0x133: mes = "Noviembre"; break; // Nov
                case 0x10C: mes = "Diciembre"; break; // Dec
                default: mes = "Error"; break;
}
            mes_encontrado = true;
        }        
        // Si encontramos ambos, podemos salir
        if(hora_encontrada && dia_encontrado && num_encontrado && mes_encontrado) {
            return;
        }
    }
}


void menu_pri(void){

    char str_horas[3], str_minutos[3], str_segundos[3],str_dia[3];
sprintf(str_horas, "%02d", horas);
sprintf(str_minutos, "%02d", minutos);
sprintf(str_segundos, "%02d", segundos);
sprintf(str_dia,"%02d",dia);
    SSD1306_I2C_SetFont( &SSD1306, SOVIET_FONTS);
    SSD1306_I2C_Update( &SSD1306);
    SSD1306_I2C_ClearDisplay(&SSD1306);
    SSD1306_I2C_Cadena(&SSD1306,30,10,str_horas);
    SSD1306_I2C_Caracter(&SSD1306,50,10,':');
    SSD1306_I2C_Cadena(&SSD1306,60,10,str_minutos);
    SSD1306_I2C_Caracter(&SSD1306,80,10,':');
    SSD1306_I2C_Cadena(&SSD1306,90,10,str_segundos);
    SSD1306_I2C_Cadena(&SSD1306,10,30,semanat);
    SSD1306_I2C_Caracter(&SSD1306,70,30,',');
    SSD1306_I2C_Cadena(&SSD1306,80,30,str_dia);
    SSD1306_I2C_SetFont( &SSD1306, SMALL_FONTS);
    SSD1306_I2C_Cadena(&SSD1306,10,50,mes);
}
void menu_wea(void){
        char temp_str[10];  // Buffer para guardar la cadena de temperatura
    float temp = conversor_temperatura();  // Obtener la temperatura
    // Convertir el float a string con dos decimales
    sprintf(temp_str, "%.2f", temp);
    char milib_str[10];  // Buffer para guardar la cadena de temperatura
    int milib = conversor_milibar();  // Obtener la temperatura
    sprintf(milib_str, "%d", milib);
    char hum_str[10];  // Buffer para guardar la cadena de temperatura
    int hum= conversor_humedad();  // Obtener la temperatura
    sprintf(hum_str, "%d", hum);
    SSD1306_I2C_Update( &SSD1306);
    SSD1306_I2C_ClearDisplay(&SSD1306);
    SSD1306_I2C_SetFont( &SSD1306, SMALL_FONTS);
    SSD1306_I2C_Cadena( &SSD1306, 0,15,"T:");
    SSD1306_I2C_Cadena( &SSD1306,10,15,temp_str);    
    SSD1306_I2C_Cadena( &SSD1306, 60,15,"C");
    SSD1306_I2C_Caracter( &SSD1306, 64,15,'�');
    SSD1306_I2C_Cadena( &SSD1306, 0,30,"P:");
    SSD1306_I2C_Cadena(&SSD1306, 10,30,milib_str);
    SSD1306_I2C_Cadena( &SSD1306, 40,30,"mb");
    SSD1306_I2C_Cadena( &SSD1306, 0,45,"H:");
    SSD1306_I2C_Cadena(&SSD1306, 10,45,hum_str);
    SSD1306_I2C_Caracter( &SSD1306, 38,45,'%');
    SSD1306_I2C_Cadena( &SSD1306, 44,45,"RC");

}
void menu_ala(){

    if(PORTEbits.RE1==0){
        while(PORTEbits.RE1==1);
        if(PORTEbits.RE1==0){
        alahor++;
        if(alahor==24){
            alahor=0;
        }
        }
    }
    if(PORTEbits.RE0==0){
        while(PORTEbits.RE0==1);
        if(PORTEbits.RE0==0){
        alamin++;
        if(alamin==60){
            alamin=0;
        }
        }
    }
    char str_alahor[3], str_alamin[3], str_alaseg[3],str_dia[3];
    sprintf(str_alahor, "%02d", alahor);
    sprintf(str_alamin, "%02d", alamin);

    sprintf(str_alaseg, "%02d", alaseg);
    SSD1306_I2C_SetFont( &SSD1306, SOVIET_FONTS);
    SSD1306_I2C_Update( &SSD1306);
    SSD1306_I2C_ClearDisplay(&SSD1306);
    SSD1306_I2C_Cadena(&SSD1306,24,10,str_alahor);
    SSD1306_I2C_Caracter(&SSD1306,44,10,':');
    SSD1306_I2C_Cadena(&SSD1306,54,10,str_alamin);
    SSD1306_I2C_Caracter(&SSD1306,74,10,':');
    SSD1306_I2C_Cadena(&SSD1306,84,10,str_alaseg);
        if(alamin==minutos && alahor==horas){
        SSD1306_I2C_Cadena(&SSD1306,10,40,'Alarma');
    }
    return;
}
void men_op(void){
    SSD1306_I2C_ClearDisplay(&SSD1306);
    SSD1306_I2C_SetFont( &SSD1306, SMALL_FONTS);
    SSD1306_I2C_Cadena( &SSD1306,20,5,"Alarma");
    SSD1306_I2C_Imagen(&SSD1306,campal,0,0,15,16);
    SSD1306_I2C_Cadena( &SSD1306,20,20,"Cronometro");
    SSD1306_I2C_Imagen(&SSD1306,cronometro,0,20,15,16);
    SSD1306_I2C_Cadena( &SSD1306,20,35,"Sincronizado");
    SSD1306_I2C_Imagen(&SSD1306,cronometro,0,40,15,16);
    SSD1306_I2C_Update( &SSD1306);
}



void main(void){
    configuro();
    for(int start=0; start<2;start++){
    SSD1306_I2C_SetFont( &SSD1306, BIG_FONTS);
    SSD1306_I2C_Cadena( &SSD1306,0,0,"GUSWATCH");
    SSD1306_I2C_Imagen(&SSD1306,chatgpt,48,30,32,32);
    SSD1306_I2C_Update( &SSD1306);
    __delay_ms(1000);
    SSD1306_I2C_ClearDisplay(&SSD1306);
    __delay_ms(1000);

    __delay_ms(1000);
    sincronizador();

    }
    while(1){
        if(PORTBbits.RB4==0){
            while(PORTBbits.RB4==1);
            if(PORTBbits.RB4==0){
            pantalla++;
            if(pantalla==4){
                pantalla=0;
            }
        }
        }
        switch (pantalla){
            case 0:
                menu_pri();
                break;
            case 1:
                menu_wea();
                break;
            case 2:
                menu_ala();
                break;
            case 3:
                men_op();
                break;
        }
        if(alamin==minutos && alahor==horas){
            LATBbits.LATB0=1;
            
            
        }
    }
}
void __interrupt(irq(IRQ_CCP1)) CCP1_ISR(void){
    PIR3bits.CCP1IF = 0;
    if(ticks == 15){
        ticks = 0;
        if(segundos == 59){
            segundos = 0;
            if(minutos == 59){
                minutos = 0;
                if(horas == 23){
                    horas = 0;
                }
                else{
                    horas++;
                }
            }
            else{
                minutos++;
            }
        }
        else{
            segundos++;
        }
    }
    else{
        ticks++;
    }
}

void __interrupt(irq(default)) default_ISR(void){
    
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
