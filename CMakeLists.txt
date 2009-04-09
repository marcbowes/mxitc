# project name
PROJECT(MXIT-CLIENT)

# 2.4.3 is actual minimum, but you have to add additional hacks
# uncomment this if you need 2.4.3 compatibility
# if(COMMAND cmake_policy)
#	 cmake_policy(SET CMP0003 NEW)
# endif(COMMAND cmake_policy)
cmake_minimum_required(VERSION 2.6.0)

# find and setup Qt4 for this project
find_package(Qt4 REQUIRED)

# tell cmake to process CMakeLists.txt in these subdirs
subdirs(src)

