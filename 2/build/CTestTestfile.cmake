# CMake generated Testfile for 
# Source directory: /home/timmyhussain/Documents/16.35/2
# Build directory: /home/timmyhussain/Documents/16.35/2/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TestController1 "test_controller" "fail")
add_test(TestController2 "test_controller" "pass")
add_test(Test_Controller_limit "test_controller" "controller1")
add_test(Test_Controller_negative "test_controller" "controller2")
add_test(Test_Controller_zero "test_controller" "controller3")
