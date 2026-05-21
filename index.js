const memory = new WebAssembly.Memory({
  initial: 10,
});

async function main() {
  let importObject = {
    env: {
      consoleLog: (pointer, size) => {
        const memory = new Uint8Array(wasmInstance.exports.memory.buffer);
        console.log('size', size);
        const charCodes = memory.subarray(pointer, pointer + size);
        const output = String.fromCharCode(...charCodes)
        //let output = "";
        //for (let i = 0; i < size; ++i) {
        //  output += String.fromCharCode(memory[pointer + i]);
        //}
        console.log("Wasm consoleLog: ", output);
      },
    }
  };
  const { instance: wasmInstance }  = await WebAssembly.instantiateStreaming(fetch('build/main.wasm'), importObject);
  wasmInstance.exports.main();
}

main();
