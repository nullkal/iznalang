
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

BISON_TARGET(IznaParser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)

SET(SRCS
	node.hh
	izna.cc
	value.hh
	integer.hh
	integer.cc
	boolean.hh
	boolean.cc
	exception.hh
	${BISON_IznaParser_OUTPUTS})

ADD_EXECUTABLE(izna ${SRCS})

