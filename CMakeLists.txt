cmake_minimum_required(VERSION 3.16)
project(VolumeReg VERSION 0.0.1)

set(ITK_DIR "D:\\BP\\itk\\ITK-build")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(VolumeRegistration
	"src/Export.cpp"
	"include/VolumeRegistration.h" 
	"include/DcmReader.h" 
	"src/DcmReader.cpp" 
	"include/Export.h" 
	"include/Difference.h" 
	"src/Difference.cpp" 
	"include/Registration.h"
	"src/Registration.cpp"
	"utilities/Logging.cpp"
	"utilities/Logging.h"
	"utilities/Timer.h" "include/RegistrationMonitoring.h" "src/RegistrationMonitoring.cpp" "include/MultiResRegistration.h" "include/RegistrationInterfaceCommand.h")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
target_link_libraries(VolumeRegistration PUBLIC ${ITK_LIBRARIES})

target_include_directories(VolumeRegistration PUBLIC "include" "utilities")