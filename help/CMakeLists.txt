# TODO: restore chm compilation

find_program(ZIP_EXECUTABLE zip)

function(make_help lang source_dir)
  set(output "help${lang}.zip")
  set(target "help${lang}")

  add_custom_target("${target}" DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${output}")
  add_custom_command(OUTPUT "${output}"
    COMMAND "${ZIP_EXECUTABLE}" "-Xqr" "${CMAKE_CURRENT_BINARY_DIR}/${output}" "./*"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${source_dir}"
    )

  add_dependencies(helpfiles "${target}")
  pwsafe_register_resource(helpfiles
    NAME ${target}
    LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${output}"
    )
endfunction()

add_custom_target(helpfiles ALL)

make_help(EN default)
make_help(DE pwsafeDE)
make_help(ES pwsafeES)
make_help(FR pwsafeFR)
make_help(RU pwsafeRU)
if (NOT WIN32)
make_help(PL pwsafePL) # pwsafePL -- nothing there :-(
make_help(ZH pwsafeZH) # pwsafeZH -- currently breaks my hhc :-(
endif()

