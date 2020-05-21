![venum.logo](https://i.imgur.com/raM6O1h.png)

venum is an attempt to make enums more meaningful.
The purpose of this library is not to polish up the existing concept of C/C++ venums,
but instead to create a new type of enums: venums

Short for virtual-enum, venums try to give constants more data to play with.
Every constant has its own name and ordinal. If you are a Java fanatic you may notice similarities
as this library is inspired by Java enums.

As a little sidenote, the Virtual in venum does not stand for polymorphism but
rather the fact that venums simulate enums and are not real enums.

Warning: This library defines a massive amount of macros that begin with VENUM_
         so be sure that you don't use any of these besides the below mentioned ones.

# Table of contents
1. [Installation](#installation)
2. [Getting Started](#getting-started)
   * [Tools](#tools)
   * [Venum](#venum)
   * [Constants](#constants)
3. [Creation of venums](#creation-of-venums)
   * [Defining a simple venum](#defining-a-simple-venum)
   * [Defining an associative venum](#defining-an-associative-venum)
      1. [ID and Values](#id-and-values)
      2. [Body](#body)
      3. [Attributes](#attributes)
4. [Making use of venums](#making-use-of-venums)
   * [Get constant data](#get-constant-data)
   * [Get all constants](#get-all-constants)
   * [Get constant from name](#get-constant-from-name)

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
## Tools
The venum library tries to make enum creation as simple as possible. Since venum classes are huge
this is done via macros.

Below are the 3 macros that should be used for creating venums:

| Macro                | Description                                                           |
| -------------------- | --------------------------------------------------------------------- |
| VENUM_CREATE         | Creates a simple venum with fixed constants.                          |
| VENUM_CREATE_ASSOC   | Creates an associative venum with associated values.                  |
| VENUM_BASE           | An utility macro used for overriding constructors in advanced venums. |

Besides these macros there are also classes that allow to use venums for special situations:

| Class    | Description                                             |
| -------- | ------------------------------------------------------- |
| VenumMap | An array that maps venum constants to a set of values.  |

## Venum
A venum is, similar to a trivial enum, collection of constants. Other than enums, though,
they cannot hold values that are not defined in them (optimally) and store additional data that trivial enums do not.


## Constants
Constants are the heart of every venum.
Constants are a compound of data that are unique to their instance.  
Every constant gets an unique name and an ordinal number which denotes their position in the venum's constant storage.  
Constants also have the ability to contain user defined data and functions that do not change their internal state.


# Creation of venums
## Defining a simple venum
To define a simple venum which doesn't associate any special values to its constants we can do the following:
```C++
// Create an venum called CarType and specify constants
VENUM_CREATE(CarType
    Mercedes,
    Volkswagen,
    RollsRoyce
)
```

It's as simple as that, nothing more has to be done.
You now have access to the CarType venum and its constants. (more below)

## Defining an associative venum
### ID and VALUES
Creating an associative venum is slightly more work than creating a simple venum,
difficulty is the same however.
```C++
// Create an venum called PhoneType and specify constants
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
to encapsulate it with the **ID** keyword. (keyword here refers to the venum macro name **ID**, **VALUES**, **BODY** and **ATTRIB**)  
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
venums have an internal checking mechanism whether constructors are private or public.  
This is a safety mechanism so the user can't create any new constants and use them which
could result in undefined behaviour if not used correctly.

### Attributes
One last thing we didn't discuss now is *attributes*!  
Attributes are a refined way of changing the behaviour of venum creation.

For the time beeing, there are only 4 attributes that are available:

| Attribute    | Description                                                                         | Values        |
| ------------ | ----------------------------------------------------------------------------------- | ------------- |
| CTOR_UNIFORM | Allows to toggle the ability of having diverse constant constructors.               | true/false    |
| CTOR_PRIVATE | Allows to toggle whether constant constructors have to be private or can be public. | true/false    |
| NULL_CONST   | Allows the venum to have a null constant.                                            | true/false    |
| RETENTION    | Specifies the validation time of the venum.                                          | RUNTIME/CLASS |

As we addressed before, you cannot have non-constexpr variables to be contained by constants.  
This is due to the fact that constants are constant expression by default, however, if this is
a problem you will need to turn this off.  
This disadvantage of this is that you will lose all constant expression benefits of your venum.

To disable constexpr qualification we need to specify the corresponding attribute and change it to
**RUNTIME**:
```
ATTRIB(RETENTION(RUNTIME))
```

This is what our venum looks like now:
```C++
VENUM_CREATE_ASSOC
(
    ID(PhoneType),
    VALUES
    (
        (Samsumg)("Samsung", 400),
        (IPhone)("Apple",   10000),
        (Nokia)("Nokia",   200),
        (Huaweii)("Huaweii", 10)
    ),
    BODY
    (
    private:
        const char *const vendor {};
        const int price {};

        constexpr PhoneTypeConstant(const char *name, int ordinal, const char *vendor, int price)
            : venum_BASE(name, ordinal),
              vendor(vendor), price(price)
        {}

    public:
        constexpr const char* getVendor() { return vendor; }
        constexpr int getPrice() { return price; }
    ),
    ATTRIB
    (
        RETENTION(RUNTIME)
    )
)
```

## Making use of venums
Now that we sorted out how to create our venums, we also need to know what we can do with them. 
After all, we also want to have a use for them and don't just let them stay there doing nothing.

### Get constant data
To gain access to the data of a constant, we can either directly access the constant,
or through the venum instance and the dereference operator.
```C++
// Access vendor directly through constant
PhoneType::Samsung.getVendor()

// or through instance
PhoneType type = PhoneType::Samsung;
type->getVendor();
```

Note that the instance "type" may point to a null-constant,
which can lead to undefined behaviour if not checked beforehand.

### Get all constants
The "values()" function of any venum will return a reference to an internal array that contains all the constants.

### Get constant from name
Since constants can be refered to by name, we can also resolve them by their name.  
For this to work, every venum contains a function called "valueOf" which accepts a string and returns the constant
or a nullptr that is found to be equivalent to this string.

Note that if a venum doesn't accept null-constants,
this will throw an exception if no constant with the specified name was found.
