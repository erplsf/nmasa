#ifndef SUBPROCESS_H_
#define SUBPROCESS_H_

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

class Subprocess {
public:
  Subprocess(const char* file, const char* args[]) {
    int out_fd[2];
    int err_fd[2];

    if (pipe(out_fd) != 0) {
      perror("pipe in subprocess");
      exit(EXIT_FAILURE);
    }

    if (pipe(err_fd) != 0) {
      perror("pipe in subprocess");
      exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
      perror("fork in subprocess");
      exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child
      if (close(out_fd[0]) == -1) {
        perror("close in child");
        exit(EXIT_FAILURE);
      }
      if (close(err_fd[0]) == -1) {
        perror("close in child");
        exit(EXIT_FAILURE);
      }
      if (dup2(out_fd[1], STDOUT_FILENO) == -1) {
        perror("dup2 in child");
        exit(EXIT_FAILURE);
      }
      if (close(out_fd[1]) == -1) {
        perror("close in child");
        exit(EXIT_FAILURE);
      }
      if (dup2(err_fd[1], STDERR_FILENO) == -1) {
        perror("dup2 in child");
        exit(EXIT_FAILURE);
      }
      if (close(err_fd[1]) == -1) {
        perror("close in child");
        exit(EXIT_FAILURE);
      }

      if (execvp(file, const_cast<char* const*>(args)) == -1) {
        perror("execvp in child");
        exit(EXIT_FAILURE);
      }
    } else { // parent
      if (close(out_fd[1]) == -1) {
        perror("close in parent");
        exit(EXIT_FAILURE);
      };
      if (close(err_fd[1]) == -1) {
        perror("close in parent");
        exit(EXIT_FAILURE);
      };
      out = out_fd[0];
      err = err_fd[0];
    }
  }

  ~Subprocess() {
    if (out != -1) {
      close(out);
    }
    if (err != -1) {
      close(err);
    }
  }

  int out_pipe() const { return out; };
  int err_pipe() const { return err; };

private:
  int out;
  int err;
  pid_t pid;
};

#endif // SUBPROCESS_H_
