#include <gb/gb.h>
#include <stdio.h>
#include "sprites/SimpleFish.c"
#include "sprites/Spear.c"

// Define UINT8 type
typedef unsigned char UINT8;

// Define the GameCharacter struct
struct GameCharacter
{
    UBYTE spriteids[6];
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

// Function prototypes
void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y);
void setupFishSprites(struct GameCharacter *fish, const UINT8 *spriteTiles);
void switchFishSprite(UBYTE isMovingRight);
void setupFish();

struct GameCharacter fish;
UBYTE spritesize = 8;
UBYTE facingRight = 1; // 1 for right, 0 for left

const UINT8 rightFacingSpriteTiles[] = {0, 1, 2, 3, 4, 5};
const UINT8 leftFacingSpriteTiles[] = {6, 7, 8, 9, 10, 11};

void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y)
{
    UINT8 i; // Declare the loop variable outside the loop
    for (i = 0; i < 6; i++)
    {
        move_sprite(character->spriteids[i], x + (i % 3) * spritesize, y + (i / 3) * spritesize);
    }
}

void setupFishSprites(struct GameCharacter *fish, const UINT8 *spriteTiles)
{
    UINT8 i; // Declare the loop variable outside the loop
    for (i = 0; i < 6; i++)
    {
        set_sprite_tile(i, spriteTiles[i]);
        fish->spriteids[i] = i;
    }
    movegamecharacter(fish, fish->x, fish->y);
}

void switchFishSprite(UBYTE isMovingRight)
{
    if (isMovingRight)
    {
        setupFishSprites(&fish, rightFacingSpriteTiles);
        facingRight = 1;
    }
    else
    {
        setupFishSprites(&fish, leftFacingSpriteTiles);
        facingRight = 0;
    }
}

void setupFish()
{
    fish.x = 80;
    fish.y = 130;
    fish.width = 22;
    fish.height = 11;

    // Initialize the fish facing right as the default
    if (facingRight)
    {
        setupFishSprites(&fish, rightFacingSpriteTiles); // Default to right-facing
    }
    else
    {
        setupFishSprites(&fish, leftFacingSpriteTiles); // Default to left-facing
    }
}

void main()
{
    set_sprite_data(0, 12, SimpleFish);
    setupFish();
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
        UBYTE joypadState = joypad();
        UBYTE isMovingRight = facingRight; // Start by assuming fish is facing its current direction

        if (joypadState & J_LEFT)
        {
            fish.x -= 2;       // Move left
            isMovingRight = 0; // Fish is moving left
        }
        if (joypadState & J_RIGHT)
        {
            fish.x += 2;       // Move right
            isMovingRight = 1; // Fish is moving right
        }

        // Only update the sprite appearance if the direction changes
        if (isMovingRight != facingRight)
        {
            switchFishSprite(isMovingRight);
        }

        // Move the sprite to the new position
        movegamecharacter(&fish, fish.x, fish.y);

        wait_vbl_done(); // Wait for the vertical blank interrupt
    }
}
