set( TESTS run_tests )

add_executable( ${TESTS}
  tests_MeshReader.c
  tests_DualGrid.c
  main.c
)

target_link_libraries( ${TESTS}
  utils
)

install( TARGETS ${TESTS} RUNTIME DESTINATION ${BIN} )
