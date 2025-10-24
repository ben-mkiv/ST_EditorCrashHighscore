// Copyright Epic Games, Inc. All Rights Reserved.

#include "ST_EditorCrashHighscore.h"

#include "ST_ECH_Quotes.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Serialization/BufferArchive.h"

#define LOCTEXT_NAMESPACE "FST_EditorCrashHighscoreModule"

void FST_EditorCrashHighscoreModule::StartupModule()
{
	// use the crash detection from the AssetEditorSubsystem
	bool bCleanShutdown = true;
	GConfig->GetBool(TEXT("ST_EditorCrashHighscore"), TEXT("CleanShutdown"), bCleanShutdown, GEditorPerProjectIni);

	// we set that value to false now, it will be set to true on module shutdown
	GConfig->SetBool(TEXT("ST_EditorCrashHighscore"), TEXT("CleanShutdown"), false, GEditorPerProjectIni);

	if(!bCleanShutdown)
	{
		FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("ST_EditorCrashHighscore.dat"));
		
		TArray<uint8> BinaryArray;
		if(FFileHelper::LoadFileToArray(BinaryArray, *FilePath))
		{
			FMemoryReader Reader(BinaryArray, true);
			Reader << CrashCount;
		}

		CrashCount.FindOrAdd(FDateTime::Today())++;

		
		// we can write it right back with the increased counter
		FBufferArchive Archive;
		Archive << CrashCount; // TMap serialization operator exists for basic types
		FFileHelper::SaveArrayToFile(Archive, *FilePath);
		
		FEditorDelegates::OnEditorInitialized.AddLambda([Crashes = CrashCount.FindChecked(FDateTime::Today())](double Duration)
		{
			FText Quote = INVTEXT("another crash, huh?");
			
			const TSoftObjectPtr<UDataTable> DT_Soft = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/ST_EditorCrashHighscore/CrashQuotes.CrashQuotes")));
			if(UDataTable *DT = DT_Soft.LoadSynchronous())
			{
				if(!DT->GetRowMap().IsEmpty())
				{				
					const int32 RandomIndex = FMath::RandRange(0, DT->GetRowMap().Num() - 1);
					TMap<FName, uint8*>::TConstIterator It(DT->GetRowMap());
					for (int32 i = 0; i < RandomIndex; ++i)
					{
						++It;
					}

					if(FST_ECH_CrashQuote* QuoteEntry = DT->FindRow<FST_ECH_CrashQuote>(It.Key(), TEXT("")))
					{
						Quote = QuoteEntry->Quote;
					}					 
				}				
			}
			
			ShowEditorNotification(FString::Printf(TEXT("%s\ntotal crashes today: %d"), *Quote.ToString(), Crashes), SNotificationItem::ECompletionState::CS_Fail);
		});
	}
}

void FST_EditorCrashHighscoreModule::ShutdownModule()
{
	GConfig->SetBool(TEXT("ST_EditorCrashHighscore"), TEXT("CleanShutdown"), true, GEditorPerProjectIni);
}

void FST_EditorCrashHighscoreModule::ShowEditorNotification(const FString Message, SNotificationItem::ECompletionState State)
{
	// Create notification info
	FNotificationInfo Info(FText::FromString(Message));
	Info.bFireAndForget = true;          // auto-dismiss
	Info.FadeInDuration = 0.2f;
	Info.FadeOutDuration = 0.5f;
	Info.ExpireDuration = 5.0f;          // visible for 5 seconds
	Info.bUseLargeFont = false;

	// Add it to the notification manager
	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);

	if (Notification.IsValid())
	{
		Notification->SetCompletionState(State);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FST_EditorCrashHighscoreModule, ST_EditorCrashHighscore)