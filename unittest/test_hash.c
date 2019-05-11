// Copyright (C) 2010-2019 Joel Rosdahl
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// This file contains tests for functions in hash.c.

#include "../src/ccache.h"
#include "../src/hash.h"
#include "framework.h"

TEST_SUITE(mdfour)

TEST(test_vectors_from_rfc_1320_should_be_correct)
{
	{
		struct hash *h = hash_init();
#ifdef USE_BLAKE2
		hash_checksum("blake2b");
		hash_string(h, "");
		CHECK_STR_EQ_FREE2("cae66941d9efbd404e4d88758ea67670-0", hash_result(h));
		hash_reset(h);
#endif
#ifdef USE_XXHASH
		hash_checksum("xxh64");
		hash_string(h, "");
		CHECK_STR_EQ_FREE2("ef46db3751d8e999ef46db3751d8e999-0", hash_result(h));
		hash_reset(h);
#endif
		hash_checksum("md4");
		hash_string(h, "");
		CHECK_STR_EQ_FREE2("31d6cfe0d16ae931b73c59d7e0c089c0-0", hash_result(h));
		hash_free(h);
	}

	{
		struct hash *h = hash_init();
#ifdef USE_BLAKE2
		hash_checksum("blake2b");
		hash_string(h, "a");
		CHECK_STR_EQ_FREE2("27c35e6e9373877f29e562464e46497e-1", hash_result(h));
		hash_reset(h);
#endif
#ifdef USE_XXHASH
		hash_checksum("xxh64");
		hash_string(h, "a");
		CHECK_STR_EQ_FREE2("d24ec4f1a98c6e5bd24ec4f1a98c6e5b-1", hash_result(h));
		hash_reset(h);
#endif
		hash_checksum("md4");
		hash_string(h, "a");
		CHECK_STR_EQ_FREE2("bde52cb31de33e46245e05fbdbd6fb24-1", hash_result(h));
		hash_free(h);
	}

	{
		struct hash *h = hash_init();
#ifdef USE_BLAKE2
		hash_checksum("blake2b");
		hash_string(h, "message digest");
		CHECK_STR_EQ_FREE2("a235c121347fdd24feffe048dbe68ccc-14", hash_result(h));
		hash_reset(h);
#endif
		hash_checksum("md4");
		hash_string(h, "message digest");
		CHECK_STR_EQ_FREE2("d9130a8164549fe818874806e1c7014b-14", hash_result(h));
		hash_free(h);
	}

	{
		struct hash *h = hash_init();
#ifdef USE_BLAKE2
		hash_checksum("blake2b");
		hash_string(
			h,
			"12345678901234567890123456789012345678901234567890123456789012345678901"
			"234567890");
		CHECK_STR_EQ_FREE2("3242cc3901ffad79cb164104a9486881-80", hash_result(h));
		hash_reset(h);
#endif
		hash_checksum("md4");
		hash_string(
			h,
			"12345678901234567890123456789012345678901234567890123456789012345678901"
			"234567890");
		CHECK_STR_EQ_FREE2("e33b4ddc9c38f2199c3e7b164fcc0536-80", hash_result(h));
		hash_free(h);
	}
}

TEST(hash_result_should_not_alter_state)
{
	struct hash *h = hash_init();

#ifdef USE_BLAKE2
	hash_checksum("blake2b");
	hash_string(h, "message");
	free(hash_result(h));
	hash_string(h, " digest");
	CHECK_STR_EQ_FREE2("a235c121347fdd24feffe048dbe68ccc-14", hash_result(h));
	hash_reset(h);
#endif
#ifdef USE_XXHASH
	hash_checksum("xxh64");
	hash_string(h, "message");
	free(hash_result(h));
	hash_string(h, " digest");
	CHECK_STR_EQ_FREE2("066ed728fceeb3be066ed728fceeb3be-14", hash_result(h));
	hash_reset(h);
#endif
	hash_checksum("md4");
	hash_string(h, "message");
	free(hash_result(h));
	hash_string(h, " digest");
	CHECK_STR_EQ_FREE2("d9130a8164549fe818874806e1c7014b-14", hash_result(h));
	hash_free(h);
}

TEST(hash_result_should_be_idempotent)
{
	struct hash *h = hash_init();

#ifdef USE_BLAKE2
	hash_checksum("blake2b");
	hash_string(h, "");
	CHECK_STR_EQ_FREE2("cae66941d9efbd404e4d88758ea67670-0", hash_result(h));
	CHECK_STR_EQ_FREE2("cae66941d9efbd404e4d88758ea67670-0", hash_result(h));
	hash_reset(h);
#endif
#ifdef USE_XXHASH
	hash_checksum("xxh64");
	hash_string(h, "");
	CHECK_STR_EQ_FREE2("ef46db3751d8e999ef46db3751d8e999-0", hash_result(h));
	CHECK_STR_EQ_FREE2("ef46db3751d8e999ef46db3751d8e999-0", hash_result(h));
	hash_reset(h);
#endif
	hash_checksum("md4");
	hash_string(h, "");
	CHECK_STR_EQ_FREE2("31d6cfe0d16ae931b73c59d7e0c089c0-0", hash_result(h));
	CHECK_STR_EQ_FREE2("31d6cfe0d16ae931b73c59d7e0c089c0-0", hash_result(h));
	hash_free(h);
}

TEST_SUITE_END
