#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>


#include "../include/vpl/doubly_linkedlist.h"

/* --- Test Case 1: Initialization --- */
void test_dlinkedlist_init(void) {
  dlinkedlist_t *dll = dlinkedlist_init();
  CU_ASSERT_PTR_NOT_NULL(dll);
  CU_ASSERT_EQUAL(dll->size, 0);
  CU_ASSERT_PTR_NULL(dll->head);
  CU_ASSERT_PTR_NULL(dll->tail);
  dlinkedlist_free(&dll);
}

/* --- Test Case 2: Append & Tail Logic --- */
void test_dlinkedlist_append(void) {
  dlinkedlist_t *dll = dlinkedlist_init();
  int v1 = 10, v2 = 20;

  // Append to empty: head and tail should be the same
  dll->append(dll, INT, &v1);
  CU_ASSERT_EQUAL(dll->size, 1);
  CU_ASSERT_PTR_EQUAL(dll->head, dll->tail);

  // Second append: verify bidirectional links
  dll->append(dll, INT, &v2);
  CU_ASSERT_PTR_EQUAL(dll->tail->prev, dll->head);
  CU_ASSERT_PTR_EQUAL(dll->head->next, dll->tail);
  CU_ASSERT_EQUAL(dll->tail->data.value.ival, 20);

  dlinkedlist_free(&dll);
}

/* --- Test Case 3: O(n/2) Get Optimization --- */
void test_dlinkedlist_get(void) {
  dlinkedlist_t *dll = dlinkedlist_init();
  int values[] = {1, 2, 3, 4, 5};
  for(int i = 0; i < 5; i++) dll->append(dll, INT, &values[i]);

  // Test first half (forward from head)
  dnode_t *node0 = dll->get(dll, 0);
  CU_ASSERT_EQUAL(node0->data.value.ival, 1);

  // Test second half (backward from tail)
  dnode_t *node4 = dll->get(dll, 4);
  CU_ASSERT_EQUAL(node4->data.value.ival, 5);

  dlinkedlist_free(&dll);
}

/* --- Test Case 4: Pop (O(1) Verification) --- */
void test_dlinkedlist_pop(void) {
  dlinkedlist_t *dll = dlinkedlist_init();
  int v1 = 100, v2 = 200;
  dll->append(dll, INT, &v1);
  dll->append(dll, INT, &v2);

  dnode_t *popped = dll->pop(dll);
  CU_ASSERT_EQUAL(popped->data.value.ival, 200);
  CU_ASSERT_EQUAL(dll->size, 1);
  CU_ASSERT_PTR_EQUAL(dll->head, dll->tail);
  CU_ASSERT_PTR_NULL(dll->tail->next); // Integrity check

  free(popped); // Caller responsibility
  dlinkedlist_free(&dll);
}

/* --- Test Case 5: Reverse Logic --- */
void test_dlinkedlist_reverse(void) {
  dlinkedlist_t *dll = dlinkedlist_init();
  int v1 = 1, v2 = 2, v3 = 3;
  dll->append(dll, INT, &v1);
  dll->append(dll, INT, &v2);
  dll->append(dll, INT, &v3);

  dll->reverse(dll);
  // After reverse: 3 <-> 2 <-> 1
  CU_ASSERT_EQUAL(dll->head->data.value.ival, 3);
  CU_ASSERT_EQUAL(dll->tail->data.value.ival, 1);
  CU_ASSERT_PTR_EQUAL(dll->head->next->next, dll->tail);
  CU_ASSERT_PTR_EQUAL(dll->tail->prev->prev, dll->head);

  dlinkedlist_free(&dll);
}

/* --- Main Test Runner --- */
int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("DoublyLinkedList_Suite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Register all tests
  CU_add_test(pSuite, "Init Test", test_dlinkedlist_init);
  CU_add_test(pSuite, "Append/Tail Test", test_dlinkedlist_append);
  CU_add_test(pSuite, "Get Optimization Test", test_dlinkedlist_get);
  CU_add_test(pSuite, "Pop O(1) Test", test_dlinkedlist_pop);
  CU_add_test(pSuite, "Reverse Logic Test", test_dlinkedlist_reverse);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}

