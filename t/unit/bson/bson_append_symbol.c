#include "test.h"

static void
test_bson_symbol (void)
{
  bson_t *b;

  /* Test #1: A single symbol element, with default size. */
  b = bson_finish (bson_append_symbol (bson_new (), "hello", "world", -1));
  ok (lmc_error_isok (b),
      "bson_append_symbol() works");

  cmp_ok (bson_size (b), "==", 22, "BSON symbol element size check");
  ok (memcmp (bson_data (b),
	      "\026\000\000\000\016\150\145\154\154\157\000\006\000\000\000"
	      "\167\157\162\154\144\000\000",
	      bson_size (b)) == 0,
      "BSON symbol element contents check");
  bson_free (b);

  /* Test #2: A single symbol element, with explicit length. */
  b = bson_finish
    (bson_append_symbol (bson_new (), "goodbye",
			 "cruel world, this garbage is gone.",
			 strlen ("cruel world")));
  ok (lmc_error_isok (b),
      "bson_append_symbol() with explicit length works");

  cmp_ok (bson_size (b), "==", 30, "BSON symbol element size check, #2");
  ok (memcmp (bson_data (b),
	      "\036\000\000\000\016\147\157\157\144\142\171\145\000\014\000"
	      "\000\000\143\162\165\145\154\040\167\157\162\154\144\000\000",
	      bson_size (b)) == 0,
      "BSON symbol element contents check, #2");
  bson_free (b);

  /* Test #3: Negative test, passing invalid arguments. */
  b = bson_new ();

  b = bson_append_symbol (b, "hello", "world", -42);
  ok (!lmc_error_isok (b),
      "bson_append_symbol() should fail with invalid length");
  lmc_error_reset (b);

  b = bson_append_symbol (b, "hello", NULL, -1);
  ok (!lmc_error_isok (b),
      "bson_append_symbol() should fail without a string");
  lmc_error_reset (b);

  b = bson_append_symbol (b, NULL, "world", -1);
  ok (!lmc_error_isok (b),
      "bson_append_symbol() should fail without a key name");
  lmc_error_reset (b);

  ok (bson_append_symbol (NULL, "hello", "world", -1) == NULL,
      "bson_append_symbol() should fail without a BSON object");

  bson_finish (b);
  cmp_ok (bson_size (b), "==", 5,
	  "BSON object should be empty");

  b = bson_append_symbol (b, "hello", "world", -1);
  ok (lmc_error_get_errn (b) == EBUSY,
      "Appending to a finished element should fail");

  bson_free (b);
}

RUN_TEST (12, bson_symbol);
