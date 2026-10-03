// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#include "VmdImporter.h"
#include "MMDImportHelper.h"

namespace MMD4UE4
{

	DEFINE_LOG_CATEGORY(LogMMD4UE4_VmdMotionInfo)

	VmdMotionInfo::VmdMotionInfo()
	{
		maxFrame = 0;
		minFrame = 0;
	}

	VmdMotionInfo::~VmdMotionInfo()
	{
	}

	bool VmdMotionInfo::VMDLoaderBinary(
		const uint8 *& Buffer,
		const uint8 * BufferEnd
		)
	{
		if (Buffer == nullptr || BufferEnd == nullptr || Buffer > BufferEnd)
		{
			return false;
		}

		auto ReadBytes = [&Buffer, BufferEnd](void* Destination, uint64 Size) -> bool
		{
			if (Buffer == nullptr || Buffer > BufferEnd ||
				Size > static_cast<uint64>(BufferEnd - Buffer))
			{
				return false;
			}
			if (Destination != nullptr && Size > 0)
			{
				FMemory::Memcpy(Destination, Buffer, static_cast<SIZE_T>(Size));
			}
			Buffer += Size;
			return true;
		};

		auto CanReadRecords = [&Buffer, BufferEnd](int32 Count, uint64 RecordSize) -> bool
		{
			return Count >= 0 && Buffer <= BufferEnd &&
				static_cast<uint64>(Count) <= static_cast<uint64>(BufferEnd - Buffer) / RecordSize;
		};

		VmdReadMotionData readData;
		{
			if (!ReadBytes(&readData.vmdHeader, sizeof(readData.vmdHeader)))
			{
				return false;
			}
			// 确定是否为VMD文件的临时版本
			if (readData.vmdHeader.header[0] == 'V' &&
				readData.vmdHeader.header[1] == 'o' && 
				readData.vmdHeader.header[2] == 'c')
			{
				UE_LOG(LogMMD4UE4_VmdMotionInfo, Warning, TEXT("VMD Import START /Correct Magic[Vocaloid Motion Data 0002]"));
			}
			else
			{
				//UE_LOG(LogMMD4UE4_PmdMeshInfo, Error, TEXT("PMX Import FALSE/Return /UnCorrect Magic[PMX]"));
				return false;
			}

			// 设置每个数据的起始地址
			{
				//Key VMD
				if (!ReadBytes(&readData.vmdKeyCount, sizeof(readData.vmdKeyCount)) ||
					!CanReadRecords(readData.vmdKeyCount, 111))
				{
					return false;
				}

				//memcopySize = sizeof(VMD_KEY); //5.8
				readData.vmdKeyList.AddZeroed(readData.vmdKeyCount);
				
				for (int32 i = 0; i < readData.vmdKeyCount; ++i)
				{
					VMD_KEY * vmdKeyPtr = &readData.vmdKeyList[i];

					//Bone NAME
					if (!ReadBytes(&vmdKeyPtr->Name[0], sizeof(vmdKeyPtr->Name)))
					{
						return false;
					}
					//posandQurt
					if (!ReadBytes(&vmdKeyPtr->Frame, sizeof(int32) + sizeof(float) * (4 + 3)))
					{
						return false;
					}
					//bezier
					if (!ReadBytes(&vmdKeyPtr->Bezier, sizeof(vmdKeyPtr->Bezier)))
					{
						return false;
					}
					//
					//dummy bezier
					if (!ReadBytes(nullptr, 48 * sizeof(uint8)))
					{
						return false;
					}
				}
			}
			// 设置每个数据的起始地址
			{
				//Key Fase VMD
				if (!ReadBytes(&readData.vmdFaceCount, sizeof(readData.vmdFaceCount)) ||
					!CanReadRecords(readData.vmdFaceCount, 23))
				{
					return false;
				}

				//memcopySize = sizeof(VMD_FACE_KEY);//111
				readData.vmdFaceList.AddZeroed(readData.vmdFaceCount);
				for (int32 i = 0; i < readData.vmdFaceCount; ++i)
				{
					VMD_FACE_KEY * vmdFacePtr = &readData.vmdFaceList[i];

					//Bone NAME
					if (!ReadBytes(&vmdFacePtr->Name[0], sizeof(vmdFacePtr->Name)))
					{
						return false;
					}
					//frame and value
					if (!ReadBytes(&vmdFacePtr->Frame, sizeof(int32) + sizeof(float)))
					{
						return false;
					}
				}
			}
			// 设置每个数据的起始地址
			{
				//Key Camera VMD
				if (!ReadBytes(&readData.vmdCameraCount, sizeof(readData.vmdCameraCount)) ||
					!CanReadRecords(readData.vmdCameraCount, 61))
				{
					return false;
				}

				if (readData.vmdCameraCount > 0)
				{
					//memcopySize = sizeof(VMD_CAMERA);//61
					readData.vmdCameraList.AddZeroed(readData.vmdCameraCount);
					for (int32 i = 0; i < readData.vmdCameraCount; ++i)
					{
						VMD_CAMERA * vmdCameraPtr = &readData.vmdCameraList[i];

						//Freme No
						if (!ReadBytes(&vmdCameraPtr->Frame, sizeof(uint32) * (1) + sizeof(float) * (1 + 3 + 3)))
						{
							return false;
						}

						//Interpolation[6][4]
						if (!ReadBytes(&vmdCameraPtr->Interpolation[0][0][0], sizeof(uint8) * (6 * 4)))
						{
							return false;
						}

						//ViewingAngle + Perspective
						if (!ReadBytes(&vmdCameraPtr->ViewingAngle, sizeof(uint32) * (1) + sizeof(uint8) * (1)))
						{
							return false;
						}
					}
				}
			}
		}

		if (!ConvertVMDFromReadData(&readData))
		{
			// convert err
			return false;
		}
		
		return true;
	}

	bool VmdMotionInfo::ConvertVMDFromReadData(
		VmdReadMotionData * readData
		)
	{
		check(readData);
		if (!readData)
		{
			return false;
		}

		ModelName = ConvertMMDSJISToFString(
			reinterpret_cast<uint8 *>(readData->vmdHeader.modelName),
			sizeof(readData->vmdHeader.modelName)
			);

		int arrayIndx;
		int arrayIndxPre;
		FString(trackName);
		TArray<FString> tempTrackNameList;
		{
			//Keys
			TArray<VmdKeyTrackList>	tempKeyBoneList;
			VmdKeyTrackList * vmdKeyTrackPtr = nullptr;//5.8//不能删
			//VMD_KEY * vmdKeyPtr = NULL;

			//VMD Key
			tempTrackNameList.Empty();
			for (int32 i = 0; i < readData->vmdKeyCount; i++)
			{
				// get ptr
				VMD_KEY * vmdKeyPtr = &(readData->vmdKeyList[i]);
				trackName = ConvertMMDSJISToFString(
					reinterpret_cast<uint8 *>(vmdKeyPtr->Name),
					sizeof(vmdKeyPtr->Name)
					);
				
				arrayIndxPre = tempTrackNameList.Num();
				arrayIndx = tempTrackNameList.AddUnique(trackName);
				if (tempTrackNameList.Num() > arrayIndxPre)
				{
					arrayIndx = tempKeyBoneList.Add(VmdKeyTrackList());//5.8//不能删
					vmdKeyTrackPtr = &(tempKeyBoneList[arrayIndx]);//5.8//不能删
					vmdKeyTrackPtr->TrackName = trackName;
				}
				else
				{
					vmdKeyTrackPtr = &(tempKeyBoneList[arrayIndx]);//5.8//不能删
					/*VmdKeyTrackList * vmdKeyTrackPtr = &(tempKeyBoneList[arrayIndx]);
					
					//exist
					for (int k = 0; k < tempKeyBoneList.Num(); k++)
					{
					vmdKeyTrackPtr = &(tempKeyBoneList[k]);
					if (vmdKeyTrackPtr->TrackName.Equals(trackName))
					{
					break;
					}
					vmdKeyTrackPtr = NULL;
					}*/ //5.8
				}
				check(vmdKeyTrackPtr);
				arrayIndx = vmdKeyTrackPtr->keyList.Add(*vmdKeyPtr); //5.8
				vmdKeyTrackPtr->maxFrameCount
					= FMath::Max(vmdKeyPtr->Frame, vmdKeyTrackPtr->maxFrameCount);
				vmdKeyTrackPtr->minFrameCount
					= FMath::Min(vmdKeyPtr->Frame, vmdKeyTrackPtr->minFrameCount);
				maxFrame = FMath::Max(vmdKeyPtr->Frame, maxFrame);
				minFrame = FMath::Min(vmdKeyPtr->Frame, minFrame);
			}
			//为便于按照Frame顺序计算，生成对List的索引参照进行排序的数组。但是太浪费的处理。由于VMD的顺序不同。
			for (int i = 0; i < tempKeyBoneList.Num(); i++)
			{// all bone

				if (tempKeyBoneList[i].keyList.Num() > 0)
				{
					//init : insert array index = 0
					tempKeyBoneList[i].sortIndexList.Add(0);
				}
				else
				{
					continue;
				}
				//sort all vmd-key frames
				for (int k = 1; k < tempKeyBoneList[i].keyList.Num(); k++)
				{
					bool isSetList = false;
					//TBD:2分钟搜索应该轻量地去，但是因为很麻烦，所以先做线性搜索
					for (int s = 0; s < tempKeyBoneList[i].sortIndexList.Num(); s++)
					{
						//如果这次的值比现在的位置大吗？
						if (tempKeyBoneList[i].keyList[k].Frame <
							tempKeyBoneList[i].keyList[tempKeyBoneList[i].sortIndexList[s]].Frame)
						{
							/*if (s + 1 == tempKeyBoneList[i].sortIndexList.Num())
							{
							//insert array index = k;
							tempKeyBoneList[i].sortIndexList.Add(k);
							}
							else*/
							{
								//insert array index = k;
								tempKeyBoneList[i].sortIndexList.Insert(k, s);
							}
							//check list size == now index num
							//check(tempKeyBoneList[i].sortIndexList.Num() == k+1);
							isSetList = true;
							break;
						}
					}
					if (!isSetList)
					{
						//add last
						tempKeyBoneList[i].sortIndexList.Add(k);
					}
				}
			}
			keyBoneList = tempKeyBoneList;
		}
		{
			//Skins
			TArray<VmdFaceTrackList> tempKeyFaceList;
			VmdFaceTrackList * vmdFaceTrackPtr = nullptr;//5.8//不能删
			//VMD_FACE_KEY * vmdFacePtr = NULL;
			//坡口
			//Facc
			tempTrackNameList.Empty();
			for (int32 i = 0; i < readData->vmdFaceCount; i++)
			{
				VMD_FACE_KEY * vmdFacePtr = &(readData->vmdFaceList[i]);
				trackName = ConvertMMDSJISToFString(
					reinterpret_cast<uint8 *>(vmdFacePtr->Name),
					sizeof(vmdFacePtr->Name)
					);
				arrayIndxPre = tempTrackNameList.Num();
				arrayIndx = tempTrackNameList.AddUnique(trackName);
				if (tempTrackNameList.Num() > arrayIndxPre)
				{
					arrayIndx = tempKeyFaceList.Add(VmdFaceTrackList());//5.8//不能删
					vmdFaceTrackPtr = &(tempKeyFaceList[arrayIndx]);//5.8//不能删
					vmdFaceTrackPtr->TrackName = trackName;
				}
				else
				{
					vmdFaceTrackPtr = &(tempKeyFaceList[arrayIndx]);//5.8//不能删
					/*VmdFaceTrackList *vmdFaceTrackPtr = &(tempKeyFaceList[arrayIndx]);

					//exist
					for (int k = 0; k < tempKeyBoneList.Num(); k++)
					{
					vmdKeyTrackPtr = &(tempKeyBoneList[k]);
					if (vmdKeyTrackPtr->TrackName.Equals(trackName))
					{
					break;
					}
					vmdKeyTrackPtr = NULL;
					}*/ //5.8
				}
				check(vmdFaceTrackPtr);
				arrayIndx = vmdFaceTrackPtr->keyList.Add(*vmdFacePtr); //5.8
				vmdFaceTrackPtr->maxFrameCount
					= FMath::Max(vmdFacePtr->Frame, vmdFaceTrackPtr->maxFrameCount);
				vmdFaceTrackPtr->minFrameCount
					= FMath::Min(vmdFacePtr->Frame, vmdFaceTrackPtr->minFrameCount);
				maxFrame = FMath::Max(vmdFacePtr->Frame, maxFrame);
				minFrame = FMath::Min(vmdFacePtr->Frame, minFrame);
			}
			//为便于按照Frame顺序计算，生成对List的索引参照进行排序的数组。但是太浪费的处理。由于VMD的顺序不同。
			for (int i = 0; i < tempKeyFaceList.Num(); i++)
			{// all bone
				if (tempKeyFaceList[i].keyList.Num() > 0)
				{
					//init : insert array index = 0
					tempKeyFaceList[i].sortIndexList.Add(0);
				}
				else
				{
					continue;
				}
				//sort all vmd-key frames
				for (int k = 1; k < tempKeyFaceList[i].keyList.Num(); k++)
				{
					bool isSetList = false;
					//TBD:2分钟搜索应该轻量地去，但是因为很麻烦，所以先做线性搜索
					for (int s = 0; s < tempKeyFaceList[i].sortIndexList.Num(); s++)
					{
						//もし現在の位置よりも今回の値が大きいか？
						if (tempKeyFaceList[i].keyList[k].Frame <
							tempKeyFaceList[i].keyList[tempKeyFaceList[i].sortIndexList[s]].Frame)
						{
							/*if (s + 1 == tempKeyBoneList[i].sortIndexList.Num())
							{
							//insert array index = k;
							tempKeyBoneList[i].sortIndexList.Add(k);
							}
							else*/
							{
								//insert array index = k;
								tempKeyFaceList[i].sortIndexList.Insert(k, s);
							}
							//check list size == now index num
							//check(tempKeyBoneList[i].sortIndexList.Num() == k+1);
							isSetList = true;
							break;
						}
					}
					if (!isSetList)
					{
						//add last
						tempKeyFaceList[i].sortIndexList.Add(k);
					}
				}
			}
			keyFaceList = tempKeyFaceList;
		}
		if (readData->vmdCameraCount > 0)
		{
			//Keys
			TArray<VmdCameraTrackList>	tempKeyCamList;
			//Camera Key
			tempTrackNameList.Empty();
			tempKeyCamList.Empty();
			tempKeyCamList.Add(VmdCameraTrackList());//VMDにcamはひとつしかない為
			trackName = "MMDCamera000";//固定値
			VmdCameraTrackList * vmdCamKeyTrackPtr = &(tempKeyCamList[0]);//VMDにcamはひとつしかない為
			vmdCamKeyTrackPtr->TrackName = trackName;
			for (int32 i = 0; i < readData->vmdCameraCount; i++)
			{
				VMD_CAMERA * vmdCamKeyPtr = &(readData->vmdCameraList[i]);
				check(vmdCamKeyTrackPtr);
				arrayIndx = vmdCamKeyTrackPtr->keyList.Add(*vmdCamKeyPtr); //5.8
				vmdCamKeyTrackPtr->maxFrameCount
					= FMath::Max(vmdCamKeyPtr->Frame, vmdCamKeyTrackPtr->maxFrameCount);
				vmdCamKeyTrackPtr->minFrameCount
					= FMath::Min(vmdCamKeyPtr->Frame, vmdCamKeyTrackPtr->minFrameCount);
				maxFrame = FMath::Max(vmdCamKeyPtr->Frame, maxFrame);
				minFrame = FMath::Min(vmdCamKeyPtr->Frame, minFrame);
			}
			// 为便于按照Frame顺序计算，生成对List的索引参照进行排序的数组。但是太浪费的处理。由于VMD的顺序不同。
			for (int i = 0; i < tempKeyCamList.Num(); i++)
			{// all bone
				if (tempKeyCamList[i].keyList.Num() > 0)
				{
					//init : insert array index = 0
					tempKeyCamList[i].sortIndexList.Add(0);
				}
				else
				{
					continue;
				}
				//sort all vmd-key frames
				for (int k = 1; k < tempKeyCamList[i].keyList.Num(); k++)
				{
					bool isSetList = false;
					//TBD:2分探索で軽量に行くべきだが面倒くさいのでとりあえず、線形探索にする
					for (int s = 0; s < tempKeyCamList[i].sortIndexList.Num(); s++)
					{
						//もし現在の位置よりも今回の値が大きいか？
						if (tempKeyCamList[i].keyList[k].Frame <
							tempKeyCamList[i].keyList[tempKeyCamList[i].sortIndexList[s]].Frame)
						{
							/*if (s + 1 == tempKeyBoneList[i].sortIndexList.Num())
							{
							//insert array index = k;
							tempKeyBoneList[i].sortIndexList.Add(k);
							}
							else*/
							{
								//insert array index = k;
								tempKeyCamList[i].sortIndexList.Insert(k, s);
							}
							//check list size == now index num
							//check(tempKeyBoneList[i].sortIndexList.Num() == k+1);
							isSetList = true;
							break;
						}
					}
					if (!isSetList)
					{
						//add last
						tempKeyCamList[i].sortIndexList.Add(k);
					}
				}
			}
			keyCameraList = tempKeyCamList;
		}

		// end phase
		maxFrame++;			//min 1frame ?
		return true;
	}

	//如果在指定列表中有相应的Frame名称，则返回该索引值。异常值=-1。

	/*void Fsting(const int targetName)
	{
		printf("%p\targetName", &targetName);
	}*/

	int32 VmdMotionInfo::FindKeyTrackName(
	    //FString targetName,
	    FString(targetName), //5.8 //常量引用传递
		EVMDKEYFRAMETYPE listType)
	{
		int32  index = -1;
		//★TBD:VMD的骨骼名称为15Byte限制，应按前方一致检索
		//由于麻烦，以完全一致型检索骨骼名称尾（帧名）
		if (listType == EVMDKEYFRAMETYPE::EVMD_KEYBONE)
		{
			for (int i = 0; i < keyBoneList.Num(); i++)
			{
				if (keyBoneList[i].TrackName.Equals(targetName))
				{
					index = i;
					break;
				}
			}

		}
		else if (listType == EVMDKEYFRAMETYPE::EVMD_KEYFACE)
		{
			for (int i = 0; i < keyFaceList.Num(); i++)
			{
				if (keyFaceList[i].TrackName.Equals(targetName))
				{
					index = i;
					break;
				}
			}
		}
		else
		{
		}

		return index;
	}
}