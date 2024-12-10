#ifndef INC_SSD__H_
#define	INC_SSD__H_

/* DEFINIR EL MICROCONTROLADOR A USAR */
#if defined(__XC8)
    #include <xc.h>
    #include "main.h"
    #include "cabecera.h"
    #include "I2C_LCD.h"
    #define delay_ms(x)  __delay_ms(x)
    #define delay_us(x)  __delay_us(x)
#elif defined(__XC16)
    #include <xc.h>
    #include "Configuracion.h"
    #define delay_ms(x)  __delay_ms(x)
    #define delay_us(x)  __delay_us(x)
    #include "I2C.h"
#elif defined(__STM32F4)
    #include "main.h"
    #include "SysTick.h"
    #include "I2C.h"
    #define delay_ms(x)  Delay_ms(x)
#elif defined(USE_HAL_DRIVER)
    #include "i2c.h"
    #include "main.h"
    extern I2C_HandleTypeDef hi2c1;
    #define delay_ms(x) HAL_Delay(x)
#endif

#define SSD1306_COMMAND_LEN                             2
#define SSD1306_DATA_LEN                                17

typedef enum
{
  SSD1306_BLACK,
  SSD1306_WHITE,
  SSD1306_INVERSE
}color_t;

typedef struct
{
  const uint8_t *font;        
  uint8_t x_size;
  uint8_t y_size;
  uint8_t offset;
  uint8_t numchars;
  uint8_t inverted;
} Font_t;

typedef struct
{
    uint8_t height;
    uint8_t width;
    uint16_t right;
    uint16_t left;
    uint16_t center;
    uint8_t address;
    uint8_t cmd[SSD1306_COMMAND_LEN];
    uint8_t data[SSD1306_DATA_LEN];
    uint8_t contrast;
    color_t color;
    Font_t Font;
}ssd1306_t;

/* SSD1306 Display Type*/
#define SSD1306_128_64
//#define SSD1306_128_32
// #define SSD1306_96_16

#if defined SSD1306_128_64
  #define SSD1306_WIDTH                                 128
  #define SSD1306_HEIGHT                                64
#endif
#if defined SSD1306_128_32
  #define SSD1306_WIDTH                                 128
  #define SSD1306_HEIGHT                                32
#endif
#if defined SSD1306_96_16
  #define SSD1306_WIDTH                                 96
  #define SSD1306_HEIGHT                                16
#endif

/* I2C COMMAND TABLE */
#define SSD1306_COMMAND                                 0x00
#define SSD1306_DATA                                    0xC0
#define SSD1306_DATA_CONTINUE                           0x40
#define SSD1306_ADDR                                    0x3C

/* DIMENSION SSD1306 */
#define SSD1306_LEFT                                    0
#define SSD1306_RIGHT                                   254
#define SSD1306_CENTER                                  255

/* FUNDAMENTAL COMMAND TABLE */
#define SSD1306_CONTRAST_CONTROL                        0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME                   0xA4
#define SSD1306_DISPLAY_ALL_ON                          0xA5
#define SSD1306_DISPLAY_NORMAL                          0xA6
#define SSD1306_DISPLAY_INVERT                          0xA7
#define SSD1306_DISPLAY_OFF                             0xAE
#define SSD1306_DISPLAY_ON                              0xAF

/* SCROLLING COMMAND TABLE */
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_VERTICAL_SCROLL_AREA                    0xA3

/* ADDRESSING SETTING COMMAND TABLE */
#define SSD1306_LOWER_COLUMN_START                      0x00
#define SSD1306_HIGHER_COLUMN_START                     0x10
#define SSD1306_MEMORY_ADDRESS_MODE                     0x20
#define SSD1306_COLUMN_ADDRESS                          0x21
#define SSD1306_PAGE_ADDRESS                            0x22
#define SSD1306_PAGE_START_ADDRESS                      0xB0

/* HARDWARE CONFIGURATION COMMAND TABLE */
#define SSD1306_DISPLAY_START_LINE                      0x40
#define SSD1306_SEGMENT_REMAP                           0xA0
#define SSD1306_MULTIPLEX_RATIO                         0xA8
#define SSD1306_COM_SCAN_DIRECTION_INC                  0xC0
#define SSD1306_COM_SCAN_DIRECTION_DEC                  0xC8
#define SSD1306_DISPLAY_OFFSET                          0xD3
#define SSD1306_HARDWARE_PINS_CONFIG                    0xDA
#define SSD1306_CHARGE_PUMP                             0x8D

/* TIMING & DRIVING SCHEME SETTING COMMAND TABLE */
#define SSD1306_CLOCK_DIV_RATIO                         0xD5
#define SSD1306_PRECHARGE_PERIOD                        0xD9
#define SSD1306_VCOM_DESELECT                           0xDB
#define SSD1306_NOP                                     0xE3


/*Memory Buffer Display*/
static uint8_t buffer[SSD1306_HEIGHT * SSD1306_WIDTH / 8] =
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#if (SSD1306_HEIGHT * SSD1306_WIDTH > 96*16)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#if (SSD1306_HEIGHT == 64)
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
#endif
};

/*Function Prototypes*/
void SSD1306_I2C_Init( ssd1306_t * SSD1306 );

void SSD1306_I2C_Update( ssd1306_t * SSD1306  );

void SSD1306_I2C_SetContrast( ssd1306_t * SSD1306, uint8_t contrast );

void SSD1306_I2C_ClearDisplay( ssd1306_t * SSD1306 );

void SSD1306_I2C_FillDisplay( ssd1306_t * SSD1306 );

void SSD1306_I2C_DrawPixel( ssd1306_t * SSD1306, int16_t x, int16_t y );

void SSD1306_I2C_InvertDisplay( ssd1306_t * SSD1306, uint8_t value );

void SSD1306_I2C_SetFont( ssd1306_t * SSD1306 , const uint8_t *font);

void SSD1306_I2C_InvertFont( ssd1306_t * SSD1306, uint8_t invert_status );

void SSD1306_I2C_Linea( ssd1306_t * SSD1306, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end);

void SSD1306_I2C_LineaVertical( ssd1306_t * SSD1306, int16_t y_start, int16_t y_end, int16_t x_pos);

void SSD1306_I2C_LineaHorizontal( ssd1306_t * SSD1306, int16_t x_start, int16_t x_end, int16_t y_pos);

void SSD1306_I2C_Rectangulo( ssd1306_t * SSD1306, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

void SSD1306_I2C_RellenoRectangulo( ssd1306_t * SSD1306, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

void SSD1306_I2C_Triangulo( ssd1306_t * SSD1306, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

void SSD1306_I2C_Circulo( ssd1306_t * SSD1306, int16_t x0, int16_t y0, int16_t r);

void SSD1306_I2C_Imagen( ssd1306_t * SSD1306, const uint8_t *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height );

void SSD1306_I2C_Caracter( ssd1306_t * SSD1306, int16_t x, int16_t y, char value );

void SSD1306_I2C_Cadena( ssd1306_t * SSD1306, int16_t x, int16_t y, char *text);

void SSD1306_I2C_Animation(ssd1306_t * SSD1306, const uint8_t *image, uint8_t width, uint8_t height);

#endif	/* SSD1306_H */