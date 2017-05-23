set (SOURCES
	main.cpp
	ChatWindow.cpp
	Message.cpp
)

add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} Qt5::Widgets Qt5::Network)
