cmake_minimum_required(VERSION 3.20)
project(car_dodge
        VERSION 0.1.0.0
        LANGUAGES CXX
        HOMEPAGE_URL "https://github.com/khromenokroman/car_dodge"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(car_dodge
        src/main.cpp
        src/game_data.cpp
)

target_link_libraries(car_dodge raylib)

install(TARGETS car_dodge RUNTIME DESTINATION bin)
install(DIRECTORY src/images/ DESTINATION share/car_dodge/images)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "car-dodge")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "khromenokroman")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "khromenokroman")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS TRUE)
include(CPack)
