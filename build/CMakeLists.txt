project(Druid)

include_directories(
        ../code
        ../code/compiler_tools
        ../code/data_structures
        ../code/tests
)
set(SOURCE_FILES ../code/druid.c)
add_executable(druid ${SOURCE_FILES})