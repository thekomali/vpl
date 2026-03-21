#include "../include/vpl/deque_ll.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// Helper to initialize a suite
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/**
 * @brief Test Initialization and Empty State
 */
void test_init_and_empty(void) {
  dequell_t *dq = dequell_init();
  CU_ASSERT_PTR_NOT_NULL(dq);
  CU_ASSERT_TRUE(dq->is_empty(dq));
  CU_ASSERT_EQUAL(dq->size, 0);
  dequell_free(&dq);
  CU_ASSERT_PTR_NULL(dq); // Verify pointer nullification
}

/**
 * @brief Test Enqueue and Dequeue Front (Normal & Edge Cases)
 */
void test_front_operations(void) {
  dequell_t *dq = dequell_init();
  int val1 = 10;

  // Edge Case: Dequeue from empty
  element_t empty_el = dq->dequeue_front(dq);
  CU_ASSERT_EQUAL(empty_el.etype, 0);

  // Normal Enqueue
  CU_ASSERT_TRUE(dq->enqueue_front(dq, INT, &val1));
  CU_ASSERT_EQUAL(dq->size, 1);
  CU_ASSERT_FALSE(dq->is_empty(dq));

  // Peek Check
  const element_t *peeked = dq->peek_front(dq);
  CU_ASSERT_PTR_NOT_NULL(peeked);
  CU_ASSERT_EQUAL(peeked->value.ival, 10);

  // Dequeue to Empty
  element_t el = dq->dequeue_front(dq);
  CU_ASSERT_EQUAL(el.value.ival, 10);
  CU_ASSERT_TRUE(dq->is_empty(dq));
  CU_ASSERT_PTR_NULL(dq->head);
  CU_ASSERT_PTR_NULL(dq->tail);

  dequell_free(&dq);
}

/**
 * @brief Test Rear Operations and Mixed Enqueue/Dequeue
 */
void test_rear_and_mixed(void) {
  dequell_t *dq = dequell_init();
  int v1 = 1, v2 = 2;
  char *s1 = "hello";

  dq->enqueue_front(dq, INT, &v1);   // [1]
  dq->enqueue_rear(dq, STR, s1);    // [1, "hello"]
  dq->enqueue_front(dq, INT, &v2);  // [2, 1, "hello"]

  CU_ASSERT_EQUAL(dq->size, 3);
  CU_ASSERT_EQUAL(dq->peek_front(dq)->value.ival, 2);
  CU_ASSERT_STRING_EQUAL(dq->peek_rear(dq)->value.sval, "hello");

  // Dequeue Rear
  element_t el_r = dq->dequeue_rear(dq);
  CU_ASSERT_EQUAL(el_r.etype, STR);
  CU_ASSERT_STRING_EQUAL(el_r.value.sval, "hello");
  free(el_r.value.sval); // Caller must free string

  CU_ASSERT_EQUAL(dq->size, 2);
  dequell_free(&dq);
}

/**
 * @brief Test String Deep Copying and Freeing
 */
void test_string_handling(void) {
  dequell_t *dq = dequell_init();
  char str[] = "temporary";

  dq->enqueue_front(dq, STR, str);
  str[0] = 'z'; // Modify original buffer

  // Verify deep copy (should still be 'temporary')
  CU_ASSERT_STRING_EQUAL(dq->peek_front(dq)->value.sval, "temporary");

  dequell_free(&dq); // Should not leak memory
}

int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("Deque_LL_TestSuite", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Register Tests
  CU_add_test(pSuite, "test of init and empty", test_init_and_empty);
  CU_add_test(pSuite, "test of front operations", test_front_operations);
  CU_add_test(pSuite, "test of rear and mixed operations", test_rear_and_mixed);
  CU_add_test(pSuite, "test of string deep copy", test_string_handling);

  // Run tests
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
