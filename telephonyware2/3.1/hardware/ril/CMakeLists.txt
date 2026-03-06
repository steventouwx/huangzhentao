cmake_minimum_required(VERSION 2.6)

add_subdirectory(fusion/libril_spm)
add_subdirectory(fusion/mtk-ril)
add_subdirectory(fusion/rild)
add_subdirectory(fusion/libcarrierconfig)

add_subdirectory(libmipc)
add_subdirectory(libratconfig)
add_subdirectory(libtrm)

add_subdirectory(platformlib/android/libmtkconfig)
add_subdirectory(platformlib/android/libmtkhardware_legacy)
add_subdirectory(platformlib/android/libmtklog)
add_subdirectory(platformlib/android/libmtkproperty)
#add_subdirectory(platformlib/android/nanopb-c)

add_subdirectory(platformlib/common)

