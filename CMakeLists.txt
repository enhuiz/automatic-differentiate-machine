project(autodiff)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
    src/autodiff/*.h
    src/autodiff/*.cc
)

add_executable(repl ${SOURCES} src/repl.cc)
add_executable(main ${SOURCES} src/main.cc)
add_executable(demo ${SOURCES} src/demo.cc)