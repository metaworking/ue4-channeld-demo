﻿#pragma once
#include "ReplicatedActorDecorator.h"

struct FCPPClassInfo
{
	FString HeadFilePath;
	FModuleInfo ModuleInfo;
};

struct FReplicatorCode
{
	TSharedPtr<FReplicatedActorDecorator> Target;

	FString HeadFileName;
	FString HeadCode;

	FString CppFileName;
	FString CppCode;

	FString ProtoFileName;
	FString ProtoDefinitions;

	FString IncludeActorCode;
	FString RegisterReplicatorCode;
};

struct FReplicatorCodeBundle
{
	FString RegisterReplicatorFileCode;

	TArray<FReplicatorCode> ReplicatorCodes;

	FString GlobalStructCodes;

	FString GlobalStructProtoDefinitions;
};

class REPLICATORGENERATOR_API FReplicatorCodeGenerator
{
public:
	bool RefreshModuleInfoByClassName();

	FString GetClassHeadFilePath(const FString& ClassName);

	bool Generate(TArray<UClass*> TargetActors, FReplicatorCodeBundle& ReplicatorCodeBundle);
	bool GenerateActorCode(UClass* TargetActor, FReplicatorCode& ReplicatorCode, FString& ResultMessage);

protected:
	TMap<FString, FModuleInfo> ModuleInfoByClassName;
	TMap<FString, FCPPClassInfo> CPPClassInfoMap;

	inline void ProcessHeaderFiles(const TArray<FString>& Files, const FManifestModule& ManifestModule);
};