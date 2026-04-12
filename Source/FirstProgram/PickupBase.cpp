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

	if (!PickupDataTable.ToSoftObjectPath().IsNull() && !PickupItemID.IsNone()) {
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereBeginOverlap);
		InitializePickup();
	}
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::InitializePickup() {
	UDataTable* LoadedTable = PickupDataTable.LoadSynchronous();
	if (LoadedTable != nullptr) {
		const FItemData* ItemDataRow = LoadedTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());
		if (ItemDataRow != nullptr) {
			UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.LoadSynchronous();
			if (TempItemDefinition != nullptr) {
				ReferenceItem = TempItemDefinition->CreateItemCopy();
				UStaticMesh* WorldMesh = TempItemDefinition->WorldMesh.LoadSynchronous();
				if (WorldMesh != nullptr)
				{
					PickupMeshComponent->SetStaticMesh(WorldMesh);
				}
				PickupMeshComponent->SetVisibility(true);
				SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
	}
}

void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Attempting a pickup collision"));

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *OtherActor->GetClass()->GetName()));

	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TRIGGERED"));
		Character->GiveItem(ReferenceItem);
		PickupMeshComponent->SetVisibility(false);
		PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("NOT TRIGGERED"));
	}

	if (bShouldRespawn) {
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::InitializePickup, RespawnTime, false);
	}
}

void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupItemID)) {
		UDataTable* Table = PickupDataTable.LoadSynchronous();
		if (Table && !PickupItemID.IsNone()) {
			const FItemData* ItemDataRow = Table->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());
			if (ItemDataRow) {
				UItemDefinition* TempItemDef = ItemDataRow->ItemBase.LoadSynchronous();
				if (TempItemDef) {
					PickupMeshComponent->SetStaticMesh(TempItemDef->WorldMesh.LoadSynchronous());
				}
			}
		}
	}
}