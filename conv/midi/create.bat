@echo off
SET /p FILENAME=Enter midi file: 
midicsv-1.1\midicsv %FILENAME%.mid %FILENAME%.csv 