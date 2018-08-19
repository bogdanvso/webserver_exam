#ifndef MIME_TYPES_H
#define MIME_TYPES_H

#include <string>

namespace server
{
    namespace mime_types
    {
        std::string extension_to_type(const std::string& extension);
    }
}

#endif // !MIME_TYPES_H
