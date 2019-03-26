#pragma once

bool extern run_heap_tests();

#define TEST(test_name, test) \
if(test()) \
	printf("%s: OK\n", test_name); \
else \
	printf("%s: FAILED\n", test_name); \

