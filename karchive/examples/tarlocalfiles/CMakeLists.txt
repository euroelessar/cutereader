cmake_minimum_required(VERSION 2.8.12)

project(TarLocalFiles)

find_package(KArchive "5.0.0" REQUIRED)

add_executable(tarlocalfiles main.cpp)
target_link_libraries(tarlocalfiles KF5::KArchive)
