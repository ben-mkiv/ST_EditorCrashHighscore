// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Widgets/Notifications/SNotificationList.h"


class FST_EditorCrashHighscoreModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void ShowEditorNotification(const FString Message, SNotificationItem::ECompletionState State);

	TMap<FDateTime, int32> CrashCount;
};
