// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGame.h"
#include "Blueprint/UserWidget.h"
#include "JoinWidget.generated.h"

/**
 * 
 */
class 	UEditableTextBox;
class LoginManager;
class UCanvasPanel;
class UTextBlock;
UCLASS()
class TPSGAME_API UJoinWidget : public UUserWidget
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
	UEditableTextBox* NICKNAME;
	UPROPERTY()
	UCanvasPanel* MessagePanel;
	UPROPERTY()
	UTextBlock* MessageBlock;

	UPROPERTY()
	UTextBlock* IdError;

	UPROPERTY()
	UTextBlock* PwdError;

	UPROPERTY()
	UTextBlock* NameError;

	LoginManager* m_LoginManager;

private:
	UFUNCTION()
	void PrintMessage(const FString ErrorCode);
	UFUNCTION()
	bool CheckEmptyData();
public:
	UFUNCTION(BlueprintCallable)
	void TryJoin();
	UFUNCTION(BlueprintCallable)
    void CloseMessage();
	UFUNCTION(BlueprintCallable)
	void ClearText();
	

};
