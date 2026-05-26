#define assert(expression) if (!(expression)) {__builtin_trap();}
#define arrayCount(arr) sizeof(arr) / sizeof(arr[0])

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
  Position posArr[2];
  int keysPressed[4];
  int currentKeyPressIndex;
  Direction direction;
  int width;
  int height;
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

void keyDown(uint32 keyCode) {
  //log("key down");
  for (int i = 0; i < player.currentKeyPressIndex; ++i) {
    if (keyCode == player.keysPressed[i]) {
      //already pressed
      //log("already pressed");
      return;
      break;
    }
  }
  //log("NOT ALREADY PRESSED");

  int directionIndex = getDirectionIndex(keyCode);

  if (directionIndex >= 0 && player.currentKeyPressIndex < sizeof(player.keysPressed)) {
    Direction keyDirection = movementDirections[directionIndex];
    player.direction.xDelta += keyDirection.xDelta;
    player.direction.yDelta += keyDirection.yDelta;

    player.keysPressed[player.currentKeyPressIndex] = keyCode;
    ++player.currentKeyPressIndex;
    assert((player.direction.xDelta <= 1 || player.direction.xDelta >= -1) || (player.direction.yDelta <= 1 || player.direction.yDelta >= -1))
  }
};

void keyUp(uint32 keyCode) {
  //log("key up");
  for (int i = 0; i < player.currentKeyPressIndex; ++i) {
    if (keyCode == player.keysPressed[i]) {
      int directionIndex = getDirectionIndex(keyCode);
      Direction keyDirection = movementDirections[directionIndex];
      player.direction.xDelta -= keyDirection.xDelta;
      player.direction.yDelta -= keyDirection.yDelta;

      assert((player.direction.xDelta <= 1 || player.direction.xDelta >= -1) || (player.direction.yDelta <= 1 || player.direction.yDelta >= -1))
      // if not last item, move all items after it back a spot
      if (i != player.currentKeyPressIndex - 1) {
        for (int keyIndex = i; keyIndex < player.currentKeyPressIndex - 1; ++keyIndex)  {
          player.keysPressed[keyIndex] = player.keysPressed[keyIndex + 1];
          player.keysPressed[keyIndex + 1] = 0;
        }
      }
      --player.currentKeyPressIndex;
      break;
    }
  }
  //log("key up done");
}

int startGame() {
  log("hello world");
  player.height = 50;
  player.width = 50;
  player.posArr[0] = (Position) {10.0f + player.width, 10.0f};
  player.posArr[1] = (Position) {10.0f, 10.0f};


  for (int i = 0; i < arrayCount(player.posArr); ++i) {
      //log("drawing");
      canvas_fillRect(0x555555, player.posArr[i].x, player.posArr[i].y, player.width, player.height);
  }

  return 0;
}

int updateGame(int screenWidth, int screenHeight) {
    int targetFrameTime = 16;
    float movementSpeed = 5.0f;

    float newPosX = player.posArr[0].x + (player.direction.xDelta * movementSpeed);
    float newPosY = player.posArr[0].y + (player.direction.yDelta * movementSpeed);

    //TODO: also check for canvas width and height
    if (newPosX >= 0 && newPosX <= screenWidth - player.width) {
        for (int i = 1; i < arrayCount(player.posArr); ++i) {
          player.posArr[i].x = player.posArr[i - 1].x - (player.direction.xDelta ? (player.direction.xDelta > 0 ? player.width : -player.width) : 0);
        }
        player.posArr[0].x = newPosX;
    }
    if (newPosY >= 0 && newPosY <= screenHeight - player.height) {
        for (int i = 1; i < arrayCount(player.posArr); ++i) {
          player.posArr[i].y = player.posArr[i - 1].y - (player.direction.yDelta ? (player.direction.yDelta > 0 ? player.height : -player.height) : 0);
        }
        player.posArr[0].y = newPosY;
    }

    canvas_clear();
    for (int i = 0; i < arrayCount(player.posArr); ++i) {
        canvas_fillRect(0x555555, player.posArr[i].x, player.posArr[i].y, player.width, player.height);
    }

    return 0;
}
