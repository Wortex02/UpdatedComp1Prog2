// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InventoryWidget.h"
#include "Item.h"
#include "ItemDatabase.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()), InventoryWidgetClass);
	InteractWidget = CreateWidget(Cast<APlayerController>(GetController()), InteractWidgetClass);
	InventoryWidget->AddToViewport(0);
	InteractWidget->AddToViewport(0);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractCheck();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		if (LookAction)
		{
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		}
		if (InteractAction)
		{
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		}
		if (InventoryAction)
		{
			PlayerEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
		}
	}
}

void APlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = Cast<APlayerController>(GetController())->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->ClearAllMappings();

		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void APlayerCharacter::Move(const FInputActionValue& ActionValue)
{
	AddMovementInput(GetActorForwardVector(), ActionValue[0]);

	AddMovementInput(GetActorRightVector(), ActionValue[1]);
}

void APlayerCharacter::Look(const FInputActionValue& ActionValue)
{
	AddControllerYawInput(ActionValue[0]);
	AddControllerPitchInput(ActionValue[1]);
}

void APlayerCharacter::Interact()
{
	if (Cast<AItem>(InteractHitResult.GetActor()))
	{
		FItemData* Data = ItemDatabase->Items.FindByPredicate([&](const FItemData& ItemData)
			{
				return ItemData.Class == InteractHitResult.GetActor()->GetClass();
			});
		Inventory.Emplace(*Data);
		InteractHitResult.GetActor()->Destroy();
	}

}

void APlayerCharacter::InteractCheck()
{
	Cast<APlayerController>(GetController())->GetPlayerViewPoint(ViewVector, ViewRotation);
	FVector VecDirection = ViewRotation.Vector() * 500;
	InteractVectorEnd = ViewVector + VecDirection;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(InteractHitResult, ViewVector, InteractVectorEnd, ECollisionChannel::ECC_GameTraceChannel1, QueryParams);
	if (Cast<AItem>(InteractHitResult.GetActor()))
	{
		InteractWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerCharacter::ToggleInventory()
{
	if (!InventoryWidget->IsVisible())
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->RefreshInventory(Inventory);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->SetCinematicMode(true, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(GetController())->SetCinematicMode(false, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}