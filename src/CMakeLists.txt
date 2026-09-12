# ============================================================
# Core Libraries
# ============================================================

# Add reusable libraries here.
#
# Example:
#
# add_library(
#   MyLibrary
#   STATIC
#   my_library.c
# )
# target_include_directories(
#   MyLibrary
#   PUBLIC
#   "${PROJECT_SOURCE_DIR}/include"
# )
# target_link_libraries(
#   MyLibrary
#   PRIVATE
#   project_warnings
#   project_sanitizers
# )

add_executable(
  MyShell_app
  myshell.c
)
target_link_libraries(
  MyShell_app
  PRIVATE
  project_warnings
  project_sanitizers
)
