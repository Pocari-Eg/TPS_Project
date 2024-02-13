// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LoginWidget.h"

#include "TPSGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Network/LoginManager.h"
#include "Widget/JoinWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("idBox")));
	PASSWORD = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("pwdBox")));
    m_JoinWidget=Cast<UJoinWidget>(GetWidgetFromName(TEXT("BP_JoinWidget")));
	IdError = Cast<UTextBlock>(GetWidgetFromName(TEXT("idError")));
	PwdError = Cast<UTextBlock>(GetWidgetFromName(TEXT("pwdError")));
	MessagePanel=Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MessagePanel")));
	
	m_LoginManager = new LoginManager();


	ID->SetText(FText::FromString(""));
	PASSWORD->SetText(FText::FromString(""));
	IdError->SetText(FText::FromString(""));
	PwdError->SetText(FText::FromString(""));
}

void ULoginWidget::TryLogin()
{
	if(!CheckEmptyData())
	{
		const FString ErrorCode=m_LoginManager->Login(ID->GetText(),PASSWORD->GetText());
		PrintMessage(ErrorCode);
	}
	
}

void ULoginWidget::OpenStartMessage()
{
	MessagePanel->SetVisibility(ESlateVisibility::Visible);
}

void ULoginWidget::PrintMessage(const FString ErrorCode)
{
	if (ErrorCode.Equals("NoJoin")||ErrorCode.Equals("PasswordError")) {
		IdError->SetText(FText::FromString("ID or Password Error"));
		PwdError->SetText(FText::FromString("ID or Password Error"));
		return;
	}
	else if(ErrorCode.Equals("Error"))
	{
		TLOG_E(TEXT("Error"));
		return;
	}
	else
	{
		TLOG_E(TEXT("Success"));
		OpenStartMessage();
	   auto instance= Cast<UTPSGameInstance>(GetGameInstance());
		instance->SetNickName((ErrorCode));
		TLOG_E(TEXT("Set NickName %s"), *instance->GetNickName());
		return;
	}

}

bool ULoginWidget::CheckEmptyData()
{	bool result = false;

	if(ID->GetText().ToString().Equals(""))
	{
		IdError->SetText(FText::FromString("Empty ID"));
		result=true;
		
	}
	else
	{
		IdError->SetText(FText::FromString(""));
	}
	if(PASSWORD->GetText().ToString().Equals(""))
	{
		PwdError->SetText(FText::FromString("Empty Password"));
		result=true;
		
	}
	else{
		PwdError->SetText(FText::FromString(""));
	}

	return result;
}

