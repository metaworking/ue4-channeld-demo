#include "TpsSpatialChannelDataProcessor.h"

#include "GameFramework/PlayerState.h"

bool FTpsSpatialChannelDataProcessor::Merge(const google::protobuf::Message* SrcMsg, google::protobuf::Message* DstMsg)
{
	DstMsg->MergeFrom(*SrcMsg);
	return true;
}

bool FTpsSpatialChannelDataProcessor::UpdateChannelData(UObject* TargetObj, google::protobuf::Message* ChannelData)
{
	// Don't send Spatial channel data update to channeld, as the channel data is maintained via Spawn and Destroy messages.
	return false;
}

const google::protobuf::Message* FTpsSpatialChannelDataProcessor::GetStateFromChannelData(
	google::protobuf::Message* ChannelData, UClass* TargetClass, uint32 NetGUID, bool& bIsRemoved)
{
	if (TargetClass != UObject::StaticClass())
	{
		return nullptr;
	}
	
	auto SpatialChannelData = static_cast<unrealpb::SpatialChannelData*>(ChannelData);
	auto Entry = SpatialChannelData->mutable_entities()->find(NetGUID);
	if (Entry != SpatialChannelData->mutable_entities()->end())
	{
		bIsRemoved = false;
		return &Entry->second;
	}
	else
	{
		bIsRemoved = true;
		return nullptr;
	}
}

void FTpsSpatialChannelDataProcessor::SetStateToChannelData(const google::protobuf::Message* State,
	google::protobuf::Message* ChannelData, UClass* TargetClass, uint32 NetGUID)
{
	if (TargetClass != UObject::StaticClass())
	{
		return;
	}
	
	auto SpatialChannelData = static_cast<unrealpb::SpatialChannelData*>(ChannelData);
	auto Entry = SpatialChannelData->mutable_entities()->find(NetGUID);
	if (Entry != SpatialChannelData->mutable_entities()->end())
	{
		Entry->second.MergeFrom(*State);
	}
	else
	{
		SpatialChannelData->mutable_entities()->insert({ NetGUID, *static_cast<const unrealpb::SpatialEntityState*>(State) });
	}
}
