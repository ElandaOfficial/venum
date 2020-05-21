
#pragma once

/**********************************************************************************************************************/
// region import/export
//======================================================================================================================
// Should an uninitialised enum be internally uninitialised or point to a nullptr?
#if VENUM_DEFAULT_INITIALISE
#   define VENUM_DEFAULT_POINTER { nullptr }
#else
#   define VENUM_DEFAULT_POINTER
#endif

// Utility overloads
#include "venum.ppo"

// Standard overloads
#include <array>
#include <assert.h>
#include <type_traits>
//======================================================================================================================
// endregion import/export
/**********************************************************************************************************************/
// region intern
//======================================================================================================================
#define VENUM_INTERN_CONCAT(X, Y) X##Y

// Workaround for MSVC argument expansion bug
#define VENUM_INTERN_EXPAND(...) __VA_ARGS__

#define VENUM_INTERN_UNPACK(X) VENUM_INTERN_EXPAND X

#define VENUM_INTERN_EMPTY(...)

// MSVC doesn't know the optional comma trick like Clang and GCC do so we cannot support this compiler
#if defined(MSC_VER_) && !(__cplusplus >= 202002L)
#   define VENUM_INTERN_COUNT_PLACEHOLDER(...) , __VA_ARGS__,
#   define VENUM_INTERN_PREP_COMMA(...)        , __VA_ARGS__
#else
#   if __cplusplus >= 202002L
#       define VENUM_INTERN_COUNT_PLACEHOLDER(...) __VA_OPT__(,) __VA_ARGS__,
#       define VENUM_INTERN_PREP_COMMA(...)        __VA_OPT__(,) __VA_ARGS__
#   else
#       define VENUM_INTERN_COUNT_PLACEHOLDER(...) ,##__VA_ARGS__,
#       define VENUM_INTERN_PREP_COMMA(...)        ,##__VA_ARGS__
#   endif
#endif


// Number shift resolver
#define VENUM_INTERN_COUNT(X, _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, _11, _12, _13, _14, _15, _16, _17, \
                              _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, \
                              _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, \
                              _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, \
                              _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, \
                              _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100,          \
                              COUNT, ...)  COUNT

#define VENUM_INTERN_SIZE_1(...) VENUM_INTERN_EXPAND(VENUM_INTERN_COUNT(__VA_ARGS__)) // Indirection for arg macro to expand first
#define VENUM_INTERN_SIZE(...) VENUM_INTERN_EXPAND \
                               ( \
                                   VENUM_INTERN_SIZE_1(VENUM_INTERN_COUNT_PLACEHOLDER(__VA_ARGS__) 100, \
                                                       99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, \
                                                       83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, \
                                                       67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, \
                                                       51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, \
                                                       35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
                                                       19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, \
                                                       1, 0) \
                               )

#define VENUM_INTERN_HARD_GLUE(X) VENUM_INTERN_CONCAT(X, _)
#define VENUM_INTERN_HARD_GLUE_1(X, Y) VENUM_INTERN_CONCAT(X, Y)
#define VENUM_INTERN_OVERLOAD_GLUE(X, Y) VENUM_INTERN_HARD_GLUE_1(VENUM_INTERN_HARD_GLUE(X), Y)
#define VENUM_INTERN_OVERLOAD(FUNC, ...) VENUM_INTERN_EXPAND \
                                         ( \
                                             VENUM_INTERN_OVERLOAD_GLUE(FUNC, VENUM_INTERN_SIZE(__VA_ARGS__)) \
                                             (__VA_ARGS__) \
                                         )

// Simple macro per argument executor
#define VENUM_INTERN_FOR(ACTION, ...) VENUM_INTERN_EXPAND \
                                      ( \
                                          VENUM_INTERN_OVERLOAD_GLUE(VENUM_INTERN_FOR, VENUM_INTERN_SIZE(__VA_ARGS__)) \
                                          (ACTION, (), __VA_ARGS__) \
                                      )

// Simple macro per argument executor
#define VENUM_INTERN_FOR_UNCO(ACTION, ...) VENUM_INTERN_EXPAND \
                                           ( \
                                               VENUM_INTERN_OVERLOAD_GLUE \
                                               ( \
                                                   VENUM_INTERN_FOR_UNCO, VENUM_INTERN_SIZE(__VA_ARGS__) \
                                               ) (ACTION, (), __VA_ARGS__) \
                                           )

// Simple macro per argument executor
#define VENUM_INTERN_FOR_WD(ACTION, ARGS, ...) VENUM_INTERN_EXPAND \
                                               ( \
                                                   VENUM_INTERN_OVERLOAD_GLUE \
                                                   ( \
                                                       VENUM_INTERN_FOR, VENUM_INTERN_SIZE(__VA_ARGS__) \
                                                   ) (ACTION, ARGS, __VA_ARGS__) \
                                               )

// Simple macro per argument executor
#define VENUM_INTERN_FOR_UNCO_WD(ACTION, ARGS, ...) VENUM_INTERN_EXPAND \
                                                    ( \
                                                        VENUM_INTERN_OVERLOAD_GLUE \
                                                        ( \
                                                            VENUM_INTERN_FOR_UNCO, VENUM_INTERN_SIZE(__VA_ARGS__) \
                                                        ) (ACTION, ARGS, __VA_ARGS__) \
                                                    )
                                                 
// Get argument at index (starting at 1)
#define VENUM_INTERN_GET_AT(INDEX, ...) VENUM_INTERN_EXPAND \
                                        ( \
                                            VENUM_INTERN_CONCAT(VENUM_INTERN_GET_AT_, INDEX)(__VA_ARGS__) \
                                        )

#define VENUM_INTERN_GET_LAST(...) VENUM_INTERN_EXPAND(VENUM_INTERN_GET_AT(VENUM_INTERN_SIZE(__VA_ARGS__), __VA_ARGS__))

#define VENUM_INTERN_UNCOMMATISE(...) VENUM_INTERN_EXPAND \
                                      ( \
                                          VENUM_INTERN_OVERLOAD(VENUM_INTERN_UNCOMMATISE, __VA_ARGS__) \
                                      )

#define VENUM_INTERN_STRINGIFY(...) VENUM_INTERN_EXPAND \
                                    ( \
                                        VENUM_INTERN_OVERLOAD(VENUM_INTERN_STRINGIFY, __VA_ARGS__) \
                                    )


#define VENUM_INTERN_CUT_LAST(...) VENUM_INTERN_EXPAND \
                                   ( \
                                       VENUM_INTERN_OVERLOAD(VENUM_INTERN_CUT_LAST, __VA_ARGS__) \
                                   )
                                   
#define VENUM_INTERN_IF_TRUE(X) X
#define VENUM_INTERN_IF_FALSE(X)
#define VENUM_INTERN_IF_NOT_TRUE(X)
#define VENUM_INTERN_IF_NOT_FALSE(X) X

#define VENUM_INTERN_IF_CAT(COND) VENUM_INTERN_CONCAT(VENUM_INTERN_IF_, COND)
#define VENUM_INTERN_IF(COND, ACTION) VENUM_INTERN_IF_CAT(COND) (ACTION)

#define VENUM_INTERN_IF_NOT_CAT(COND) VENUM_INTERN_CONCAT(VENUM_INTERN_IF_NOT_, COND)
#define VENUM_INTERN_IF_NOT(COND, ACTION) VENUM_INTERN_IF_NOT_CAT(COND) (ACTION)

#define VENUM_INTERN_IF_ELSE(COND, TRUE_ACT, FALSE_ACT) \
        VENUM_INTERN_IF(COND, TRUE_ACT) VENUM_INTERN_IF_NOT(COND, FALSE_ACT)

//======================================================================================================================
// endregion intern
/**********************************************************************************************************************/
// region helpers
//======================================================================================================================
// Naming shortcuts
#define VENUM_INTERN_ENUM(NAME)     VENUM_INTERN_CONCAT(NAME, Enum)
#define VENUM_INTERN_CONSTANT(NAME) VENUM_INTERN_CONCAT(NAME, Constant)

// Pair unpacking (max 2)
#define VENUM_INTERN_PAIR_APPEND_COMMA(X) (X),
#define VENUM_INTERN_PAIR_SEPERATE(X) VENUM_INTERN_PAIR_APPEND_COMMA X
#define VENUM_INTERN_PAIR_UNPACK(X) VENUM_INTERN_UNPACK(X)
#define VENUM_INTERN_PAIR_VAL_1(X) VENUM_INTERN_PAIR_UNPACK(VENUM_INTERN_GET_AT(1, VENUM_INTERN_PAIR_SEPERATE(X)))
#define VENUM_INTERN_PAIR_VAL_2(X) VENUM_INTERN_PAIR_UNPACK(VENUM_INTERN_GET_AT(2, VENUM_INTERN_PAIR_SEPERATE(X)))

#define VENUM_INTERN_PAIR_VAL_1L(X, Y) VENUM_INTERN_PAIR_VAL_1(X)
#define VENUM_INTERN_PAIR_VAL_2L(X, Y) VENUM_INTERN_PAIR_VAL_2(X)

// Loop macros
#define VENUM_INTERN_CONSTANT_DEF_COMMA(...) VENUM_INTERN_EXPAND(VENUM_INTERN_PREP_COMMA(__VA_ARGS__))
#define VENUM_INTERN_CONSTANT_DEF(X, Y) \
        EnumConstant(__NAMES[static_cast<int>(__ORDINALS::VENUM_INTERN_PAIR_VAL_1(X))], \
                     static_cast<int>(__ORDINALS::VENUM_INTERN_PAIR_VAL_1(X)) \
                     VENUM_INTERN_CONSTANT_DEF_COMMA(VENUM_INTERN_PAIR_VAL_2(X)))
#define VENUM_INTERN_CONSTANT_REF(X, Y) &X = __DATA::__VALUES[static_cast<int>(__DATA::__ORDINALS::X)]
#define VENUM_INTERN_CONSTANT_GET_BODY(...) VENUM_INTERN_EXPAND(VENUM_INTERN_PREP_COMMA(__VA_ARGS__))
#define VENUM_INTERN_TRIVIAL_CONST(X, Y) (X)()

#define VENUM_INTERN_PUBLIC_CTOR_CHECK_ASSERT_2(X, Y, Z) static_assert \
                                                         ( \
                                                             !std::is_constructible_v \
                                                             < \
                                                                 venum::enum_defs:: VENUM_INTERN_CONSTANT(Y), \
                                                                 const char*, int VENUM_INTERN_PREP_COMMA(Z) \
                                                             >, \
                                                             VENUM_INTERN_STRINGIFY(VENUM_INTERN_PAIR_VAL_1(X)) \
                                                             "'s constructor is public which is not " \
                                                             "allowed (CTOR_PUBLIC)" \
                                                         );

#define VENUM_INTERN_PUBLIC_CTOR_CHECK_ASSERT(X, Y) VENUM_INTERN_PUBLIC_CTOR_CHECK_ASSERT_2 \
                                                    ( \
                                                        X, Y, VENUM_INTERN_UNICTOR_PARPACK(X) \
                                                    )

#define VENUM_INTERN_PARPACK_DECLTYPE(X, F) std::decay_t<decltype(X)>
#define VENUM_INTERN_MAKE_PARPACK_TYPES(...) VENUM_INTERN_EXPAND \
                                             ( \
                                                 VENUM_INTERN_FOR(VENUM_INTERN_PARPACK_DECLTYPE, __VA_ARGS__) \
                                             )

#define VENUM_INTERN_UNICTOR_PARPACK(X) VENUM_INTERN_MAKE_PARPACK_TYPES(VENUM_INTERN_PAIR_VAL_2(X))
#define VENUM_INTERN_UNICTOR_ASSERT(X, Y) \
        venum::type_traits::isConstructibleFromArgs<Unictor>(VENUM_INTERN_PAIR_VAL_2(X))

#define VENUM_INTERN_GET_CONSTEXPR(...) VENUM_INTERN_GET_ATTRIB(RETENTION, __VA_ARGS__)

#define VENUM_INTERN_CREATE_ASSOC(ID, VALUES, BODY, ATTRIBUTES) \
namespace venum::enum_defs \
{ \
VENUM_INTERN_SECTOR_CONSTANT_CLASS(ID, (BODY), VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES)) \
struct VENUM_INTERN_ENUM(ID) \
{ \
private: \
    template<class, class> friend class venum::EnumMap; \
    VENUM_INTERN_GET_ATTRIB(CTOR_UNIFORM, ATTRIBUTES)(VALUES) \
    \
public: \
    using EnumConstant = VENUM_INTERN_CONSTANT(ID); \
    \
private: \
    VENUM_INTERN_SECTOR_DATA(ID, VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VALUES) \
    \
public: \
    using ID = VENUM_INTERN_ENUM(ID); \
    VENUM_INTERN_SECTOR_TRAITS(VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES)) \
    static VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES) \
           const std::array<EnumConstant, __DATA::__LENGTH> &values() noexcept { return __DATA::__VALUES; } \
    VENUM_INTERN_SECTOR_CONSTANTS(VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VALUES) \
    VENUM_INTERN_SECTOR_BODY(ID, VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES)) \
    VENUM_INTERN_SECTOR_UTILITY(ID, VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES), \
                                VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), \
                                VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1, VALUES)) \
    \
private: \
    const EnumConstant *constantValue VENUM_DEFAULT_POINTER; \
}; \
} \
VENUM_INTERN_GET_ATTRIB(CTOR_PRIVATE, ATTRIBUTES)(ID, VALUES) \
using ID = venum::enum_defs::VENUM_INTERN_ENUM(ID);

//======================================================================================================================
// endregion helpers
/**********************************************************************************************************************/
// region sectors
//======================================================================================================================
// comparison
#define VENUM_INTERN_SECTOR_COMPARISON(NAME, CONSTEXPR, ALLOW_NULL) \
        ALLOW_NULL(true)(CONSTEXPR bool operator==(std::nullptr_t) const noexcept { return !constantValue; } \
        CONSTEXPR bool operator!=(std::nullptr_t) const noexcept { return constantValue; }) \
        CONSTEXPR bool operator==(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() == other->ordinal(); } \
        CONSTEXPR bool operator!=(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() != other->ordinal(); } \
        CONSTEXPR bool operator==(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() == constant.ordinal(); } \
        CONSTEXPR bool operator!=(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() != constant.ordinal(); } \
        CONSTEXPR bool operator>(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal()  > other->ordinal(); } \
        CONSTEXPR bool operator>=(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() >= other->ordinal(); } \
        CONSTEXPR bool operator<(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal()  < other->ordinal(); } \
        CONSTEXPR bool operator<=(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() <= other->ordinal(); } \
        CONSTEXPR bool operator>(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal()  > constant.ordinal(); } \
        CONSTEXPR bool operator>=(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() >= constant.ordinal(); } \
        CONSTEXPR bool operator<(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal()  < constant.ordinal(); } \
        CONSTEXPR bool operator<=(const EnumConstant &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() <= constant.ordinal(); }

#define VENUM_INTERN_SECTOR_TRAITS(ALLOW_NULL) \
        struct Traits \
        { \
            static constexpr bool acceptsNullValues = ALLOW_NULL(true)(true) ALLOW_NULL(false)(false); \
        };

// Assignment and cast/return
#define VENUM_INTERN_SECTOR_EXCHANGE(NAME, CONSTEXPR, ALLOW_NULL) \
        CONSTEXPR NAME &operator=(const EnumConstant &constant) noexcept { constantValue = &constant; return *this; } \
        ALLOW_NULL(true)(CONSTEXPR NAME &operator=(std::nullptr_t) noexcept { constantValue = nullptr; return *this; }) \
        ALLOW_NULL(true)(CONSTEXPR explicit operator bool() const noexcept { return constantValue; }) \
        CONSTEXPR operator int() const noexcept { assert(constantValue); return constantValue->ordinal(); }

// Construct
#define VENUM_INTERN_SECTOR_CONSTRUCT(NAME, CONSTEXPR, ALLOW_NULL) \
        ALLOW_NULL(true)(VENUM_INTERN_ENUM(NAME)() noexcept = default;) \
        CONSTEXPR VENUM_INTERN_ENUM(NAME)(const EnumConstant &constant) noexcept : constantValue(&constant) {} \
        ALLOW_NULL(true)(CONSTEXPR VENUM_INTERN_ENUM(NAME)(std::nullptr_t) noexcept : constantValue(nullptr) {})

// Resolve constant
#define VENUM_INTERN_SECTOR_RESOLVE(CONSTEXPR) \
        CONSTEXPR const EnumConstant &operator*()  const noexcept { assert(constantValue); return *constantValue; } \
        CONSTEXPR const EnumConstant *operator->() const noexcept { assert(constantValue); return  constantValue; }

// Default enum body
#define VENUM_INTERN_SECTOR_BODY(NAME, CONSTEXPR, ALLOW_NULL) \
        VENUM_INTERN_SECTOR_COMPARISON(NAME, CONSTEXPR, ALLOW_NULL) VENUM_INTERN_SECTOR_EXCHANGE(NAME, CONSTEXPR, ALLOW_NULL) \
        VENUM_INTERN_SECTOR_CONSTRUCT(NAME, CONSTEXPR, ALLOW_NULL)  VENUM_INTERN_SECTOR_RESOLVE(CONSTEXPR)

// Enum data
#define VENUM_INTERN_SECTOR_DATA(NAME, CONSTEXPR, ...) VENUM_INTERN_EXPAND \
        ( \
        struct __DATA \
        { \
            static CONSTEXPR const int __LENGTH { VENUM_INTERN_SIZE(__VA_ARGS__) }; \
            \
            enum class __ORDINALS { VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, __VA_ARGS__) }; \
            \
            static CONSTEXPR const std::array<const char*, __LENGTH> __NAMES \
            { \
                VENUM_INTERN_STRINGIFY(VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, __VA_ARGS__)) \
            }; \
            \
            static CONSTEXPR const std::array<EnumConstant, __LENGTH> __VALUES \
            { \
                VENUM_INTERN_FOR(VENUM_INTERN_CONSTANT_DEF, __VA_ARGS__) \
            };\
        }; \
        )

#define VENUM_INTERN_SECTOR_CONSTANT_CLASS(NAME, BODY, CONSTEXPR) \
        class VENUM_INTERN_CONSTANT(NAME) \
        { \
        public: \
            CONSTEXPR const char *name() const noexcept { return nameValue; } \
            CONSTEXPR int ordinal() const noexcept { return ordinalValue; } \
            CONSTEXPR operator int() const noexcept { return ordinalValue; } \
            \
        private: \
            friend class VENUM_INTERN_ENUM(NAME); \
            \
            const char *const nameValue; \
            const int ordinalValue; \
            \
            CONSTEXPR VENUM_INTERN_CONSTANT(NAME)(const char *name, int ordinal) noexcept \
                : nameValue(name), ordinalValue(ordinal) {} \
            VENUM_INTERN_CONSTANT(NAME)() = delete; \
            VENUM_INTERN_CONSTANT(NAME)(const VENUM_INTERN_CONSTANT(NAME)&) = default; \
            VENUM_INTERN_CONSTANT(NAME)(VENUM_INTERN_CONSTANT(NAME)&&)      = default; \
            \
        public: \
            VENUM_INTERN_UNPACK(BODY) \
        };
        
// Enum constants
#define VENUM_INTERN_SECTOR_CONSTANTS(CONSTEXPR, ...) VENUM_INTERN_EXPAND \
                                                      ( \
                                                          static CONSTEXPR const EnumConstant VENUM_INTERN_FOR \
                                                          ( \
                                                              VENUM_INTERN_CONSTANT_REF, \
                                                              VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, __VA_ARGS__) \
                                                          ) \
                                                      );
                                           
#define VENUM_INTERN_SECTOR_UNISON_CTOR_DEF(...) VENUM_INTERN_EXPAND \
                                                 ( \
                                                     using Unictor = venum::type_traits::unisonConstructor \
                                                                     < \
                                                                            VENUM_INTERN_UNICTOR_PARPACK \
                                                                            ( \
                                                                                VENUM_INTERN_GET_AT(1, __VA_ARGS__) \
                                                                            ) \
                                                                     > \
                                                 );

#define VENUM_INTERN_SECTOR_UNISON_CTOR_CHECK(...) VENUM_INTERN_EXPAND \
                                                   ( \
                                                       static_assert \
                                                       ( \
                                                           venum::type_traits::allAreTrue \
                                                           < \
                                                               VENUM_INTERN_FOR \
                                                               ( \
                                                                   VENUM_INTERN_UNICTOR_ASSERT, __VA_ARGS__ \
                                                               ) \
                                                           >, \
                                                           "Constants must have a uniform constructor (CTOR_UNIFORM)" \
                                                       ) \
                                                   );

#define VENUM_INTERN_SECTOR_UTILITY(NAME, ALLOW_NULL, CONSTEXPR, VALUES) \
        static CONSTEXPR NAME valueOf(const char *name) ALLOW_NULL(true)(noexcept) \
        { \
            if (name == nullptr) \
                ALLOW_NULL(true)(return nullptr;) \
                ALLOW_NULL(false)(throw std::logic_error(VENUM_INTERN_STRINGIFY(NAME) \
                                                         " doesn't accept null constants!")); \
            \
            for (int i = 0; i < __DATA::__LENGTH; ++i) \
            { \
                const char *iname = __DATA::__NAMES[i]; \
                int char_count    = 0;\
                bool equals       = false; \
                \
                for (;;) \
                { \
                    const char ch_name  = name [char_count]; \
                    const char ch_iname = iname[char_count++];\
                    \
                    if (!ch_name || !ch_iname) \
                    { \
                        equals = (ch_name == ch_iname); \
                        break; \
                    } \
                    \
                    if (ch_name != ch_iname) break; \
                } \
                \
                if (!equals) continue; \
                \
                return __DATA::__VALUES[i]; \
            } \
            \
            ALLOW_NULL(true)(return nullptr;) \
            ALLOW_NULL(false)(throw std::logic_error(VENUM_INTERN_STRINGIFY(NAME) " doesn't contain any constants " \
                                                     "with the specified name!")); \
        }
        
#define VENUM_INTERN_SECTOR_PUBLIC_CTOR_CHECK(NAME, ...) VENUM_INTERN_EXPAND \
                                                         ( \
                                                             VENUM_INTERN_FOR_UNCO_WD \
                                                             ( \
                                                                 VENUM_INTERN_PUBLIC_CTOR_CHECK_ASSERT, \
                                                                 NAME, __VA_ARGS__ \
                                                             ) \
                                                         )
                                                         
//======================================================================================================================
// endregion sectors
/**********************************************************************************************************************/
// region keywords
//======================================================================================================================
// ID keyword
#define VENUM_INTERN_KEYWORD_ID_ID(...) __VA_ARGS__
#define VENUM_INTERN_KEYWORD_ID_VALUES(...)
#define VENUM_INTERN_KEYWORD_ID_BODY(...)
#define VENUM_INTERN_KEYWORD_ID_ATTRIB(...)
#define VENUM_INTERN_KEYWORD_ID_UNPACK(X, Y) VENUM_INTERN_KEYWORD_ID_##X
#define VENUM_INTERN_KEYWORD_ID(...) VENUM_INTERN_EXPAND \
                                     ( \
                                         VENUM_INTERN_FOR_UNCO(VENUM_INTERN_KEYWORD_ID_UNPACK, __VA_ARGS__) \
                                     )

// VALUES keyword
#define VENUM_INTERN_KEYWORD_VALUES_ID(...)
#define VENUM_INTERN_KEYWORD_VALUES_VALUES(...) __VA_ARGS__
#define VENUM_INTERN_KEYWORD_VALUES_BODY(...)
#define VENUM_INTERN_KEYWORD_VALUES_ATTRIB(...)
#define VENUM_INTERN_KEYWORD_VALUES_UNPACK(X, Y) VENUM_INTERN_KEYWORD_VALUES_##X
#define VENUM_INTERN_KEYWORD_VALUES(...) VENUM_INTERN_EXPAND \
                                         ( \
                                             VENUM_INTERN_FOR_UNCO(VENUM_INTERN_KEYWORD_VALUES_UNPACK, __VA_ARGS__) \
                                         )

// BODY keyword
#define VENUM_INTERN_KEYWORD_BODY_ID(...)
#define VENUM_INTERN_KEYWORD_BODY_VALUES(...)
#define VENUM_INTERN_KEYWORD_BODY_BODY(...) __VA_ARGS__
#define VENUM_INTERN_KEYWORD_BODY_ATTRIB(...)
#define VENUM_INTERN_KEYWORD_BODY_UNPACK(X, Y) VENUM_INTERN_KEYWORD_BODY_##X
#define VENUM_INTERN_KEYWORD_BODY(...) VENUM_INTERN_EXPAND \
                                       ( \
                                           VENUM_INTERN_FOR_UNCO(VENUM_INTERN_KEYWORD_BODY_UNPACK, __VA_ARGS__) \
                                       )

// ATTRIB keyword
#define VENUM_INTERN_KEYWORD_ATTRIB_ID(...)
#define VENUM_INTERN_KEYWORD_ATTRIB_VALUES(...)
#define VENUM_INTERN_KEYWORD_ATTRIB_BODY(...)
#define VENUM_INTERN_KEYWORD_ATTRIB_ATTRIB(...) __VA_ARGS__
#define VENUM_INTERN_KEYWORD_ATTRIB_UNPACK(X, Y) VENUM_INTERN_KEYWORD_ATTRIB_##X
#define VENUM_INTERN_KEYWORD_ATTRIB(...) VENUM_INTERN_EXPAND \
                                         ( \
                                             VENUM_INTERN_FOR_UNCO(VENUM_INTERN_KEYWORD_ATTRIB_UNPACK, __VA_ARGS__) \
                                         )

// Keyword resolver
#define VENUM_INTERN_GET_KEYWORD_1(NAME, ...) VENUM_INTERN_EXPAND \
                                              ( \
                                                  VENUM_INTERN_CONCAT(VENUM_INTERN_KEYWORD_, NAME)(__VA_ARGS__) \
                                              )

#define VENUM_INTERN_GET_KEYWORD(NAME, ...) VENUM_INTERN_EXPAND \
                                            ( \
                                                VENUM_INTERN_GET_KEYWORD_1(NAME, __VA_ARGS__) \
                                            )
//======================================================================================================================
// endregion keywords
/**********************************************************************************************************************/
// region attributes
//======================================================================================================================
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_EXPAND(...) \
        VENUM_INTERN_SECTOR_UNISON_CTOR_DEF(__VA_ARGS__) \
        VENUM_INTERN_SECTOR_UNISON_CTOR_CHECK(__VA_ARGS__);
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_true() VENUM_INTERN_ATTRIB_CTOR_UNIFORM_EXPAND
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_false() VENUM_INTERN_EMPTY
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_CTOR_UNIFORM(X, ...) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_CTOR_UNIFORM_, X)()
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_CTOR_PRIVATE(...)
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_RETENTION(...)
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_NULL_CONST(...)
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_UNPACK(X, Y) VENUM_INTERN_ATTRIB_CTOR_UNIFORM_##X
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM(...) VENUM_INTERN_EXPAND \
                                              ( \
                                                  VENUM_INTERN_FOR_UNCO \
                                                  ( \
                                                      VENUM_INTERN_ATTRIB_CTOR_UNIFORM_UNPACK, __VA_ARGS__ \
                                                  ) \
                                              )

#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_EXPAND(ID, ...) VENUM_INTERN_SECTOR_PUBLIC_CTOR_CHECK(ID, __VA_ARGS__)
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_true() VENUM_INTERN_ATTRIB_CTOR_PRIVATE_EXPAND
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_false() VENUM_INTERN_EMPTY
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_CTOR_UNIFORM(...)
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_CTOR_PRIVATE(X, ...) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_CTOR_PRIVATE_, X)()
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_RETENTION(...)
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_NULL_CONST(...)
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_UNPACK(X, Y) VENUM_INTERN_ATTRIB_CTOR_PRIVATE_##X
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE(...) VENUM_INTERN_EXPAND \
                                              ( \
                                                  VENUM_INTERN_FOR_UNCO \
                                                  ( \
                                                      VENUM_INTERN_ATTRIB_CTOR_PRIVATE_UNPACK, __VA_ARGS__ \
                                                  ) \
                                              )
                                              
#define VENUM_INTERN_ATTRIB_RETENTION_RUNTIME() inline
#define VENUM_INTERN_ATTRIB_RETENTION_CLASS() constexpr
#define VENUM_INTERN_ATTRIB_RETENTION_CTOR_UNIFORM(...)
#define VENUM_INTERN_ATTRIB_RETENTION_CTOR_PRIVATE(...)
#define VENUM_INTERN_ATTRIB_RETENTION_RETENTION(X, ...) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_RETENTION_, X)()
#define VENUM_INTERN_ATTRIB_RETENTION_NULL_CONST(...)
#define VENUM_INTERN_ATTRIB_RETENTION_UNPACK(X, Y) VENUM_INTERN_ATTRIB_RETENTION_##X
#define VENUM_INTERN_ATTRIB_RETENTION(...) VENUM_INTERN_EXPAND \
                                           ( \
                                               VENUM_INTERN_FOR_UNCO \
                                               ( \
                                                   VENUM_INTERN_ATTRIB_RETENTION_UNPACK, __VA_ARGS__ \
                                               ) \
                                           )

#define VENUM_INTERN_ATTRIB_NULL_CONST_EXPAND()
#define VENUM_INTERN_ATTRIB_NULL_CONST_true_true() VENUM_INTERN_EXPAND
#define VENUM_INTERN_ATTRIB_NULL_CONST_true_false() VENUM_INTERN_EMPTY
#define VENUM_INTERN_ATTRIB_NULL_CONST_false_false() VENUM_INTERN_EXPAND
#define VENUM_INTERN_ATTRIB_NULL_CONST_false_true() VENUM_INTERN_EMPTY

#define VENUM_INTERN_ATTRIB_NULL_CONST_true(X) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_NULL_CONST_true_, X)()
#define VENUM_INTERN_ATTRIB_NULL_CONST_false(X) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_NULL_CONST_false_, X)()

#define VENUM_INTERN_ATTRIB_NULL_CONST_CTOR_UNIFORM(...)
#define VENUM_INTERN_ATTRIB_NULL_CONST_CTOR_PRIVATE(...)
#define VENUM_INTERN_ATTRIB_NULL_CONST_RETENTION(...)
#define VENUM_INTERN_ATTRIB_NULL_CONST_NULL_CONST(X, ...) VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_NULL_CONST_, X)
#define VENUM_INTERN_ATTRIB_NULL_CONST_UNPACK(X, Y) VENUM_INTERN_ATTRIB_NULL_CONST_##X
#define VENUM_INTERN_ATTRIB_NULL_CONST(...) VENUM_INTERN_EXPAND \
                                            ( \
                                                VENUM_INTERN_FOR_UNCO \
                                                ( \
                                                    VENUM_INTERN_ATTRIB_NULL_CONST_UNPACK, __VA_ARGS__ \
                                                ) \
                                            )

// Defaults
#define VENUM_INTERN_ATTRIB_CTOR_UNIFORM_DEFAULT() VENUM_INTERN_ATTRIB_CTOR_UNIFORM_true()
#define VENUM_INTERN_ATTRIB_CTOR_PRIVATE_DEFAULT() VENUM_INTERN_ATTRIB_CTOR_PRIVATE_true()
#define VENUM_INTERN_ATTRIB_RETENTION_DEFAULT()    VENUM_INTERN_ATTRIB_RETENTION_CLASS()
#define VENUM_INTERN_ATTRIB_NULL_CONST_DEFAULT()   VENUM_INTERN_ATTRIB_NULL_CONST_true

#define VENUM_INTERN_COND_HAS_ATTRIB_0(...) FALSE
#define VENUM_INTERN_COND_HAS_ATTRIB_1(...) TRUE
#define VENUM_INTERN_COND_HAS_ATTRIB_R(...) VENUM_INTERN_EXPAND \
                                            ( \
                                                VENUM_INTERN_OVERLOAD(VENUM_INTERN_COND_HAS_ATTRIB, __VA_ARGS__) \
                                            )
                                            
#define VENUM_INTERN_COND_HAS_ATTRIB(NAME, ...) VENUM_INTERN_COND_HAS_ATTRIB_R(VENUM_INTERN_GET_ATTRIB_1(NAME, __VA_ARGS__))

#define VENUM_INTERN_GET_ATTRIB_DEFAULT_1(NAME) VENUM_INTERN_CONCAT(NAME, _DEFAULT)
#define VENUM_INTERN_GET_ATTRIB_DEFAULT(NAME) VENUM_INTERN_GET_ATTRIB_DEFAULT_1(VENUM_INTERN_ATTRIB_##NAME)()

// Attribute resolver
#define VENUM_INTERN_GET_ATTRIB_1(NAME, ...) VENUM_INTERN_EXPAND \
                                             ( \
                                                 VENUM_INTERN_CONCAT(VENUM_INTERN_ATTRIB_, NAME)(__VA_ARGS__) \
                                             )
                                             
#define VENUM_INTERN_GET_ATTRIB(NAME, ...) VENUM_INTERN_EXPAND \
                                           ( \
                                               VENUM_INTERN_IF_ELSE \
                                               ( \
                                                   VENUM_INTERN_COND_HAS_ATTRIB(NAME, __VA_ARGS__), \
                                                   VENUM_INTERN_GET_ATTRIB_1(NAME, __VA_ARGS__), \
                                                   VENUM_INTERN_GET_ATTRIB_DEFAULT(NAME) \
                                               ) \
                                           )

//======================================================================================================================
// endregion attributes
/**********************************************************************************************************************/
// region public
//======================================================================================================================
#define VENUM_BASE(NAME, ORDINAL) nameValue(NAME), ordinalValue(ORDINAL)

#define VENUM_CREATE_ASSOC(...) VENUM_INTERN_EXPAND \
                                ( \
                                    VENUM_INTERN_CREATE_ASSOC \
                                    ( \
                                         VENUM_INTERN_GET_KEYWORD(ID,     __VA_ARGS__), \
                                         VENUM_INTERN_GET_KEYWORD(VALUES, __VA_ARGS__), \
                                         VENUM_INTERN_GET_KEYWORD(BODY,   __VA_ARGS__), \
                                         VENUM_INTERN_GET_KEYWORD(ATTRIB, __VA_ARGS__)  \
                                    ) \
                                )

#define VENUM_CREATE(NAME, ...) VENUM_INTERN_EXPAND \
                                ( \
                                    VENUM_CREATE_ASSOC \
                                    ( \
                                        ID(NAME), \
                                        VALUES(VENUM_INTERN_FOR(VENUM_INTERN_TRIVIAL_CONST, __VA_ARGS__)) \
                                    ) \
                                )
//======================================================================================================================
// endregion public
/**********************************************************************************************************************/
// region classes
//======================================================================================================================
namespace venum
{
    namespace type_traits
    {
    template<class Type, class ...Args>
    constexpr bool isConstructibleFromArgs(Args &&...args) noexcept
    {
        return std::is_constructible_v<Type, std::decay_t<decltype(args)>...>;
    }
    
    template<bool ...Conditions> constexpr bool allAreTrue = (Conditions && ...);
    
    template<class ...Args>
    struct unisonConstructor final
    {
        constexpr unisonConstructor(Args &&...args) noexcept
        {}
    };
    
    template<>
    struct unisonConstructor<void> final
    {
        constexpr unisonConstructor() noexcept {}
    };
    }

template<class EnumType, class ValueType>
class EnumMap
{
public:
    static_assert(std::is_default_constructible_v<ValueType>, "ValueType must be a DefaultConstructible type");
    
    //==================================================================================================================
    /* struct Iterator
    {
        using self_type         = Iterator;
        using value_type        = std::pair<const typename EnumType::EnumConstant&, ValueType>;
        using reference         = std::pair<const typename EnumType::EnumConstant&, ValueType&>;
        using const_reference   = std::pair<const typename EnumType::EnumConstant&, const ValueType&>;
        using pointer           = std::pair<const typename EnumType::EnumConstant&, ValueType*>;
        using const_pointer     = std::pair<const typename EnumType::EnumConstant&, const ValueType*>;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = int;
        
        
    };*/
    
    //==================================================================================================================
    ValueType& operator[](EnumType key) noexcept
    {
        if (key.constantValue == nullptr)
        {
            return data[data.size() - 1];
        }
        
        return data[key->ordinal()];
    }
    
    const ValueType& operator[](EnumType key) const noexcept
    {
        if (key.constantValue == nullptr)
        {
            return data[data.size() - 1];
        }
        
        return data[key->ordinal()];
    }
    
    ValueType& operator[](const typename EnumType::EnumConstant &key) noexcept
    {
        return data[key.ordinal()];
    }
    
    const ValueType& operator[](const typename EnumType::EnumConstant &key) const noexcept
    {
        return data[key.ordinal()];
    }

    
    //==================================================================================================================
    /**
     *  Resets all constant mapped values to their respective type's default.
     */
    constexpr void clear() noexcept
    {
        std::fill_n(data.begin(), data.end(), ValueType());
    }
    
    //==================================================================================================================
    /**
     *  Returns true if either the enum contains no constants or all mapped constants have default values.
     *  @return True if default or empty
     */
    constexpr bool empty() const noexcept
    {
        for (auto &value : data)
        {
            if (value != ValueType())
            {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     *  Gets the size of the map.
     *  @return The size
     */
    constexpr int size() const noexcept
    {
        return data.size();
    }
    
    //==================================================================================================================
    auto begin() noexcept
    {
        return data.begin();
    }
    
    auto end() noexcept
    {
        return data.end();
    }
    
    auto begin() const noexcept
    {
        return data.begin();
    }
    
    auto end() const noexcept
    {
        return data.end();
    }
    
    auto rbegin() noexcept
    {
        return data.rbegin();
    }
    
    auto rend() noexcept
    {
        return data.rend();
    }
    
    auto rbegin() const noexcept
    {
        return data.rbegin();
    }
    
    auto rend() const noexcept
    {
        return data.rend();
    }
    
    auto cbegin() const noexcept
    {
        return data.cbegin();
    }
    
    auto cend() const noexcept
    {
        return data.cend();
    }
    
    auto crbegin() const noexcept
    {
        return data.crbegin();
    }
    
    auto crend() const noexcept
    {
        return data.crend();
    }
    
private:
    static constexpr int getEndLength() noexcept
    {
        return EnumType::__DATA::__LENGTH + (EnumType::Traits::acceptsNullValues ? 1 : 0);
    }
    
    std::array<ValueType, getEndLength()> data;
};
}
//======================================================================================================================
// endregion classes
/**********************************************************************************************************************/
