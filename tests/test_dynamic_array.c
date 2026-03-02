#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/vpl/dynamic_array.h"


/* Test Helper: Initialize an array */
darray_t* setup_da() {
  return darray_init(5);
}


void test_darray_init(void) {
  darray_t *da = darray_init(10);

  CU_ASSERT_PTR_NOT_NULL(da);
  if (da) {
    CU_ASSERT_EQUAL(da->size, 0);
    CU_ASSERT_EQUAL(da->capacity, 10);
    darray_free(&da);
  }

  // Edge Case: Invalid capacity
  CU_ASSERT_PTR_NULL(darray_init(0));
  CU_ASSERT_PTR_NULL(darray_init(0));
}


void test_darray_append(void) {
  darray_t *da = setup_da();
  int val = 42;

  // Success path
  int idx = da->append(da, INT, &val);
  CU_ASSERT_EQUAL(idx, 0);
  CU_ASSERT_EQUAL(da->size, 1);

  // Edge Case: NULL self or NULL value
  CU_ASSERT_EQUAL(da->append(NULL, INT, &val), -1);
  CU_ASSERT_EQUAL(da->append(da, INT, NULL), -1);

  darray_free(&da);
}


void test_darray_get(void) {
  darray_t *da = setup_da();
  int val = 100;
  da->append(da, INT, &val);

  // Success path
  element_t *el = da->get(da, 0);
  CU_ASSERT_PTR_NOT_NULL(el);
  CU_ASSERT_EQUAL(el->value.ival, 100);

  // Edge Case: Out of bounds (Size is 1, so index 1 is invalid)
  CU_ASSERT_PTR_NULL(da->get(da, 1));
  CU_ASSERT_PTR_NULL(da->get(da, -1));

  darray_free(&da);
}


void test_darray_insert(void) {
  darray_t *da = setup_da();
  int v1 = 10, v2 = 20;
  da->append(da, INT, &v1);

  // Success path: Insert at start
  bool success = da->insert(da, 0, INT, &v2);
  CU_ASSERT_TRUE(success);
  CU_ASSERT_EQUAL(da->get(da, 0)->value.ival, 20);
  CU_ASSERT_EQUAL(da->get(da, 1)->value.ival, 10);

  // Edge Case: Insert beyond size
  CU_ASSERT_FALSE(da->insert(da, 5, INT, &v1));

  darray_free(&da);
}


void test_darray_pop(void) {
  darray_t *da = setup_da();
  int val = 55;
  da->append(da, INT, &val);

  // Success path
  element_t *el = da->pop(da);
  CU_ASSERT_PTR_NOT_NULL(el);
  CU_ASSERT_EQUAL(el->value.ival, 55);
  CU_ASSERT_EQUAL(da->size, 0);

  // Pop on empty array
  CU_ASSERT_PTR_NULL(da->pop(da));

  free(el); // Remember to free popped element if not part of array
  darray_free(&da);
}


void test_darray_index(void) {
  darray_t *da = setup_da();
  int v1 = 10, v2 = 20;
  da->append(da, INT, &v1);
  da->append(da, INT, &v2);

  // Success path
  CU_ASSERT_EQUAL(da->index(da, INT, &v2), 1);

  // Value not found
  int v3 = 99;
  CU_ASSERT_EQUAL(da->index(da, INT, &v3), -1);

  darray_free(&da);
}


void test_darray_remove(void) {
  darray_t *da = setup_da();
  int v1 = 10, v2 = 20;
  da->append(da, INT, &v1);
  da->append(da, INT, &v2);

  // Success path
  CU_ASSERT_TRUE(da->remove(da, INT, &v1));
  CU_ASSERT_EQUAL(da->size, 1);
  CU_ASSERT_EQUAL(da->get(da, 0)->value.ival, 20);

  // Remove non-existent
  int v3 = 99;
  CU_ASSERT_FALSE(da->remove(da, INT, &v3));

  darray_free(&da);
}


void test_darray_reverse(void) {
  darray_t *da = darray_init(5);
  int values[] = {10, 20, 30, 40};

  // Test 1: Even number of elements (4)
  for(int i = 0; i < 4; i++) da->append(da, INT, &values[i]);

  da->reverse(da);

  CU_ASSERT_EQUAL(da->get(da, 0)->value.ival, 40);
  CU_ASSERT_EQUAL(da->get(da, 1)->value.ival, 30);
  CU_ASSERT_EQUAL(da->get(da, 2)->value.ival, 20);
  CU_ASSERT_EQUAL(da->get(da, 3)->value.ival, 10);

  // Test 2: Odd number of elements (5)
  int extra = 50;
  da->reverse(da); // Back to {10, 20, 30, 40}
  da->append(da, INT, &extra); // {10, 20, 30, 40, 50}

  da->reverse(da); // Should be {50, 40, 30, 20, 10}

  CU_ASSERT_EQUAL(da->get(da, 0)->value.ival, 50);
  CU_ASSERT_EQUAL(da->get(da, 2)->value.ival, 30); // Middle stays
  CU_ASSERT_EQUAL(da->get(da, 4)->value.ival, 10);

  // Edge Case: Empty array
  darray_t *empty = darray_init(2);
  empty->reverse(empty); // Should not crash
  CU_ASSERT_EQUAL(empty->size, 0);

  // Edge Case: Single element
  int single = 100;
  empty->append(empty, INT, &single);
  empty->reverse(empty);
  CU_ASSERT_EQUAL(empty->get(empty, 0)->value.ival, 100);

  darray_free(&da);
  darray_free(&empty);
}




/*** === TESTRUN STARTS HERE === ***/
int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite pSuite = CU_add_suite("DArray_Test_Suite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add tests to suite
  CU_add_test(pSuite, "test of darray_init()", test_darray_init);
  CU_add_test(pSuite, "test of append()", test_darray_append);
  CU_add_test(pSuite, "test of get()", test_darray_get);
  CU_add_test(pSuite, "test of insert()", test_darray_insert);
  CU_add_test(pSuite, "test of pop()", test_darray_pop);
  CU_add_test(pSuite, "test of index()", test_darray_index);
  CU_add_test(pSuite, "test of remove()", test_darray_remove);
  CU_add_test(pSuite, "test of reverse()", test_darray_reverse);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
