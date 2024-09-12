#include <gb/gb.h>
#include <stdio.h>
#include "sprites/SimpleFish.c"
#include "GameCharacter.c"

// Define UINT8 type
typedef unsigned char UINT8;

// Function prototypes
void movegamecharacter(struct GameCharacter *character, UINT8 x, UINT8 y, UBYTE vertical);
void setupFishSprites(struct GameCharacter *fish, const UINT8 *spriteTiles);
void setupSpearSprites(struct GameCharacter *spear, const UINT8 *spriteTiles);
void switchFishSprite(UBYTE isMovingRight);
void setupFish();
void setupSpear(struct GameCharacter *spear, UINT8 x, UINT8 y);

// Declare GameCharacter struct for fish and spear
struct GameCharacter fish;
struct GameCharacter spear;
struct GameCharacter spear1;
struct GameCharacter spear2;
struct GameCharacter *spears[] = {&spear, &spear1, &spear2};

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
void setupSpear(struct GameCharacter *spear, UINT8 x, UINT8 y)
{
    spear->x = x; // Set initial x position
    spear->y = y; // Set initial y position
    spear->width = spritesize;
    spear->height = 48;

    // Set up spear using its own sprite tiles and sprite IDs
    setupSpearSprites(spear, spearSpriteTiles); // This remains the same
}

// Simple PRNG function
UINT8 get_random()
{
    static UINT8 state = 0;
    state = (state * 33 + 17) & 0xFF; // Use smaller constants
    return state;
}

UINT8 random_seed = 0;

void main()
{
    int i;
    struct GameCharacter *spears[] = {&spear, &spear1, &spear2};
    set_sprite_data(0, 18, SimpleFish); // Load SimpleFish tiles into sprite memory

    setupFish(); // Set up the fish character

    // Define an array of pointers to handle all spears

    // Initialize multiple spears with different positions
    setupSpear(spears[0], 80, 0);  // First spear at x=80, y=0
    setupSpear(spears[1], 40, 0);  // Second spear at x=40, y=0
    setupSpear(spears[2], 120, 0); // Third spear at x=120, y=0

    SHOW_SPRITES;
    DISPLAY_ON;

    // Declare 'i' here at the beginning of the block

    // Main game loop
    while (1)
    {
        UBYTE joypadState = joypad();
        UBYTE isMovingRight = facingRight; // Assume the fish is facing its current direction

        // Fish movement logic remains unchanged
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

        // Update and move each spear in the array
        for (i = 0; i < 3; i++) // 'i' is now valid here
        {
            struct GameCharacter *s = spears[i]; // Pointer to the current spear

            s->y += 2;       // Adjust speed by changing the value (e.g., 1, 2, 3, etc.)
            if (s->y >= 160) // Reset spear when it reaches the bottom of the screen
            {
                s->y = 0;                  // Reset to the top of the screen
                s->x = get_random() % 160; // Randomize x position on reset
            }
            movegamecharacter(s, s->x, s->y, 1); // Move the spear (1 for vertical alignment)
        }

        wait_vbl_done(); // Wait for vertical blank to avoid tearing
    }
}
