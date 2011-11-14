set (PROJECT_OUTPUT_NAME Bank)
project(${PROJECT_OUTPUT_NAME})

cmake_minimum_required( VERSION 2.6)

INCLUDE(FindQt4)
find_package( Qt4 REQUIRED )
SET(QT_USE_QTSQL 1)
include( ${QT_USE_FILE})
add_definitions(-DUNICODE)

include_directories(
        src
        ${CMAKE_CURRENT_BINARY_DIR}
)

################################################################
# src
################################################################

set (src_SRC
src/main.cpp
src/mainwindowimpl.cpp
src/qmyadddialog.cpp
src/qmypreferences.cpp
)

set (src_HEADERS
src/mainwindowimpl.h
src/qmyadddialog.h
src/qmypreferences.h
)

################################################################
# all
################################################################
set(
        SOURCES
        ${src_SRC}
)

set(
        HEADERS
        ${src_HEADERS}
)

qt4_wrap_cpp( MOC_SOURCES ${HEADERS} )

set( RESOURCES resource.qrc )

qt4_add_resources( QRC_SOURCES ${RESOURCES} )


add_executable( ${PROJECT_OUTPUT_NAME} ${SOURCES} ${MOC_SOURCES} ${QRC_SOURCES})

target_link_libraries( ${PROJECT_OUTPUT_NAME} ${QT_LIBRARIES} )
