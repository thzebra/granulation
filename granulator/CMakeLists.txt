cmake_minimum_required(VERSION 3.1)
project(granulator LANGUAGES CXX)

set(EXECNAME granulator)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 5.3 REQUIRED COMPONENTS Core Widgets Multimedia)

set (HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Envelope.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SinusoidalEnvelope.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Grain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GranulatorInterface.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DrawingArea.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SequenceStrategy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RegularSequenceStrategy.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SourceData.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomSourceData.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomWindowSource.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Granulator.hpp"
)

set (SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Envelope.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SinusoidalEnvelope.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Grain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GranulatorInterface.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DrawingArea.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SequenceStrategy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RegularSequenceStrategy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomWindowSource.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RandomSourceData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SourceData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Granulator.cpp"
)

add_executable(${EXECNAME} ${HEADERS} ${SOURCES})
target_link_libraries(${EXECNAME} PUBLIC Qt5::Core Qt5::Widgets Qt5::Multimedia m rtaudio)
