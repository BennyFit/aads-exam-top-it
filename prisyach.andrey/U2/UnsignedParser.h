#ifndef UNSIGNED_PARSER_H_INCLUDED
#define UNSIGNED_PARSER_H_INCLUDED

#include <cstddef>

bool parseUnsignedSizeT(const char* begin,
                        const char* end,
                        std::size_t& value,
                        const char*& next);

#endif
