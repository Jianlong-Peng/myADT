SET(test_files
    test_array
    test_tree_tools
    test_heap
    test_vector
    test_stack
    test_string
)

if(MSVC)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
endif()
INCLUDE_DIRECTORIES("../include")
#LINK_DIRECTORIES("D:\myADT\lib")

foreach(name ${test_files})
    ADD_EXECUTABLE(${name} ${name}.c)
	if(MSVC)
		TARGET_LINK_LIBRARIES(${name} ${ADT_LIBRARY})
	else()
		TARGET_LINK_LIBRARIES(${name} ${ADT_LIBRARY} m)
	endif()
    INSTALL(TARGETS ${name} RUNTIME DESTINATION test)
endforeach()

if(WIN32)
	INSTALL(PROGRAMS dot_draw.py DESTINATION test)
endif()
