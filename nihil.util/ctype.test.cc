// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.util;

namespace {

TEST_CASE("ctype: space", "[ctype]") {
	auto is_utf8_space =
		nihil::ctype_is(std::ctype_base::space,
				std::locale("C.UTF-8"));

	// '\v' (vertical tab) is a space
	REQUIRE(nihil::is_space('\v') == true);
	REQUIRE(nihil::is_space(L'\v') == true);

	REQUIRE(nihil::is_c_space('\v') == true);
	REQUIRE(nihil::is_c_space(L'\v') == true);

	REQUIRE(is_utf8_space('\v') == true);
	REQUIRE(is_utf8_space(L'\v') == true);

	// 'x' is not a space
	REQUIRE(nihil::is_space('x') == false);
	REQUIRE(nihil::is_space(L'x') == false);

	REQUIRE(nihil::is_c_space('x') == false);
	REQUIRE(nihil::is_c_space(L'x') == false);

	REQUIRE(is_utf8_space('x') == false);
	REQUIRE(is_utf8_space(L'x') == false);

	// U+2003 EM SPACE is a space
	REQUIRE(nihil::is_space(L'\u2003') == false);
	REQUIRE(nihil::is_c_space(L'\u2003') == false);
	REQUIRE(is_utf8_space(L'\u2003') == true);
}

TEST_CASE("ctype: print", "[ctype]") {
	auto is_utf8_print =
		nihil::ctype_is(std::ctype_base::print,
				std::locale("C.UTF-8"));

	// 'x' is printable
	REQUIRE(nihil::is_print('x') == true);
	REQUIRE(nihil::is_print(L'x') == true);

	REQUIRE(nihil::is_c_print('x') == true);
	REQUIRE(nihil::is_c_print(L'x') == true);

	REQUIRE(is_utf8_print('x') == true);
	REQUIRE(is_utf8_print(L'x') == true);

	// '\003' is not printable
	REQUIRE(nihil::is_print('\003') == false);
	REQUIRE(nihil::is_print(L'\003') == false);

	REQUIRE(nihil::is_c_print('\003') == false);
	REQUIRE(nihil::is_c_print(L'\003') == false);

	REQUIRE(is_utf8_print('\003') == false);
	REQUIRE(is_utf8_print(L'\003') == false);

	// U+0410 CYRILLIC CAPITAL LETTER A is printable
	REQUIRE(nihil::is_print(L'\u0410') == false);
	REQUIRE(nihil::is_c_print(L'\u0410') == false);
	REQUIRE(is_utf8_print(L'\u0410') == true);
}

TEST_CASE("ctype: cntrl", "[ctype]") {
	auto is_utf8_cntrl =
		nihil::ctype_is(std::ctype_base::cntrl,
				std::locale("C.UTF-8"));

	// '\003' is a control character
	REQUIRE(nihil::is_cntrl('\003') == true);
	REQUIRE(nihil::is_cntrl(L'\003') == true);

	REQUIRE(nihil::is_c_cntrl('\003') == true);
	REQUIRE(nihil::is_c_cntrl(L'\003') == true);

	REQUIRE(is_utf8_cntrl('\003') == true);
	REQUIRE(is_utf8_cntrl(L'\003') == true);


	// 'x' is not a control character
	REQUIRE(nihil::is_cntrl('x') == false);
	REQUIRE(nihil::is_cntrl(L'x') == false);

	REQUIRE(nihil::is_c_cntrl('x') == false);
	REQUIRE(nihil::is_c_cntrl(L'x') == false);

	REQUIRE(is_utf8_cntrl('x') == false);
	REQUIRE(is_utf8_cntrl(L'x') == false);

	// U+00AD SOFT HYPHEN is a control character.
	REQUIRE(nihil::is_cntrl(L'\u00ad') == false);
	REQUIRE(nihil::is_c_cntrl(L'\u00ad') == false);
	REQUIRE(is_utf8_cntrl(L'\u00ad') == true);
}

TEST_CASE("ctype: upper", "[ctype]") {
	auto is_utf8_upper =
		nihil::ctype_is(std::ctype_base::upper,
				std::locale("C.UTF-8"));

	// 'A' is upper case
	REQUIRE(nihil::is_upper('A') == true);
	REQUIRE(nihil::is_upper(L'A') == true);

	REQUIRE(nihil::is_c_upper('A') == true);
	REQUIRE(nihil::is_c_upper(L'A') == true);

	REQUIRE(is_utf8_upper('A') == true);
	REQUIRE(is_utf8_upper(L'A') == true);

	// 'a' is not upper case
	REQUIRE(nihil::is_upper('a') == false);
	REQUIRE(nihil::is_upper(L'a') == false);

	REQUIRE(nihil::is_c_upper('a') == false);
	REQUIRE(nihil::is_c_upper(L'a') == false);

	REQUIRE(is_utf8_upper('a') == false);
	REQUIRE(is_utf8_upper(L'a') == false);

	// U+0410 CYRILLIC CAPITAL LETTER A is upper case
	REQUIRE(nihil::is_upper(L'\u0410') == false);
	REQUIRE(nihil::is_c_upper(L'\u0410') == false);
	REQUIRE(is_utf8_upper(L'\u0410') == true);
}

TEST_CASE("ctype: lower", "[ctype]") {
	auto is_utf8_lower =
		nihil::ctype_is(std::ctype_base::lower,
				std::locale("C.UTF-8"));

	// 'a' is lower case
	REQUIRE(nihil::is_lower('a') == true);
	REQUIRE(nihil::is_lower(L'a') == true);

	REQUIRE(nihil::is_c_lower('a') == true);
	REQUIRE(nihil::is_c_lower(L'a') == true);

	REQUIRE(is_utf8_lower('a') == true);
	REQUIRE(is_utf8_lower(L'a') == true);

	// 'A' is not lower case
	REQUIRE(nihil::is_lower('A') == false);
	REQUIRE(nihil::is_lower(L'A') == false);

	REQUIRE(nihil::is_c_lower('A') == false);
	REQUIRE(nihil::is_c_lower(L'A') == false);

	REQUIRE(is_utf8_lower('A') == false);
	REQUIRE(is_utf8_lower(L'A') == false);

	// U+0430 CYRILLIC SMALL LETTER A
	REQUIRE(nihil::is_lower(L'\u0430') == false);
	REQUIRE(nihil::is_c_lower(L'\u0430') == false);
	REQUIRE(is_utf8_lower(L'\u0430') == true);
}

TEST_CASE("ctype: alpha", "[ctype]") {
	auto is_utf8_alpha =
		nihil::ctype_is(std::ctype_base::alpha,
				std::locale("C.UTF-8"));

	// 'a' is alphabetical
	REQUIRE(nihil::is_alpha('a') == true);
	REQUIRE(nihil::is_alpha(L'a') == true);

	REQUIRE(nihil::is_c_alpha('a') == true);
	REQUIRE(nihil::is_c_alpha(L'a') == true);

	REQUIRE(is_utf8_alpha('a') == true);
	REQUIRE(is_utf8_alpha(L'a') == true);

	// '1' is not alphabetical
	REQUIRE(nihil::is_alpha('1') == false);
	REQUIRE(nihil::is_alpha(L'1') == false);

	REQUIRE(nihil::is_c_alpha('1') == false);
	REQUIRE(nihil::is_c_alpha(L'1') == false);

	REQUIRE(is_utf8_alpha('1') == false);
	REQUIRE(is_utf8_alpha(L'1') == false);

	// U+0430 CYRILLIC SMALL LETTER A
	REQUIRE(nihil::is_alpha(L'\u0430') == false);
	REQUIRE(nihil::is_c_alpha(L'\u0430') == false);
	REQUIRE(is_utf8_alpha(L'\u0430') == true);
}

TEST_CASE("ctype: digit", "[ctype]") {
	auto is_utf8_digit =
		nihil::ctype_is(std::ctype_base::digit,
				std::locale("C.UTF-8"));

	// '1' is a digit
	REQUIRE(nihil::is_digit('1') == true);
	REQUIRE(nihil::is_digit(L'1') == true);

	REQUIRE(nihil::is_c_digit('1') == true);
	REQUIRE(nihil::is_c_digit(L'1') == true);

	REQUIRE(is_utf8_digit('1') == true);
	REQUIRE(is_utf8_digit(L'1') == true);

	// 'a' is not a digit
	REQUIRE(nihil::is_digit('a') == false);
	REQUIRE(nihil::is_digit(L'a') == false);

	REQUIRE(nihil::is_c_digit('a') == false);
	REQUIRE(nihil::is_c_digit(L'a') == false);

	REQUIRE(is_utf8_digit('a') == false);
	REQUIRE(is_utf8_digit(L'a') == false);

	// U+0660 ARABIC-INDIC DIGIT ZERO
	REQUIRE(nihil::is_digit(L'\u0660') == false);
	REQUIRE(nihil::is_c_digit(L'\u0660') == false);
	REQUIRE(is_utf8_digit(L'\u0660') == true);
}

TEST_CASE("ctype: punct", "[ctype]") {
	auto is_utf8_punct =
		nihil::ctype_is(std::ctype_base::punct,
				std::locale("C.UTF-8"));

	// ';' is punctuation
	REQUIRE(nihil::is_punct(';') == true);
	REQUIRE(nihil::is_punct(L';') == true);

	REQUIRE(nihil::is_c_punct(';') == true);
	REQUIRE(nihil::is_c_punct(L';') == true);

	REQUIRE(is_utf8_punct(';') == true);
	REQUIRE(is_utf8_punct(L';') == true);

	// 'a' is not punctuation
	REQUIRE(nihil::is_punct('a') == false);
	REQUIRE(nihil::is_punct(L'a') == false);

	REQUIRE(nihil::is_c_punct('a') == false);
	REQUIRE(nihil::is_c_punct(L'a') == false);

	REQUIRE(is_utf8_punct('a') == false);
	REQUIRE(is_utf8_punct(L'a') == false);

	// U+00A1 INVERTED EXCLAMATION MARK
	REQUIRE(nihil::is_punct(L'\u00A1') == false);
	REQUIRE(nihil::is_c_punct(L'\u00A1') == false);
	REQUIRE(is_utf8_punct(L'\u00A1') == true);
}

TEST_CASE("ctype: xdigit", "[ctype]") {
	auto is_utf8_xdigit =
		nihil::ctype_is(std::ctype_base::xdigit,
				std::locale("C.UTF-8"));

	// 'f' is an xdigit
	REQUIRE(nihil::is_xdigit('f') == true);
	REQUIRE(nihil::is_xdigit(L'f') == true);

	REQUIRE(nihil::is_c_xdigit('f') == true);
	REQUIRE(nihil::is_c_xdigit(L'f') == true);

	REQUIRE(is_utf8_xdigit('f') == true);
	REQUIRE(is_utf8_xdigit(L'f') == true);

	// 'g' is not an xdigit
	REQUIRE(nihil::is_xdigit('g') == false);
	REQUIRE(nihil::is_xdigit(L'g') == false);

	REQUIRE(nihil::is_c_xdigit('g') == false);
	REQUIRE(nihil::is_c_xdigit(L'g') == false);

	REQUIRE(is_utf8_xdigit('g') == false);
	REQUIRE(is_utf8_xdigit(L'g') == false);
}

TEST_CASE("ctype: blank", "[ctype]") {
	auto is_utf8_blank =
		nihil::ctype_is(std::ctype_base::blank,
				std::locale("C.UTF-8"));

	// '\t' is a blank
	REQUIRE(nihil::is_blank('\t') == true);
	REQUIRE(nihil::is_blank(L'\t') == true);

	REQUIRE(nihil::is_c_blank('\t') == true);
	REQUIRE(nihil::is_c_blank(L'\t') == true);

	REQUIRE(is_utf8_blank('\t') == true);
	REQUIRE(is_utf8_blank(L'\t') == true);

	// '\v' is not a blank
	REQUIRE(nihil::is_blank('\v') == false);
	REQUIRE(nihil::is_blank(L'\v') == false);

	REQUIRE(nihil::is_c_blank('\v') == false);
	REQUIRE(nihil::is_c_blank(L'\v') == false);

	REQUIRE(is_utf8_blank('\v') == false);
	REQUIRE(is_utf8_blank(L'\v') == false);

	// There don't seem to be any UTF-8 blank characters, at least
	// in FreeBSD libc.
}

TEST_CASE("ctype: alnum", "[ctype]") {
	auto is_utf8_alnum =
		nihil::ctype_is(std::ctype_base::alnum,
				std::locale("C.UTF-8"));

	// 'a' is alphanumeric
	REQUIRE(nihil::is_alnum('a') == true);
	REQUIRE(nihil::is_alnum(L'a') == true);

	REQUIRE(nihil::is_c_alnum('a') == true);
	REQUIRE(nihil::is_c_alnum(L'a') == true);

	REQUIRE(is_utf8_alnum('a') == true);
	REQUIRE(is_utf8_alnum(L'a') == true);

	// '\t' is not a alnum
	REQUIRE(nihil::is_alnum('\t') == false);
	REQUIRE(nihil::is_alnum(L'\t') == false);

	REQUIRE(nihil::is_c_alnum('\t') == false);
	REQUIRE(nihil::is_c_alnum(L'\t') == false);

	REQUIRE(is_utf8_alnum('\t') == false);
	REQUIRE(is_utf8_alnum(L'\t') == false);

	// U+0430 CYRILLIC SMALL LETTER A
	REQUIRE(nihil::is_alnum(L'\u0430') == false);
	REQUIRE(nihil::is_c_alnum(L'\u0430') == false);
	REQUIRE(is_utf8_alnum(L'\u0430') == true);
}

TEST_CASE("ctype: graph", "[ctype]") {
	auto is_utf8_graph =
		nihil::ctype_is(std::ctype_base::graph,
				std::locale("C.UTF-8"));

	// 'a' is graphical 
	REQUIRE(nihil::is_graph('a') == true);
	REQUIRE(nihil::is_graph(L'a') == true);

	REQUIRE(nihil::is_c_graph('a') == true);
	REQUIRE(nihil::is_c_graph(L'a') == true);

	REQUIRE(is_utf8_graph('a') == true);
	REQUIRE(is_utf8_graph(L'a') == true);

	// '\t' is not graphical
	REQUIRE(nihil::is_graph('\t') == false);
	REQUIRE(nihil::is_graph(L'\t') == false);

	REQUIRE(nihil::is_c_graph('\t') == false);
	REQUIRE(nihil::is_c_graph(L'\t') == false);

	REQUIRE(is_utf8_graph('\t') == false);
	REQUIRE(is_utf8_graph(L'\t') == false);

	// U+0430 CYRILLIC SMALL LETTER A
	REQUIRE(nihil::is_graph(L'\u0430') == false);
	REQUIRE(nihil::is_c_graph(L'\u0430') == false);
	REQUIRE(is_utf8_graph(L'\u0430') == true);
}

} // anonymous namespace
