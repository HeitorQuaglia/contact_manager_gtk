#include "util.h"
#include <uuid/uuid.h>

int generate_uuid(char *uuid_str) {
    if (!uuid_str) {
        return UUID_GEN_ERR;
    }

    uuid_t bin_uuid;
    uuid_generate_random(bin_uuid);

#ifdef capitaluuid
    uuid_unparse_upper(bin_uuid, uuid_str);
#elif lowercaseuuid
    uuid_unparse_lower(bin_uuid, uuid_str);
#else
    uuid_unparse(bin_uuid, uuid_str);
#endif

    return UUID_GEN_SUCCESS;
}
