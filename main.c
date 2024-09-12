#include <gb/gb.h>
#include <stdio.h>
#include "GameCharacter.c"
#include "sprites/SimpleFish.c"

struct GameCharacter fishr;
struct GameCharacter fishl;
UBYTE spritesize = 8;

void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y)
{
    move_sprite(character->spriteids[0], x, y);
    move_sprite(character->spriteids[1], x + spritesize, y);
    move_sprite(character->spriteids[2], x + spritesize * 2, y);
    move_sprite(character->spriteids[3], x, y + spritesize);
    move_sprite(character->spriteids[4], x + spritesize, y + spritesize);
    move_sprite(character->spriteids[5], x + spritesize * 2, y + spritesize);
}

void setupfishr()
{
    fishr.x = 80;
    fishr.y = 130;
    fishr.width = 22;
    fishr.height = 11;

    set_sprite_tile(0, 0);
    fishr.spriteids[0] = 0;
    set_sprite_tile(1, 1);
    fishr.spriteids[1] = 1;
    set_sprite_tile(2, 2);
    fishr.spriteids[2] = 2;
    set_sprite_tile(3, 3);
    fishr.spriteids[3] = 3;
    set_sprite_tile(4, 4);
    fishr.spriteids[4] = 4;
    set_sprite_tile(5, 5);
    fishr.spriteids[5] = 5;

    movegamecharacter(&fishr, fishr.x, fishr.y);
}

void setupfishl()
{
    fishl.x = 102;
    fishl.y = 130;
    fishl.width = 22;
    fishl.height = 11;

    set_sprite_tile(6, 6);
    fishl.spriteids[0] = 6;

    set_sprite_tile(7, 7);
    fishl.spriteids[1] = 7;

    set_sprite_tile(8, 8);
    fishl.spriteids[2] = 8;

    set_sprite_tile(9, 9);
    fishl.spriteids[3] = 9;

    set_sprite_tile(10, 10);
    fishl.spriteids[4] = 10;

    set_sprite_tile(11, 11);
    fishl.spriteids[5] = 11;

    movegamecharacter(&fishl, fishl.x, fishl.y);
}

void main()
{
    set_sprite_data(0, 12, SimpleFish);
    setupfishr();
    setupfishl();
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
    }
}
