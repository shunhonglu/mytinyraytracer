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

# add *extern libraries*
add_subdirectory(ext)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        include/utils.h
        include/mymath.h
        include/Ray.h
        include/Camera.h
        include/Screen.h
        include/Render.h
        include/RenderPara.h
        include/ThreadPool.h
        include/AABB.h
        include/ticktock.h
        include/PDF.h
        include/OBN.h
        include/Hittable/Hittable.h
        include/Hittable/Hittable_list.h
        include/Hittable/Sphere.h
        include/Hittable/BVH_Node.h
        include/Hittable/Triangle.h
        include/Material/Material.h
        include/Material/Light.h
        include/Material/Micorofacet.h
        include/Texture/Texture.h
        include/Texture/Perlin.h
        src/Sphere.cpp
        src/Triangle.cpp
        src/Hittable_list.cpp
        src/BVH_Node.cpp
        src/Render.cpp
        src/RenderPara.cpp
        src/Micorofacet.cpp

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

target_include_directories(mytinyraytracer PUBLIC include)
target_include_directories(mytinyraytracer PUBLIC include/Hittable)
target_include_directories(mytinyraytracer PUBLIC include/Material)
target_include_directories(mytinyraytracer PUBLIC include/Texture)

# link *TBB*
target_link_libraries(mytinyraytracer PUBLIC TBB::tbb)

# link *rt_stb_image_lib*
target_link_libraries(mytinyraytracer PUBLIC rt_stb_image_lib)
target_include_directories(mytinyraytracer PUBLIC ext/rt_stb_image)

# link *spdlog*
target_link_libraries(mytinyraytracer PUBLIC spdlog::spdlog)

#link *rt_tiny_obj_loader_lib*
target_link_libraries(mytinyraytracer PUBLIC rt_tiny_obj_loader_lib)
target_include_directories(mytinyraytracer PUBLIC ext/rt_tiny_obj_loader)

# link *Eigen*
target_include_directories(mytinyraytracer PUBLIC ext/Eigen)

# link *nlohmann*
target_include_directories(mytinyraytracer PUBLIC ext/nlohmann)

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
