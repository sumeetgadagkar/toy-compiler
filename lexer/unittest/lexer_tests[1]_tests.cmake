add_test( HelloTest.BasicAssertions /home/sumeet/Documents/projects/toy-compiler/lexer/unittest/lexer_tests [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( HelloTest.BasicAssertions PROPERTIES WORKING_DIRECTORY /home/sumeet/Documents/projects/toy-compiler/lexer/unittest SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( lexer_tests_TESTS HelloTest.BasicAssertions)
