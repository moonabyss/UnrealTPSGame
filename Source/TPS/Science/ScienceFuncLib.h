// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"

UCLASS()
class TPS_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = "Science", BlueprintPure)
    static int32 Fibonacci(const int32 Value);

    /**
     * @brief Calculates factorial for the input number.
     * https://en.wikipedia.org/wiki/Factorial
     * @param   [in] Value  Number for which factorial should be calculated.
     * @return              Factorial or (-1) for negative numbers
     */
    UFUNCTION(Category = "Science", BlueprintPure)
    static int32 Factorial(const int32 Value);
};