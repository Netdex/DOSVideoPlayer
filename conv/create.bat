@echo off
set LOCATION=D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\frames\gup katyusha
set PREFIX=katyusha
set EXTENSION=png
set COUNT=1466
set FPS=24

FrameExtract "%LOCATION%" %PREFIX% %EXTENSION% %COUNT% %FPS%
LZ4FramePack %PREFIX%.bin