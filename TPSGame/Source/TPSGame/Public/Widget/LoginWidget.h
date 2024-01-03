// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
class 	UEditableTextBox;
class  UJoinWidget;
class UTextBlock;
class LoginManager;
class UCanvasPanel;
UCLASS()
class TPSGAME_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	// 위젯을 초기화
	virtual void NativeConstruct() override;
private:

	UPROPERTY()
	UEditableTextBox* ID;
	UPROPERTY()
	UEditableTextBox* PASSWORD;
	UPROPERTY()
	UJoinWidget* m_JoinWidget;

	UPROPERTY()
	UTextBlock* IdError;
	UPROPERTY()
	UTextBlock* PwdError;

	LoginManager* m_LoginManager;

	UCanvasPanel* MessagePanel;
public:

	UFUNCTION(BlueprintCallable)
	void TryLogin();
    UFUNCTION(BlueprintCallable)
	void OpenStartMessage();
	
private:
	UFUNCTION()
	void PrintMessage(const FString ErrorCode);
	UFUNCTION()
	bool CheckEmptyData();
	
};
