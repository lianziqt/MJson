#pragma once

#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <string>
#include <stdexcept>

namespace M {

    

        class Exception final : public std::logic_error {
        public:
            Exception(const std::string& errMsg) : logic_error(errMsg) { }
        };

    

}

#endif