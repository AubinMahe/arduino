#pragma once

namespace json {

   enum Status {

      /*  0 */ SUCCESS,

      /*  1 */ OPEN_BRACE_EXPECTED,
      /*  2 */ COMMA_EXPECTED,
      /*  3 */ CLOSE_BRACE_EXPECTED,
      /*  4 */ CLOSE_BRACKET_EXPECTED,
      /*  5 */ BEGIN_OF_STRING_NOT_FOUND,
      /*  6 */ END_OF_STRING_NOT_FOUND,
      /*  7 */ ATTRIBUTE_NOT_FOUND,
      /*  8 */ ERROR_PARSING_VALUE,

      /*  9 */ UNEXPECTED_ATTRIBUTE,
      /* 10 */ TYPE_MISMATCH,

      /* 11 */ BUFFER_OVERFLOW,
   };
}
