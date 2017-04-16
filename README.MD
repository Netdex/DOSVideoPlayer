# BadAppleDOS
`This project is a work-in-progress.`

A trojan designed to nuke a hard drive and make it play Bad Apple on boot.
[demo](https://streamable.com/gbiy2)

## Components

### Data Encoding
#### Movie File Format
``` c
/* Video is stored in a series of LZ4 compressed 256-color bitmaps */

/* LZ4 compressed frame */
struct frame_data {
  unsigned char *palette;       // 768 byte palette
  int size;
  unsigned char *data;
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

#### Lyric File Format
```
/* Lyrics are stored in a text file */
[EVENT_COUNT]
[PHONEME] [DELAY]
...
[PHONEME] [DELAY]
```
### Data Decoding and Rendering

### MBR Write + HDD Format
