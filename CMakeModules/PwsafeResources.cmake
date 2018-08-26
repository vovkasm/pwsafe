
include(CMakeParseArguments)

# functions

function(pwsafe_register_resource resource_target)
  set(options)
  set(singleArgs NAME LOCATION INSTALL_DIR)
  set(multiArgs)
  cmake_parse_arguments(res "${options}" "${singleArgs}" "${multiArgs}" ${ARGN})

  set_property(TARGET ${resource_target} APPEND PROPERTY RESOURCE_NAMES "${res_NAME}")
  set_property(TARGET ${resource_target} PROPERTY "RESOURCE_LOCATION_${res_NAME}" "${res_LOCATION}")
  set_property(TARGET ${resource_target} PROPERTY "RESOURCE_INSTALL_DIR_${res_NAME}" "${res_INSTALL_DIR}")
endfunction()

function(pwsafe_add_resources target)
  set(options)
  set(singleArgs FROM)
  set(multiArgs)
  cmake_parse_arguments(res "${options}" "${singleArgs}" "${multiArgs}" ${ARGN})

  if (NOT DEFINED res_FROM)
    message(ERROR "FROM target must be specified")
  endif()

  set(resource_target ${res_FROM})

  get_property(res_names TARGET ${resource_target} PROPERTY RESOURCE_NAMES)
  foreach(name ${res_names})
    get_property(file TARGET ${resource_target} PROPERTY "RESOURCE_LOCATION_${name}")
    get_property(install_dir TARGET ${resource_target} PROPERTY "RESOURCE_INSTALL_DIR_${name}")
    if (DEFINED install_dir)
      set(macosx_package_location "Resources/${install_dir}")
    else ()
      set(macosx_package_location "Resources")
    endif()
    set_source_files_properties(${file} PROPERTIES
      GENERATED 1
      MACOSX_PACKAGE_LOCATION ${macosx_package_location}
      )
    target_sources(${target} PUBLIC ${file})
  endforeach()
  add_dependencies(${target} ${resource_target})
endfunction()
