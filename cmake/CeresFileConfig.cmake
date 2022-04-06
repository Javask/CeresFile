include(GNUInstallDirs)


install(TARGETS CeresFile
    EXPORT CeresFile-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT CeresFile-targets
  FILE
    CeresFileTargets.cmake
  NAMESPACE
    CeresFile::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/CeresFile
)

get_filename_component(CeresFile_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

if(NOT TARGET CeresFile::CeresFile)
    include("${CeresFile_CMAKE_DIR}/CeresFileTargets.cmake")
endif()