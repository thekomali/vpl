#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/vpl/queue_ll.h"

static queuell_t *q = NULL;

/**
 * SETUP: Runs before EACH test case if using CU_add_suite_with_setup_and_teardown
 * or once per suite if using CU_add_suite.
 * For simplicity, we initialize here.
 */
int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  if (q) queuell_free(&q);
  return 0;
}

void setup(void) {
  q = queuell_init();
}

void teardown(void) {
  queuell_free(&q);
}

// ==========================
// TESTFUNCTIONS CASES
// ==========================

void test_init_and_empty(void) {
  CU_ASSERT_PTR_NOT_NULL(q);
  CU_ASSERT_TRUE(q->is_empty(q));
  CU_ASSERT_EQUAL(q->size, 0);
}

void test_enqueue_int(void) {
  int val = 42;
  CU_ASSERT_TRUE(q->enqueue(q, INT, &val));
  CU_ASSERT_FALSE(q->is_empty(q));
  CU_ASSERT_EQUAL(q->size, 1);

  element_t peeked = q->peek(q);
  CU_ASSERT_EQUAL(peeked.value.ival, 42);
}

void test_dequeue_order(void) {
  int v1 = 10, v2 = 20;
  q->enqueue(q, INT, &v1);
  q->enqueue(q, INT, &v2);

  element_t e1 = q->dequeue(q);
  CU_ASSERT_EQUAL(e1.value.ival, 10);
  CU_ASSERT_EQUAL(q->size, 1);

  element_t e2 = q->dequeue(q);
  CU_ASSERT_EQUAL(e2.value.ival, 20);
  CU_ASSERT_TRUE(q->is_empty(q));
}

void test_string_ownership(void) {
  char *str = "Hello Queue";
  q->enqueue(q, STR, str);

  element_t e = q->dequeue(q);
  CU_ASSERT_STRING_EQUAL(e.value.sval, "Hello Queue");

  // EDGE CASE: User is responsible for freeing dequeued strings
  free(e.value.sval);
}

void test_edge_empty_ops(void) {
  // Dequeue from empty
  element_t e = q->dequeue(q);
  CU_ASSERT_EQUAL(e.value.ival, 0);

  // Peek at empty
  e = q->peek(q);
  CU_ASSERT_EQUAL(e.value.ival, 0);
}

void test_edge_single_node_pointers(void) {
  int val = 100;
  q->enqueue(q, INT, &val);

  // Head and Tail should be same
  CU_ASSERT_PTR_EQUAL(q->head, q->tail);

  q->dequeue(q);
  // Head and Tail should both be NULL after dequeueing the only node
  CU_ASSERT_PTR_NULL(q->head);
  CU_ASSERT_PTR_NULL(q->tail);
}

// ==========================
// RUNNER
// ==========================
int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  // Use CU_add_suite_with_setup_and_teardown to run setup/teardown per test
  CU_pSuite suite = CU_add_suite_with_setup_and_teardown(
    "QueueLL_Suite", init_suite, clean_suite, setup, teardown
  );

  if (NULL == suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(suite, "test_init_and_empty", test_init_and_empty);
  CU_add_test(suite, "test_enqueue_int", test_enqueue_int);
  CU_add_test(suite, "test_dequeue_order", test_dequeue_order);
  CU_add_test(suite, "test_string_ownership", test_string_ownership);
  CU_add_test(suite, "test_edge_empty_ops", test_edge_empty_ops);
  CU_add_test(suite, "test_edge_single_node_pointers", test_edge_single_node_pointers);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
