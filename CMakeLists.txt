cmake_minimum_required (VERSION 2.6)

#-------------------------------------------------------------------------------
# Имя и версия проекта
#-------------------------------------------------------------------------------
set ( PROJECT Demo )
set ( PROJECT_DESCRIPTION "WEB Server and Buffer Project" )
set ( PROJECT_VERSION_MAJOR 1 )
set ( PROJECT_VERSION_MINOR 0 )
set ( PROJECT_VERSION_PATCH 0 )
project ( ${PROJECT} )


#-------------------------------------------------------------------------------
# Установка переменных CMake
#-------------------------------------------------------------------------------
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../bin )



#-------------------------------------------------------------------------------
# Установка флагов компиляции ( -DCMAKE_BUILD_TYPE=[Debug,Release] )
#-------------------------------------------------------------------------------
set ( CMAKE_CXX_FLAGS "-W -Wall" )
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DBOOST_DISABLE_ASSERTS" )

add_definitions ( "-D_FILE_OFFSET_BITS=64" )

# Sources
file(GLOB HTTP_SERVER_SOURCES "src/*.cpp")

# Headers
file(GLOB HTTP_SERVER_HEADERS "src/*.h")

#-------------------------------------------------------------------------------
# Установка путей для поиска файлов проекта
#-------------------------------------------------------------------------------
include_directories ( 
                       src 
                    )

#-------------------------------------------------------------------------------
# Список библиотек к линковке
#-------------------------------------------------------------------------------
set ( HTTP_SERVER_LIBRARIES
     pthread
    )

#-------------------------------------------------------------------------------
# Cделать бинарник, добавить к линковце цели список библиотек
#-------------------------------------------------------------------------------

#Server
add_executable ( ${PROJECT}_server  ${HTTP_SERVER_HEADERS} ${HTTP_SERVER_SOURCES}  sample/main_server.cpp)
target_link_libraries ( ${PROJECT}_server  ${HTTP_SERVER_LIBRARIES}  )

#Buffer
add_executable ( ${PROJECT}_buffer  ${HTTP_SERVER_HEADERS} ${HTTP_SERVER_SOURCES}  sample/main_buffer.cpp)
target_link_libraries ( ${PROJECT}_buffer  ${HTTP_SERVER_LIBRARIES}  )