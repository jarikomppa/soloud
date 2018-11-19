@echo off
call vcvars32.bat
cl /LD /MT /Ox /nologo soloud_gamemaker_dll.c soloud_gamemaker_dll.def user32.lib  
