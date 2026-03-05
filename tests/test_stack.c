#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include "../include/vpl/stack.h"

static stack_t *test_stack = NULL;
#define CAPACITY 3

// This runs BEFORE each test
void setup(void) {
  test_stack = stack_init(CAPACITY);
}

// This runs AFTER each test
void teardown(void) {
  stack_free(&test_stack);
}

int init_suite(void) {
  test_stack = stack_init(CAPACITY);
  return (test_stack == NULL) ? -1 : 0;
}

int clean_suite(void) {
  stack_free(&test_stack);
  return 0;
}

// Test Initial State & Edge Cases
void test_stack_empty_and_full(void) {
  CU_ASSERT_TRUE(test_stack->is_empty(test_stack));
  CU_ASSERT_FALSE(test_stack->is_full(test_stack));

  // Underflow check: Pop from empty stack
  element_t e = test_stack->pop(test_stack);
  CU_ASSERT_EQUAL(e.etype, 0); // Assuming 0 is your 'null' etype
}

void test_stack_overflow(void) {
  int val = 10;
  // Fill to capacity
  for (int i = 0; i < CAPACITY; i++) {
    CU_ASSERT_TRUE(test_stack->push(test_stack, INT, &val));
  }
  CU_ASSERT_TRUE(test_stack->is_full(test_stack));

  // Overflow check: Push to full stack
  CU_ASSERT_FALSE(test_stack->push(test_stack, INT, &val));
}



// Test String Ownership (Deep Copy)
void test_stack_string_handling(void) {


  char *orig = "Hello CUnit";

  // 1. Verify push succeeded
  bool pushed = test_stack->push(test_stack, STR, orig);
  CU_ASSERT_TRUE_FATAL(pushed);

  // 2. Pop the element
  element_t e = test_stack->pop(test_stack);

  // 3. CRITICAL: Check etype before touching sval
  CU_ASSERT_EQUAL(e.etype, STR);

  // 4. Ensure sval is NOT NULL before string comparison
  CU_ASSERT_PTR_NOT_NULL_FATAL(e.value.sval);

  CU_ASSERT_STRING_EQUAL(e.value.sval, orig);
  CU_ASSERT_PTR_NOT_EQUAL(e.value.sval, orig); // Verify it's a deep copy

  free(e.value.sval);
}




int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  CU_pSuite pSuite = CU_add_suite_with_setup_and_teardown(
                        "Stack_Test_Suite",
                        NULL,                 // suit init, run once
                        NULL,                 // suit cleanup, run once
                        setup,                // test setup (run before each test)
                        teardown              // test cleanup (run after each test)
                    );
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "test of empty/full logic", test_stack_empty_and_full);
  CU_add_test(pSuite, "test of overflow handling", test_stack_overflow);
  CU_add_test(pSuite, "test of string deep copy", test_stack_string_handling);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}

