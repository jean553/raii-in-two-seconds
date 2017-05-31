# Understand RAII in two seconds

## Explanation

#### What means RAII ?

RAII means `Resource Acquisition Is Initialisation`.
This acronym is by far hard to understand despite the fact that it hides a simple concept.

#### What is the problem ?

Let's take the following code as an example:

```c++
#include <iostream>

class Something
{

public:

    Something(const unsigned short& data) :
        data(data)
    {
    }

private:

    unsigned short data;
};

int main()
{
    Something* thing = new Something(10);

    /* some code... */

    throw std::runtime_error("something bad happened...");

    /* some code... */

    delete thing;

    return EXIT_SUCCESS;
}
```

In the example above, an object of the class `Something` is created on the `heap`.
Some code is executed and an exception is thrown in the middle of the program.
According to the code before and after the `throw` instruction,
the program may catch the exception somewhere else or simply terminates.
The object of the class `Something` created just before on the `heap` may not be deleted
because the `delete` instruction may not be reached. In that case, there is a memory leak.

#### The solution

```c++
#include <iostream>

class Something
{

public:

    Something(const unsigned short& data) :
        data(data)
    {
    }

private:

    unsigned short data;
};

class RAIIClass
{

public:

    RAIIClass(Something* something) :
        something(something)
    {
    }

    ~RAIIClass()
    {
        delete something;
    }

private:

    Something* something;
};

int main()
{
    Something* thing = new Something(10);
    RAIIClass raii(thing);

    /* some code... */

    throw std::runtime_error("something bad happened...");

    /* some code... */

    return EXIT_SUCCESS;
}
```

The solution is to create a new class called `RAIIClass`
that contains a pointer to a `Something` object.
A `RAIIClass` object is created on the `stack`
after the `Something` object on the `heap`.

C++ guarantees that when the execution leaves the current scope,
every resources allocated on the `stack` are deleted
(their destructors are called).
It happens even if an exception makes the execution leaves the scope.

In the example above, the `thing` object cannot be deleted
because it is allocated on the `heap`.
However, the `raii` object allocated on the `stack` is deleted.
Its constructor is called: this constructor contains the `delete`
instruction of the `Something` object of the heap,
so the `Something` object is deleted too. There is no memory leak.

#### Is there a different way to implement the solution ?

Yes, the `heap` object can be created directly by the RAII class.

```c++
RAIIClass(const unsigned short& data)
{
    something = new Something(data);
}
```

#### Is RAII used a lot in C++ ?

Yes, especially in C++11/14 with the smart pointers 
and with others standard library classes.

In general, RAII is used to ensure that some actions
will be executed, no matter what happens during the execution
(and even if the expected actions cannot be reached).
Those actions can be `heap` resource deletion
as we have just seen, but there are others cases
like closing a file, closing a socket... etc...

## Execute the example

```
g++ main.cpp -o program
./program
```
