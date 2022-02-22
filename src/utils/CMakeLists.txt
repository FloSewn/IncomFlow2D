#***********************************************************
# Module: utils
#***********************************************************
set( MODULE_UTILS utils )
set( UTILS_SRC ${SRC}/utils )

# Define source files
set( UTILS_MAIN
  bstrlib.c
  bstrlib_wrapper.c
  MeshReader.c
  PrimaryGrid.c
  )

# Define library
add_library( ${MODULE_UTILS} STATIC ${UTILS_MAIN} )

target_include_directories( ${MODULE_UTILS} PUBLIC
  $<BUILD_INTERFACE:${UTILS_SRC}>
  PRIVATE ${INCOMFLOW_SRC}
)

target_link_libraries( ${MODULE_UTILS}
  INTERFACE m
)

install( TARGETS utils DESTINATION ${LIBS} )
