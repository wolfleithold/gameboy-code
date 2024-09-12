#include <gb/gb.h>
#include <stdio.h>
#include "sprites/SimpleFish.c"
#include "GameCharacter.c"

// Define UINT8 type
typedef unsigned char UINT8;

// Function prototypes
void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y, UBYTE horizontal);
void setupFishSprites(struct GameCharacter *fish, const UINT8 *spriteTiles);
void setupSpearSprites(struct GameCharacter *spear, const UINT8 *spriteTiles);
void switchFishSprite(UBYTE isMovingRight);
void setupFish();
void setupSpear();

// Declare GameCharacter struct for fish and spear
struct GameCharacter fish;
struct GameCharacter spear;

UBYTE spritesize = 8;
UBYTE facingRight = 1; // 1 for right, 0 for left

// Define sprite tile arrays for fish and spear
const UINT8 rightFacingSpriteTiles[] = {0, 1, 2, 3, 4, 5};
const UINT8 leftFacingSpriteTiles[] = {6, 7, 8, 9, 10, 11};
const UINT8 spearSpriteTiles[] = {12, 13, 14, 15, 16, 17};

// Function to move a character's sprites on the screen
void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y, UBYTE vertical)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        if (vertical)
        {
            // Align sprites in a 1x6 column vertically
            move_sprite(character->spriteids[i], x, y + i * spritesize);
        }
        else
        {
            // Align sprites in a 2x3 grid (for fish)
            move_sprite(character->spriteids[i], x + (i % 3) * spritesize, y + (i / 3) * spritesize);
        }
    }
}

// Set up the fish sprite tiles
void setupFishSprites(struct GameCharacter *fish, const UINT8 *spriteTiles)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        set_sprite_tile(i, spriteTiles[i]); // Fish uses sprite IDs 0-5
        fish->spriteids[i] = i;
    }
    movegamecharacter(fish, fish->x, fish->y, 0); // Use '0' for 2x3 grid alignment
}

// Set up the spear sprite tiles
void setupSpearSprites(struct GameCharacter *spear, const UINT8 *spriteTiles)
{
    UINT8 i;
    for (i = 0; i < 6; i++)
    {
        set_sprite_tile(i + 6, spriteTiles[i]); // Spear uses sprite IDs 6-11
        spear->spriteids[i] = i + 6;
    }
    movegamecharacter(spear, spear->x, spear->y, 1); // Use '1' to align horizontally
}

// Switch the fish sprite direction based on movement
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

// Set up the fish character
void setupFish()
{
    fish.x = 80;
    fish.y = 130;
    fish.width = 22;
    fish.height = 11;

    // Initialize the fish facing right by default
    if (facingRight)
    {
        setupFishSprites(&fish, rightFacingSpriteTiles);
    }
    else
    {
        setupFishSprites(&fish, leftFacingSpriteTiles);
    }
}

// Set up the spear character
void setupSpear()
{
    spear.x = 80; // Position the spear
    spear.y = 16; // Adjust this if necessary to keep it on-screen
    spear.width = spritesize;
    spear.height = 48;

    // Set up spear using its own sprite tiles and sprite IDs
    setupSpearSprites(&spear, spearSpriteTiles);
}

void main()
{
    set_sprite_data(0, 18, SimpleFish); // Load SimpleFish tiles into sprite memory

    setupFish();  // Set up the fish character
    setupSpear(); // Set up the spear character

    SHOW_SPRITES;
    DISPLAY_ON;

    // Main game loop
    while (1)
    {
        UBYTE joypadState = joypad();
        UBYTE isMovingRight = facingRight; // Assume the fish is facing its current direction

        // Move left or right based on player input
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

        // Update the sprite appearance if the direction changes
        if (isMovingRight != facingRight)
        {
            switchFishSprite(isMovingRight);
        }

        // Move the fish sprite to the new position
        movegamecharacter(&fish, fish.x, fish.y, 0);

        spear.y += 2;       // Adjust speed by changing the value (e.g., 1 for slower, 2 for faster)
        if (spear.y >= 160) // If the spear goes off the bottom of the screen, reset its position
        {
            spear.y = 0; // Reset spear to the top of the screen
        }

        // Move the spear sprite to the new position
        movegamecharacter(&spear, spear.x, spear.y, 1); // Spear falls down

        wait_vbl_done(); // Wait for the vertical blank to sync the display
    }
}
