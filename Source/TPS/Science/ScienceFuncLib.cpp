// My game copyright

#include "TPS/Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScience, All, All);

int32 UScienceFuncLib::Fibonacci(const int32 Value)
{
    // check(Value >= 0);
    if (Value < 0)
    {
        UE_LOG(LogScience, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}

int32 UScienceFuncLib::Factorial(const int32 Value)
{
    if (Value < 0) return -1;

    int32 Fac = 1;
    for (int i = 1; i <= Value; ++i)
    {
        Fac *= i;
    }

    return Fac;
}
