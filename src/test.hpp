#pragma once

namespace sdizo{
  namespace tests{
    bool run_array_tests();
    bool run_list_tests();
    bool run_heap_tests();
    bool test_array();
    bool test_list();
    bool test_list2();
    bool test_heap();
    bool test_bst();
    bool run_array_tests();
    bool run_list_tests();
    bool run_heap_tests();
    bool run_bst_tests();
  }
}

#define TEST(test_name, test) \
if(test()) \
  printf("%s: \u001b[32mOK\u001b[0m\n", test_name); \
else \
  printf("%s: \u001b[31mFAILED\u001b[0m\n", test_name); \

