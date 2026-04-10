// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "ItemDefinition.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	check(PickupMeshComponent != nullptr);
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(SphereComponent != nullptr);
	SphereComponent->SetupAttachment(PickupMeshComponent);
	SphereComponent->SetSphereRadius(32.f);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePickup();
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::InitializePickup() {
	if (PickupDataTable && !PickupItemID.IsNone()) {
		const FItemData* ItemDataRow = PickupDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());
		ReferenceItem = NewObject<UItemDefinition>(this, UItemDefinition::StaticClass());
		ReferenceItem->ID = ItemDataRow->ID;
		ReferenceItem->ItemType = ItemDataRow->ItemType;
		ReferenceItem->ItemText = ItemDataRow->ItemText;
		ReferenceItem->WorldMesh = ItemDataRow->ItemBase->WorldMesh;

		UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.Get();
		if (TempItemDefinition->WorldMesh.IsValid()) {
			PickupMeshComponent->SetStaticMesh(TempItemDefinition->WorldMesh.Get());
		}
		else {
			UStaticMesh* WorldMesh = TempItemDefinition->WorldMesh.LoadSynchronous();
			PickupMeshComponent->SetStaticMesh(WorldMesh);
		}
		PickupMeshComponent->SetVisibility(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);
	}
}

void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Attempting a pickup collision"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *OtherActor->GetClass()->GetName()));

	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TRIGGERED"));
		SphereComponent->OnComponentBeginOverlap.RemoveAll(this);
		PickupMeshComponent->SetVisibility(false);
		PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NOT TRIGGERED"));
	}

	if (bShouldRespawn) {
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::InitializePickup, RespawnTime, false, -1);
	}
}
