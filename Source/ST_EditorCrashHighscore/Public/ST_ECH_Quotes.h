#pragma once

#include "ST_ECH_Quotes.generated.h"


USTRUCT()
struct FST_ECH_CrashQuote : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	FText Quote;

	UPROPERTY(EditAnywhere)
	FName Type;
};
