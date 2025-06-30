# This source code is released into the public domain.

add_library(nihil.generator STATIC)
target_sources(nihil.generator
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.generator.ccm

	generator.ccm
	byte_allocator.ccm
	coroutine_traits.ccm
	elements_of.ccm
	forward.ccm
	generator_promise_base.ccm
	generator_promise.ccm
	manual_lifetime.ccm
	promise_base_alloc.ccm
	util.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.generator.test generator.test.cc)
	target_link_libraries(nihil.generator.test PRIVATE
		nihil.generator
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.generator.test)
endif()
