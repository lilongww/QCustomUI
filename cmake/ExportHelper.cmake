function(export_config target_name installdir includedir)
    include(CMakePackageConfigHelpers)

    configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/${target_name}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${target_name}Config.cmake"
        INSTALL_DESTINATION "${installdir}/cmake/"
        PATH_VARS installdir includedir)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${target_name}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${target_name}ConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${target_name}Config.cmake" DESTINATION "${installdir}/cmake/")
    set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
    export(PACKAGE ${target_name})
endfunction()

function(export_install_target target_name includedir libdir bindir)
    install(TARGETS ${target_name}
        EXPORT ${target_name}
        ARCHIVE DESTINATION ${libdir}
        RUNTIME DESTINATION ${bindir}/${CMAKE_BUILD_TYPE}
        PUBLIC_HEADER DESTINATION ${includedir}/${target_name})

    install(EXPORT ${target_name} FILE "${target_name}Targets.cmake" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cmake")
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake" DESTINATION "${libdir}" FILES_MATCHING PATTERN "*.cmake")
endfunction()