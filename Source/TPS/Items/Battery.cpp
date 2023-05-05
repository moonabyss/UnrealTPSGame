// My game copyright

#include "TPS/Items/Battery.h"

using namespace TPS;

constexpr float ChargeAmount = 0.1f;

TPS::Battery::Battery(float PercentIn)
{
    SetPercent(PercentIn);
}

void TPS::Battery::Charge()
{
    SetPercent(Percent + ChargeAmount);
}

void TPS::Battery::UnCharge()
{
    SetPercent(Percent - ChargeAmount);
}

float TPS::Battery::GetPercent() const
{
    return Percent;
}

FColor TPS::Battery::GetColor() const
{
    if (Percent > 0.8f) return FColor::Green;
    if (Percent > 0.3f) return FColor::Yellow;
    return FColor::Red;
}

FString TPS::Battery::ToString() const
{
    return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}

void TPS::Battery::SetPercent(float PercentIn)
{
    Percent = FMath::Clamp(PercentIn, 0.0f, 1.0f);
}
