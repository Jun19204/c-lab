# ============================================================
# Core Libraries
# ============================================================
#
# add_library(MyLibrary1 STATIC my_library1.c)
# add_library(MyLibrary2 STATIC my_library2.c)

# ============================================================
# Header-only Libraries
# ============================================================
#
# add_library(MyHeaderOnly_headers INTERFACE)
# target_include_directories(
#   MyHeaderOnly_headers
#   INTERFACE
#     "${PROJECT_SOURCE_DIR}/include"
# )
# target_compile_features(
#   MyHeaderOnly_headers
#   INTERFACE
#     c_std_17
# )

# ============================================================
# Common Static Library Settings
# ============================================================
#
# set(
#   ALL_LIBS
#     MyLibrary1_lib
#     MyLibrary2
# )
#
# foreach(lib IN LISTS ALL_LIBS)
#   target_include_directories(
#     ${lib}
#     PUBLIC
#       "${PROJECT_SOURCE_DIR}/include"
#   )
#   target_compile_features(
#     ${lib}
#     PUBLIC
#       c_std_17
#   )
#   target_link_libraries(
#     ${lib}
#     PRIVATE
#       project_warnings
#       project_sanitizers
#   )
# endforeach()



# ==========================================================
# Add Executable
# ==========================================================

add_executable(MyShell_app myshell.c)
target_link_libraries(
  MyShell_app
  PRIVATE
    project_warnings
    project_sanitizers
)

