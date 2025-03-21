// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryEntry.h"
#include "ItemDatabase.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"
#include "Components/Button.h"
#include "InventoryWidget.h"
#include "Item.h"

void UInventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UItemUIObject* UIItem = Cast<UItemUIObject>(ListItemObject);
	ItemName->SetText(FText::FromString(UIItem->ItemData.ItemName.ToString()));

	DropButton->OnClicked.Clear();
	DropButton->OnClicked.AddDynamic(this, &UInventoryEntry::DropItem);
}

void UInventoryEntry::DropItem()
{/*
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());
	FItemData ItemData = GetListItem<UItemUIObject>()->ItemData;
	GetWorld()->SpawnActor<AItem>(ItemData.Class, PlayerCharacter->InteractVectorEnd, FRotator());
	PlayerCharacter->Inventory.Remove(ItemData);
	PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	DropButton->OnClicked.Clear();*/
}
