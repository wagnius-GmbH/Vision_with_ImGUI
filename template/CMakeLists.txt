# CMakeList.txt: CMake-Projekt für "ImGuiTemplate". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (${PROJECT_NAME} "ImGuiTemplate.cpp"  "header.h" "ImGui.hpp" "Enum.h" "Vision.hpp")

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
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
