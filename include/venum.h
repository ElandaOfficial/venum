
#pragma once

#ifndef VENUM_DEFAULT_INITIALISE
#   define VENUM_DEFAULT_INITIALISE 1
#endif

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
#include <bitset>
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
                                   
#define VENUM_INTERN_IF_true(X) X
#define VENUM_INTERN_IF_false(X)
#define VENUM_INTERN_IF_NOT_true(X)
#define VENUM_INTERN_IF_NOT_false(X) X

#define VENUM_INTERN_IF_CAT(COND) VENUM_INTERN_CONCAT(VENUM_INTERN_IF_, COND)
#define VENUM_INTERN_IF(COND, ACTION) VENUM_INTERN_IF_CAT(COND) (ACTION)

#define VENUM_INTERN_IF_NOT_CAT(COND) VENUM_INTERN_CONCAT(VENUM_INTERN_IF_NOT_, COND)
#define VENUM_INTERN_IF_NOT(COND, ACTION) VENUM_INTERN_IF_NOT_CAT(COND) (ACTION)

#define VENUM_INTERN_IF_ELSE(COND, true_ACT, false_ACT) \
        VENUM_INTERN_IF(COND, true_ACT) VENUM_INTERN_IF_NOT(COND, false_ACT)

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
        ConstantType(__DATA::__NAMES[static_cast<int>(__DATA::__ORDINALS::VENUM_INTERN_PAIR_VAL_1(X))], \
                     static_cast<int>(__DATA::__ORDINALS::VENUM_INTERN_PAIR_VAL_1(X)) \
                     VENUM_INTERN_CONSTANT_DEF_COMMA(VENUM_INTERN_PAIR_VAL_2(X)))
#define VENUM_INTERN_CONSTANT_REF(X, Y) VENUM_INTERN_PAIR_VAL_1(X) = VENUM_INTERN_CONSTANT_DEF(X, Y)



#define VENUM_INTERN_CONSTANT_GET_BODY(...) VENUM_INTERN_EXPAND(VENUM_INTERN_PREP_COMMA(__VA_ARGS__))
#define VENUM_INTERN_TRIVIAL_CONST(X, Y) (X)()

#define VENUM_INTERN_PUBLIC_CTOR_CHECK_ASSERT_2(X, Y, Z) static_assert \
                                                         ( \
                                                             !std::is_constructible_v \
                                                             < \
                                                                 venum::enum_defs:: VENUM_INTERN_CONSTANT(Y), \
                                                                 const std::string_view, int VENUM_INTERN_PREP_COMMA(Z) \
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
        ::venum::type_traits::isConstructibleFromArgs<Unictor>(VENUM_INTERN_PAIR_VAL_2(X))

#define VENUM_INTERN_GET_CONSTEXPR(...) VENUM_INTERN_GET_ATTRIB(RETENTION, __VA_ARGS__)

#define VENUM_INTERN_GET_IF_CONSTEXPR_constexpr() true
#define VENUM_INTERN_GET_IF_CONSTEXPR_inline() false
#define VENUM_INTERN_GET_IF_CONSTEXPR(X) VENUM_INTERN_CONCAT(VENUM_INTERN_GET_IF_CONSTEXPR_, X)()

#define VENUM_INTERN_CREATE_ASSOC(ID, VALUES, BODY, ATTRIBUTES) \
namespace venum                                                 \
{                                                               \
namespace enum_defs \
{ \
VENUM_INTERN_SECTOR_CONSTANT_CLASS(ID, (BODY), VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES)) \
class VENUM_INTERN_ENUM(ID) \
{ \
private: \
    template<class, class> friend class ::venum::VenumMap; \
    template<class> friend class ::venum::VenumSet; \
    VENUM_INTERN_GET_ATTRIB(CTOR_UNIFORM, ATTRIBUTES)(VALUES) \
    \
public: \
    using ConstantType = VENUM_INTERN_CONSTANT(ID); \
    \
private: \
    VENUM_INTERN_SECTOR_DATA(ID, VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VALUES) \
    \
public: \
    using ID = VENUM_INTERN_ENUM(ID); \
    using TParam = const ConstantType&; \
    VENUM_INTERN_SECTOR_TRAITS(VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES), VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES)) \
    VENUM_INTERN_SECTOR_CONSTANTS(VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VALUES) \
    static VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES) \
           const std::array<::venum::ConstReferenceWrapper<ID>, __DATA::__LENGTH> values \
           { VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, VALUES) }; \
    VENUM_INTERN_SECTOR_BODY(ID, VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES)) \
    VENUM_INTERN_SECTOR_UTILITY(ID, VENUM_INTERN_GET_ATTRIB(NULL_CONST, ATTRIBUTES), \
                                VENUM_INTERN_GET_CONSTEXPR(ATTRIBUTES), \
                                VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1, VALUES)) \
    \
private: \
    const ConstantType *constantValue VENUM_DEFAULT_POINTER; \
}; \
}                                                               \
\
template<> struct is_venum_type<enum_defs::VENUM_INTERN_ENUM(ID)> : std::true_type {};  \
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
        CONSTEXPR bool operator==(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() == constant.ordinal(); } \
        CONSTEXPR bool operator!=(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() != constant.ordinal(); } \
        CONSTEXPR bool operator>(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal()  > other->ordinal(); } \
        CONSTEXPR bool operator>=(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() >= other->ordinal(); } \
        CONSTEXPR bool operator<(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal()  < other->ordinal(); } \
        CONSTEXPR bool operator<=(const NAME &other) const noexcept \
        { assert(constantValue && other.constantValue); return constantValue->ordinal() <= other->ordinal(); } \
        CONSTEXPR bool operator>(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal()  > constant.ordinal(); } \
        CONSTEXPR bool operator>=(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() >= constant.ordinal(); } \
        CONSTEXPR bool operator<(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal()  < constant.ordinal(); } \
        CONSTEXPR bool operator<=(const ConstantType &constant) const noexcept \
        { assert(constantValue); return constantValue->ordinal() <= constant.ordinal(); }

#define VENUM_INTERN_SECTOR_TRAITS(ALLOW_NULL, CONSTEXPR) \
        struct Traits \
        { \
            static constexpr bool acceptsNullValues    = ALLOW_NULL(true)(true) ALLOW_NULL(false)(false); \
            static constexpr bool isConstantExpression = VENUM_INTERN_GET_IF_CONSTEXPR(CONSTEXPR); \
        };

// Assignment and cast/return
#define VENUM_INTERN_SECTOR_EXCHANGE(NAME, CONSTEXPR, ALLOW_NULL) \
        CONSTEXPR NAME &operator=(const ConstantType &constant) noexcept { constantValue = &constant; return *this; } \
        ALLOW_NULL(true)(CONSTEXPR NAME &operator=(std::nullptr_t) noexcept { constantValue = nullptr; return *this; }) \
        ALLOW_NULL(true)(CONSTEXPR operator bool() const noexcept { return constantValue; }) \
        explicit CONSTEXPR operator int() const noexcept { assert(constantValue); return constantValue->ordinal(); }

// Construct
#define VENUM_INTERN_SECTOR_CONSTRUCT(NAME, CONSTEXPR, ALLOW_NULL) \
        ALLOW_NULL(true)(VENUM_INTERN_ENUM(NAME)() noexcept = default;) \
        CONSTEXPR VENUM_INTERN_ENUM(NAME)(const ConstantType &constant) noexcept : constantValue(&constant) {} \
        ALLOW_NULL(true)(CONSTEXPR VENUM_INTERN_ENUM(NAME)(std::nullptr_t) noexcept : constantValue(nullptr) {})

// Resolve constant
#define VENUM_INTERN_SECTOR_RESOLVE(CONSTEXPR) \
        CONSTEXPR const ConstantType &operator*()  const noexcept { assert(constantValue); return *constantValue; } \
        CONSTEXPR const ConstantType *operator->() const noexcept { assert(constantValue); return  constantValue; }

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
            static CONSTEXPR const std::array<const std::string_view, __LENGTH> __NAMES \
            { \
                VENUM_INTERN_STRINGIFY(VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, __VA_ARGS__)) \
            }; \
        }; \
        )

#define VENUM_INTERN_SECTOR_CONSTANT_CLASS(NAME, BODY, CONSTEXPR) \
        class VENUM_INTERN_CONSTANT(NAME) \
        { \
        public: \
            CONSTEXPR const std::string_view name() const noexcept { return nameValue; } \
            CONSTEXPR int ordinal() const noexcept { return ordinalValue; } \
            CONSTEXPR operator int() const noexcept { return ordinalValue; } \
            \
        private: \
            friend class VENUM_INTERN_ENUM(NAME); \
            \
            std::string_view const nameValue; \
            const int ordinalValue; \
            \
            CONSTEXPR VENUM_INTERN_CONSTANT(NAME)(std::string_view name, int ordinal) noexcept \
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
                                                          static CONSTEXPR const ConstantType VENUM_INTERN_FOR \
                                                          ( \
                                                              VENUM_INTERN_CONSTANT_REF, \
                                                              __VA_ARGS__ \
                                                          ) \
                                                      );
//VENUM_INTERN_FOR(VENUM_INTERN_PAIR_VAL_1L, __VA_ARGS__)

#define VENUM_INTERN_SECTOR_UNISON_CTOR_DEF(...) VENUM_INTERN_EXPAND \
                                                 ( \
                                                     using Unictor = ::venum::type_traits::unisonConstructor \
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
                                                           ::venum::type_traits::allAreTrue \
                                                           < \
                                                               VENUM_INTERN_FOR \
                                                               ( \
                                                                   VENUM_INTERN_UNICTOR_ASSERT, __VA_ARGS__ \
                                                               ) \
                                                           >, \
                                                           "Constants must have a uniform constructor (CTOR_UNIFORM)" \
                                                       ) \
                                                   );

#define VENUM_INTERN_SECTOR_UTILITY(NAME, ALLOW_NULL, CONSTEXPR, VALUES)                                               \
        static CONSTEXPR NAME valueOf(std::string_view name, bool ignoreCase = false) ALLOW_NULL(true)(noexcept)       \
        {                                                                                                              \
            for (int i = 0; i < __DATA::__LENGTH; ++i)                                                                 \
            {                                                                                                          \
                const std::string_view const_name = __DATA::__NAMES[i];                                                \
                bool found = true;                                                                                     \
                                                                                                                       \
                if (const_name.length() != name.length()) continue;                                                    \
                                                                                                                       \
                for (int j = 0; j < const_name.length(); ++j)                                                          \
                {                                                                                                      \
                    const char n = (ignoreCase ? std::tolower(name[i])       : name[i]);                               \
                    const char c = (ignoreCase ? std::tolower(const_name[i]) : const_name[i]);                         \
                                                                                                                       \
                    if (n != c)                                                                                        \
                    {                                                                                                  \
                        found = false;                                                                                 \
                        break;                                                                                         \
                    }                                                                                                  \
                }                                                                                                      \
                                                                                                                       \
                if (found) return values[i].get();                                                                     \
            }                                                                                                          \
                                                                                                                       \
            ALLOW_NULL(true)(return nullptr;)                                                                          \
            ALLOW_NULL(false)(throw std::logic_error(VENUM_INTERN_STRINGIFY(NAME) " doesn't contain any constants "    \
                                                     "with the specified name!"));                                     \
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

#define VENUM_INTERN_COND_HAS_ATTRIB_0(...) false
#define VENUM_INTERN_COND_HAS_ATTRIB_1(...) true
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
    
    template<class T>
    class ConstReferenceWrapper
    {
    public:
        using ConstantType = typename T::ConstantType;
        
        constexpr ConstReferenceWrapper(const ConstantType &value) : value(value) {}
        constexpr operator T() const noexcept { return value; }
        constexpr T get() const noexcept { return value; }
        constexpr const ConstantType* operator->() const { return &value; }
    private:
        const ConstantType &value;
    };
    
    template<class VenumType>
    inline constexpr int distance(VenumType from, VenumType to) noexcept
    {
        assert(from != nullptr && to != nullptr); // Can't dereference nullptr
        return from->ordinal() - to->ordinal();
    }
    
    template<class VenumType>
    inline constexpr VenumType max(VenumType val1, VenumType val2) noexcept
    {
        assert(val1 != nullptr && val2 != nullptr); // Can't dereference nullptr
        return val1 > val2 ? val1 : val2;
    }
    
    template<class VenumType>
    inline constexpr VenumType min(VenumType val1, VenumType val2) noexcept
    {
        assert(val1 != nullptr && val2 != nullptr); // Can't dereference nullptr
        return val1 < val2 ? val1 : val2;
    }
    
    template<class VenumType>
    inline constexpr VenumType clamp(VenumType val, VenumType from, VenumType to) noexcept
    {
        assert(from != nullptr && to != nullptr && val != nullptr); // Can't dereference nullptr
        
        VenumType max = max(from, to);
        VenumType min = min(from, to);
        
        return val > max ? max : (val < min ? min : val);
    }
    
    template<class T> struct is_venum_type : std::false_type {};
    
    template<class T>
    inline constexpr bool is_venum_type_v = is_venum_type<T>::value;
    
    /**
     *  A simple lightweight map for venum declarations.
     *  This map implementation will map values to constants, since there is only a given amount of constants this
     *  implementation uses a fixed-size array as underlying container, thus will always occupy the max amount of data
     *  it can hold.
     *
     *  @tparam VenumType The venum
     *  @tparam ValueType The type to map
     */
    template<class VenumType, class ValueType>
    class VenumMap
    {
        static constexpr int getEndLength() noexcept
        {
            return VenumType::__DATA::__LENGTH + (VenumType::Traits::acceptsNullValues ? 1 : 0);
        }
    
        //==============================================================================================================
        using DataMap = std::array<std::pair<VenumType, ValueType>, getEndLength()>;
    
        //==============================================================================================================
        static constexpr bool canBeNull = VenumType::Traits::acceptsNullValues;
        
    public:
        static_assert(std::is_default_constructible_v<ValueType>, "ValueType must be a DefaultConstructible type");
    
        //==============================================================================================================
        using Iterator             = typename DataMap::iterator;
        using ConstIterator        = typename DataMap::const_iterator;
        using ReverseIterator      = typename DataMap::reverse_iterator;
        using ConstReverseIterator = typename DataMap::const_reverse_iterator;
    
        //==============================================================================================================
        /**
         *  Constructs a VenumMap with all constant associated default values of type ValueType.
         */
        constexpr VenumMap() noexcept
        {
            for (const auto &value : VenumType::values)
            {
                data[value->ordinal()] = std::make_pair<VenumType, ValueType>(value, ValueType());
            }
            
            if constexpr (canBeNull)
            {
                data[getEndLength() - 1] = std::make_pair<VenumType, ValueType>(nullptr, ValueType());
            }
        }
        
        /**
         *  Constructs a VenumMap with all the given values.
         *  Note that the internal array will only accept that many values of the given initializer_list as it can hold.
         *
         *  @param values The values to store for the constants.
         */
        constexpr VenumMap(std::initializer_list<ValueType> values) noexcept
            : VenumMap()
        {
            for (auto it = values.begin(); it != values.end(); ++it)
            {
                const int dist = std::distance(it, values.begin());
                
                if (dist >= data.size())
                {
                    break;
                }
                
                data[dist].second = *it;
            }
        }
    
        //==============================================================================================================
        constexpr ValueType& operator[](VenumType key) noexcept
        {
            if constexpr (canBeNull)
            {
                if (key.constantValue == nullptr)
                {
                    return data[data.size() - 1].second;
                }
            }
            
            return data[key->ordinal()].second;
        }
        
        constexpr const ValueType& operator[](VenumType key) const noexcept
        {
            if constexpr (canBeNull)
            {
                if (key.constantValue == nullptr)
                {
                    return data[data.size() - 1].second;
                }
            }
            
            return data[key->ordinal()].second;
        }
        
        constexpr ValueType& operator[](const typename VenumType::ConstantType &key) noexcept
        {
            return data[key.ordinal()].second;
        }
        
        constexpr const ValueType& operator[](const typename VenumType::ConstantType &key) const noexcept
        {
            return data[key.ordinal()].second;
        }
    
        //==============================================================================================================
        /**
         *  Resets all constant mapped values to their respective type's default.
         */
        constexpr void clear() noexcept
        {
            DataMap empty_map;
            std::swap(data, empty_map);
        }
    
        //==============================================================================================================
        /**
         *  Returns true if the map doesn't contain any constants.
         *  Note that this will only be true if the given venum doesn't contain any constants.
         *
         *  @return True if defaulted or empty
         */
        constexpr bool empty() const noexcept
        {
            return data.empty();
        }
        
        /**
         *  Gets the size of the map.
         *  Usually, this will return the venums amount of constants (+1 if the venum allows null constants).
         *
         *  @return The size
         */
        constexpr int size() const noexcept
        {
            return data.size();
        }
    
        //==============================================================================================================
        constexpr Iterator begin() noexcept { return data.begin(); }
        constexpr Iterator end()   noexcept { return data.end(); }
        
        constexpr ConstIterator begin() const noexcept { return data.begin(); }
        constexpr ConstIterator end()   const noexcept { return data.end();}
        
        constexpr ConstIterator cbegin() const noexcept { return data.cbegin(); }
        constexpr ConstIterator cend()   const noexcept { return data.cend(); }
        
        constexpr ReverseIterator rbegin() noexcept { return data.rbegin(); }
        constexpr ReverseIterator rend()   noexcept { return data.rend(); }
        
        constexpr ConstReverseIterator rbegin() const noexcept { return data.rbegin(); }
        constexpr ConstReverseIterator rend()   const noexcept { return data.rend(); }
        
        constexpr ConstReverseIterator crbegin() const noexcept { return data.crbegin(); }
        constexpr ConstReverseIterator crend()   const noexcept { return data.crend(); }
        
    private:
        DataMap data;
    };
    
    /**
     *  A simple lightweight set for venum declarations.
     *  This set implementation will map values to bits at positions that matches the constants ordinals, since there is
     *  only a given amount of constants this implementation uses a bitset as underlying container, thus will always
     *  occupy the max amount of data it can hold.
     *
     *  Note that while VenumMap allows null-constants, VenumSet does not.
     *
     *  @tparam VenumType The venum
     */
    template<class VenumType>
    class VenumSet
    {
        //==============================================================================================================
        using DataSet = std::bitset<VenumType::__DATA::__LENGTH>;
    
        //==============================================================================================================
        static constexpr bool canBeNull = VenumType::Traits::acceptsNullValues;
        
    public:
        //==============================================================================================================
        class VenumSetIterator
        {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = int;
            using value_type        = VenumType;
            using reference         = value_type;
            using pointer           = value_type;
    
            //==========================================================================================================
            VenumSetIterator(const VenumSet &set, int position = 0) noexcept
                : position(position), set(set)
            {}
    
            //==========================================================================================================
            bool operator==(const VenumSetIterator &other) const noexcept
            {
                return position == other.position && &set == &other.set;
            }
        
            bool operator!=(const VenumSetIterator &other) const noexcept
            {
                return position != other.position || &set != &other.set;
            }
    
            //==========================================================================================================
            reference operator*() const
            {
                auto iterator = set.data.test(position) ? Iterator_Start + position : nullptr;
                assert(iterator);
                return *iterator;
            }
        
            pointer operator->() const
            {
                auto iterator = set.data.test(position) ? Iterator_Start + position : nullptr;
                assert(iterator);
                return *iterator;
            }
    
            //==========================================================================================================
            VenumSetIterator& operator++()
            {
                while (++position <= set.msb && !set.data.test(position));
                return *this;
            }
        
            VenumSetIterator operator++(int)
            {
                VenumSetIterator it (*this);
                ++(*this);
                return it;
            }
        
            VenumSetIterator& operator--()
            {
                while (--position > 0 && !set.data.test(position));
                return *this;
            }
        
            VenumSetIterator operator--(int)
            {
                VenumSetIterator it (*this);
                --position;
                return it;
            }
    
        private:
            friend class VenumSet;
    
            //==========================================================================================================
            using IteratorType = decltype(VenumType::values.begin());
    
            //==========================================================================================================
            static constexpr IteratorType Iterator_Start = VenumType::values.begin();
    
            //==========================================================================================================
            int position { 0 };
            const VenumSet &set;
        };
    
        //==============================================================================================================
        using ConstantType    = typename VenumType::ConstantType;
        using Iterator        = VenumSetIterator;
        using ReverseIterator = std::reverse_iterator<Iterator>;
    
        //==============================================================================================================
        /**
         *  Gets a new VenumSet with all constants set.
         *  @return A full set
         */
        constexpr static VenumSet all() noexcept
        {
            VenumSet set(DataSet().set());
            set.msb = VenumType::values.size() - 1;
            return set;
        }
        
        /**
         *  Gets a new VenumSet that contains the opposite constants of the given VenumSet.
         *  This means, constants that are set will be removed and the other way around.
         *
         *  @param other The other VenumSet to invert
         *  @return The complemented VenumSet
         */
        constexpr static VenumSet complementOf(const VenumSet &other) noexcept
        {
            VenumSet set(DataSet(other.data).flip());
            
            for (int i = (set.data.size() - 1); i >= 0; --i)
            {
                if (set.data[i])
                {
                    set.msb = i;
                    break;
                }
            }
            
            return set;
        }
        
        /**
         *  Gets a new VenumSet from a range of constants.
         *
         *  @param from The first point of the range of constants to add
         *  @param to   The second point of the range of constants to add
         */
        constexpr static VenumSet range(VenumType from, VenumType to) noexcept
        {
            if constexpr (canBeNull)
            {
                if (from == nullptr || to == nullptr)
                {
                    return {};
                }
            }
            
            const int end = max(from, to)->ordinal();
            VenumSet set;
            
            for (int i = min(from, to)->ordinal(); i <= end; ++i)
            {
                set.data[i] = true;
            }
            
            set.msb = end;
            
            return set;
        }
    
        //==============================================================================================================
        /**
         *  Constructs a new VenumSet without any set constants.
         */
        VenumSet() = default;
        
        /**
         *  Constructs a new VenumSet from a container full of constants and adopts these.
         *  Null-constants will be ignored.
         *
         *  @tparam Container The container type
         *  @param container The container to adopt the constants from
         */
        template<template<class...> class Container>
        explicit VenumSet(const Container<VenumType> &container)
        {
            for (auto &constant : container)
            {
                if constexpr (canBeNull)
                {
                    if (constant == nullptr)
                    {
                        continue;
                    }
                }
                
                const int ordinal = constant->ordinal();
                
                if (ordinal > msb)
                {
                    msb = ordinal;
                }
                
                data[ordinal] = true;
            }
        }
        
        /**
         *  Constructs a new VenumSet from a list of constants.
         *  @tparam Constants The constant type
         *  @param constants The constant list to adopt
         */
        template<class ...Constants, class = std::enable_if_t<((  std::is_same_v<ConstantType, std::decay_t<Constants>>
                                                               || std::is_same_v<VenumType,    std::decay_t<Constants>>)
                                                               && ...)>>
        VenumSet(Constants &&...constants) noexcept
        {
            for (const auto &ct : std::array<VenumType, sizeof...(Constants)> { std::forward<Constants>(constants)... })
            {
                const int ordinal = ct->ordinal();
        
                if (ordinal > msb)
                {
                    msb = ordinal;
                }
                
                data[ordinal] = true;
            }
        }
        
        //==============================================================================================================
        /**
         *  Adds a constant to the set.
         *  Null-constants will be ignored and return an empty iterator.
         *
         *  @tparam VenumConstant The type of constant
         *  @param constant The constant to add
         *  @return A pair consisting of an iterator pointing to the element and a boolean denoting whether the element
         *          was freshly inserted or not
         */
        std::pair<Iterator, bool> emplace(VenumType constant) noexcept
        {
            if constexpr (canBeNull)
            {
                if (constant == nullptr)
                {
                    return std::make_pair(Iterator(*this), false);
                }
            }
            
            const int ordinal = constant->ordinal();
            
            if (ordinal > msb)
            {
                msb = ordinal;
            }
            
            auto bitref = data[ordinal];
            return std::make_pair(Iterator(*this, ordinal), !std::exchange(bitref, true));
        }
    
        //==============================================================================================================
        /**
         *  Removes a constant from the set.
         *
         *  @param position The iterator pointing to the element which should be removed
         *  @return An iterator pointing to the next element
         */
        Iterator erase(Iterator position) noexcept
        {
            if (position.position == msb)
            {
                Iterator temp(position);
                msb = std::max(0, --temp.position);
            }
            
            data[position.position] = false;
            return ++position;
        }
        
        /**
         *  Removes a range of constants from the set.
         *
         *  @param first The start iterator pointing to the element range which should be removed
         *  @param last  The end iterator pointing to the element range which should be removed
         *  @return An iterator pointing to the next element
         */
        Iterator erase(Iterator first, Iterator last) noexcept
        {
            assert(last.position < end().position && first.position >= begin().position);
            
            if (msb >= first.position && msb < last.position)
            {
                Iterator temp(first);
                msb = std::max(0, --temp.position);
            }
            
            while (first != last)
            {
                data[first.position] = false;
                ++first;
            }
            
            return ++first;
        }
        
        /**
         *  Removes a constant from the set with the given key.
         *
         *  @param key The constant to remove
         *  @return 0 if no such element was found 1 if there was a constant that was removed
         */
        int erase(VenumType key)
        {
            assert(key != nullptr); // cannot dereference nullptr
            
            const int ordinal = key->ordinal();
            
            if (data[ordinal])
            {
                erase(Iterator(data, ordinal));
                return 1;
            }
            
            return 0;
        }
    
        //==============================================================================================================
        /**
         *  Removes all constants from the set.
         */
        void clear() noexcept
        {
            data.reset();
            msb = 0;
        }
    
        //==============================================================================================================
        /**
         *  Determines whether the constant was found or not.
         *
         *  @param constant The constant to search for
         *  @return True if such a constant was found, false if not
         */
        bool contains(VenumType constant) const noexcept
        {
            if constexpr (canBeNull)
            {
                if (constant == nullptr)
                {
                    return false;
                }
            }
            
            return data[constant->ordinal()];
        }
        
        /**
         *  Gets whether the set contains no constants.
         *  @return True if there are no constants set, false if
         */
        bool empty() const noexcept
        {
            return data.none();
        }
    
        //==============================================================================================================
        /**
         *  Gets the amount of set constants.
         *  @return The amounf of constants
         */
        int size() const noexcept
        {
            return data.count();
        }
    
        //==============================================================================================================
        Iterator begin() const noexcept
        {
            const int first_index = data._Find_first();
            return Iterator(*this, first_index < data.size() ? first_index : 0);
        }
        
        Iterator end() const noexcept
        {
            return Iterator(*this, empty() ? 0 : msb + 1);
        }
        
        ReverseIterator rbegin() const noexcept
        {
            return ReverseIterator(end());
        }
        
        ReverseIterator rend() const noexcept
        {
            return ReverseIterator(begin());
        }
        
    private:
        DataSet data;
        int msb { 0 };
        
        //==============================================================================================================
        explicit constexpr VenumSet(DataSet bitset) noexcept
            : data(std::move(bitset))
        {}
    };
}
//======================================================================================================================
// endregion classes
/**********************************************************************************************************************/
