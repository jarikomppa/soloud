@echo off
del soloud.gmez
cd gm_temp
7z a -r -y ../../glue/soloud.gmez * 
cd ..
rd /s /q gm_temp
del soloud_gamemaker_dll.*
del soloud.extension.gmx
