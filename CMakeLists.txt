cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(mytinyraytracer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# add *TBB*
find_package(TBB REQUIRED)

# add *rt_stb_image* as a library
add_subdirectory(rt_stb_image)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mymath.h
        Ray.h
        Hittable.h
        Sphere.h
        Sphere.cpp
        Hittable_list.h
        Hittable_list.cpp
        Camera.h
        utils.h
        ThreadPool.h
        Material.h
        AABB.h
        BVH_Node.h
        BVH_Node.cpp
        Texture.h
        Perlin.h
        Light.h
        Rect.h
        ticktock.h
        Render.h
        Screen.h
        Scene.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mytinyraytracer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mytinyraytracer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(mytinyraytracer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(mytinyraytracer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# link *TBB*
target_link_libraries(mytinyraytracer PUBLIC TBB::tbb)

target_include_directories(mytinyraytracer PUBLIC Eigen)

# link *rt_stb_image_lib*
target_link_libraries(mytinyraytracer PUBLIC rt_stb_image_lib)
target_include_directories(mytinyraytracer PUBLIC rt_stb_image)

target_link_libraries(mytinyraytracer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(mytinyraytracer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS mytinyraytracer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mytinyraytracer)
endif()
