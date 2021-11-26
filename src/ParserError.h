//
// Created by gregofi1 on 11/26/21.
//

#ifndef LAMP_PARSERERROR_H
#define LAMP_PARSERERROR_H

#include <exception>
#include <string>

class ParserError : std::exception {
public:
    explicit ParserError(std::string s) : s(std::move(s)) {}

    [[nodiscard]]
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
private:
    std::string s;
};


#endif //LAMP_PARSERERROR_H
