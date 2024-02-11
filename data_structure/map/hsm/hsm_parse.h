#ifndef HSM_PARSE_H
#define HSM_PARSE_H

#include "hsm.h"
#include "../../../serialize/parse.h"

#define hsm_make_parse_h(name, key_type, value_type) \
    PARSE_DATA_H(JOIN_TOKENS(name, _hsm), HSM(name))

#define hsm_make_parse(name, key_type, value_type, free_map, free_key, free_value, parse_key, parse_value) \
    PARSE_MAP(JOIN_TOKENS(name, _hsm), HSM(name), key_type, value_type, HSM_FUNC(name, new), HSM_FUNC(name, insert), free_map, free_key, free_value, parse_key, parse_value)

#endif
