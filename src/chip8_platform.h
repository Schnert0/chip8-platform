#ifndef CHIP8_PLATFORM_H
#define CHIP8_PLATFORM_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

void Platform_SetBuzzer(uint8_t buzzerValue);
void Platform_SetDelay(uint8_t delayValue);
void Platform_WaitForDelayValue(uint8_t delayValue);

void Platform_ClearDisplay();
void Platform_SetIndex(uint8_t* index);
uint8_t Platform_DrawSprite(uint8_t x, uint8_t y, uint8_t n);

uint8_t Platform_WaitForKey();

uint8_t Platform_Random(uint8_t mask);

// Main game entry point
void entry();

#endif // CHIP8_PLATFORM_H
