if [ ! -d bin ]; then
  mkdir bin
fi

gcc src/cave-explorer.c src/chip8_platform.c -o bin/cave-explorer -lSDL2
