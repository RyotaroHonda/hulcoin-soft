set(MY_OUTPUT_NAME ${LIB_NAME})
set(MY_TARGET ${FW_NAME}${LIB_NAME})


set(MY_PUB_HEADER
  "DwgFuncs.hh"
  )

file(GLOB_RECURSE SRCS "*.cc")

add_library(${MY_TARGET} STATIC
  ${SRCS}
  )

target_compile_options(${MY_TARGET} PRIVATE
  ${my_cxx_options}
  )
target_compile_features(${MY_TARGET} PRIVATE
  ${my_cxx_features}
  )
target_include_directories(${MY_TARGET} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
target_link_libraries(${MY_TARGET} PUBLIC
  HulCore;
  )

set_target_properties(${MY_TARGET} PROPERTIES
  OUTPUT_NAME ${MY_OUTPUT_NAME}
  PUBLIC_HEADER "${MY_PUB_HEADER}"
  )

install(TARGETS ${MY_TARGET}
  ARCHIVE       DESTINATION ${CMAKE_INSTALL_PREFIX}/${FW_NAME}/lib64
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/${FW_NAME}/include
  )
