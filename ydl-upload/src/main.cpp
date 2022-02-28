#include <fmt/core.h>
#include <stdio.h>

const size_t chunk_size = 1024;

int main(int argc, char* argv[]) {
  (void)argv;
  if (argc != 2) {
    fmt::print(stderr, "Exactly one argument is expected - video URL!\n");
    return EXIT_FAILURE;
  }
  const auto url = argv[1];
  // fmt::print("url: {}\n", url);
  const auto cmd = fmt::format("yt-dlp -j '{}'", url);
  const auto pipe = popen(cmd.c_str(), "r");
  std::string result;
  std::string buf;
  buf.resize(chunk_size);
  size_t bytes;
  while ((bytes = fread(buf.data(), 1, buf.size(), pipe)) > 0) {
    fmt::print(stderr, "chars read: {}\n", bytes);
    result.append(buf.begin(), buf.begin() + bytes);
  };
  // fmt::print("read: {}", result);
  fmt::print("{}", result);
  pclose(pipe);
  return EXIT_SUCCESS;
}
