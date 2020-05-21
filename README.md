![venum.logo](https://i.imgur.com/raM6O1h.png)

VEnum is an attempt to make enums more meaningful.
The purpose of this library is not to polish up the existing concept of C/C++ enums,
but instead to create a new type of enums: VEnums

Short for Virtual-Enum, VEnums try to give constants more data to play with.
Every constant has its own name and ordinal. If you are a Java fanatic you may notice similarities
as this library is inspired by Java enums.

As a little sidenote, the Virtual in VEnum does not stand for polymorphism but
rather the fact that VEnums simulate enums and are not real enums.

Warning: This library defines a massive amount of macros that begin with VENUM_
         so be sure that you don't use any of these besides the below mentioned ones.

# Installation
Installation is pretty darn easy. Nothing more than including the venum core header and that's it.
The cmake file included is for running the test included in this repo, which you can find in
the folder denoted as "test"

To run the test, run the included cmakelists.txt with one of the available
exporters and build your project:

```bash
mkdir build
cd build
cmake ..
```

# Getting Started
The VEnum library tries to make enum creation as simple as possible. Since VEnum classes are huge
this is done via macros.

Below are the 3 macros that should be used for creating enums:

| Macro                | Description                                                          |
| -------------------- | -------------------------------------------------------------------- |
| VENUM_CREATE         | Creates a simple enum with fixed constants.                          |
| VENUM_CREATE_ASSOC   | Creates an associative enum with associated values.                     |
| VENUM_BASE           | An utility macro used for overriding constructors in advanced enums. |

Besides these macros there are also classes that allow to use VEnums for special situations:

| Class   | Description                                            |
| ------- | ------------------------------------------------------ |
| EnumMap | An array that maps VEnum constants to a set of values. |

# Usage
## Defining a simple VEnum
To define a simple enum which doesn't associate any special values
to its constants we can do the following:
```C++
// Create an enum called CarType and specify constants
VENUM_CREATE(CarType
    Mercedes,
    Volkswagen,
    RollsRoyce
)
```

It's as simple as that, nothing more has to be done.
You now have access to the CarType enum and its constants. (more below)

## Defining an associative VEnum
Creating an associative VEnum is slightly more work than creating a simple VEnum,
difficulty is the same however.
```C++
// Create an enum called PhoneType and specify constants
VENUM_CREATE_ASSOC
(
    ID(PhoneType),
    VALUES
    (
        (Samsumg)(),
        (IPhone)(),
        (Nokia)(),
        (Huaweii)()
    )
)
```

As we can see, this time we had to write a little more than before.

While we specified the name as first argument in the first example, we had
to encapsulate it with the **ID** keyword. (keyword here refers to the VEnum macro name **ID**, **VALUES**, **BODY** and **ATTRIB**)  
The **VALUES** keyword is, in turn, responsible for declaring the constants we want to have.

I think you already noticed the difference, didn't you?  
Exactly, the constants are parenthesised. 
While the first group describes the constant's name, the second group is responsible for
defining the constant's associated values.

But unfortunately, just putting values in there would do more harm than good, as the compiler doesn't know
where to put them to interpret them correctly.  
For exactly this reason we introduce another important keyword: **BODY**

### BODY
The body keyword is there for giving constants additional information about data it
has and can hold.

So, let's say we want our constants to hold the vendor and price of any model.

```C++
VALUES
(
    (Samsumg)("Samsung", 400),
    (IPhone) ("Apple",   10000),
    (Nokia)  ("Nokia",   200),
    (Huaweii)("Huaweii", 10)
)
```
To stop this piece of code to show us errors we now need to give our constants their custom body.

```C++
BODY
(
private:
    constexpr PhoneTypeConstant(const char *name, int ordinal, const char *vendor, int price)
        : VENUM_BASE(name, ordinal)
    {}
)
```

What we did here is introducing a new constructor that takes our two arguments we specified above.
It is important that the constructor is a constant expression or it won't work.  
Unfortunately, this doesn't let us create constants with types that are not valid constant expressions,
but we do have a solution for this to which we will get later on.

You will need to also add accessors so you get access to these variables.
And it is important to know that all the constants have to have the same constructor which will be
determined by the first constant.

As you probably already noticed, we also added two other parameters to the constructor *name* and *ordinal*, these
are important to be written as the first two arguments, as they are responsible for
passing the constant's name and ordinal to the constant.  
The VENUM_BASE(name, ordinal) is just a convenience define so that you don't need to
assign the first two parameters yourself.

Another thing to note is that the constructor is private, this is intentional because
VEnums have an internal checking mechanism whether constructors are private or public.  
This is a safety mechanism so the user can't create any new constants and use them which
could result in undefined behaviour if not used correctly.

### Attributes
One last thing we didn't discuss now is *attributes*!  
Attributes are a refined way of chaning the behaviour of VEnum creation.

For the time beeing, there are only 4 attributes that are available:

| Attribute    | Description                                                                         | Values        |
| ------------ | ----------------------------------------------------------------------------------- | ------------- |
| CTOR_UNIFORM | Allows to toggle the ability of having diverse constant constructors.               | true/false    |
| CTOR_PRIVATE | Allows to toggle whether constant constructors have to be private or can be public. | true/false    |
| NULL_CONST   | Allows the enum to have a null constant.                                            | true/false    |
| RETENTION    | Specifies the validation time of the enum.                                          | RUNTIME/CLASS |

As we addressed before, you cannot have non-constexpr variables to be contained by constants.  
This is due to the fact that constants are constant expression by default, however, if this is
a problem you will need to turn this off.  
This disadvantage of this is that you will lose all constant expression benefits of your enum.

To disable constexpr qualification we need to specify the corresponding attribute and change it to
**RUNTIME**:
```
ATTRIB(RETENTION(RUNTIME))
```

This is what our enum looks like now:
```C++
VENUM_CREATE_ASSOC
(
    ID(PhoneType),
    VALUES
    (
        (Samsumg)("Samsung", 400),
        (IPhone) ("Apple",   10000),
        (Nokia)  ("Nokia",   200),
        (Huaweii)("Huaweii", 10)
    ),
    BODY
    (
    private:
        const char *const vendor {};
        const int price {};

        constexpr PhoneTypeConstant(const char *name, int ordinal, const char *vendor, int price)
            : VENUM_BASE(name, ordinal),
              vendor(vendor), price(price)
        {}

    public:
        constexpr const char* getVendor() { return vendor; }
        constexpr int getPrice() { return price; }
    )
    ATTRIB
    (
        RETENTION(RUNTIME)
    )
)
```

