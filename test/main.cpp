#include <iostream>
#include "../src/venum.h"

#include "simple_venum_example.h"   // A simple virtual-enum declaration
#include "advanced_venum_example.h" // An advanced virtual-enum declaration
#include "enummap_example.h"        // Showcases the use of an EnumMap in combination with virtual-enums

#define VENUM_UNIT_TEST 0

int main()
{
    using garage::CarType;

    std::cout << std::endl << "======= Venum CarType constants: " << std::endl;
    std::cout << "Value array CarType: ";
    
    // Looping through all constants of the virtual-enum (values() is an array of all registered constants)
    // Constants can only be fetched by const reference, copying is not possible.
    for (const CarType::ConstantType &val : CarType::values())
    {
        // Prints the constant's name and corresponding ordinal
        // Will output: Mercedes: 0; Volkswagen: 1; RollsRoyce: 2;
        std::cout
            << val.name()    // Acquire the name of a constant
            << ": "
            << val.ordinal() // Acquire the ordinal of a constant
            << "; ";
    }
    
    std::cout << std::endl << std::endl << "======= Venum CarType initialisation: " << std::endl;
    
    // The default constructor may null-initialise or not initialise the
    // virtual-enum variable at all. (depending on the VENUM_DEFAULT_INITIALISE define)
    CarType default_car_type;
    
    // Would throw an error
    // std::cout << default_car_type->name();
    
    // Explicitly null-initialised virtual enum
    // Since CarType is a nullable virtual-enum, it will treat nullptr as an
    // additional stateless constant.
    CarType nullptr_car_type = nullptr;
    
    // Would throw an error
    // std::cout << nullptr_car_type->name();
    
    // Initialised with the Mercedes constant
    CarType explicit_car_type = CarType::Mercedes;
    
    std::cout << "Explicitly assigned CarType instance: " << explicit_car_type->name() << std::endl;
    
    // To gain access to a constant, we can use the member or direct dereference operator
    const char *default_name = explicit_car_type->name();
    
    // Get a constant from a name
    // If CarType is a nullable virtual-enum, this will return nullptr if no constant was found
    // If not it will throw a logical_error and terminate if uncaught
    CarType resolved_car_type = CarType::valueOf("RollsRoyce");
    
    if (resolved_car_type != nullptr) // Check for nullptr
    {
        std::cout << "Name resolved CarType instance: " << resolved_car_type->name() << std::endl;
    }
    
    // We also have enum-traits, which specify special information about a virtual-enum that can be used
    CarType::Traits::acceptsNullValues; // Tells us whether this is a nullable virtual-enum
    
    std::cout << std::endl << "======= Traits CarType: " << std::endl;
    
    // will print true as simple enums always accept null constants
    std::cout << "Is null a valid constant: " << (CarType::Traits::acceptsNullValues ? "true" : "false") << std::endl;
    
    showcase_venummap(); // VenumMap output
    showcase_venumset(); // VenumSet output
    return 0;
}
