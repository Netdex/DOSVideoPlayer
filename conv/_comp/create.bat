@echo off
set LOCATION=D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\badapple w lyric
set PREFIX=bal
set EXTENSION=png
set COUNT=5259
set FPS=24

FrameExtract "%LOCATION%" %PREFIX% %EXTENSION% %COUNT% %FPS%
LZ4FramePack %PREFIX%.bin