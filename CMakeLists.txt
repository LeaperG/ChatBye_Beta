cmake_minimum_required(VERSION 3.5)

project(ChatBye VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dictionary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pages/message_manager)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(PROJECT_SOURCES
        main.cpp
        client/main_window.cpp
        client/main_window.h

        client/pages/page_base.cpp
        client/pages/page_base.h

        client/pages/port_page.cpp
        client/pages/port_page.h

        client/pages/name_page.cpp
        client/pages/name_page.h

        client/pages/chat_page.cpp
        client/pages/chat_page.h

        server/server.h server/server.cpp

        android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat

        client/resources/resources.qrc

        helpers/dictionary/server_message_dictionary.h helpers/dictionary/server_message_dictionary.cpp

        client/pages/message_manager/message_manager.h client/pages/message_manager/message_manager.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ChatBye
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        client/pages/page_base.h client/pages/page_base.cpp
        client/pages/port_page.h client/pages/port_page.cpp
        client/pages/name_page.h client/pages/name_page.cpp
        client/pages/chat_page.h client/pages/chat_page.cpp

        server/server.h server/server.cpp

        android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat



        client/resources/resources.qrc

        helpers/dictionary/server_message_dictionary.h helpers/dictionary/server_message_dictionary.cpp
        client/pages/message_manager/message_manager.h client/pages/message_manager/message_manager.cpp

        helpers/message_enum/message_source.h helpers/message_enum/message_source.cpp

        helpers/json/json_message_builder.h helpers/json/json_message_builder.cpp

        helpers/message_enum/message_type.h  helpers/message_enum/message_type.cpp
        client/ui/helpers/message_helper.h client/ui/helpers/message_helper.cpp

        client/pages/settings_page/settings_page.h client/pages/settings_page/settings_page.cpp

        client/ui/custom_widgets/custom_widget_base.h client/ui/custom_widgets/custom_widget_base.cpp
        client/ui/custom_widgets/color_selector.h client/ui/custom_widgets/color_selector.cpp
        client/ui/custom_widgets/font_picker.h client/ui/custom_widgets/font_picker.cpp

        client/pages/image_settings/image_settings_manager.h client/pages/image_settings/image_settings_manager.cpp

        client/ui/custom_widgets/chat_list_widget.h client/ui/custom_widgets/chat_list_widget.cpp
        client/ui/helpers/shadow_helper.h client/ui/helpers/shadow_helper.cpp

        client/core/network_manager.h client/core/network_manager.cpp
        client/core/discovery_manager.h client/core/discovery_manager.cpp
        client/core/server_launcher.h client/core/server_launcher.cpp
        client/core/message_handler.h client/core/message_handler.cpp

        client/ui/styles/style_manager.h client/ui/styles/style_manager.cpp

        helpers/message_enum/host_binding_mode.h helpers/message_enum/host_binding_mode.cpp

        client/ui/styles/chat_page_style.h client/ui/styles/chat_page_style.cpp
        client/ui/styles/name_page_style.h client/ui/styles/name_page_style.cpp
        client/ui/styles/port_page_style.h client/ui/styles/port_page_style.cpp

        client/core/host_info.h client/core/host_info.cpp
        client/app_controller.h client/app_controller.cpp

        helpers/message_enum/interface_filter.h
        helpers/message_enum/scan_mode.h

        server/core/client_manager.h server/core/client_manager.cpp
        server/core/message_dispatcher.h server/core/message_dispatcher.cpp
        server/core/udp_service.h server/core/udp_service.cpp
        server/core/tcp_sender.h server/core/tcp_sender.cpp
        server/server_controller.h server/server_controller.cpp
        client/ui/custom_widgets/progress_dialog.h client/ui/custom_widgets/progressdialog.cpp











    )
# Define target properties for Android with Qt 6 as:
    set_property(TARGET ChatBye APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)

# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ChatBye SHARED
            ${PROJECT_SOURCES}
        )

# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ChatBye
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ChatBye PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ChatBye PRIVATE Qt6::Network)
target_link_libraries(ChatBye PRIVATE Qt6::Concurrent)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ChatBye)
endif()
set_target_properties(ChatBye PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ChatBye
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ChatBye)
endif()
