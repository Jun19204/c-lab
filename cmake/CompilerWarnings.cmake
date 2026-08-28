set(
  PROJECT_WARNING_FLAGS

  # 기본 경고
  -Wall 
  -Wextra
  -Wpedantic

  # C 언어 필수/타입 안전성 경고
  -Wstrict-prototypes
  -Wmissing-prototypes
  -Wimplicit-function-declaration
  -Wvla

  # 포인터 / 한정자 안전성 경고
  -Wcast-qual       # const 한정자를 제거하는 위험한 캐스트 감지
  -Wcast-align      # 잠재적 포인터 미정렬(Alignment) 경고

  # 정적 분석 및 스코프 경고
  -Wshadow          # 변수 섀도잉 방지
  -Wnull-dereference # null 포인터 역참조 정적 분석
)
