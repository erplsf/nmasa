#include <fmt/core.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

// READ LATER:
// https://stackoverflow.com/questions/59526181/multipart-upload-s3-using-aws-c-sdk

// const size_t chunk_size = 1024;
// pipe ends: read -> write
// class ExecR {
// public:
//   ExecR() { pipe(fd); }
//   ~ExecR() {
//     for (const auto descriptor: fd) {
//       if (descriptor != -1)
//         close(descriptor);
//     }
//   }

// private:
//   int fd[2]; // read -> write
//   int es;
// }

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fmt::print(stderr, "Exactly one argument is expected - video URL!\n");
    return EXIT_FAILURE;
  }
  const auto url = argv[1];
  (void)url;

  // const auto message = fmt::format("[{} L{}]: message", __FILE__, __LINE__);
  // fmt::print("{}\n", message);

  int o_pipe[2];
  if (pipe(o_pipe) != 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  };

  const auto pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // child
    fmt::print("I'm a child!\n");
    close(o_pipe[0]);               // close input in child
    dup2(o_pipe[1], STDOUT_FILENO); // TODO: handle exit codes
    dup2(o_pipe[1], STDERR_FILENO);
    close(o_pipe[1]); // close output after duplicating it
    const char* args[] = {"ytp-dlp", NULL};
    execvp("yt-dlp", const_cast<char* const*>(args));
  } else { // parent
    fmt::print("I'm a parent!\n");
    close(o_pipe[1]); // close output in parent
    int status;
    const auto wpid = waitpid(pid, &status, 0);
    if (wpid == -1) {
      fmt::print(stderr, "waitpid\n");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      int es = WEXITSTATUS(status);
      fmt::print("Child exited with status: {}!\n", es);
    }
  }

  std::vector<std::byte> data;
  data.reserve(1e+8); // 100 MB
  (void)data;

  return EXIT_SUCCESS;
}
