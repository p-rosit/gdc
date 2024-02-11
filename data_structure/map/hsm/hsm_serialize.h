#ifndef HSM_SERIALIZE_H
#define HSM_SERIALIZE_H

#include "../../../serialize/serialize.h"

#define hsm_make_serialize_h(name, key_type, value_type) \
    SERIALIZE_DATA_H(JOIN_TOKENS(name, _hsm), HSM(name))

#define hsm_make_serialize(name, key_type, value_type, key_to_json, value_to_json) \
    SERIALIZE_MAP(JOIN_TOKENS(name, _hsm), HSM(name), key_type, value_type, for_each_hsm, key_to_json, value_to_json)

#endif
