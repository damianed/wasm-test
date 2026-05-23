zig cc -nostdlib -target wasm32-freestanding \
  -std=c99 -fno-sanitize=all -g -fdebug-compilation-dir=".." \
  -Wl,--no-entry -Wl,--export=startGame -Wl,--export=updateGame -Wl,--export=keyDown -Wl,--export=keyUp \
  main.c -o ./build/main.wasm



