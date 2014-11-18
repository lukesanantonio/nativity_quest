
# Copyright (C) 2014 Luke San Antonio
# All rights reserved.

macro(add_data_target target)
  add_custom_target(${target} ${ARGV1})
endmacro()
macro(add_target_data target filename)
  add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND}
                     ARGS -E copy_if_different
                     ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
                     ${CMAKE_CURRENT_BINARY_DIR}/${filename})
  install(FILES ${filename} DESTINATION share/${PROJECT_NAME})
endmacro()
macro(add_target_directory target dirname)
  add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND}
                     ARGS -E copy_directory
                     ${CMAKE_CURRENT_SOURCE_DIR}/${dirname}
                     ${CMAKE_CURRENT_BINARY_DIR}/${dirname})
  install(DIRECTORY ${dirname} DESTINATION share/${PROJECT_NAME})
endmacro()
