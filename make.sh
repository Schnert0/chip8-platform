if [ ! -d bin ]; then
  mkdir bin
fi

gcc src/chip8_platform/chip8_platform.c src/games/cave-explorer.c -o bin/cave-explorer -lSDL2
