#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/vpl/linked_list.h"

// helper for string
char *test_str = "test_data";

// 1. Test Initialization
void test_linkedlist_init(void) {
  linkedlist_t *ll = linkedlist_init();
  CU_ASSERT_PTR_NOT_NULL(ll);
  CU_ASSERT_EQUAL(ll->size, 0);
  CU_ASSERT_PTR_NULL(ll->head);
  CU_ASSERT_PTR_NULL(ll->tail);
  linkedlist_free(&ll);
}

// 2. Test Append and Tail reference
void test_linkedlist_append(void) {
  linkedlist_t *ll = linkedlist_init();
  int val1 = 10;

  // Edge Case: Append to empty list
  ll->append(ll, INT, &val1);
  CU_ASSERT_EQUAL(ll->size, 1);
  CU_ASSERT_PTR_EQUAL(ll->head, ll->tail);
  CU_ASSERT_EQUAL(ll->head->data.value.ival, 10);

  // Normal case: Multiple elements
  int val2 = 20;
  ll->append(ll, INT, &val2);
  CU_ASSERT_EQUAL(ll->size, 2);
  CU_ASSERT_PTR_NOT_EQUAL(ll->head, ll->tail);
  CU_ASSERT_EQUAL(ll->tail->data.value.ival, 20);

  linkedlist_free(&ll);
}

// 3. Test Insert and Edge Conditions
void test_linkedlist_insert(void) {
  linkedlist_t *ll = linkedlist_init();
  int v1 = 1, v2 = 2, v3 = 3;

  // Edge Case: Insert at index 0 (empty)
  CU_ASSERT_TRUE(ll->insert(ll, 0, INT, &v1));

  // Edge Case: Insert at size (acts as append)
  CU_ASSERT_TRUE(ll->insert(ll, 1, INT, &v2));
  CU_ASSERT_EQUAL(ll->tail->data.value.ival, 2);

  // Edge Case: Insert at middle
  CU_ASSERT_TRUE(ll->insert(ll, 1, INT, &v3)); // List: 1 -> 3 -> 2
  CU_ASSERT_EQUAL(ll->get(ll, 1)->data.value.ival, 3);

  // Edge Case: Invalid index
  CU_ASSERT_FALSE(ll->insert(ll, 10, INT, &v1));
  CU_ASSERT_FALSE(ll->insert(ll, -1, INT, &v1));

  linkedlist_free(&ll);
}

// 4. Test Remove and Tail reference
void test_linkedlist_remove(void) {
  linkedlist_t *ll = linkedlist_init();
  int v1 = 10, v2 = 20;
  ll->append(ll, INT, &v1);
  ll->append(ll, INT, &v2);

  // Edge Case: Remove tail (verify tail pointer updates)
  CU_ASSERT_TRUE(ll->remove(ll, INT, &v2));
  CU_ASSERT_PTR_EQUAL(ll->head, ll->tail);
  CU_ASSERT_EQUAL(ll->size, 1);

  // Edge Case: Remove last remaining node
  CU_ASSERT_TRUE(ll->remove(ll, INT, &v1));
  CU_ASSERT_PTR_NULL(ll->head);
  CU_ASSERT_PTR_NULL(ll->tail);
  CU_ASSERT_EQUAL(ll->size, 0);

  linkedlist_free(&ll);
}

// 5. Test Pop
void test_linkedlist_pop(void) {
  linkedlist_t *ll = linkedlist_init();
  int v1 = 100;
  ll->append(ll, INT, &v1);

  // Edge Case: Pop single node
  node_t *popped = ll->pop(ll);
  CU_ASSERT_PTR_NOT_NULL(popped);
  CU_ASSERT_EQUAL(popped->data.value.ival, 100);
  CU_ASSERT_PTR_NULL(ll->head);
  CU_ASSERT_PTR_NULL(ll->tail);

  free(popped); // Manual free as per your design
  linkedlist_free(&ll);
}

// 6. Test Reverse and Tail reference
void test_linkedlist_reverse(void) {
  linkedlist_t *ll = linkedlist_init();
  int v1 = 1, v2 = 2;
  ll->append(ll, INT, &v1);
  ll->append(ll, INT, &v2);

  // Reverse list: 1 -> 2 becomes 2 -> 1
  ll->reverse(ll);
  CU_ASSERT_EQUAL(ll->head->data.value.ival, 2);
  CU_ASSERT_EQUAL(ll->tail->data.value.ival, 1);
  CU_ASSERT_PTR_NULL(ll->tail->next);

  linkedlist_free(&ll);
}



// TESTRUN STARTS HERE
int main() {
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("LinkedList_TestSuite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_add_test(pSuite, "test of init()", test_linkedlist_init);
  CU_add_test(pSuite, "test of append()", test_linkedlist_append);
  CU_add_test(pSuite, "test of insert()", test_linkedlist_insert);
  CU_add_test(pSuite, "test of remove()", test_linkedlist_remove);
  CU_add_test(pSuite, "test of pop()", test_linkedlist_pop);
  CU_add_test(pSuite, "test of reverse()", test_linkedlist_reverse);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
