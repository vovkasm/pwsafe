
include(CmakeParseArguments)

# functions

function(pwsafe_register_resource resource_target)
  set(options)
  set(singleArgs NAME LOCATION)
  set(multiArgs)
  cmake_parse_arguments(res "${options}" "${singleArgs}" "${multiArgs}" ${ARGN})

  set_property(TARGET ${resource_target} APPEND PROPERTY RESOURCE_NAMES "${res_NAME}")
  set_property(TARGET ${resource_target} PROPERTY "RESOURCE_LOCATION_${res_NAME}" "${res_LOCATION}")
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
    set_source_files_properties(${file} PROPERTIES
      GENERATED 1
      MACOSX_PACKAGE_LOCATION Resources
      )
    target_sources(${target} PUBLIC ${file})
  endforeach()
  add_dependencies(${target} ${resource_target})
endfunction()
