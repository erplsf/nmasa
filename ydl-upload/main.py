#!/usr/bin/env python3

import sys
import bitmath

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def process_stdin():
    buffer_size = bitmath.MB(100).bytes
    while True:
     try:
        buffer = memoryview(sys.stdin.buffer.read(buffer_size))
        bytes_read = len(buffer)
        if bytes_read == 0:
            break
        nice_repr = bitmath.Byte(bytes_read).best_prefix().format("{value:.2f}{unit}")
        eprint(f'[ydl-upload] Read {nice_repr} from pipe successfully!')
        sys.stdout.buffer.write(buffer)
     except EOFError:
        break

if __name__ == "__main__":
   process_stdin()
