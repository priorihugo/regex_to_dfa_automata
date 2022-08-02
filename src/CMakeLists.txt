add_library(sources Automato.h 
                    Automato.cpp)
#target_include_directories(sources ${PROJECT_SOURCE_DIR}/src)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC sources)

add_executable(teste1 teste1.cpp)
target_link_libraries(teste1 PUBLIC sources)
