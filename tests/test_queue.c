#include "../include/vpl/queue.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>


/* --- Setup and Teardown --- */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/* --- Test Cases --- */

void test_init_and_free(void) {
  queue_t *q = queue_init(5);
  CU_ASSERT_PTR_NOT_NULL(q);
  CU_ASSERT_EQUAL(q->capacity, 5);
  CU_ASSERT_EQUAL(q->size, 0);
  CU_ASSERT_TRUE(q->is_empty(q));

  queue_free(&q);
  CU_ASSERT_PTR_NULL(q); // Pointer should be NULL after free
}

void test_basic_enqueue_dequeue(void) {
  queue_t *q = queue_init(3);
  int val1 = 10;
  double val2 = 20.5;
  char *val3 = "Hello";

  // Enqueue
  CU_ASSERT_TRUE(q->enqueue(q, INT, &val1));
  CU_ASSERT_TRUE(q->enqueue(q, DOU, &val2));
  CU_ASSERT_TRUE(q->enqueue(q, STR, val3)); // Pass string pointer directly

  CU_ASSERT_EQUAL(q->size, 3);
  CU_ASSERT_TRUE(q->is_full(q));

  // Peek
  const element_t *pk = q->peek(q);
  CU_ASSERT_EQUAL(pk->value.ival, 10);
  CU_ASSERT_EQUAL(pk->etype, INT);

  // Dequeue
  element_t e1 = q->dequeue(q);
  CU_ASSERT_EQUAL(e1.value.ival, 10);

  element_t e2 = q->dequeue(q);
  CU_ASSERT_EQUAL(e2.value.dval, 20.5);

  element_t e3 = q->dequeue(q);
  CU_ASSERT_STRING_EQUAL(e3.value.sval, "Hello");

  // Crucial: we own the dequeued string memory
  free(e3.value.sval);

  CU_ASSERT_TRUE(q->is_empty(q));
  queue_free(&q);
}

void test_circular_wrap_around(void) {
  queue_t *q = queue_init(2);
  int a = 1, b = 2, c = 3;

  // Fill the queue
  q->enqueue(q, INT, &a);
  q->enqueue(q, INT, &b);

  // Dequeue one to create a hole at index 0
  q->dequeue(q);

  // Enqueue one more (should wrap around to index 0)
  CU_ASSERT_TRUE(q->enqueue(q, INT, &c));
  CU_ASSERT_EQUAL(q->rear, 0);
  CU_ASSERT_TRUE(q->is_full(q));

  queue_free(&q);
}

void test_edge_cases(void) {
  queue_t *q = queue_init(1);

  // 1. Dequeue empty
  element_t empty = q->dequeue(q);
  CU_ASSERT_EQUAL(empty.etype, 0); // Should be default initialized

  // 2. Peek empty
  CU_ASSERT_PTR_NULL(q->peek(q));

  // 3. Overflow
  int val = 100;
  q->enqueue(q, INT, &val);
  CU_ASSERT_FALSE(q->enqueue(q, INT, &val)); // Should return false

  // 4. Invalid init
  CU_ASSERT_PTR_NULL(queue_init(0));
  CU_ASSERT_PTR_NULL(queue_init(-1));

  queue_free(&q);
}

void test_string_leak_in_free(void) {
  queue_t *q = queue_init(5);
  char *s = "Leaked String";

  // Enqueue strings but DON'T dequeue them
  q->enqueue(q, STR, s);
  q->enqueue(q, STR, s);

  // queue_free should handle the strdup internal pointers
  queue_free(&q);
  CU_PASS("Queue freed with internal strings successfully");
}

/* --- Main Test Runner --- */
int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("Circular_Queue_Suite", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "Init and Free", test_init_and_free);
  CU_add_test(pSuite, "Basic Ops", test_basic_enqueue_dequeue);
  CU_add_test(pSuite, "Wrap Around Logic", test_circular_wrap_around);
  CU_add_test(pSuite, "Edge Cases", test_edge_cases);
  CU_add_test(pSuite, "Memory Ownership in Free", test_string_leak_in_free);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
