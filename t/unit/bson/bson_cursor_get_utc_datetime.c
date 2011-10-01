#include "test.h"

static void
test_bson_cursor_get_utc_datetime (void)
{
  bson_t *b;
  bson_cursor_t *c;
  int64_t d = (int64_t)987654;

  ok (bson_cursor_get_utc_datetime (NULL, &d) == FALSE,
      "bson_cursor_get_utc_datetime() with a NULL cursor fails");

  b = test_bson_generate_full ();
  c = bson_cursor_new (b);

  ok (bson_cursor_get_utc_datetime (c, NULL) == FALSE,
      "bson_cursor_get_utc_datetime() with a NULL destination fails");
  ok (bson_cursor_get_utc_datetime (c, &d) == FALSE,
      "bson_cursor_get_utc_datetime() at the initial position fails");
  cmp_ok (d, "==", 987654,
	  "destination remains unchanged after failed cursor operations");
  bson_cursor_free (c);

  c = bson_cursor_find (bson_cursor_new (b), "date");
  ok (bson_cursor_get_utc_datetime (c, &d),
      "bson_cursor_get_utc_datetime() works");
  ok (d == 1294860709000,
      "bson_cursor_get_utc_datetime() returns the correct result");

  bson_cursor_next (c);
  ok (bson_cursor_get_utc_datetime (c, &d) == FALSE,
      "bson_cursor_get_utc_datetime() should fail when the cursor points to "
      "non-datetime data");

  bson_cursor_free (c);
  bson_free (b);
}

RUN_TEST (7, bson_cursor_get_utc_datetime);
