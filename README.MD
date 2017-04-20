# BadAppleDOS
`This project is a work-in-progress.`

Rule 48: If it exists, bad apple can run on it.
[demo](https://streamable.com/84inx)

## FAQ
**Q:** What is this?<br>
**A:** Basically just a small video player written in C for DOS.

**Q:** Why is the audio so terrible?<br>
**A:** I wanted it to work on modern computers, and modern sound cards are quite complicated to set up, so my only option was the internal speaker.

**Q:** Why is the video so terrible?<br>
**A:** I'm sticking with VGA mode 13h, so enjoy it in its 240 color, 320x200 pixel glory. (16 colors reserved for text palette)

**Q:** How do I run this?<br>
**A:** Try to read some of the documentation, or you could just email me.

## Workflow
1. Export raw video file as series of PNG files (must be 320x200) at a reasonable FPS (less than 70fps). Exported files must be in format prefix_000000.png in ascending order.

2. Modify parameters of conv/\_comp/create.bat
```
set LOCATION=[full path to folder storing PNG files]
set PREFIX=[prefix of all png files]
set EXTENSION=[extension (usually png)]
set COUNT=[number of frames]
set FPS=[frames per second]
```

3. Run create.bat, it will extract and compress the frame data

4. Copy the .lz4hc-64000 file to dos/bin and rename it accordingly

## Components
### Data Encoding
#### Movie File Format
``` c
/* 
 * Video is stored in a series of LZ4 compressed 256-color bitmaps 
 * We update the palette every frame
 */

/* LZ4 compressed frame */
struct frame_data {
  unsigned char palette_size;   // palette size minus one, since 256 is the limit
  unsigned char *palette;       // 18-bit color palette
  int size;
  unsigned char *data;
};

struct video_data {
  unsigned short frame_count;   // total frame count of entire movie
  unsigned char frame_rate;     // frames to display per second
  struct frame_data *frames;    // compressed frame data
} video_binary;

```

Tools for compressing video files are in conv/\_comp

FrameExtract.exe extracts frames out of a series of PNG files, and exports them to a large binary, its source is at conv/\_utilties/FramePack/FrameExtract

LZ4FramePack.exe compresses the frames with the LZ4 compression algorithm, its source is at conv/\_utilities/LZ4FramePack

#### Audio File Format
``` c
/* 
 * Audio is stored in a series of single channel beepcodes
 * We can only use the internal speaker for generating sound,
 * since newer computers don't have SoundBlaster chips
 */

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

Tools for compressing audio files are in conv/midi, edit then run create.bat with the proper parameters.

Relies on AudioPack.exe, whose source is at conv/\_utilities/FramePack/AudioPack
