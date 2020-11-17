#include "text.h"
#include "mylib.h"

void drawString(int x, int y, char *str, uint16_t color)
{
    while( *str )
    {
        drawChar( x, y, *str, color );
        x += 6;
        str++;
    }
}

void drawChar(int x, int y, char ch, uint16_t color)
{
    for( int c = 0; c < 8; c++ )
    {
        for( int r = 0; r < 6; r++ )
        {
            //Gives either a 0 or a 1 out of the array
            if( fontdata_6x8[OFFSET(c, r, 6) + (ch * 48)] == 1 )
            {
                setPixel( x + r, y + c, color );
            }
        }
    }
}
