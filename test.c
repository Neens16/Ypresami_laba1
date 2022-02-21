#include <check.h>
#include <stdio.h>

#include "finder.h"

START_TEST(checker_test_ok) {
  result my1 = {{"R", "v"}, 2, 0};
  char *argv1[3];
  argv1[1] = ".";
  result my_1 = checker(2, argv1);
  ck_assert_int_eq(my1.count, my_1.count);
  ck_assert_str_eq(my1.name[0], my_1.name[0]);
  ck_assert_str_eq(my1.name[1], my_1.name[1]);
  ck_assert_int_eq(my1.error, my_1.error);

  ////////////////////

  result my2 = {{"Ypresami_laba1"}, 1, 0};
  char *argv2[3];
  argv2[1] = "../";
  result my_2 = checker(2, argv2);
  ck_assert_int_eq(my2.count, my_2.count);
  ck_assert_str_eq(my2.name[0], my_2.name[0]);
  ck_assert_int_eq(my2.error, my_2.error);

  ////////////////////

  result my3 = {{"crypto", "mpiaa", "PycharmProjects", "Desktop", "rtl8821ce",
                 "Public", "Downloads", "univer", "pythonProject2", "snap",
                 "Documents", "school21", "pythonProject1"},
                13,
                0};
  char *argv3[3];
  argv3[1] = "../../";
  result my_3 = checker(2, argv3);
  ck_assert_int_eq(my3.count, my_3.count);
  ck_assert_str_eq(my3.name[0], my_3.name[0]);
  ck_assert_int_eq(my3.error, my_3.error);
}
END_TEST

START_TEST(checker_test_nan) {
  result my1 = {{}, 0, 1};
  char *argv1[3];
  argv1[1] = "doesnt_exist";
  result my_1 = checker(2, argv1);
  ck_assert_int_eq(my1.count, my_1.count);
  ck_assert_str_eq(my1.name[0], my_1.name[0]);
  ck_assert_int_eq(my1.error, my_1.error);

  ////////////////////

  result my2 = {{}, 0, 1};
  char *argv2[3];
  argv2[1] = ".../";
  result my_2 = checker(2, argv2);
  ck_assert_int_eq(my2.count, my_2.count);
  ck_assert_str_eq(my2.name[0], my_2.name[0]);
  ck_assert_int_eq(my2.error, my_2.error);
}
END_TEST

START_TEST(check_on_dir_test) {
  int ans1 = 0;
  int ans_1 = checkOnDir("../");
  ck_assert_int_eq(ans1, ans_1);

  ////////////////////

  int ans2 = 0;
  int ans_2 = checkOnDir("../../../../../../");
  ck_assert_int_eq(ans2, ans_2);

  ////////////////////

  int ans3 = 1;
  int ans_3 = checkOnDir("finder.c");
  ck_assert_int_eq(ans3, ans_3);
}
END_TEST

int main() {
  Suite *s1 = suite_create("checker");
  TCase *check_case_ok = tcase_create("checker_test_ok");
  TCase *check_case_nan = tcase_create("checker_test_nan");
  TCase *check_on_dir = tcase_create("check_on_dir_test");
  SRunner *sr = srunner_create(s1);
  suite_add_tcase(s1, check_case_ok);
  tcase_add_test(check_case_ok, checker_test_ok);
  suite_add_tcase(s1, check_case_nan);
  tcase_add_test(check_case_nan, checker_test_nan);
  suite_add_tcase(s1, check_on_dir);
  tcase_add_test(check_on_dir, check_on_dir_test);
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}
