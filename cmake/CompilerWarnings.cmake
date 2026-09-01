if(MSVC)

  set(
    PROJECT_WARNING_FLAGS
    /W4
  )

elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")

  set(
    PROJECT_WARNING_FLAGS

    # 기본 경고
    -Wall
    -Wextra
    -Wpedantic

    # 타입 변환
    -Wconversion
    -Wsign-conversion

    # 포인터 / 타입 안전성
    -Wcast-qual
    -Wcast-align
    -Wpointer-arith

    # 함수 선언 / 프로토타입
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wmissing-declarations

    # 선언 / 초기화
    -Wshadow
    -Wundef
    -Winit-self

    # 문자열 / printf 계열
    -Wformat=2

    # 논리 / 제어 흐름
    -Wswitch
    -Wswitch-enum

    # 기타 위험 요소
    -Wnull-dereference
    -Wdouble-promotion
    -Wvla
  )

  # GCC 전용
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")

    list(
      APPEND PROJECT_WARNING_FLAGS
      -Wlogical-op
      -Wduplicated-cond
      -Wduplicated-branches
      -Wrestrict
    )

  # Clang / AppleClang 전용
  elseif(CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang")

    list(
      APPEND PROJECT_WARNING_FLAGS
      -Wcomma
      -Warray-bounds-pointer-arithmetic
    )

  endif()

endif()

