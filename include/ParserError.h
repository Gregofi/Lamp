#ifndef LAMP_PARSERERROR_H
#define LAMP_PARSERERROR_H

#include <exception>
#include <string>
#include "include/Token.h"

class ParserError : std::exception {
public:
    explicit ParserError(std::string s, SourceLocation loc) : s(std::move(s)), loc(loc) {}

    [[nodiscard]]
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

    std::string s;
    SourceLocation loc;
};


#endif //LAMP_PARSERERROR_H
