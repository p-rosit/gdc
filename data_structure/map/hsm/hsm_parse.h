#ifndef HSM_PARSE_H
#define HSM_PARSE_H

#include "hsm.h"
#include "../../../serialize/parse.h"

#define hsm_make_parse_h(name, key_type, value_type) \
    PARSE_DATA_H(JOIN_TOKENS(name, _hsm), HSM(name))

#define hsm_make_parse(name, key_type, value_type, parse_key, parse_value) \
    PARSE_MAP(JOIN_TOKENS(name, _hsm), HSM(name), key_type, value_type, HSM_FUNC(name, new), HSM_FUNC(name, insert), HSM_FUNC(name, free), parse_key, parse_value)

#endif
