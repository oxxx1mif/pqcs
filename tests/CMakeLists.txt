add_executable(test_primes test_primes.cpp)
target_link_libraries(test_primes PRIVATE pqcs_math Boost::unit_test_framework)
add_test(NAME PrimesMontgomeryTests COMMAND test_primes)
