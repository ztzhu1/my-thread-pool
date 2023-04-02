foreach (source_abs_filename ${SRC_LIST})
    get_filename_component(source_filename ${source_abs_filename} NAME)
    string(REPLACE ".cpp" "" source_name ${source_filename})
    add_library(
        ${source_name} 
        OBJECT
        ${source_abs_filename}
    )
    set(
        ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES}
        $<TARGET_OBJECTS:${source_name}>
    )
endforeach ()

add_library(${PROJECT_NAME} STATIC ${ALL_OBJECT_FILES})
target_link_libraries(
    ${PROJECT_NAME}
    fmt
)