if(NOT WIN32)
  cmake_minimum_required(VERSION 3.25)
  project(GluinoNative)

  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  set(PROJ GluinoNative)
  set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/GluinoNative)

  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

    configure_file(${PROJ_DIR}/src/exports.cpp ${PROJ_DIR}/src/exports.mm COPYONLY)

    file(GLOB SOURCES "${PROJ_DIR}/src/exports.mm" "${PROJ_DIR}/src/platform/macos/*.mm")
  elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.0)
    pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

    file(GLOB SOURCES "${PROJ_DIR}/src/exports.cpp" "${PROJ_DIR}/src/platform/linux/*.cpp")
  endif()

  add_library(${PROJ} SHARED ${SOURCES})

  set_target_properties(${PROJ} PROPERTIES OUTPUT_NAME ${PROJ} PREFIX "")

  target_include_directories(${PROJ} PRIVATE ${PROJ_DIR}/include)

  if(APPLE)
    target_include_directories(${PROJ} PRIVATE ${PROJ_DIR}/include/platform/macos)

    find_library(COCOA_LIBRARY Cocoa)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(WEBKIT_LIBRARY WebKit)
    find_library(USER_NOTIFICATIONS_FRAMEWORK UserNotifications)
    target_link_libraries(${PROJ} ${COCOA_LIBRARY} ${FOUNDATION_LIBRARY} ${WEBKIT_LIBRARY} ${USER_NOTIFICATIONS_FRAMEWORK})
  elseif(UNIX AND NOT APPLE)
    target_include_directories(${PROJ} PRIVATE ${PROJ_DIR}/include/platform/linux)
    target_include_directories(${PROJ} PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS})

    target_link_libraries(${PROJ} ${GTK3_LIBRARIES} ${WEBKIT2GTK_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
  endif()
endif()
