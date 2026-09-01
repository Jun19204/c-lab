function(enable_sanitizers target)

  if(NOT USE_SANITIZER)
    return()
  endif()

  if(MSVC)

    message(STATUS "Enable MSVC AddressSanitizer (ASan)")

    target_compile_options(
      ${target}
      INTERFACE
        /fsanitize=address
    )

    target_link_options(
      ${target}
      INTERFACE
        /fsanitize=address
    )

  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")

    message(STATUS "Enable GCC/Clang ASan + UBSan")

    target_compile_options(
      ${target}
      INTERFACE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(
      ${target}
      INTERFACE
        -fsanitize=address,undefined
    )

  endif()

endfunction()

