# SPDX-FileCopyrightText: Joey De Vries
#
# SPDX-License-Identifier: CC-BY-4.0

target_sources(
	${PROJECT_NAME}
	PRIVATE
		"main.cpp"
		"Shader.cpp"
		# If you want more source files, add them here...
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}"
)