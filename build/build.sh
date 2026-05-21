zig cc -nostdlib -target wasm32-freestanding \
  -std=c99 -fno-sanitize=all -g \
  -Wl,--no-entry -Wl,--export=main \
  main.c -o ./build/main.wasm



