add_library(stb SHARED stb.cpp)
set_target_properties(stb PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)