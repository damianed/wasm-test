#define assert(expression) if (!(expression)) {__builtin_trap();}

extern void consoleLog(char *string, int size) __attribute__((import_module("env"), import_name("consoleLog")));

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
  return 0;
}
