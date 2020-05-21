void showcase_enummap()
{
    // Declaring an enum map which maps constants to doubles
    venum::EnumMap<CarType, double> prices;
    
    // Assigning and fetching via the constants
    prices[CarType::Mercedes]   = 20000.49;
    prices[CarType::Volkswagen] = 8000.28;
    prices[CarType::RollsRoyce] = 100000.00;
    
    // even nullptr is a valid assignable constant, but only if the virtual-enum is nullable
    prices[nullptr] = 0.0;
    
    for (double val : prices)
    {
        std::cout << val << std::endl;
    }
}
