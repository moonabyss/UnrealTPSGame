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
    static int32 Fibonacci(int32 Value);
};
