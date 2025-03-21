// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/ListView.h"


void UInventoryWidget::RefreshInventory(const TArray<FItemData>& Inventory)
{
	ItemList->ClearListItems();
	for (FItemData ItemData : Inventory)
	{
		UItemUIObject* UIItem = NewObject<UItemUIObject>();
		UIItem->ItemData = ItemData;
		ItemList->AddItem(UIItem);
	}
}