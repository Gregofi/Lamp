#include "include/ParserError.h"

const char *ParserError::what() const noexcept {
    return s.c_str();
}
