//
// Created by filip on 6/28/21.
//

#ifndef LAMP_UTILITY_H
#define LAMP_UTILITY_H

class MessageException : public std::exception
{
    std::string mess;
public:
    explicit MessageException(std::string mess) : mess(std::move(mess)) {}
    [[nodiscard]] const char *what() const noexcept override { return mess.c_str(); };
};


#endif //LAMP_UTILITY_H
