@echo off

set cur_dir=%~dp0
set test_dir=%cur_dir%SelfTest\
set target_dir=%test_dir%target\Debug\

cmake %test_dir%build\.. %test_dir%build\.
cmake --build %test_dir%build\.

@REM start %target_dir%SelfTest.exe