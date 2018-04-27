cmake_minimum_required(VERSION 3.0.0)
project(EasyBackup)

set(SRC
	backup.cxx
	backup_page.cxx
	configure_page.cxx
	main.cxx
	settings.cxx
	tabwidget.cxx
	tinyxml2.cpp
	window.cxx
	icons.qrc
)

if (APPLE)
	add_executable(EasyBackup MACOSX_BUNDLE ${SRC})
else()
	add_executable(EasyBackup ${SRC})
endif()

target_link_libraries(EasyBackup
	Qt5::Core
	Qt5::Widgets
)

if (UNIX)
	if (APPLE)
		install(TARGETS EasyBackup BUNDLE DESTINATION /Applications)
	else()
		install(TARGETS EasyBackup DESTINATION /usr/local/bin)
	endif()
endif()
