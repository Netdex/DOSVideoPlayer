# BadAppleDOS
`This project is a work-in-progress.`

A trojan designed to nuke a hard drive and make it play Bad Apple on boot.

## Components

### Data Encoding
#### Movie File Format
``` c
/* Video is stored in a series of RLE compressed bitmaps */

struct chunk_data {
  unsigned char data;           // Color data of said pixel
  unsigned short repeat_count;  // Number of times the following data repeats
};

struct frame_data {
  unsigned short chunk_count;   // Number of chunks in this frame
  struct chunk_data *chunks;    // List of all chunks
};

struct video_data {
  unsigned short frame_count;   // Total frame count of entire movie
  unsigned char frame_rate;     // Frames to display per second
  struct frame_data *frames;    // List of all frames
} video_binary;

```

#### Audio File Format
``` c
/* Audio is stored in a series of single channel beepcodes */

struct frame_data {
  unsigned short frame;
  unsigned char action;
  unsigned short frequency;
};

struct audio_data {
  unsigned short frame_count;
  struct frame_data *frames;
} audio_binary;

```
### Data Decoding and Rendering

### MBR Write + HDD Format
