#define assert(expression) if (!(expression)) {__builtin_trap();}

#define bool32 int
#define uint32 unsigned int
#define uint8 unsigned char
#define int8 signed char

#define PI 3.14159265358979323846

#define internal static
#define local_persist static
#define global_variable static

// NOTE: macro to be able to use js functions more easily without having to write all of this:
//   extern void function_name (args) __attribute__((import_module("env"), import_name("function_name")));
// every time
//  JS_FUNC(void, consoleLog, (char *String, int size)); // converts to:
//  extern void consoleLog(char *String, int size) __attribute__((import_module("env"), import_name("consoleLog")));
#define JS_FUNC(return_type, func_name, func_signature) extern return_type func_name func_signature __attribute__((import_module("env"), import_name(#func_name)));

JS_FUNC(void, consoleLog, (char *String, int size));
// TODO: maybe wrap this functions so they're easier to use from here for most cases
// and make these ones as close as posible to the canvas functionallity to allow for flexibility
JS_FUNC(void, canvas_fillRect, (int color, int startX, int startY, int width, int height));
JS_FUNC(void, canvas_arc, (int centerX, int centerY, float radious, float startAngle, float endAngle, bool32 counterclockwise));
JS_FUNC(void, canvas_clear, ());

// I think the compiler optimized this out
//void sleepMs(int ms) {
//    double start = now();
//    volatile double t;
//    while ((t = now() - start) < ms) {
//        // wait
//    }
//}

typedef struct {
  char *value;
  int length;
} String;

typedef enum {
  UP,
  DOWN,
  RIGHT,
  LEFT
} InputAction;

typedef struct {
  uint32 code;
  InputAction action;
} Input;

typedef struct {
  float x;
  float y;
} Position;

#define UP_CODE 87
#define DOWN_CODE 83
#define RIGHT_CODE 68
#define LEFT_CODE 65

global_variable Position currentPosition = {10.0f, 10.0f};
global_variable float movementSpeed = 5.0f;

int stringLength(char *s, int maxLength) {
  int result = 0;
  while (result < maxLength && *s++ != '\0') {
    ++result;
  }
  assert(*--s == '\0');
  return result;
}

String buildString(char *value) {
  String res = {value, stringLength(value, 255)};
  return res;
}

// TODO: set key down to set a movement direction and start moving
// and add keyUp event to stop
void keyDown(uint32 keyCode) {
  switch (keyCode) {
    case UP_CODE: {
      currentPosition.y -= movementSpeed;
    } break;
    case DOWN_CODE: {
      currentPosition.y += movementSpeed;
    } break;
    case RIGHT_CODE: {
      currentPosition.x += movementSpeed;
    } break;
    case LEFT_CODE: {
      currentPosition.x -= movementSpeed;
    } break;
    default: {
      return;
    }
  };

  //TODO: find a way to limit movement to the canvas/screen size
  canvas_fillRect(0x555555, currentPosition.x, currentPosition.y, 100, 100);
};

int main() {
  canvas_fillRect(0x555555, currentPosition.x, currentPosition.y, 100, 100);
  return 0;
}
