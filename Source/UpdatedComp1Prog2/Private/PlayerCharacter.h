// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputActionValue.h>
#include "ItemDatabase.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UInventoryWidget;

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PawnClientRestart() override;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InventoryAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* MappingContext;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractWidgetClass;

	UPROPERTY()
	UUserWidget* InteractWidget;


	UPROPERTY(EditDefaultsOnly)
	UItemDatabase* ItemDatabase;

	TArray<FItemData> Inventory;



	void Move(const FInputActionValue& ActionValue);

	void Look(const FInputActionValue& ActionValue);

	void Interact();

	void InteractCheck();

	void ToggleInventory();

	FVector ViewVector;
	FRotator ViewRotation;
	FVector InteractVectorEnd;
	FHitResult InteractHitResult;
};
