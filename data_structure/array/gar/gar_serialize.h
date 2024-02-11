#include "gar.h"
#include "../../../serialize/serialize.h"
#include "../../../serialize/parse.h"

#define gar_make_serialize_h(name, type) \
    SERIALIZE_DATA_H(JOIN_TOKENS(name, _gar), GAR(name))

#define gar_make_parse_h(name, type) \
    PARSE_DATA_H(JOIN_TOKENS(name, _gar), GAR(name))


#define gar_make_serialize(name, type, value_to_json) \
    SERIALIZE_ARRAY(JOIN_TOKENS(name, _gar), GAR(name), type, for_each_gar, value_to_json)

#define gar_make_parse(name, type, json_to_value) \
    PARSE_ARRAY(JOIN_TOKENS(name, _gar), GAR(name), type, GAR_FUNC(name, new), GAR_FUNC(name, push), GAR_FUNC(name, free), json_to_value)


gar_make_serialize_h(int, int)
gar_make_parse_h(int, int)

