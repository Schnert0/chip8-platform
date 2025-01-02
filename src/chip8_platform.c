#include "chip8_platform.h"
#include "SDL2/SDL_render.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;
static uint32_t colors[32][64];

static uint8_t pixels[32][64];
static uint8_t* indexRegister;
static bool keys[16];
static uint8_t lastKey;
static uint8_t delay, buzzer;
static bool wasPressed;


static void Platform__Tick();
static void Platform__CleanupAndExit();
static void Platform__SetKey(int sym, bool isPressed);


static void Platform__Tick() {
  if (delay > 0)
    delay--;
  if (buzzer > 0)
    buzzer--;

  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        Platform__CleanupAndExit();
        break;

      case SDL_KEYDOWN:
        Platform__SetKey(event.key.keysym.sym, true);
        break;

      case SDL_KEYUP:
        Platform__SetKey(event.key.keysym.sym, false);
        break;
    }
  }

  for (uint32_t y = 0; y < 32; y++) {
    for (uint32_t x = 0; x < 64; x++) {
      if (pixels[y][x])
        colors[y][x] = 0xffffffff;
      else
        colors[y][x] = 0x000000ff;
    }
  }

  SDL_UpdateTexture(texture, NULL, colors, 64*sizeof(uint32_t));
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}


static void Platform__CleanupAndExit() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  exit(0);
}


static void Platform__SetKey(int sym, bool isPressed) {
  uint8_t key;
  switch (sym) {
  case SDLK_1: key = 0x1; break;
  case SDLK_2: key = 0x2; break;
  case SDLK_3: key = 0x3; break;
  case SDLK_4: key = 0xc; break;
  case SDLK_q: key = 0x4; break;
  case SDLK_w: key = 0x5; break;
  case SDLK_e: key = 0x6; break;
  case SDLK_r: key = 0xd; break;
  case SDLK_a: key = 0x7; break;
  case SDLK_s: key = 0x8; break;
  case SDLK_d: key = 0x9; break;
  case SDLK_f: key = 0xe; break;
  case SDLK_z: key = 0xa; break;
  case SDLK_x: key = 0x0; break;
  case SDLK_c: key = 0xb; break;
  case SDLK_v: key = 0xf; break;

  default: return;
  }

  keys[key] = isPressed;
  if (isPressed) {
    wasPressed = true;
    lastKey = key;
  }
}


void Platform_SetBuzzer(uint8_t buzzerValue) {
  buzzer = buzzerValue;
}


void Platform_SetDelay(uint8_t delayValue) {
  delay = delayValue;
}


void Platform_WaitForDelayValue(uint8_t delayValue) {
  while (delay > delayValue) {
    Platform__Tick();
  }
}


void Platform_ClearDisplay() {
  memset(pixels, 0, sizeof(pixels));
}


void Platform_SetIndex(uint8_t* i) {
  indexRegister = i;
}


uint8_t Platform_DrawSprite(uint8_t x, uint8_t y, uint8_t n) {
  uint8_t flag = 0;

  for (int32_t py = 0; py < n; py++) {
    uint8_t row = indexRegister[py];
    for (int32_t px = 0; px < 8; px++) {
      if (row & (0x80 >> px)) {
        uint8_t* pixel = &pixels[(y+py) & 31][(x+px) & 63];
        flag |= ((*pixel) != 0);
        (*pixel) ^= 1;
      }
    }
  }

  return flag;
}


uint8_t Platform_WaitForKey() {
  wasPressed = false;

  while (!wasPressed) {
    Platform__Tick();
  }

  return lastKey;
}


uint8_t Platform_Random(uint8_t mask) {
  return rand() & mask;
}


int main(int argc, char** argv) {
  srand(time(NULL));
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
  if (SDL_CreateWindowAndRenderer(640, 360, SDL_WINDOW_SHOWN, &window, &renderer)) {
    printf("Unable to create window and renderer: %s\n", SDL_GetError());
    return 0;
  }
  SDL_SetWindowTitle(window, "Cave Explorer");
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 64, 32);

  entry();

  return 0;
}
