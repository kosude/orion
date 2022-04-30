# function to load a text file into a variable which is then input into a
# preprocessor definition that can be accessed in the source code.
# NOTE! this function can only be called ONCE, before configuring the preset resource file.
function(load_resources)
    # parse given arguments
    cmake_parse_arguments(
        _ARG
        ""
        ""
        "PATHS"
        ${ARGN}
    )

    # loop through the given files
    foreach(path ${_ARG_PATHS})
        # add comment first
        string(APPEND _define_str "// ${path}")

        # get the contents of the file
        file(READ ${path} _file_contents)

        # split the file into lines
        string(REGEX REPLACE ";" "\\\\;" _file_lines "${_file_contents}")
        string(REGEX REPLACE "\n" ";" _file_lines "${_file_lines}")

        # get the second line (the one that contains the definition name)
        list(GET _file_lines 2 _file_def_line)

        # regex-match the definition name out of said line
        string(REGEX MATCH "// (.*)" _ ${_file_def_line})

        # add define statement
        string(APPEND _define_str "\n#define ${CMAKE_MATCH_1} \"\\\n")

        # add each line to the definition string
        foreach(line ${_file_lines})
            # get length of the line to check if it empty]
            string(REGEX MATCHALL ".[^\t]" matches "${line}")
            list(LENGTH matches linelen)

            if (linelen GREATER 0)
                # get the first and last characters of the line
                string(REGEX MATCH "^(.)" _first_char "${line}")
                string(REGEX MATCH "(.)$" _last_char "${line}")

                # check if the first character is a hash-tag
                string(COMPARE NOTEQUAL "${_first_char}" "#" _not_prepro)
                # check if the last character is a brace
                string(COMPARE NOTEQUAL "${_last_char}" "{" _not_brace)
                # check if the last character is a colon
                string(COMPARE NOTEQUAL "${_last_char}" ":" _not_colon)
                # check if the last character is a semicolon
                string(COMPARE NOTEQUAL "${_last_char}" ";" _not_semicolon)

                # add line to definition string
                string(APPEND _define_str "${line}\\n\\\n")
            endif()
        endforeach()

        # add the closing quotation mark to the end of the definition
        string(APPEND _define_str "}\"\n")
    endforeach()

    # set the JGL_LOADED_RESOURCES public variable.
    set(ORION_LOADED_RESOURCES "${_define_str}" PARENT_SCOPE)
endfunction()

load_resources(
    PATHS
        "${CMAKE_CURRENT_LIST_DIR}/basic.frag.glsl"
        "${CMAKE_CURRENT_LIST_DIR}/basic.vert.glsl"
)

configure_file("${PROJECT_SOURCE_DIR}/CMake/generate/shaderpresets.h.in" "${PROJECT_BINARY_DIR}/generated/shaderpresets.h" @ONLY)