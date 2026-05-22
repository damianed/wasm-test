class RenderQueue {
  constructor() {
    this.items = [];
    this.isRunningRender = false;
  }

  enqueue(element) {
    this.items.push(element)
    this.render();
  }

  denqueue() {
    return this.items.shift();
  }

  render() {
    if (this.isRunningRender) return;
    this.isRunningRender = true;
    const frameTimeMs = 33;

    const execCall = () => {
      const fn = this.denqueue();
      requestAnimationFrame(fn);
      //fn()

      if (this.items.length > 0) {
        setTimeout(execCall, frameTimeMs);
      } else {
        this.isRunningRender = false;
      }
    }

    setTimeout(execCall, frameTimeMs);
  }
}

document.addEventListener("DOMContentLoaded", () =>  {
  const canvas = document.getElementById("myCanvas");
  const renderQueue = new RenderQueue();

  function clearCanvas() {
    const ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  }

  function fillRect(color, startX, startY, width, height) {
    clearCanvas();
    const ctx = canvas.getContext("2d");
    const hexColorStr = "#" + color.toString(16);
    ctx.fillStyle = hexColorStr;
    ctx.fillRect(startX, startY, width, height);
  };

  function arc(centerX, centerY, radius, startAngle, endAngle, counterclockwise) {
    const ctx = canvas.getContext("2d");
    ctx.beginPath();
    ctx.arc(centerX, centerY, radius, startAngle, endAngle, counterclockwise);
    ctx.stroke();
  }

  function readStringFromMemory(memory, pointer, size) {
    const charCodes = memory.subarray(pointer, pointer + size);
    return String.fromCharCode(...charCodes);
  }

  async function main() {
    let importObject = {
      env: {
        consoleLog: (pointer, size) => {
          const memory = new Uint8Array(wasmInstance.exports.memory.buffer);
          const output = readStringFromMemory(memory, pointer, size)
          console.log("Wasm consoleLog: ", output);
        },
        canvas_clear: () => renderQueue.enqueue(() => clearCanvas()),
        canvas_fillRect: (color, startX, startY, width, height) => renderQueue.enqueue(() => fillRect(color, startX, startY, width, height)),
        canvas_arc: (centerX, centerY, radius, startangle, endAngle, counterclockwise) => renderQueue.enqueue(() => arc(centerX, centerY, radius, startangle, endAngle, counterclockwise)),
      }
    };
    const { instance: wasmInstance }  = await WebAssembly.instantiateStreaming(fetch('build/main.wasm'), importObject);
    wasmInstance.exports.main();
  }

  main();
});
