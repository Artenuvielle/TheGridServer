# Rename this file to, e.g. linux_64.cmake or vs11_32.cmake, and configure your default settings below.
# In your build-tree-directory call 'cmake -C this-file-with-path path-to-source-code'.
set(USER_CMAKE_TEXT "initialized by '${CMAKE_CURRENT_LIST_FILE}'")

#################################################################################
##### 					WHAT YOU CAN EASILY CHANGE 							#####

# Should your program be compiled with 64bit? If yes, set "FALSE" to "TRUE" in 
# the next line. Be sure you have the right libraries in C:/libraries/install/...!
set(User_use64BitCompiler FALSE CACHE BOOL ${USER_CMAKE_TEXT})

# How should your program be named? Change "MyProject" in the next line
set(User_ProjectName "TheGridServer" CACHE STRING ${USER_CMAKE_TEXT})

# Do you want to build a "Debug" or a "Release" version? Change "Release"
# in the next line if necessary. Please avoid Typos!
set(User_Build_Configuration Release CACHE STRING ${USER_CMAKE_TEXT})


##################################################################################


#####				From here on it gets more complicated					######
#####		If you dont know what you are doing, stop it from here!!!!		######

##################################################################################


set(compiler64 "")
set(libPathArchitecture "vs11_32")
if(User_use64BitCompiler)
	set(compiler64 " Win64")
	set(libPathArchitecture "vs11_64" CACHE STRING ${USER_CMAKE_TEXT})
else()
	set(libPathArchitecture "vs11_32" CACHE STRING ${USER_CMAKE_TEXT})
endif()


set(USER_CMAKE_TEXT "initialized by '${CMAKE_CURRENT_LIST_FILE}'")

set(CMAKE_CONFIGURATION_TYPES ${User_Build_Configuration} CACHE STRING ${USER_CMAKE_TEXT})
if(WIN32)	
	set(CMAKE_GENERATOR "Visual Studio 11${compiler64}" CACHE INTERNAL ${USER_CMAKE_TEXT})
endif()
set(CMAKE_INSTALL_PREFIX "." CACHE PATH ${USER_CMAKE_TEXT})

