# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie hier die globale Konfiguration aus,
# und schließen Sie Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.1)

project ("template")

# Schließen Sie Unterprojekte ein.
add_subdirectory ("template")

set(PROJECT_MAJOR_VERSION 0)
set(PROJECT_MINOR_VERSION 1)
set(PROJECT_PATCH_VERSION 0)


