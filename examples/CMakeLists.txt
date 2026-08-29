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
