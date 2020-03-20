cmake_minimum_required(VERSION 3.15)
project(RawCv)

set(CMAKE_CXX_STANDARD 14)

message(svb-loading-library-for-thread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )

find_package( OpenCV REQUIRED )
add_executable(RawCv src/main.cpp
        src/application/config/AppConfig.h src/application/config/AppConfig.cpp
        src/application/controllers/MainLogic.h src/application/controllers/MainLogic.cpp
        src/application/controllers/MyImageAnalyser.h src/application/controllers/MyImageAnalyser.cpp
        src/application/controllers/StateController.h src/application/controllers/StateController.cpp
        src/application/controllers/RoboticArmController.h src/application/controllers/RoboticArmController.cpp
        src/application/logic/InputStorage.h src/application/logic/InputStorage.cpp
        src/application/logic/image/ImageDisplayItem.h
        src/application/logic/image/ImagePreprocessItem.h
        src/application/logic/message/StatusMessageItem.h
        src/application/logic/message/MessageComposer.h src/application/logic/message/MessageComposer.cpp
        src/application/logic/message/StatusMessageStorage.h src/application/logic/message/StatusMessageStorage.cpp
        src/application/logic/robotic_arm/MoveVisualization.h
        src/application/logic/robotic_arm/RoboticArm.h
        src/application/logic/robotic_arm/RoboticArmMove.h
        src/application/logic/servo/ServoIface.h
        src/application/logic/servo/ServoMove.h
        src/application/logic/servo/ServoRange.h
        src/application/logic/servo/ServoRotationFB.h
        src/application/logic/servo/ServoRotationLR.h
        src/application/modules/PickUpObject.h
        src/application/modules/iface/CVModule.h

        src/libs/json/JSON.h src/libs/json/JSON.cpp
        src/libs/json/JSONValue.h src/libs/json/JSONValue.cpp
        src/libs/usb/UsbCommunicator.h src/libs/usb/UsbCommunicator.cpp

        src/system/config/Constants.h
        src/system/controllers/DebugFrames.h src/system/controllers/DebugFrames.cpp
        src/system/controllers/ImageAnalyser.h src/system/controllers/ImageAnalyser.cpp
        src/system/filters/BinaryFilter.h src/system/filters/BinaryFilter.cpp
        src/system/helper/BBHelper.h src/system/helper/BBHelper.cpp
        src/system/helper/ColorManipulation.h src/system/helper/ColorManipulation.cpp
        src/system/helper/DrawMethodHelper.h src/system/helper/DrawmethodHelper.cpp
        src/system/helper/FileSystemHelper.h src/system/helper/FileSystemHelper.cpp
        src/system/helper/MathHelper.h
        src/system/helper/MyLog.h
        src/system/helper/RotateBBHelper.h src/system/helper/RotateBBHelper.cpp
        src/system/helper/StringHelper.h src/system/helper/StringHelper.cpp
        src/system/helper/TimeHelper.h src/system/helper/TimeHelper.cpp
        src/system/logic/input/ProcessingFacade.h src/system/logic/input/ProcessingFacade.cpp
        src/system/logic/input/keyboard/abstract/Move.h
        src/system/logic/input/path/CameraPath.h
        src/system/logic/input/path/DirPath.h
        src/system/logic/input/path/SourcePath.h
        src/system/logic/input/path/UrlPath.h
        src/system/logic/input/path/VideoPath.h
        src/system/logic/input/processing/DirImageFrameProcessing.h src/system/logic/input/processing/DirImageFrameProcessing.cpp
        src/system/logic/input/processing/FrameProcessing.h
        src/system/logic/input/processing/VideoFrameProcessing.h src/system/logic/input/processing/VideoFrameProcessing.cpp
        src/system/logic/input/processing/VideoFrameProcessingLocalCamera.h src/system/logic/input/processing/VideoFrameProcessingLocalCamera.cpp
        src/system/logic/input/processing/VideoFrameProcessingRunEveryFrame.h src/system/logic/input/processing/VideoFrameProcessingRunEveryFrame.cpp

        src/testing/abstract/TestIface.h
        src/testing/RoboticArmControllingTest.h src/testing/RoboticArmControllingTest.cpp
        )
message(svb-load-opencv-libs)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(RawCv ${OpenCV_LIBS})