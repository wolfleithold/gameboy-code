#include <gb/gb.h>

typedef unsigned char UINT8;

struct GameCharacter
{
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
    UINT8 spriteids[6]; // Adjust the size if necessary
};
