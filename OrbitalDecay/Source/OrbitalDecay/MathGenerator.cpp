#include "MathGenerator.h"

FMathQuestion UMathGenerator::GenerateQuestion(FString Type, int32 Level)
{
    FMathQuestion NewQ;

    int32 MaxRange = FMath::Max(1, Level * 10);
    int32 A = FMath::RandRange(1, MaxRange);
    int32 B = FMath::RandRange(1, MaxRange);

    if (Type == "a")
    {
        NewQ.CorrectAnswer = A + B;
        NewQ.QuestionText = FString::Printf(TEXT("%d + %d = "), A, B);
    }
    else if (Type == "s")
    {
        int32 High = FMath::Max(A, B);
        int32 Low = FMath::Min(A, B);
        NewQ.CorrectAnswer = High - Low;
        NewQ.QuestionText = FString::Printf(TEXT("%d - %d = "), High, Low);
    }
    else if (Type == "m")
    {
        B = FMath::RandRange(1, FMath::Min(Level + 2, 12));
        NewQ.CorrectAnswer = A * B;
        NewQ.QuestionText = FString::Printf(TEXT("%d x %d = "), A, B);
    }
    else if (Type == "d")
    {
        int32 Answer = FMath::RandRange(1, FMath::Max(1, MaxRange / 2));
        int32 Divisor = FMath::RandRange(2, FMath::Min(Level + 2, 10));
        int32 Product = Answer * Divisor;
        NewQ.CorrectAnswer = Answer;
        NewQ.QuestionText = FString::Printf(TEXT("%d / %d = "), Product, Divisor);
    }
    return NewQ;
}