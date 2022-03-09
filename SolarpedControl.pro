QT += widgets serialport multimedia

TEMPLATE = app
#CONFIG -= app_bundle
CONFIG += no_keywords #Solve Python SLOT ISSUE
CONFIG += c++14

SOURCES += \
    file_writer.cpp \
    image_processor.cpp \
    main.cpp \
    serial_port_writer.cpp \
    solarped_control_interface.cpp \
    solarped_control_interface_logic.cpp \
    trainer_controller.cpp

HEADERS += \
    environment.h \
    file_writer.h \
    image_processor.h \
    model.h \
    reinforcement_learning.h \
    serial_port_writer.h \
    solarped_control_interface.h \
    solarped_control_interface_logic.h \
    trainer_controller.h \
    utilities.h


RESOURCES +=

INCLUDEPATH += -I "C:\Qt\libtorch\include"
INCLUDEPATH += -I "C:\Qt\libtorch\include\torch\csrc\api\include"
INCLUDEPATH += -I "C:\Program Files (x86)\Eigen3\include\eigen3"

#INCLUDEPATH += -I "C:\Users\Vulcan\AppData\Local\Programs\Python\Python39\include"
#INCLUDEPATH += -I "C:\Program Files\Python36\include"
INCLUDEPATH += -I "C:\Qt\opencv\build\include"
INCLUDEPATH += -I "C:\Qt\opencv_contrib\modules\tracking\include"
INCLUDEPATH += -I "C:\Qt\opencv_contrib\modules\cudev\include"
INCLUDEPATH += -I "C:\Qt\qtcsv\include"
#INCLUDEPATH += "C:\Program Files\Python36"


#LIBS += -L"C:\Program Files\Python36"
#LIBS += "C:\Program Files\Python36\python3.dll"
#LIBS += "C:\Program Files\Python36\python36.dll"
#LIBS += -L"C:\Program Files\Python36\libs" -l"python3" -l"python36"

LIBS += -LC:\Qt\libtorch\lib
LIBS += C:\Qt\libtorch\lib\*.lib
#LIBS += C:\Qt\libtorch\lib\*.dll

LIBS += -LC:\Qt\qtcsv\build\Release
#LIBS += -L"C:\Qt\opencv-build\bin"
LIBS += -LC:\Qt\opencv-build\lib\Release
LIBS += -LC:\Qt\opencv-build\bin\Release
LIBS += C:\Qt\opencv-build\lib\Release\*.lib
#LIBS += C:\Qt\opencv-build\bin\Release\*.dll
#LIBS += C:\opencv-build\bin\libopencv_calib3d454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_core454.dll
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_core454.lib
#LIBS += C:\opencv-build\bin\libopencv_dnn454.dll
#LIBS += C:\opencv-build\bin\libopencv_features2d454.dll
#LIBS += C:\opencv-build\bin\libopencv_flann454.dll
#LIBS += C:\opencv-build\bin\libopencv_gapi454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_highgui454.dll
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_highgui454.lib
#LIBS += C:\opencv-build\bin\libopencv_imgcodecs454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_imgproc454.dll
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_imgproc454.lib
#LIBS += C:\opencv-build\bin\libopencv_ml454.dll
#LIBS += C:\opencv-build\bin\libopencv_objdetect454.dll
#LIBS += C:\opencv-build\bin\libopencv_photo454.dll
#LIBS += C:\opencv-build\bin\libopencv_stitching454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_video454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_videoio454.dll
#LIBS += C:\Qt\opencv-build\bin\libopencv_tracking454.dll
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_video454.lib
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_videoio454.lib
##LIBS += C:\Qt\opencv-build\lib\Release\opencv_tracking454.lib
#LIBS += C:\opencv-build\bin\opencv_videoio_ffmpeg454_64.dll
#LIBS += C:\Qt\qtcsv\build\libqtcsv.dll
LIBS += C:\Qt\qtcsv\build\Release\*.lib


#LIBS += -L"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\lib\x64"
#LIBS += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\lib\x64\*.lib"

#unix {
#QMAKE_LFLAGS += -Wl,--no-as-needed
#}
#win32 {
#QMAKE_LFLAGS += -INCLUDE:?warp_size@cuda@at@@YAHXZ
#QMAKE_LFLAGS += -INCLUDE:?searchsorted_cuda@native@at@@YA?AVTensor@2@AEBV32@0_N1@Z
#QMAKE_LFLAGS += /machine:x64
#}



DISTFILES +=

