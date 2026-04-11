// Fill out your copyright notice in the Description page of Project Settings.


#include "DartLauncher.h"
#include "MyCharacter.h"

void ADartLauncher::Use()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Using the dart launcher!"));
}

void ADartLauncher::BindInputAction(const UInputAction* InputToBind)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(InputToBind, ETriggerEvent::Triggered, this, &ADartLauncher::Use);
		}
	}
}

void ADartLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}