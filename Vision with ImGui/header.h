// ImGuiTemplate.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <iostream>

// Common name space for the project
using namespace std;

// ImGui and its dependencies
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// implot
#include <implot.h>

// opencv dependencies 
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

// loading pictures to memory 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Project files
#include "Enum.h"
#include "configuration/Version.h"

// Suppress Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to an 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#endif

inline const char* convertStringToConstChar(const std::string& str) {
    return str.c_str();
}


inline std::string convertIntToString(int number) {
    return std::to_string(number);
}
