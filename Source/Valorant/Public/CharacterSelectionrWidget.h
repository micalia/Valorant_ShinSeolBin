// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionrWidget.generated.h"

/**
 * 
 */
UCLASS()
class VALORANT_API UCharacterSelectionrWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = button)
	class UButton* bt_1Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_2Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_3Player; 
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_Selection;
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UButton* bt_SelectionLok;

	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer1Anim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer2Anim;
	UPROPERTY(EditAnywhere,meta = (BindWidgetAnim),Transient, Category = Anim)
	class UWidgetAnimation* SelectionPlayer3Anim;
	
	
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = button)
	class UCircularThrobber* LoadingIcon;


	UFUNCTION()
	void Selection1Player();
	UFUNCTION()
	void Selection2Player();
	UFUNCTION()
	void Selection3Player();

	//SB
	bool bStartChk;
UPROPERTY()
	class UNetGameInstance* gi;
	UFUNCTION(BlueprintCallable)
	void EnterInGame();
};
