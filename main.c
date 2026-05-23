#define assert(expression) if (!(expression)) {__builtin_trap();}

#define bool32 int
#define uint32 unsigned int
#define int16 short
#define uint16 unsigned short
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
JS_FUNC(int, now, ());

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

//NOTE: these need to be in the same
//order than the movementDirections array
//below
typedef enum {
  UP,
  DOWN,
  RIGHT,
  LEFT
} DirectionIndex;

typedef struct {
  float x;
  float y;
} Position;

typedef struct {
  int16 xDelta;
  int16 yDelta;
} Direction;

typedef struct {
  Position pos;
  int keysPressed[4];
  int currentKeyPressIndex;
  Direction direction;
} PlayerState;

#define UP_CODE 87
#define DOWN_CODE 83
#define RIGHT_CODE 68
#define LEFT_CODE 65

global_variable PlayerState player = {};
global_variable Direction movementDirections[] = {
  {0, -1}, // up
  {0, 1}, // down
  {1, 0}, // right
  {-1, 0}, // left
};

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

void log(char *output) {
  String out = buildString(output);
  consoleLog(out.value, out.length);
}

internal inline int getDirectionIndex(uint32 keyCode) {
  uint32 index = -1;
  switch(keyCode) {
    case UP_CODE: {
      index = UP;
    } break;
    case DOWN_CODE: {
      index = DOWN;
    } break;
    case LEFT_CODE: {
      index = LEFT;
    } break;
    case RIGHT_CODE: {
      index = RIGHT;
    } break;
  }

  return index;
}

// TODO: set key down to set a movement direction and start moving
// and add keyUp event to stop
void keyDown(uint32 keyCode) {
  //TODO: THERE IS A BUG HERE or IN KEYUP THAT
  //DOESN'T ADD/SUBSTRACT THE DIRECTION CORRECTLY
  //WHEN KEY IS UP
  log("key down");
  for (int i = 0; i < player.currentKeyPressIndex; ++i) {
    if (keyCode == player.keysPressed[i]) {
      //already pressed
      log("already pressed");
      return;
      break;
    }
  }
  log("NOT ALREADY PRESSED");

  int directionIndex = getDirectionIndex(keyCode);

  if (directionIndex >= 0 && player.currentKeyPressIndex < sizeof(player.keysPressed) - 1) {
    Direction keyDirection = movementDirections[directionIndex];
    player.direction.xDelta += keyDirection.xDelta;
    player.direction.yDelta += keyDirection.yDelta;

    player.keysPressed[player.currentKeyPressIndex] = keyCode;
    ++player.currentKeyPressIndex;
  }
};

void keyUp(uint32 keyCode) {
  log("key up");
  for (int i = 0; i < player.currentKeyPressIndex; ++i) {
    if (keyCode == player.keysPressed[i]) {
      int directionIndex = getDirectionIndex(keyCode);
      Direction keyDirection = movementDirections[directionIndex];
      player.direction.xDelta -= keyDirection.xDelta;
      player.direction.yDelta -= keyDirection.yDelta;

      // if not last item, move all items after it back a spot
      if (i != player.currentKeyPressIndex - 1) {
        for (int keyIndex = i; keyIndex < player.currentKeyPressIndex - 2; ++keyIndex)  {
          player.keysPressed[keyIndex] = player.keysPressed[keyIndex + 1];
        }
      }
      --player.currentKeyPressIndex;
      break;
    }
  }
  log("key up done");
}

int startGame() {
  log("hello world");
  Position startingPosition = {10.0f, 10.0f};
  player.pos = startingPosition;

  canvas_fillRect(0x555555, player.pos.x, player.pos.y, 100, 100);

  return 0;
}

int updateGame() {
    int targetFrameTime = 16;
    float movementSpeed = 5.0f;

    player.pos.x += (player.direction.xDelta * movementSpeed);
    player.pos.y += (player.direction.yDelta * movementSpeed);
    canvas_fillRect(0x555555, player.pos.x, player.pos.y, 100, 100);

    return 0;
}
