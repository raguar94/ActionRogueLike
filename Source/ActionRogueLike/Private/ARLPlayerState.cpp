// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPlayerState.h"

int32 AARLPlayerState::GetCredits() const
{
	return Credits;
}

void AARLPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

}

bool AARLPlayerState::RemoveCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// Not Enough credits available
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

	return true;
}
