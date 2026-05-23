zig cc -nostdlib -target wasm32-freestanding \
  -std=c99 -fno-sanitize=all -g \
  -Wl,--no-entry -Wl,--export=main -Wl,--export=keyDown \
  main.c -o ./build/main.wasm



