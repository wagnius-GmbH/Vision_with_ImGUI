# CMakeList.txt: CMake-Projekt für "ImGuiTemplate". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (${PROJECT_NAME} "ImGuiTemplate.cpp"  "header.h" "ImGui.hpp" "Enum.h" "Vision.hpp" "configuration/Version.h.in" "configuration/Version.h")

set(PROJECT_MAJOR_VERSION "0")
set(PROJECT_MINOR_VERSION "1")
set(PROJECT_PATCH_VERSION "0")

set(PROJECT_VERSION    ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION})
set(APP_NAME "Vision with ImGui")

# Debug
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/configuration/readme.md.in"
    "${PROJECT_SOURCE_DIR}/readme.md"
    @ONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/configuration/Version.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/configuration/Version.h"
    @ONLY
)

# Runntime
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/configuration/Version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Version.h"
    @ONLY
)


message("\n************************************")
message("Project directory = ${PROJECT_SOURCE_DIR}")
message("************************************\n")

message("************************************")
message("CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message("************************************\n")

# Install Logos/icons
message("************************************")
message("Image source = ${CMAKE_CURRENT_SOURCE_DIR}/Picts/wagnius.png")
message("Image destiantion = ${CMAKE_CURRENT_BINARY_DIR}/picts/wagnius.png")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/picts/wagnius.png  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/picts)
message("************************************\n")

# Install Logos/icons
message("************************************")
message("Copy Imgui.ini")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/configuration/imgui.ini"  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
message("************************************\n")


# Install haarcascade vision files
message("************************************")
message("Vision source = ${CMAKE_CURRENT_SOURCE_DIR}/Vision/")
message("Vision destiantion = ${CMAKE_CURRENT_BINARY_DIR}/Vision")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Vision/haarcascade_eye_tree_eyeglasses.xml  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Vision)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Vision/haarcascade_frontalface_alt.xml  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Vision)
message("************************************\n")


# ImGui dependecies 
find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(implot CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)


# OpenCV
find_package(OpenCV CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_calib3d)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_core)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_dnn)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_features2d)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_flann)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_highgui)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_imgcodecs)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_imgproc)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_ml)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_objdetect)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_photo)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_stitching)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_video)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_videoio)


# load pict to ram 
find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
