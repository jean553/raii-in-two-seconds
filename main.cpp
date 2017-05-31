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

    RAIIClass(const unsigned short& data)
    {
        something = new Something(data);
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
    RAIIClass raii(10);

    /* some code... */

    throw std::runtime_error("something bad happened...");

    /* some code... */

    return EXIT_SUCCESS;
}
