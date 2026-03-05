#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/vpl/stack_ll.h"

static stackll_t *sll = NULL;

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void setup(void) {
  sll = stackll_init();
}

void teardown(void) {
  stackll_free(&sll);
}


// basic lifecycle and empty state
void test_init_and_empty(void) {
  setup();
  CU_ASSERT_PTR_NOT_NULL(sll);
  CU_ASSERT_TRUE(sll->is_empty(sll));
  CU_ASSERT_EQUAL(sll->size, 0);
  teardown();
}

// underflow edge cases
void test_underflow(void) {
  setup();
  element_t e_pop = sll->pop(sll);
  element_t e_peek = sll->peek(sll);

  CU_ASSERT_EQUAL(e_pop.etype, 0);  // Should return {0}
  CU_ASSERT_EQUAL(e_peek.etype, 0);
  teardown();
}

// heterogeneous data & copy
void test_push_pop_heterogeneous(void) {
  setup();
  int ival = 42;
  double dval = 3.14;
  char *sval = "Stack Test";

  // Push different types
  CU_ASSERT_TRUE(sll->push(sll, INT, &ival));
  CU_ASSERT_TRUE(sll->push(sll, DOU, &dval));
  CU_ASSERT_TRUE(sll->push(sll, STR, sval));

  // Pop String (Top) - verify deep copy
  element_t e1 = sll->pop(sll);
  CU_ASSERT_EQUAL(e1.etype, STR);
  CU_ASSERT_STRING_EQUAL(e1.value.sval, sval);
  CU_ASSERT_PTR_NOT_EQUAL(e1.value.sval, sval); // Addresses must differ
  free(e1.value.sval); // User must free popped strings

  // Pop Double
  element_t e2 = sll->pop(sll);
  CU_ASSERT_EQUAL(e2.etype, DOU);
  CU_ASSERT_DOUBLE_EQUAL(e2.value.dval, dval, 0.0001);

  // Pop Int
  element_t e3 = sll->pop(sll);
  CU_ASSERT_EQUAL(e3.etype, INT);
  CU_ASSERT_EQUAL(e3.value.ival, ival);

  CU_ASSERT_TRUE(sll->is_empty(sll));
  teardown();
}


// LIFO order and peek
void test_lifo_and_peek(void) {
  setup();
  int v1 = 1, v2 = 2;
  sll->push(sll, INT, &v1);
  sll->push(sll, INT, &v2);

  // Peek should show v2 but keep size at 2
  element_t p = sll->peek(sll);
  CU_ASSERT_EQUAL(p.value.ival, 2);
  CU_ASSERT_EQUAL(sll->size, 2);

  // Pop should return v2 then v1
  CU_ASSERT_EQUAL(sll->pop(sll).value.ival, 2);
  CU_ASSERT_EQUAL(sll->pop(sll).value.ival, 1);
  teardown();
}


int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("StackLL_Suite", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "Init & Empty Check", test_init_and_empty);
  CU_add_test(pSuite, "Underflow Check", test_underflow);
  CU_add_test(pSuite, "Heterogeneous & Deep Copy", test_push_pop_heterogeneous);
  CU_add_test(pSuite, "LIFO Order & Peek", test_lifo_and_peek);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
