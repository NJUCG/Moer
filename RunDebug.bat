@echo off

set cur_dir=%~dp0
set target_dir=%cur_dir%target\Debug\
Copy /Y /V %cur_dir%extlib\assimp-vc142-mt.dll %target_dir%
Copy /Y /V %cur_dir%extlib\assimp-vc142-mt.lib %target_dir%

start %target_dir%app.exe