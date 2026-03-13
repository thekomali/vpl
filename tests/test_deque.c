#include "../include/vpl/deque.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


// Helper to setup and teardown
deque_t *dq = NULL;
const int TEST_CAP = 5;

int init_suite(void) {
  dq = deque_init(TEST_CAP);
  return (dq == NULL) ? -1 : 0;
}

int clean_suite(void) {
  if (dq) deque_free(&dq);
  return 0;
}

// ======================
// Test Cases
// ======================

void test_init_state(void) {
  CU_ASSERT_PTR_NOT_NULL(dq);
  CU_ASSERT_TRUE(dq->is_empty(dq));
  CU_ASSERT_FALSE(dq->is_full(dq));
  CU_ASSERT_EQUAL(dq->front, -1);
  CU_ASSERT_EQUAL(dq->rear, -1);
}

void test_enqueue_basic(void) {
  int val = 10;
  CU_ASSERT_TRUE(dq->enqueue_rear(dq, INT, &val));
  CU_ASSERT_FALSE(dq->is_empty(dq));

  // Peek check
  const element_t *peeked = dq->peek_front(dq);
  CU_ASSERT_PTR_NOT_NULL(peeked);
  CU_ASSERT_EQUAL(peeked->value.ival, 10);

  // Cleanup for next test
  dq->dequeue_front(dq);
}

void test_circular_wrap_around(void) {
  int val = 100;
  // 1. Reset the deque to be sure
  clean_suite(); init_suite();

  // 2. Enqueue 4 elements (Full for a capacity 5 pointer-based deque)
  for(int i = 0; i < 4; i++) {
    CU_ASSERT_TRUE(dq->enqueue_rear(dq, INT, &val));
  }
  // State: front=0, rear=3

  // 3. Dequeue 2 from the front
  dq->dequeue_front(dq);
  dq->dequeue_front(dq);
  // State: front=2, rear=3

  // 4. Enqueue 1 to rear (moves to index 4)
  CU_ASSERT_TRUE(dq->enqueue_rear(dq, INT, &val));
  // State: front=2, rear=4

  // 5. Enqueue 1 more to rear -> THIS should wrap to 0
  CU_ASSERT_TRUE(dq->enqueue_rear(dq, INT, &val));

  // Now rear MUST be 0
  CU_ASSERT_EQUAL(dq->rear, 0);
}

void test_string_handling(void) {
  // Reset queue
  clean_suite(); init_suite();

  char *text = "Hello Gemini";
  dq->enqueue_front(dq, STR, text);

  element_t popped = dq->dequeue_front(dq);
  CU_ASSERT_EQUAL(popped.etype, STR);
  CU_ASSERT_STRING_EQUAL(popped.value.sval, text);

  // CRITICAL: Caller must free the strdup'd string
  free(popped.value.sval);
}

void test_empty_full_boundaries(void) {
  // Reset
  clean_suite(); init_suite();

  // Dequeue on empty
  element_t empty_el = dq->dequeue_front(dq);
  CU_ASSERT_EQUAL(empty_el.etype, 0); // Assuming 0 is NONE/EMPTY

  // Fill to capacity
  int val = 1;
  while(!dq->is_full(dq)) {
    dq->enqueue_front(dq, INT, &val);
  }

  // Try to overfill
  CU_ASSERT_FALSE(dq->enqueue_front(dq, INT, &val));
}

// ======================
// Main Runner
// ======================
int main() {
  CU_pSuite pSuite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  pSuite = CU_add_suite("Deque_Test_Suite", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "Test Initial State", test_init_state);
  CU_add_test(pSuite, "Test Basic Enqueue/Peek", test_enqueue_basic);
  CU_add_test(pSuite, "Test Circular Wrap Around", test_circular_wrap_around);
  CU_add_test(pSuite, "Test String Memory Handling", test_string_handling);
  CU_add_test(pSuite, "Test Empty/Full Boundaries", test_empty_full_boundaries);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
