#ifndef ROCKY_SEMA_TYPE_H
#define ROCKY_SEMA_TYPE_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    SEMA_TYPE_ERROR,
    SEMA_TYPE_VOID,
    SEMA_TYPE_INT,
    SEMA_TYPE_FLOAT,
    SEMA_TYPE_BOOL,
    SEMA_TYPE_STRING,
    SEMA_TYPE_SIZE_T,
    SEMA_TYPE_POINTER,
    SEMA_TYPE_FUNCTION
} SemaTypeKind;

typedef struct SemaType SemaType;

typedef struct {
    SemaType** parameters;
    size_t parameter_count;
    SemaType* return_type;
} SemaFunctionType;

struct SemaType {
    SemaTypeKind kind;

    union {
        struct {
            SemaType* base;
        } pointer;

        SemaFunctionType function;
    } as;
};

//Constructors - returned will be released with sema_type_free().

SemaType* sema_type_new(SemaTypeKind kind);
SemaType* sema_type_pointer(const SemaType* base);
SemaType* sema_type_function(
    const SemaType* const* parameters,
    size_t parameter_count,
    const SemaType* return_type
);

// Deep copy and destruction
SemaType* sema_type_clone(const SemaType* type);
void sema_type_free(SemaType* type);

// Type compatibility
bool sema_type_equal(const SemaType* left, const SemaType* right);
bool sema_type_assignable(
    const SemaType* target,
    const SemaType* value
);

// Numeric helpers
bool sema_type_is_numeric(const SemaType* type);
bool sema_type_is_integer(const SemaType* type);
SemaTypeKind sema_type_common_numeric(
    const SemaType* left,
    const SemaType* right
);

// Useful for diagnostics.
const char* sema_type_name(const SemaType* type);

#endif