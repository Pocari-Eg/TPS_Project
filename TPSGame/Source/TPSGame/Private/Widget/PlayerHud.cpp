// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerHud.h"

#include "Character/PlayerCharacter.h"
#include "Components/TextBlock.h"


void UPlayerHud::BindPlayer(APlayerCharacter* value)
{
	//새로들어온 object를 CurrentObject에 할당
	Player =value;
	
	Player->OnHpChanged.AddUObject(this, &UPlayerHud::UpdateHPWidget);
	UpdateHPWidget();
}

void UPlayerHud::OpenDeathWidget()
{

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor=true;

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	DeathWidget->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHud::UpdateHPWidget()
{
	if (Player != nullptr) {
		
			FText NewText = FText::FromString(FString::FromInt(Player->GetHP()));
			HPText->SetText(NewText);
		}
}

void UPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("PlayerHudInit"));

	HPText =Cast<UTextBlock>(GetWidgetFromName(TEXT("HPVal")));
	DeathWidget=Cast<UUserWidget>(GetWidgetFromName(TEXT("BP_DeathWidget")));
}
