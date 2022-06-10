# CMake generated Testfile for 
# Source directory: /mnt/renderer/Zero
# Build directory: /mnt/renderer/Zero/build-new
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Zero_test-integrator "/mnt/renderer/Zero/test/target/Zero_test-integrator")
set_tests_properties(Zero_test-integrator PROPERTIES  _BACKTRACE_TRIPLES "/mnt/renderer/Zero/CMakeLists.txt;20;add_test;/mnt/renderer/Zero/CMakeLists.txt;24;Testing;/mnt/renderer/Zero/CMakeLists.txt;0;")
add_test(Zero_test-materials "/mnt/renderer/Zero/test/target/Zero_test-materials")
set_tests_properties(Zero_test-materials PROPERTIES  _BACKTRACE_TRIPLES "/mnt/renderer/Zero/CMakeLists.txt;20;add_test;/mnt/renderer/Zero/CMakeLists.txt;24;Testing;/mnt/renderer/Zero/CMakeLists.txt;0;")
subdirs("ext/catch2")
subdirs("test")
