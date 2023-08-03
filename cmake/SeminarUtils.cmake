function(build_seminar_examples SEMINAR_DIR)
  message(STATUS "Building examples for seminar ${SEMINAR_DIR}...")

  set(SEMINAR_TARGET_NAME "sem-${SEMINAR_DIR}")
  add_custom_target(${SEMINAR_TARGET_NAME})
  message(STATUS "Common target for this seminar: ${SEMINAR_TARGET_NAME}")

  file(GLOB CPPS "${CMAKE_SOURCE_DIR}/seminars/2022/${SEMINAR_DIR}/*.cpp")

  foreach(CPP IN LISTS CPPS)
    get_filename_component(FILENAME ${CPP} NAME_WE)
    set(TARGET_NAME "sem-${SEMINAR_DIR}-${FILENAME}")
    message(STATUS "\tAdding target ${TARGET_NAME}")
    add_executable(${TARGET_NAME} ${CPP})
    add_dependencies(${SEMINAR_TARGET_NAME} ${TARGET_NAME})
  endforeach()

  add_dependencies(${ALL_SEMINARS_TARGET_NAME} ${SEMINAR_TARGET_NAME})
endfunction()
