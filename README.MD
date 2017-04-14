# BadAppleDOS
`This project is a work-in-progress.`

A trojan designed to nuke a hard drive and make it play Bad Apple on boot.

## Components

### Data Encoding
#### Movie File Format
```
Binary Data Specification
============================

struct chunk_data {
  unsigned char data;           // Color data of said pixel
  unsigned short repeat_count;  // Number of times the following data repeats
}

struct frame {
  unsigned short chunk_count;   // Number of chunks in this frame
  chunk_data *chunks;           // List of all chunks
}

struct movie_data {
  unsigned short frame_count;   // Total frame count of entire movie
  unsigned char frame_rate;     // Frames to display per second
  frame *frames;                // List of all frames
} movie_binary;

```
### Data Decoding and Rendering

### MBR Write + HDD Format
