#define assert(expression) if (!(expression)) {__builtin_trap();}

#define bool32 int
#define PI 3.14159265358979323846

// NOTE: macro to be able to use js functions more easily without having to write all of this:
//   extern void function_name (args) __attribute__((import_module("env"), import_name("function_name")));
// every time
//  JS_FUNC(void, consoleLog, (char *string, int size)); // converts to:
//  extern void consoleLog(char *string, int size) __attribute__((import_module("env"), import_name("consoleLog")));
#define JS_FUNC(return_type, func_name, func_signature) extern return_type func_name func_signature __attribute__((import_module("env"), import_name(#func_name)));

JS_FUNC(void, consoleLog, (char *string, int size));
// TODO: maybe wrap this functions so there easier to user form here for most cases
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

int stringLength(char *s, int maxLength) {
  int result = 0;
  while (result < maxLength && *s++ != '\0') {
    ++result;
  }
  assert(*--s == '\0');
  return result;
}

typedef struct {
  char *value;
  int length;
} string;

string buildString(char *value) {
  string res = {value, stringLength(value, 255)};
  return res;
}

int main() {
  string output = buildString("Hello world\n");
  consoleLog(output.value, output.length);

  for (int i = 0; i < 1000; ++i) {
    //canvas_clear();
    canvas_fillRect(0x555555, i % 100, 0, 100, 100);
  }
  //
  //canvas_arc(95, 50, 40, 0, 2 * PI, 0);
  return 0;
}
