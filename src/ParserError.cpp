//
// Created by gregofi1 on 11/26/21.
//

#include "ParserError.h"

const char *ParserError::what() const noexcept {
    return s.c_str();
}
