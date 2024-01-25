#ifndef ERROR_H
#define ERROR_H

typedef enum error {
    NO_ERROR        = 0,
    MEMORY_ERROR    = 1,
    CAPACITY_ERROR  = 2,
    ALREADY_PRESENT = 3,
    NOT_PRESENT     = 5,
    INDEX_OOB       = 6,
    INCORRECT_CALL  = 7,
    PARSE_ERROR     = 8, 
    INTERNAL_ERROR  = 9,
} error_t;

#endif
