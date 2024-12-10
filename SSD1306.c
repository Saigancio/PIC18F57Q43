#include "SSD1306.h"
#include "Text_Font.h"

static void SSD1306_I2C_SendCommand( ssd1306_t * SSD1306 , uint8_t command )
{
  SSD1306->cmd[0] = 0x00;
  SSD1306->cmd[1] = command;
#if defined(__XC8)
    #if defined(_18F57Q43)
        I2C1_WRITESINGLEBYTE(SSD1306->address,SSD1306->cmd, SSD1306_COMMAND_LEN);
    #endif
#elif defined(__XC16)
    I2C_Start();
    I2C_Tx(SSD1306->address);
    I2C_Tx(SSD1306->cmd[0]);
    I2C_Tx(SSD1306->cmd[1]);
    I2C_Stop();
#elif defined(__STM32F4)
    I2C_masterTransmit(SSD1306->address, SSD1306->cmd, SSD1306_COMMAND_LEN, 100);
#elif defined(USE_HAL_DRIVER)
  HAL_I2C_Master_Transmit(&hi2c1, (SSD1306->address)<<1, SSD1306->cmd, SSD1306_COMMAND_LEN, 10);
#endif
}

static void SSD1306_I2C_SendData( ssd1306_t * SSD1306 )
{
  SSD1306->data[0] = 0x40;
  #if defined(__XC8)
    #if defined(_18F57Q43)
        I2C1_WRITESINGLEBYTE(SSD1306->address,SSD1306->data, SSD1306_DATA_LEN);
    #endif
#elif defined(__XC16)
    I2C_Start();
    I2C_Tx(SSD1306->address);
    I2C_Tx(SSD1306->data[0]);
    I2C_Tx(SSD1306->data[1]);
    I2C_Tx(SSD1306->data[2]);
    I2C_Tx(SSD1306->data[3]);
    // ... rellenar hasta 16
    I2C_Tx(SSD1306->data[16]);
    I2C_Stop();
#elif defined(__STM32F4)
    I2C_masterTransmit(SSD1306->address, SSD1306->data, SSD1306_DATA_LEN, 100);
#elif defined(USE_HAL_DRIVER)
    HAL_I2C_Master_Transmit(&hi2c1, (SSD1306->address)<<1, SSD1306->data, SSD1306_DATA_LEN, 10);
#endif
}

void SSD1306_I2C_Init( ssd1306_t * SSD1306  )
{
  delay_ms(100);
  SSD1306_I2C_SendCommand( SSD1306, SSD1306_DISPLAY_OFF);
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_CLOCK_DIV_RATIO);
  SSD1306_I2C_SendCommand( SSD1306,0x80);
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_MULTIPLEX_RATIO );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306->height - 1 );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DISPLAY_OFFSET );
  SSD1306_I2C_SendCommand( SSD1306,0x00);
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DISPLAY_START_LINE|0x00); 
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_CHARGE_PUMP );
  SSD1306_I2C_SendCommand( SSD1306,0x14);
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_MEMORY_ADDRESS_MODE );
  SSD1306_I2C_SendCommand( SSD1306,0x00);  
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_SEGMENT_REMAP| 0x01);
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_COM_SCAN_DIRECTION_DEC );
  
#if defined SSD1306_128_32
  SSD1306_I2C_SendCommand( &SSD1306,SSD1306_HARDWARE_PINS_CONFIG );
  SSD1306_I2C_SendCommand( &SSD1306,0x02 );
  SSD1306_I2C_SendCommand( &SSD1306,SSD1306_CONTRAST_CONTROL );
  SSD1306_I2C_SendCommand( &SSD1306,0x8F);
#elif defined SSD1306_128_64
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_HARDWARE_PINS_CONFIG );
  SSD1306_I2C_SendCommand( SSD1306,0x12 );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_CONTRAST_CONTROL );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306->contrast);
#endif
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_PRECHARGE_PERIOD );
  SSD1306_I2C_SendCommand( SSD1306,0xF1 );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_VCOM_DESELECT );
  SSD1306_I2C_SendCommand( SSD1306,0x40 );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DISPLAY_ALL_ON_RESUME );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DISPLAY_NORMAL );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DEACTIVATE_SCROLL );
  SSD1306_I2C_SendCommand( SSD1306,SSD1306_DISPLAY_ON );
  SSD1306_I2C_SetFont( SSD1306, SMALL_FONTS );
  delay_ms(100);
  SSD1306_I2C_Update( SSD1306);
}

void SSD1306_I2C_SetFont( ssd1306_t * SSD1306, const uint8_t *font)
{
  SSD1306->Font.font = font;
  SSD1306->Font.x_size = *(font+0);
  SSD1306->Font.y_size = *(font+1);
  SSD1306->Font.offset = *(font+2);
  SSD1306->Font.numchars = *(font+3);
  SSD1306->Font.inverted = false;
}

void SSD1306_I2C_InvertFont( ssd1306_t * SSD1306, uint8_t invert_status )
{
  if( invert_status )
  {
    SSD1306->Font.inverted = true;
  }
  else
  {
    SSD1306->Font.inverted = false;
  }
}

void SSD1306_I2C_Update(ssd1306_t * SSD1306)
{
    uint16_t i;
    uint8_t x;
    
    // Configurar la dirección de las columnas
    SSD1306_I2C_SendCommand(SSD1306, SSD1306_COLUMN_ADDRESS);
    SSD1306_I2C_SendCommand(SSD1306, 0);   // Columna inicial
    SSD1306_I2C_SendCommand(SSD1306, SSD1306->width-1); // Columna final (127)
    
    // Configurar la dirección de las páginas
    SSD1306_I2C_SendCommand(SSD1306, SSD1306_PAGE_ADDRESS);
    SSD1306_I2C_SendCommand(SSD1306, 0); // Página inicial
    SSD1306_I2C_SendCommand(SSD1306, 7); // Página final (7 para 64 píxeles de alto)

    // Para una pantalla 128x64, necesitamos enviar 1024 bytes (128 * 64/8)
    // Enviamos en bloques de 16 bytes cada vez
    for (i = 0; i < 1024; i += 16) {
        // Copiar 16 bytes del buffer a los datos
        for (x = 0; x < 16; x++) {
            SSD1306->data[x + 1] = buffer[i + x];
        }
        
        // Enviar el bloque de datos
        SSD1306_I2C_SendData(SSD1306);
    }
}
void SSD1306_I2C_SetContrast( ssd1306_t * SSD1306, uint8_t contrast )
{
  SSD1306_I2C_SendCommand( SSD1306, SSD1306_CONTRAST_CONTROL );
  SSD1306_I2C_SendCommand( SSD1306, contrast);
}

void SSD1306_I2C_ClearDisplay( ssd1306_t * SSD1306 )
{
  memset(buffer, 0, (SSD1306->width * SSD1306->height/8));
}

void SSD1306_I2C_FillDisplay( ssd1306_t * SSD1306 )
{
  memset(buffer, 0xFF, (SSD1306->width * SSD1306->height/8));
}

void SSD1306_I2C_InvertDisplay( ssd1306_t * SSD1306, uint8_t value )
{
  if( value )
  {
    SSD1306_I2C_SendCommand( SSD1306, SSD1306_DISPLAY_INVERT );
  }
  else
  {
    SSD1306_I2C_SendCommand( SSD1306, SSD1306_DISPLAY_NORMAL );
  }
}

void SSD1306_I2C_DrawPixel( ssd1306_t * SSD1306, int16_t x, int16_t y)
{
  if ( (x < 0) || (x >= SSD1306->width) || (y < 0) || (y >= SSD1306->height))
    return;

  switch (SSD1306->color)
  {
    case SSD1306_WHITE:   
      buffer[x+ (y/8)*SSD1306->width] |=  (1 << (y&7)); 
      break;
    case SSD1306_BLACK:   
      buffer[x+ (y/8)*SSD1306->width] &= ~(1 << (y&7)); 
      break;
    case SSD1306_INVERSE: 
      buffer[x+ (y/8)*SSD1306->width] ^=  (1 << (y&7)); 
      break;
  }
}

void SSD1306_I2C_Linea( ssd1306_t * SSD1306, int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end)
{
  int16_t x, y, addx, addy, dx, dy;
  int32_t P;
  int16_t i;
  dx = abs((int16_t)(x_end - x_start));
  dy = abs((int16_t)(y_end - y_start));
  x = x_start;
  y = y_start;

  if(x_start > x_end)
  {
    addx = -1;
  }
  else
  {
    addx = 1;
  }
  if(y_start > y_end)
  {
    addy = -1;
  }
  else
  {
    addy = 1;
  }
  if(dx >= dy)
  {
    P = 2*dy - dx;
    for(i=0; i<=dx; ++i)
    {
      SSD1306_I2C_DrawPixel(SSD1306, x,y);
      if(P < 0)
      {
        P += 2*dy;
        x += addx;
      }
      else
      {
        P += 2*dy - 2*dx;
        x += addx;
        y += addy;
      }
    }
  }
  else
  {
    P = 2*dx - dy;
    for(i=0; i<=dy; ++i)
    {
      SSD1306_I2C_DrawPixel(SSD1306, x,y);

      if(P < 0)
      {
        P += 2*dx;
        y += addy;
      }
      else
      {
        P += 2*dx - 2*dy;
        x += addx;
        y += addy;
      }
    }
  }
}

void SSD1306_I2C_LineaVertical ( ssd1306_t * SSD1306, int16_t y_start, int16_t y_end, int16_t x_pos)
{
  int16_t temp;
  if( y_start > y_end )
  {
    temp = y_start;
    y_start = y_end;
    y_end = temp;
  }

  while( y_start <= y_end )
  {
    SSD1306_I2C_DrawPixel(SSD1306, x_pos, y_start);
    y_start++;
  }
}

void SSD1306_I2C_LineaHorizontal( ssd1306_t * SSD1306, int16_t x_start, int16_t x_end, int16_t y_pos)
{
  int16_t temp;
  if( x_start > x_end )
  {
    temp = x_start;
    x_start = x_end;
    x_end = temp;
  }

  while( x_start <= x_end )
  {
    SSD1306_I2C_DrawPixel(SSD1306, x_start, y_pos);
    x_start++;
  }
}

void SSD1306_I2C_Rectangulo( ssd1306_t * SSD1306, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  SSD1306_I2C_LineaHorizontal(SSD1306, x1, x2, y1 );
  SSD1306_I2C_LineaHorizontal(SSD1306, x1, x2, y2 );
  SSD1306_I2C_LineaVertical(SSD1306, y1, y2, x1 );
  SSD1306_I2C_LineaVertical(SSD1306, y1, y2, x2);
}

void SSD1306_I2C_RellenoRectangulo( ssd1306_t * SSD1306, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  int16_t i;
  for( i=x1; i<=x2; i++ )
  {
    SSD1306_I2C_LineaVertical(SSD1306, y1, y2, i);
  }
}

void SSD1306_I2C_Triangulo( ssd1306_t * SSD1306, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  SSD1306_I2C_Linea(SSD1306, x0, y0, x1, y1);
  SSD1306_I2C_Linea(SSD1306, x1, y1, x2, y2);
  SSD1306_I2C_Linea(SSD1306, x2, y2, x0, y0);
}

void SSD1306_I2C_Imagen(ssd1306_t * SSD1306, const uint8_t *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
{
    // Verificar límites
    if(x >= SSD1306->width || y >= SSD1306->height)
        return;
        
    // Ajustar ancho y alto si exceden los límites de la pantalla
    if(x + width > SSD1306->width)
        width = SSD1306->width - x;
    if(y + height > SSD1306->height)
        height = SSD1306->height - y;

    // Calcular páginas (grupos de 8 píxeles verticales)
    uint8_t pages = (height + 7) / 8;
    uint8_t start_page = y / 8;
    
    // Dibujar la imagen
    for(uint8_t page = 0; page < pages; page++) {
        for(uint8_t col = 0; col < width; col++) {
            // Calcular posición en el buffer
            uint16_t buffer_pos = (start_page + page) * SSD1306->width + x + col;
            
            if(buffer_pos < (SSD1306->width * SSD1306->height / 8)) {
                buffer[buffer_pos] = *image++;
            }
        }
    }
}

void SSD1306_I2C_Circulo( ssd1306_t * SSD1306, int16_t x0, int16_t y0, int16_t r)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  SSD1306_I2C_DrawPixel(SSD1306,x0  , y0+r);
  SSD1306_I2C_DrawPixel(SSD1306,x0  , y0-r);
  SSD1306_I2C_DrawPixel(SSD1306,x0+r, y0  );
  SSD1306_I2C_DrawPixel(SSD1306,x0-r, y0  );

  while(x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    SSD1306_I2C_DrawPixel(SSD1306,x0 + x, y0 + y);
    SSD1306_I2C_DrawPixel(SSD1306,x0 - x, y0 + y);
    SSD1306_I2C_DrawPixel(SSD1306,x0 + x, y0 - y);
    SSD1306_I2C_DrawPixel(SSD1306,x0 - x, y0 - y);
    SSD1306_I2C_DrawPixel(SSD1306,x0 + y, y0 + x);
    SSD1306_I2C_DrawPixel(SSD1306,x0 - y, y0 + x);
    SSD1306_I2C_DrawPixel(SSD1306,x0 + y, y0 - x);
    SSD1306_I2C_DrawPixel(SSD1306,x0 - y, y0 - x);
  }
}

void SSD1306_I2C_Caracter( ssd1306_t * SSD1306, int16_t x, int16_t y, char value )
{
    uint16_t font_idx = 0;
    int16_t rowcnt = 0;
    int16_t cnt = 0;
    int8_t b;
    uint16_t temp = 0;

    // Calcular el índice inicial en la fuente
    font_idx = ((value-SSD1306->Font.offset)*(SSD1306->Font.x_size*(SSD1306->Font.y_size/8))) + 4;

    // Para fuentes con altura múltiplo de 8
    if( SSD1306->Font.y_size%8 == 0)
    {
        for ( rowcnt=0; rowcnt<(SSD1306->Font.y_size/8); rowcnt++ )
        {
            for( cnt=0; cnt<SSD1306->Font.x_size; cnt++ )
            {
                temp = *(SSD1306->Font.font + font_idx + cnt + (rowcnt*SSD1306->Font.x_size));
                for ( b=0; b<8; b++ )
                {
                    if ( temp & (1<<b) )
                    {
                        // Bit activo
                        SSD1306->color = SSD1306->Font.inverted ? SSD1306_BLACK : SSD1306_WHITE;
                        SSD1306_I2C_DrawPixel(SSD1306, x+cnt, y+(rowcnt*8)+b);
                    }
                    else
                    {
                        // Bit inactivo - no dibujar nada o dibujar el fondo
                        SSD1306->color = SSD1306->Font.inverted ? SSD1306_WHITE : SSD1306_BLACK;
                        // Opcional: dibuja el fondo
                        // SSD1306_I2C_DrawPixel(SSD1306, x+cnt, y+(rowcnt*8)+b);
                    }
                }
            }
        }
    }
}

void SSD1306_I2C_Cadena(ssd1306_t * SSD1306, int16_t x, int16_t y, char *text)
{
    // Verificar parámetros de entrada
    if (text == NULL || SSD1306 == NULL)
        return;

    uint8_t cnt = 0;
    uint8_t length = (uint8_t)strlen((const char*)text);
    int16_t start_x = x; // Guardar la posición x inicial

    // Calcular posición para alineación
    if (x == SSD1306->right)
    {
        x = SSD1306->width - (length * SSD1306->Font.x_size);
    }
    else if (x == SSD1306->center)
    {
        x = (SSD1306->width - (length * SSD1306->Font.x_size)) / 2;
    }

    // Verificar que la posición inicial es válida
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    // Procesar cada carácter
    while (*text != '\0')
    {
        // Manejar caracteres especiales
        if (*text == '\n')
        {
            y += SSD1306->Font.y_size; // Nueva línea
            x = start_x;               // Volver al inicio
            text++;
            continue;
        }
        if (*text == '\r')
        {
            x = start_x;  // Retorno de carro
            text++;
            continue;
        }
        // Verificar si el carácter cabe en la pantalla
        if (x + SSD1306->Font.x_size > SSD1306->width)
        {
            // Saltar a la siguiente línea si no cabe
            x = start_x;
            y += SSD1306->Font.y_size;
        }
        // Verificar si todavía hay espacio vertical
        if (y + SSD1306->Font.y_size > SSD1306->height)
        {
            break; // No hay más espacio en la pantalla
        }
        // Verificar si el carácter está dentro del rango de la fuente
        if (*text >= SSD1306->Font.offset && 
            *text < (SSD1306->Font.offset + SSD1306->Font.numchars))
        {
            // Dibujar el carácter
            SSD1306_I2C_Caracter(SSD1306, x, y, *text);
            x += SSD1306->Font.x_size; // Mover a la siguiente posición
        }
        
        text++; // Siguiente carácter
    }
}

uint16_t seed = 0;

uint8_t getRandom(uint8_t limit) {  // límite es 128 o 64
    seed++;
    uint16_t num = seed * 1103515245 + 12345;
    return (uint8_t)(num % limit);
}

#define MAX_OBJECTS 5  // Número de objetos en pantalla
#define DELTAY       // Velocidad de caída

// Estructura para mantener la posición de cada objeto
struct {
    int16_t x;
    int16_t y;
} objects[MAX_OBJECTS];


void SSD1306_I2C_Animation(ssd1306_t * SSD1306, const uint8_t *image, uint8_t width, uint8_t height) {
    static uint8_t initialized = 0;
    
    // Inicializar posiciones la primera vez
    if (!initialized) {
        for(uint8_t i = 0; i < MAX_OBJECTS; i++) {
            objects[i].x = getRandom(SSD1306->width - width);
            objects[i].y = -(height * i);  // Escalonar las posiciones iniciales
        }
        initialized = 1;
    }
    
    // Limpiar buffer
    memset(buffer, 0, (SSD1306->width * SSD1306->height/8));
    
    // Actualizar y dibujar cada objeto
    for(uint8_t i = 0; i < MAX_OBJECTS; i++) {
        // Verificar límites
        if(objects[i].x >= SSD1306->width || objects[i].y >= SSD1306->height)
            continue;
            
        // Calcular páginas para este objeto
        uint8_t pages = (height + 7) / 8;
        uint8_t start_page = objects[i].y / 8;
        
        // Dibujar el objeto
        const uint8_t *img_ptr = image;  // Puntero temporal para cada objeto
        for(uint8_t page = 0; page < pages; page++) {
            for(uint8_t col = 0; col < width; col++) {
                // Calcular posición en el buffer
                uint16_t buffer_pos = (start_page + page) * SSD1306->width + objects[i].x + col;
                
                if(buffer_pos < (SSD1306->width * SSD1306->height / 8)) {
                    buffer[buffer_pos] |= *img_ptr++;  // Usar OR para combinar objetos
                }
            }
        }
        
        // Actualizar posición Y
        objects[i].y += getRandom(9);
        
        // Si el objeto sale de la pantalla, reiniciar desde arriba
        if(objects[i].y >= SSD1306->height) {
            objects[i].x = getRandom(SSD1306->width - width);
            objects[i].y = -height;
        }
    }
    
    // Actualizar pantalla
    SSD1306_I2C_Update(SSD1306);
    __delay_ms(50);  // Ajusta según necesidad
}