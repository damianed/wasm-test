function createWorker(fn) {
  var blob = new Blob(['self.onmessage = ', fn.toString()], { type: 'text/javascript' });
  var url = URL.createObjectURL(blob);

  return new Worker(url);
}

document.addEventListener("DOMContentLoaded", () =>  {
  const canvas = document.getElementById("myCanvas");
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;

  function clearCanvas() {
    const ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  }

  function fillRect(color, startX, startY, width, height) {
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

  (async () =>  {
    let importObject = {
      env: {
        consoleLog: (pointer, size) => {
          const memory = new Uint8Array(wasmInstance.exports.memory.buffer);
          const output = readStringFromMemory(memory, pointer, size)
          console.log("Wasm consoleLog: ", output);
        },
        canvas_clear: () => clearCanvas(),
        canvas_fillRect: (color, startX, startY, width, height) => fillRect(color, startX, startY, width, height),
        canvas_arc: (centerX, centerY, radius, startangle, endAngle, counterclockwise) => arc(centerX, centerY, radius, startangle, endAngle, counterclockwise),
        now: () => performance.now(),
      }
    };
    const { instance: wasmInstance }  = await WebAssembly.instantiateStreaming(fetch('build/main.wasm'), importObject);

    wasmInstance.exports.startGame();
    const frame = (timestamp) => {
      wasmInstance.exports.updateGame(canvas.width, canvas.height);
      window.requestAnimationFrame(frame);
    }

    window.requestAnimationFrame(frame);

    window.addEventListener("keydown", (event) => {
      const keycode = event.keyCode;
      wasmInstance.exports.keyDown(keycode);
    });

    window.addEventListener("keyup", (event) => {
      const keycode = event.keyCode;
      wasmInstance.exports.keyUp(keycode);
    });
  })();


  window.addEventListener("resize", () => {
    console.log('resize');
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
  });
});
