include (OptionDependentOnPackage)
include (PrintOptionStatus)

option (SOLOUD_DYNAMIC "Set to ON to build dynamic SoLoud" OFF)
print_option_status (SOLOUD_LIBRARY_DUNAMIC "Build dynamic library")

option (SOLOUD_STATIC "Set to ON to build static SoLoud" ON)
print_option_status (SOLOUD_LIBRARY_STATIC "Build static library")

# TODO:
option (SOLOUD_BUILD_DEMOS "Set to ON for building demos" OFF)
print_option_status (SOLOUD_BUILD_EXAMPLES "Build demos")

option (SOLOUD_BACKEND_NULL "Set to ON for building NULL backend" ON)
print_option_status (SOLOUD_BACKEND_NULL "NULL backend")

option (SOLOUD_BACKEND_SDL2 "Set to ON for building SDL2 backend" ON)
print_option_status (SOLOUD_BACKEND_SDL2 "SDL2 backend")
