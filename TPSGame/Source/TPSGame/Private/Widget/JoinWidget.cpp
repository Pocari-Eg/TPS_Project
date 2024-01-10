// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/JoinWidget.h"
#include "Components/EditableTextBox.h"
#include "Network/LoginManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
void UJoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("idBox")));
	PASSWORD = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("pwdBox")));
	NickName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NickBox")));
	MessagePanel= Cast<UCanvasPanel>(GetWidgetFromName(TEXT("MessagePanel")));
	MessageBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Message")));


	IdError = Cast<UTextBlock>(GetWidgetFromName(TEXT("idError")));
	PwdError = Cast<UTextBlock>(GetWidgetFromName(TEXT("pwdError")));
	NameError = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameError")));

	ID->SetText(FText::FromString(""));
	PASSWORD->SetText(FText::FromString(""));
	NickName->SetText(FText::FromString(""));
	IdError->SetText(FText::FromString(""));
	PwdError->SetText(FText::FromString(""));
	NameError->SetText(FText::FromString(""));

	m_LoginManager = new LoginManager();
}

void UJoinWidget::ClearText()
{
	ID->SetText(FText::FromString(""));
	PASSWORD->SetText(FText::FromString(""));
	NickName->SetText(FText::FromString(""));
	IdError->SetText(FText::FromString(""));
	PwdError->SetText(FText::FromString(""));
	NameError->SetText(FText::FromString(""));
}

void UJoinWidget::TryJoin()
{
	

	if(!CheckEmptyData())
	{
		const FString ErrorCode=m_LoginManager->Join(ID->Text,PASSWORD->Text,NickName->Text);
		PrintMessage(ErrorCode);
	}

}

void UJoinWidget::CloseMessage()
{
	MessagePanel->SetVisibility(ESlateVisibility::Hidden);
	ClearText();
}

void UJoinWidget::PrintMessage(const FString ErrorCode)
{
	if (ErrorCode.Equals("Success")) {
		TLOG_E(TEXT("Success"));
		MessageBlock->SetText(FText::FromString("Join Success"));
		MessagePanel->SetVisibility(ESlateVisibility::Visible);
		return;
	};
	if (ErrorCode.Equals("AlreayJoinID")) {

		IdError->SetText(FText::FromString("Already Join ID"));
		return;
	};
	if (ErrorCode.Equals("AlreayJoinName")) {

		NameError->SetText(FText::FromString("Already Join NickName"));
		return;
	};


	if (ErrorCode.Equals("JoinError")) {
		TLOG_E(TEXT("Join Error"));
		MessageBlock->SetText(FText::FromString("Join Error"));
		MessagePanel->SetVisibility(ESlateVisibility::Visible);
		return;
	};
	if (ErrorCode.Equals("Error")) {
		TLOG_E(TEXT("Error"));
		MessageBlock->SetText(FText::FromString("Error"));
		MessagePanel->SetVisibility(ESlateVisibility::Visible);
		return;
	};
}

bool UJoinWidget::CheckEmptyData()
{

	bool result = false;

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
	if(NickName->GetText().ToString().Equals(""))
	{
		NameError->SetText(FText::FromString("Empty NickName"));
		result=true;
		
	}
	else
	{
		NameError->SetText(FText::FromString(""));
	}
	return result;
}
