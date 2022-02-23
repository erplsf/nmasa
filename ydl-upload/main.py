#!/usr/bin/env python3

from yt_dlp import YoutubeDL
import sys

def hook(data):
    if data['status'] == 'downloading':
        print('IN MY HOOK!')
        print(data)

ydl_opts = {
    'progress_hooks': [hook]
}

def main():
    link = sys.argv[1]
    print(f'Got "{link}" as a link, starting yt-dlp...')
    with YoutubeDL(ydl_opts) as ydl:
        ydl.download([link])

if __name__ == "__main__":
   main()
