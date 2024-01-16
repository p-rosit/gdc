#ifndef GDC_H
#define GDC_H

typedef enum gdc_error {
    GDC_OK              = 0,
    GDC_MEMORY_ERROR    = 1,
    GDC_CAPACITY_ERROR  = 2,
    GDC_ALREADY_PRESENT = 3,
    GDC_NOT_PRESENT     = 5,
    GDC_INDEX_OOB       = 6,
    GDC_INCORRECT_CALL  = 7,
    GDC_INTERNAL_ERROR  = 8,
} gdc_error_t;

/* Constructs: {type_name}_{structure_name}_{func_name}*/
#define GDC_FUNC(type_name, structure_name, func_name) GDC_FUNC_(type_name, structure_name, func_name)
#define GDC_FUNC_(type_name, structure_name, func_name) type_name ## _ ## structure_name ## _ ## func_name

/* Constructs: {structure_name}p_{func_name}_{type_name} */
#define GDC_FUNC_PRIVATE(type_name, structure_name, func_name) GDC_FUNC_PRIVATE_(type_name, structure_name, func_name)
#define GDC_FUNC_PRIVATE_(type_name, structure_name, func_name) structure_name ## p_ ## func_name ## _ ## type_name

/* Constructs: {type_name}_{structure_name}_t*/
#define GDC_STRUCT(type_name, structure_name) GDC_FUNC(type_name, structure_name, t)

/* Constructs: {structure_name}_{private_struct_name}_t*/
#define GDC_STRUCT_PRIVATE(structure_name, private_struct_name) GDC_FUNC_PRIVATE(t, structure_name, private_struct_name)

#define GDC_JOIN(a, b) GDC_JOIN_(a, b)
#define GDC_JOIN_(a, b) a ## b

#endif
