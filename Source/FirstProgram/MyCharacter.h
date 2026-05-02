// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UAnimInstance;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class AEquippableToolBase;
class UEquippableToolDefinition;
class UInventoryComponent;
class UItemDefinition;

UCLASS()
class FIRSTPROGRAM_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere, Category = Animation)
	TSubclassOf<UAnimInstance> FirstPersonDefaultAnimClass;

	UPROPERTY(EditAnywhere, Category = Animation)
	TSubclassOf<UAnimInstance> ThirdPersonDefaultAnimClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> UseAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tools)
	TObjectPtr<AEquippableToolBase> EquippedTool;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr< USkeletalMeshComponent> FirstPersonMeshComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector FirstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);

	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonFieldOfView = 70.0f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float FirstPersonScale = 0.6f;

	UPROPERTY(VisibleAnywhere, Category = Inventory)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	bool IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition);

	UFUNCTION()
	void GiveItem(UItemDefinition* ItemDefinition);

	UFUNCTION()
	void AttachTool(UEquippableToolDefinition* ToolDefinition);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	FVector GetCameraTargetLocation();
};
