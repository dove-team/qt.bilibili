#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "VLCQt::Core" for configuration "Release"
set_property(TARGET VLCQt::Core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(VLCQt::Core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/VLCQtCore.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Qt5::Core"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/VLCQtCore.dll"
  )

list(APPEND _cmake_import_check_targets VLCQt::Core )
list(APPEND _cmake_import_check_files_for_VLCQt::Core "${_IMPORT_PREFIX}/lib/VLCQtCore.lib" "${_IMPORT_PREFIX}/bin/VLCQtCore.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
