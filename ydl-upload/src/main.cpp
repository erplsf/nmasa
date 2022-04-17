#include <fmt/core.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "subprocess.hpp"

// READ LATER:
// https://stackoverflow.com/questions/59526181/multipart-upload-s3-using-aws-c-sdk

// const size_t chunk_size = 1024;
// pipe ends: read -> write

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fmt::print(stderr, "Exactly one argument is expected - video URL!\n");
    return EXIT_FAILURE;
  }
  const auto url = argv[1];
  (void)url;

  // const auto message = fmt::format("[{} L{}]: message", __FILE__, __LINE__);
  // fmt::print("{}\n", message);

  // int o_pipe[2];
  // if (pipe(o_pipe) != 0) {
  //   perror("pipe");
  //   exit(EXIT_FAILURE);
  // };

  // const auto pid = fork();

  // if (pid == -1) {
  //   perror("fork");
  //   exit(EXIT_FAILURE);
  // }

  // if (pid == 0) { // child
  //   fmt::print("I'm a child!\n");
  //   close(o_pipe[0]);               // close input in child
  //   dup2(o_pipe[1], STDOUT_FILENO); // TODO: handle exit codes
  //   dup2(o_pipe[1], STDERR_FILENO);
  //   close(o_pipe[1]); // close output after duplicating it
  //   const char* args[] = {"ytp-dlp", NULL};
  //   execvp("yt-dlp", const_cast<char* const*>(args));
  // } else { // parent
  //   fmt::print("I'm a parent!\n");
  //   close(o_pipe[1]); // close output in parent
  //   int status;
  //   const auto wpid = waitpid(pid, &status, 0);
  //   if (wpid == -1) {
  //     fmt::print(stderr, "waitpid\n");
  //     exit(EXIT_FAILURE);
  //   }
  //   if (WIFEXITED(status)) {
  //     int es = WEXITSTATUS(status);
  //     fmt::print("Child exited with status: {}!\n", es);
  //   }
  // }

  const char* args[] = {"ytp-dlp", "-j", url, NULL};
  Subprocess sp{"yt-dlp", args};

  const auto o_pipe = sp.out_pipe();
  std::string data;
  const std::string::size_type size = 1e+6; // 1 MB
  data.resize(size);

  auto bytes_read = 0;
  auto total_bytes_read = 0;
  std::string::size_type free_space = 0;
  while ((bytes_read = read(o_pipe, data.data() + total_bytes_read, size)) >
         0) {
    // fmt::print("bytes read: {}\n", bytes_read);
    if (bytes_read == -1) {
      perror("main read");
      exit(EXIT_FAILURE);
    }
    total_bytes_read += bytes_read;
    free_space = data.size() - total_bytes_read;
    if (free_space < size) {
      data.resize(data.size() + size);
      // fmt::print(stderr, "resizing!\n");
    }
    free_space = data.capacity() - data.size();
  }

  fmt::print("{}", data);

  // std::vector<std::byte> data;
  // data.reserve(1e+8); // 100 MB
  // (void)data;

  return EXIT_SUCCESS;
}
