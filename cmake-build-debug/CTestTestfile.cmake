# CMake generated Testfile for 
# Source directory: C:/code/Zero
# Build directory: C:/code/Zero/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Zero_test-integrator "C:/code/Zero/test/target/Zero_test-integrator.exe")
set_tests_properties(Zero_test-integrator PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
add_test(Zero_test-materials "C:/code/Zero/test/target/Zero_test-materials.exe")
set_tests_properties(Zero_test-materials PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
add_test(Zero_test-camera "C:/code/Zero/test/target/Zero_test-camera.exe")
set_tests_properties(Zero_test-camera PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
subdirs("ext/catch2")
subdirs("test")
