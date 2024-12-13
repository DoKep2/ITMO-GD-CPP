# CMake generated Testfile for 
# Source directory: /mnt/c/Users/sergo/CLionProjects/lab2
# Build directory: /mnt/c/Users/sergo/CLionProjects/lab2/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(MyTest "/mnt/c/Users/sergo/CLionProjects/lab2/build/runTests")
set_tests_properties(MyTest PROPERTIES  _BACKTRACE_TRIPLES "/mnt/c/Users/sergo/CLionProjects/lab2/CMakeLists.txt;20;add_test;/mnt/c/Users/sergo/CLionProjects/lab2/CMakeLists.txt;0;")
subdirs("lib/googletest")
