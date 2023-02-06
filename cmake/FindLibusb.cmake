function(openvisa_find_libusb libusb_lib_debug libusb_lib_release libusb_dll_debug libusb_dll_release libusb_use_static)
add_library(libusb SHARED IMPORTED)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set_target_properties(libusb PROPERTIES INTERFACE_LINK_LIBRARIES "${libusb_lib_debug}")
else()
    set_target_properties(libusb PROPERTIES INTERFACE_LINK_LIBRARIES "${libusb_lib_release}")
endif()

set_property(TARGET libusb APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_property(TARGET libusb APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

if(libusb_use_static)
set_target_properties(libusb PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${libusb_dll_debug}"
  )
set_target_properties(libusb PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${libusb_lib_release}"
  )
else()
set_target_properties(libusb PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${libusb_lib_debug}"
  IMPORTED_LOCATION_DEBUG "${libusb_dll_debug}"
  )
set_target_properties(libusb PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${libusb_lib_release}"
  IMPORTED_LOCATION_DEBUG "${libusb_dll_release}"
  )
endif()

endfunction()