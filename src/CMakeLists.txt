if (UNIX)
    file(GLOB CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    file(GLOB HPP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
    set(ALL_SOURCES ${CPP_SOURCES} ${HPP_INCLUDES})

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}

        ${PROJECT_SOURCE_DIR}/deps/APECore/include
        ${PROJECT_SOURCE_DIR}/deps/APECore/deps/duktape-1.5.0/src
        ${PROJECT_SOURCE_DIR}/deps/APECore/deps/capstone/include
    )
    add_library(APESO SHARED ${ALL_SOURCES})

    target_link_libraries(APESO APECore pthread)

    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(ARCH_BITS 64)
        add_definitions("-DBUILD_64")
    else ()
        set(ARCH_BITS 32)
    endif()

    set(OUTPUT_NAME "APEDLL_${ARCH_BITS}")

    set_target_properties(APESO PROPERTIES
        OUTPUT_NAME ${OUTPUT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
    )
endif()