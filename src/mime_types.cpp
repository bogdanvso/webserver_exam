#include "mime_types.h"

namespace server
{
    
    namespace mime_types
    {
        struct mapping
        {
            const char* extension;
            const char* mime_type;
        } mappings[] = 
        {
            { "html", "text/html" },
            { 0, 0 } // Marks end of list.
        };

        std::string extension_to_type(const std::string& extension)
        {
            for(mapping* m = mappings; m->extension; ++m)
                if(m->extension == extension)
                    return m->mime_type;
            return "text/plain";
        }
    } // mime_types
    
}   // server