#include <check.h>
#include <stdlib.h>

#include <lmc/bson-element.h>

START_TEST (test_bson_element_new)
{
  bson_element_t *e;

  e = bson_element_new ();
  fail_if (e == NULL);
  mark_point ();
  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_ref)
{
  bson_element_t *e, *e2;

  e = bson_element_ref (NULL);
  fail_unless (e == NULL);

  e = bson_element_new ();
  e2 = bson_element_ref (e);

  fail_unless (e == e2);

  fail_if (bson_element_unref (e) == NULL);
  fail_unless (bson_element_unref (e) == NULL);

  fail_unless (bson_element_unref (NULL) == NULL);
}
END_TEST

START_TEST (test_bson_element_type_get)
{
  bson_element_t *e;

  fail_unless (bson_element_type_get (NULL) == BSON_TYPE_NONE);

  e = bson_element_new ();
  fail_unless (bson_element_type_get (e) == BSON_TYPE_NONE);
  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_type_set)
{
  bson_element_t *e;

  fail_unless (bson_element_type_set (NULL, BSON_TYPE_STRING) == NULL);

  e = bson_element_new ();
  fail_unless (bson_element_type_set (e, BSON_TYPE_STRING) == e);
  fail_unless (bson_element_type_get (e) == BSON_TYPE_STRING);
  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_name_set)
{
  bson_element_t *e;

  fail_unless (bson_element_name_set (NULL, "test-name") == NULL);

  e = bson_element_new ();

  e = bson_element_name_set (e, NULL);
  fail_if (e == NULL);

  e = bson_element_name_set (e, "test-name");
  fail_if (e == NULL);

  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_name_get)
{
  bson_element_t *e;

  fail_unless (bson_element_name_get (NULL) == NULL);

  e = bson_element_new ();
  ck_assert_str_eq (bson_element_name_get (e), "");

  bson_element_name_set (e, "test-name");
  ck_assert_str_eq (bson_element_name_get (e), "test-name");

  bson_element_name_set (e, "foo");
  ck_assert_str_eq (bson_element_name_get (e), "foo");

  bson_element_name_set (e, "really-long-name");
  ck_assert_str_eq (bson_element_name_get (e), "really-long-name");

  bson_element_name_set (e, NULL);
  ck_assert_str_eq (bson_element_name_get (e), "");
  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_data_set)
{
  bson_element_t *e;
  char *str = "test string";

  fail_unless (bson_element_data_set (NULL, (uint8_t *)str,
				      strlen (str) + 1) == NULL);

  e = bson_element_new ();

  e = bson_element_data_set (e, NULL, 1);
  ck_assert (e != NULL);

  e = bson_element_data_set (e, (uint8_t *)str, 0);
  ck_assert (e != NULL);

  e = bson_element_data_set (e, (uint8_t *)str, strlen (str) + 1);
  ck_assert (e != NULL);

  bson_element_unref (e);
}
END_TEST

START_TEST (test_bson_element_data_get)
{
  bson_element_t *e;
  char *str = "test string";

  fail_unless (bson_element_data_get (NULL) == NULL);
  ck_assert_int_eq (bson_element_data_get_size (NULL), -1);

  e = bson_element_new ();

  fail_if (bson_element_data_get (e) == NULL);
  ck_assert_int_eq (bson_element_data_get_size (e), 0);

  e = bson_element_data_set (e, (uint8_t *)str, strlen (str) + 1);

  ck_assert_str_eq ((char *)bson_element_data_get (e), str);
  ck_assert_int_eq (bson_element_data_get_size (e), (int32_t)strlen (str) + 1);

  bson_element_data_set (e, NULL, 0);

  ck_assert_int_eq (bson_element_data_get_size (e), 0);

  bson_element_unref (e);
}
END_TEST

Suite *
bson_element_suite (void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create ("BSON Elements unit tests");

  tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, test_bson_element_new);
  tcase_add_test (tc_core, test_bson_element_ref);
  tcase_add_test (tc_core, test_bson_element_type_get);
  tcase_add_test (tc_core, test_bson_element_type_set);
  tcase_add_test (tc_core, test_bson_element_name_set);
  tcase_add_test (tc_core, test_bson_element_name_get);
  tcase_add_test (tc_core, test_bson_element_data_set);
  tcase_add_test (tc_core, test_bson_element_data_get);
  suite_add_tcase (s, tc_core);

  return s;
}

int
main (void)
{
  int nfailed;
  Suite *s;
  SRunner *sr;

  s = bson_element_suite ();
  sr = srunner_create (s);

  srunner_run_all (sr, CK_ENV);
  nfailed = srunner_ntests_failed (sr);
  srunner_free (sr);

  return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}