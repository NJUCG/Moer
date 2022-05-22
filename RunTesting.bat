@echo off

set cur_dir=%~dp0
set target_dir=%cur_dir%target\Debug\

cmake %cur_dir%build\.. -B %cur_dir%build\.
cmake --build %cur_dir%build\. 

cd build
ctest -C DEBUG
cd ..