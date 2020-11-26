#pragma once

#include "venum.h"

// Creates an associative virtual-enum, other than a simple virtual-enum, this way you can go further into the matter
// and specify various attributes and associated values with it.
// ID specifies the name of the enum, VALUES the constants
// Each constant must be in the format (NAME)(CONSTRUCTOR-ARGS)

// Here is a simple CarType virtual-enum which differs in nothing to a simple created virtual-enum
// We have no arguments, nothing
VENUM_CREATE_ASSOC
(
    ID(UserType),
    VALUES
    (
        (Human)  (),
        (Bot)    (),
        (Snooper)()
    )
)

// Here is another virtual-enum which does make use of associated values
// To accomplish this, we need to make use of the BODY keyword (keyword here refers to the specifiers of an assoc-venum)
// and create our own constructor that takes these arguments
VENUM_CREATE_ASSOC
(
    ID(PhoneType),
    VALUES
    (
        (Samsung)("Samsung", 400),
        (IPhone) ("Apple",   10000),
        (Nokia)  ("Nokia",   200),
        (Huaweii)("Huaweii", 10)
    ),
    BODY
    (
    private: // Note the private specifier, if we remove this we will get an assert as public constructors for
             // constants are dangerous (it will only trigger if it was used)
             
        const char *const vendor { }; // Note that we need to default construct our members
        const int price { };
    
        constexpr PhoneTypeConstant(const char *name, int ordinal, const char *vendor, int price) noexcept
            : VENUM_BASE(name, ordinal), // We need to also assign the name and ordinal of the constants
              vendor(vendor), price(price)
        {
            // This constructor initialises what the constants will look like internally
        }
    
    public:
        constexpr const char* getVendor() const noexcept
        {
            return vendor;
        }
        
        constexpr int getPrice() const noexcept
        {
            return price;
        }
    ),
    ATTRIB // Attributes allow us to specify one or more properties about a virtual-enum
    (
        // if we set this to false, we are allowed to specify public constructors in constants (default: true)
        CTOR_PRIVATE(true),
        
        // if we set this to false, we are allowed to use different constructors for individual constants
        // if this is set to true, all other constants must follow the constructor the first constant uses (default: true)
        CTOR_UNIFORM(true),
        
        // if this is set to CLASS, we are not allowed to use any arguments that are not eligible for beeing used
        // in a constant expression, as this will constexpr qualify the virtual-enum
        // if you need to use non-constexpr classes, we specify this as RUNTIME (default: CLASS)
        RETENTION(RUNTIME),
        
        // if we set this to false, the virtual-enum is not allowed to accept a nullptr constant and hides
        // all operators that have anything to do with nullptr (default: true)
        NULL_CONST(true)
    )
)
