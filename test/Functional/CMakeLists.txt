
set(DAOGRAPH_FUNCTIONAL_LIST
        test-functional-01
        test-functional-02
        test-functional-03
        test-functional-04
        test-functional-05
        test-functional-06
        test-functional-99
        )

foreach(func ${DAOGRAPH_FUNCTIONAL_LIST})
    add_executable(${func}
            $<TARGET_OBJECTS:DaoGraph>
            ${func}.cpp
            )
endforeach()
