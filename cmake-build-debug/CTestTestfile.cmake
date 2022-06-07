# CMake generated Testfile for 
# Source directory: C:/code/Zero
# Build directory: C:/code/Zero/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Zero_example-test1 "C:/code/Zero/test/target/Zero_example-test1.exe")
set_tests_properties(Zero_example-test1 PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
add_test(Zero_example-test2 "C:/code/Zero/test/target/Zero_example-test2.exe")
set_tests_properties(Zero_example-test2 PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
add_test(Zero_test-env "C:/code/Zero/test/target/Zero_test-env.exe")
set_tests_properties(Zero_test-env PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
add_test(Zero_test-integrator "C:/code/Zero/test/target/Zero_test-integrator.exe")
set_tests_properties(Zero_test-integrator PROPERTIES  _BACKTRACE_TRIPLES "C:/code/Zero/CMakeLists.txt;20;add_test;C:/code/Zero/CMakeLists.txt;24;Testing;C:/code/Zero/CMakeLists.txt;0;")
subdirs("ext/catch2")
subdirs("test")
