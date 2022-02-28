#include <fmt/core.h>

int main(int argc, char *argv[]) {
  fmt::print("got {} args.\n", argc);
  (void)argv;
}
