# ============================================================
# Examples
# ============================================================

# Add example executables here.

add_executable(
  palindrome_app
  palindrome-main.c
)
target_link_libraries(
  palindrome_app
  PRIVATE
  project_warnings
  project_sanitizers
)

add_executable(
  iostream_app
  iostream.c
)
target_link_libraries(
  iostream_app
  PRIVATE
  project_warnings
  project_sanitizers
)
