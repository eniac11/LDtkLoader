cmake_minimum_required(VERSION 3.10)

project(LDtkLoader
        VERSION 1.5.3.1
        DESCRIPTION "A C++11 loader for projects and levels created with LDtk"

)
if (NOT PROJECT_VERSION_TWEAK)
    set(PROJECT_VERSION_TWEAK 0)
endif()

# include(FeatureFindPackage)

# feature_find_package(nlohmann_json 3.11 REQUIRED)
# feature_find_package(Doxygen OPTIONAL COMPONENTS dot)

# defer_on_missing_required_packages()

find_package(nlohmann_json 3.11 REQUIRED)
find_package(Doxygen OPTIONAL_COMPONENTS dot)


include(CMakeDependentOption)

# build options
option(LDTK_NO_THROW             "Replace all throws by an error print and exit" OFF)


option(BUILD_EXAMPLES "Build examples using LDtkLoader" OFF)
cmake_dependent_option(LDTK_BUILD_SFML_EXAMPLE   "Build an example using LDtkLoader with SFML." OFF BUILD_EXAMPLES OFF)
cmake_dependent_option(LDTK_BUILD_SDL_EXAMPLE    "Build an example using LDtkLoader with SDL."  OFF BUILD_EXAMPLES OFF)
cmake_dependent_option(LDTK_BUILD_RAYLIB_EXAMPLE "Build an example using LDtkLoader with raylib." OFF BUILD_EXAMPLES OFF)
cmake_dependent_option(LDTK_BUILD_API_TEST       "Build an example that loads a file showcasing all features of LDtk."  OFF BUILD_EXAMPLES OFF)

option(LDTK_FIELD_PUBLIC_OPTIONAL "Set to ON to enable the full optional interface on Field structures" OFF)
mark_as_advanced(LDTK_FIELD_PUBLIC_OPTIONAL)

configure_file(${PROJECT_SOURCE_DIR}/src/Version.hpp.in ${PROJECT_BINARY_DIR}/include/LDtkLoader/Version.hpp)

# create the static library target
add_library(LDtkLoader)
add_library(LDtkLoader::LDtkLoader ALIAS LDtkLoader)

target_sources(LDtkLoader
    PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS include ${PROJECT_BINARY_DIR}/include
    FILES
        include/LDtkLoader/DataTypes.hpp
        include/LDtkLoader/Entity.hpp
        include/LDtkLoader/Tileset.hpp
        include/LDtkLoader/Tile.hpp
        include/LDtkLoader/Field.hpp
        include/LDtkLoader/Utils.hpp
        include/LDtkLoader/Enum.hpp
        include/LDtkLoader/defs/EntityDef.hpp
        include/LDtkLoader/defs/FieldDef.hpp
        include/LDtkLoader/defs/LayerDef.hpp
        include/LDtkLoader/Layer.hpp
        include/LDtkLoader/World.hpp
        include/LDtkLoader/containers/TagsContainer.hpp
        include/LDtkLoader/containers/FieldsContainer.hpp
        include/LDtkLoader/Level.hpp
        include/LDtkLoader/Project.hpp
        ${PROJECT_BINARY_DIR}/include/LDtkLoader/Version.hpp
    PRIVATE
        src/TagsContainer.cpp
        src/DataTypes.cpp
        src/Tile.cpp
        src/Version.hpp.in
        src/Level.cpp
        src/EntityDef.cpp
        src/World.cpp
        src/Layer.cpp
        src/Utils.cpp
        src/FieldsContainer.cpp
        src/Tileset.cpp
        src/LayerDef.cpp
        src/Entity.cpp
        src/Enum.cpp
        src/Project.cpp

)
target_link_libraries(LDtkLoader PUBLIC nlohmann_json::nlohmann_json)

target_precompile_headers(LDtkLoader PRIVATE [[<nlohmann/json.hpp>]])
if (CMAKE_DISABLE_PRECOMPILE_HEADERS)
    target_compile_definitions(LDtkLoader PRIVATE NO_LDtkLoader_PRECOMIPLED_HEADERS)
endif()

# set compile options
if (LDTK_NO_THROW)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_NO_THROW JSON_NOEXCEPTION)
endif()
if (MSVC)
    target_compile_definitions(LDtkLoader PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if (LDTK_FIELD_PUBLIC_OPTIONAL)
    target_compile_definitions(LDtkLoader PRIVATE LDTK_FIELD_PUBLIC_OPTIONAL)
endif()

# set library target properties
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 17
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (Doxygen_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            doxygen-awesome-css
            GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
            GIT_TAG v2.4.2
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)

    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_HTML_COLORSTYLE AUTO_DARK)
    set(DOXYGEN_HTML_DYNAMIC_SECTIONS YES)
    set(DOXYGEN_UML_LOOK YES)
    set(DOXYGEN_DOT_UML_DETAILS YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_CASE_SENSE_NAMES YES)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/doxygen)
    set(DOXYGEN_HTML_HEADER ${AWESOME_CSS_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        ${AWESOME_CSS_DIR}/doxygen-awesome.css
#        ${AWESOME_CSS_DIR}/doxygen-custom/custom.css
        ${AWESOME_CSS_DIR}/doxygen-awesome-sidebar-only.css
        ${AWESOME_CSS_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
        ${AWESOME_CSS_DIR}/doxygen-custom/theme-round.css
        ${AWESOME_CSS_DIR}/doxygen-custom/theme-robot.css
        ${PROJECT_SOURCE_DIR}/custom.css
    )
    set(DOXYGEN_HTML_EXTRA_FILES

            ${AWESOME_CSS_DIR}/doxygen-awesome-darkmode-toggle.js
            ${AWESOME_CSS_DIR}/doxygen-awesome-fragment-copy-button.js
            ${AWESOME_CSS_DIR}/doxygen-awesome-interactive-toc.js
            ${AWESOME_CSS_DIR}/doxygen-awesome-tabs.js
            ${AWESOME_CSS_DIR}/doxygen-awesome-paragraph-link.js
            ${AWESOME_CSS_DIR}/doxygen-custom/toggle-alternative-theme.js
    )
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_CLANG_ASSISTED_PARSING YES)
    set(DOXYGEN_CLANG_DATABASE_PATH ${CMAKE_BINARY_DIR})

    get_property(LDTKLOADER_SOURCES TARGET LDtkLoader PROPERTY HEADER_SET_public_headers )

    doxygen_add_docs(LDtkLoader_docs ${LDTKLOADER_SOURCES} README.md)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/doxygen/html DESTINATION ${CMAKE_INSTALL_DOCDIR} )
endif()

# add examples subdirectories whn requested
if (LDTK_BUILD_SFML_EXAMPLE)
    message(STATUS "LDtkSFML target available.")
    add_subdirectory(examples/SFML)
endif()

if (LDTK_BUILD_SDL_EXAMPLE)
    message(STATUS "LDtkSDL target available.")
    add_subdirectory(examples/SDL)
endif()

if (LDTK_BUILD_RAYLIB_EXAMPLE)
    message(STATUS "LDtkRaylib target available.")
    add_subdirectory(examples/Raylib)
endif()

if (LDTK_BUILD_API_TEST)
    message(STATUS "LDtkAPI_test target available.")
    add_subdirectory(examples/API_test)
endif()

# cmake install rules


# generate LDtkLoaderConfigVersion.cmake
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake"
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# generate LDtkLoaderConfig.cmake
configure_package_config_file(
    "cmake/LDtkLoaderConfig.cmake.in" "${PROJECT_BINARY_DIR}/LDtkLoaderConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader
)

# create regular installation rules
# install license and readme to ${CMAKE_INSTALL_PREFIX}/share/doc/LDtkLoader
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
# install headers to ${CMAKE_INSTALL_PREFIX}/include
# install the static library target to ${CMAKE_INSTALL_PREFIX}/lib
install(TARGETS LDtkLoader EXPORT LDtkLoaderTargets ARCHIVE LIBRARY FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT LDtkLoaderTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader NAMESPACE LDtkLoader::)
# install cmake package files to ${CMAKE_INSTALL_PREFIX}/lib/cmake/LDtkLoader
install(FILES ${PROJECT_BINARY_DIR}/LDtkLoaderConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
install(FILES ${PROJECT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)

# create build-tree export cmake module
export(TARGETS LDtkLoader FILE ${PROJECT_BINARY_DIR}/LDtkLoader.cmake NAMESPACE LDtkLoader::)
