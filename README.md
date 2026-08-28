# C 학습용 저장소

C와 Modern CMake를 기반으로 여러 C 학습 프로젝트, 실험 코드, 재사용 가능한 라이브러리 및 테스트를 하나의 Repository에서 관리하기 위한 저장소입니다.

각 구성 요소는 독립적인 CMake Target으로 관리하며, 공통 Compiler Warning, Sanitizer 및 Test 환경을 공유할 수 있습니다.

---

## Features

* C
* Modern CMake
* CMake Presets
* Library / Executable / Test 분리
* 공통 Compiler Warning 설정
* AddressSanitizer + UndefinedBehaviorSanitizer 지원
* Valgrind 검사 환경 지원
* GoogleTest 기반 Unit Test
* `compile_commands.json` 생성 지원
* 확장 가능한 Semi-Monorepo 구조

