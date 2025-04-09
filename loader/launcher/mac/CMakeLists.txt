cmake_minimum_required(VERSION 3.3.0)


add_library(FreodBootstrapper SHARED Bootstrapper.cpp)
target_compile_features(FreodBootstrapper PUBLIC cxx_std_17)
set_target_properties(FreodBootstrapper PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "FreodBootstrapper"
	LIBRARY_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${FREOD_BIN_PATH}/nightly"
)

target_link_libraries(FreodBootstrapper PRIVATE)

add_library(FakeFreod SHARED FakeFreod.cpp)
target_compile_features(FakeFreod PUBLIC cxx_std_17)
set_target_properties(FakeFreod PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "Freod"
)

set_target_properties(FreodBootstrapper PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(FreodBootstrapper PROPERTIES INSTALL_RPATH "./")

set_target_properties(FakeFreod PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(FakeFreod PROPERTIES INSTALL_RPATH "./")

target_link_libraries(FreodBootstrapper PRIVATE FakeFreod)
