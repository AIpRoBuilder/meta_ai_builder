# 对应 example 中的内容
set(DAOGRAPH_EXAMPLE_LIST
        E01-AutoPilot
        E02-MockGUI
        E03-ThirdFlow
        E04-MapReduce
        E05-JsonGraph
        )

foreach(example ${DAOGRAPH_EXAMPLE_LIST})
    add_executable(${example}
            $<TARGET_OBJECTS:DaoGraph>
            ${example}.cpp
            )
endforeach()
