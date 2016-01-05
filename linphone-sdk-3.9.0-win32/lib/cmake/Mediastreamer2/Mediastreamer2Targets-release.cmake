#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bellesip" for configuration "Release"
set_property(TARGET bellesip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bellesip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/bellesip.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bellesip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bellesip )
list(APPEND _IMPORT_CHECK_FILES_FOR_bellesip "${_IMPORT_PREFIX}/lib/bellesip.lib" "${_IMPORT_PREFIX}/bin/bellesip.dll" )

# Import target "ortp" for configuration "Release"
set_property(TARGET ortp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ortp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ortp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/ortp.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS ortp )
list(APPEND _IMPORT_CHECK_FILES_FOR_ortp "${_IMPORT_PREFIX}/lib/ortp.lib" "${_IMPORT_PREFIX}/bin/ortp.dll" )

# Import target "bzrtp" for configuration "Release"
set_property(TARGET bzrtp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bzrtp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/bzrtp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bzrtp.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bzrtp )
list(APPEND _IMPORT_CHECK_FILES_FOR_bzrtp "${_IMPORT_PREFIX}/lib/bzrtp.lib" "${_IMPORT_PREFIX}/bin/bzrtp.dll" )

# Import target "mediastreamer_base" for configuration "Release"
set_property(TARGET mediastreamer_base APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mediastreamer_base PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/mediastreamer_base.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/mediastreamer_base.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS mediastreamer_base )
list(APPEND _IMPORT_CHECK_FILES_FOR_mediastreamer_base "${_IMPORT_PREFIX}/lib/mediastreamer_base.lib" "${_IMPORT_PREFIX}/bin/mediastreamer_base.dll" )

# Import target "mediastreamer_voip" for configuration "Release"
set_property(TARGET mediastreamer_voip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mediastreamer_voip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/mediastreamer_voip.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/mediastreamer_voip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS mediastreamer_voip )
list(APPEND _IMPORT_CHECK_FILES_FOR_mediastreamer_voip "${_IMPORT_PREFIX}/lib/mediastreamer_voip.lib" "${_IMPORT_PREFIX}/bin/mediastreamer_voip.dll" )

# Import target "linphone" for configuration "Release"
set_property(TARGET linphone APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(linphone PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/linphone.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/linphone.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS linphone )
list(APPEND _IMPORT_CHECK_FILES_FOR_linphone "${_IMPORT_PREFIX}/lib/linphone.lib" "${_IMPORT_PREFIX}/bin/linphone.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
