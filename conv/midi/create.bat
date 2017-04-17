@echo off
SET FILENAME=ba_mono
SET LOCATION=D:\Security\VMSHARED\Development\code\BadAppleDOS\conv\midi
SET FPS=144

midicsv-1.1\midicsv %FILENAME%.mid %FILENAME%.csv 
AudioPack "%LOCATION%" "%FILENAME%" %FPS%