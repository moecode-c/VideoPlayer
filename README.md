# VideoPlayer

A simple video playlist manager implemented using a **singly linked list** data structure in C++.

## Features

- **Add videos** to the playlist
- **Remove videos** from the playlist
- **Play next** video in the playlist
- **Play current** video
- **Display** entire playlist
- **Reset** to the first video
- Track the current playing video

## Data Structure

This implementation uses a **singly linked list** to manage the video playlist:
- Each node contains a video object and a pointer to the next node
- The VideoPlayer maintains a head pointer and a current pointer
- Videos can be added, removed, and traversed efficiently

## Building and Running

### Prerequisites
- C++ compiler with C++11 support (g++, clang++)
- Make (optional, for using Makefile)

### Build using Make
```bash
make
```

### Build manually
```bash
g++ -std=c++11 -Wall -Wextra -o videoplayer VideoPlayer.cpp
```

### Run the program
```bash
./videoplayer
```

or with Make:
```bash
make run
```

### Clean build files
```bash
make clean
```

## Implementation Details

### Classes

1. **Video**: Represents a video with title, URL, and duration
2. **Node**: Represents a node in the singly linked list
3. **VideoPlayer**: Manages the playlist using a singly linked list

### Key Methods

- `addVideo(title, url, duration)`: Adds a new video to the end of the playlist
- `removeVideo(title)`: Removes a video from the playlist by title
- `playNext()`: Advances to and plays the next video
- `playCurrent()`: Displays the current video
- `displayPlaylist()`: Shows all videos in the playlist
- `reset()`: Resets playback to the first video
- `getSize()`: Returns the number of videos in the playlist

## Example Output

The program demonstrates the VideoPlayer functionality by:
1. Adding multiple videos to the playlist
2. Displaying the complete playlist
3. Playing videos sequentially
4. Removing a video from the playlist
5. Resetting to the first video

## Memory Management

The implementation includes proper memory management:
- Dynamically allocated videos and nodes are properly freed in the destructor
- No memory leaks when videos are removed or the player is destroyed