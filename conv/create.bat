@echo off
set LOCATION=D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\badapple
set PREFIX=badapple
set EXTENSION=png
set COUNT=5260
set FPS=24

FrameExtract "%LOCATION%" %PREFIX% %EXTENSION% %COUNT% %FPS%
LZ4FramePack badapple.bin