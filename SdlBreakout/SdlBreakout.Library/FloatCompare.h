#pragma once
#include <limits>

//https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
namespace FloatCompare
{
    union Float_t
    {
        Float_t(float num = 0.0f) : f(num) {}
        // Portable extraction of components.
        bool Negative() const { return i < 0; }
        int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
        int32_t RawExponent() const { return (i >> 23) & 0xFF; }

        int32_t i;
        float f;
#ifdef _DEBUG
        struct
        {   // Bitfields for exploration. Do not use in production code.
            uint32_t mantissa : 23;
            uint32_t exponent : 8;
            uint32_t sign : 1;
        } parts;
#endif
    };

    bool AlmostEqualUlpsAndAbs(float A, float B,
        float maxDiff, int maxUlpsDiff)
    {
        // Check if the numbers are really close -- needed
        // when comparing numbers near zero.
        float absDiff = fabs(A - B);
        if (absDiff <= maxDiff)
            return true;

        Float_t uA(A);
        Float_t uB(B);

        // Different signs means they do not match.
        if (uA.Negative() != uB.Negative())
            return false;

        // Find the difference in ULPs.
        int ulpsDiff = abs(uA.i - uB.i);
        if (ulpsDiff <= maxUlpsDiff)
            return true;

        return false;
    }

    bool AlmostEqualRelativeAndAbs(float A, float B,
        float maxDiff, float maxRelDiff = std::numeric_limits<float>::epsilon())
    {
        // Check if the numbers are really close -- needed
        // when comparing numbers near zero.
        float diff = fabs(A - B);
        if (diff <= maxDiff)
            return true;

        A = fabs(A);
        B = fabs(B);
        float largest = (B > A) ? B : A;

        if (diff <= largest * maxRelDiff)
            return true;
        return false;
    }
}