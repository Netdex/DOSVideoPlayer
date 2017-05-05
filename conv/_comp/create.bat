@echo off
set LOCATION=D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\rickroll
set PREFIX=rickroll
set EXTENSION=png
set COUNT=3181
set FPS=15
set COLORS=128

FrameExtract "%LOCATION%" %PREFIX% %EXTENSION% %COUNT% %FPS% %COLORS%
LZ4FramePack %PREFIX%.bin