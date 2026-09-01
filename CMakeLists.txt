cmake_minimum_required(VERSION 3.23)

project(
  C-LAB
  VERSION 1.0.0
  LANGUAGES C
)

# ============================================================
# MSVC Hot Reload / Debug Information
# ============================================================
if(MSVC)
  if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)

    set(
      CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<C_COMPILER_ID:MSVC>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
  endif()
endif()

# ============================================================
# C Standard
# ============================================================
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ============================================================
# Options
# ============================================================
option(
  USE_SANITIZER
  "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
  OFF
)

option(
  BUILD_EXAMPLES
  "Build example executables"
  ON
)

# ============================================================
# Testing
# ============================================================
include(CTest)

# ============================================================
# CMake Modules
# ============================================================
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(CompilerWarnings)
include(Sanitizers)

# ============================================================
# Common Warnings
# ============================================================
add_library(project_warnings INTERFACE)

target_compile_options(
  project_warnings
  INTERFACE
    $<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNING_FLAGS}>
)

# ============================================================
# Sanitizers
# ============================================================
add_library(project_sanitizers INTERFACE)

enable_sanitizers(project_sanitizers)

# ============================================================
# Core Libraries
# ============================================================
add_subdirectory(src)

# ============================================================
# Examples
# ============================================================
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# ============================================================
# Tests
# ============================================================
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

