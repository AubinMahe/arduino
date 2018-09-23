#pragma once

namespace json {

   enum Status {

      /*  0 */ SUCCESS,

      /*  1 */ OPEN_BRACE_EXPECTED,
      /*  2 */ OPEN_BRACKET_EXPECTED,
      /*  3 */ COMMA_EXPECTED,
      /*  4 */ CLOSE_BRACE_EXPECTED,
      /*  5 */ CLOSE_BRACKET_EXPECTED,
      /*  6 */ BEGIN_OF_STRING_NOT_FOUND,
      /*  7 */ END_OF_STRING_NOT_FOUND,
      /*  8 */ ATTRIBUTE_NOT_FOUND,
      /*  9 */ ERROR_PARSING_VALUE,

      /* 10 */ UNEXPECTED,
      /* 11 */ TYPE_MISMATCH,

      /* 12 */ BUFFER_OVERFLOW,
   };
}
