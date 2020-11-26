void showcase_venummap()
{
  using garage::CarType;

    std::cout << std::endl << "======= VenumMap CarType: " << std::endl;
    
    // Declaring a venum map which maps constants to doubles
    venum::VenumMap<CarType, double> prices;
    
    // Assigning and fetching via the constants
    prices[CarType::Mercedes]   = 20000.49;
    prices[CarType::Volkswagen] = 8000.28;
    prices[CarType::RollsRoyce] = 100000.00;
    
    // even nullptr is a valid assignable constant, but only if the virtual-enum is nullable
    prices[nullptr] = 0.0;
    
    for (auto &val : prices)
    {
        std::cout << val.first->name() << ": " << val.second << std::endl;
    }
}

void showcase_venumset()
{
    using garage::CarType;
    std::cout << std::endl << "======= VenumSet<CarType> values: " << std::endl;
    
    venum::VenumSet<CarType> car_set;

#if VENUM_UNIT_TEST
    std::cout << "Values empty: ";
    for (auto type : car_set)
    {
        std::cout << type->name() << "; ";
    }
    
    std::cout << std::endl;
#endif
    
    std::cout << "Values after add: ";
    
    car_set.emplace(CarType::RollsRoyce);
    car_set.emplace(CarType::Mercedes);
    
    for (auto type : car_set)
    {
        std::cout << type->name() << "; ";
    }

#if VENUM_UNIT_TEST
    std::cout << std::endl << "Values after remove: ";
    
    car_set.erase(CarType::Mercedes);
    
    for (auto type : car_set)
    {
        std::cout << type->name() << "; ";
    }
    
    std::cout << std::endl << "Create full set and iterate directly with the iterators: ";
    
    car_set = venum::VenumSet<CarType>::all();
    
    for (auto it = car_set.begin(); it != car_set.end(); ++it)
    {
        std::cout << it->name() << "; ";
    }
#endif
}
