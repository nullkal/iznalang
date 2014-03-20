
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

BISON_TARGET(IznaParser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)

SET(SRCS
	node.hh
	izna.cc
	value.hh
	value.cc
	exception.hh
	draw/input.h
	draw/input-inl.h
	draw/input.cpp
	draw/texture.h
	draw/texture-inl.h
	draw/texture.cpp
	#draw/render_target.h
	#draw/render_target.cpp
	draw/draw2d.h
	draw/draw2d-inl.h
	draw/draw2d.cpp
	${BISON_IznaParser_OUTPUTS})

ADD_EXECUTABLE(izna ${SRCS})
target_link_libraries(izna ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARY} ${PNG_LIBRARY} ${Tcmalloc_LIBRARY} ${Boost_SYSTEM_LIBRARY})
