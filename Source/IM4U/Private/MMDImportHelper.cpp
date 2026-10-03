// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#include "MMDImportHelper.h"
#include "SjisToUnicode.h"

namespace MMD4UE4
{
	FVector3f MMDImportHelper::ConvertVectorAsixToUE4FromMMD(FVector3f vec)
	{
		FVector3f temp;
		temp.Y = vec.Z*(-1);
		temp.X = vec.X*(1);
		temp.Z = vec.Y*(1);
		return temp;
	}

	//////////////////////////////////////
	// from PMD/PMX Binary Buffer To String @ TextBuf
	// 4 + n: TexBuf
	// buf : top string (top data)
	// encodeType : 0 utf-16, 1 utf-8
	//////////////////////////////////////
	FString MMDImportHelper::PMXTexBufferToFString(const uint8 ** buffer, PMXEncodeType encodeType)
	{
		FString NewString;
		uint32 size = 0;

		FMemory::Memcpy(&size, *buffer, sizeof(uint32));
		*buffer += sizeof(uint32);
		TArray<uint8> RawModData;
		RawModData.AddUninitialized(size + 1);
		FMemory::Memcpy(RawModData.GetData(), *buffer, size);
		RawModData[size] = 0;

		if (encodeType == PMXEncodeType_UTF16LE)
		{
			RawModData.Add(0);
			//NewString.Append((TCHAR*)RawModData.GetData());
			NewString.Append(reinterpret_cast<TCHAR*>(RawModData.GetData()));
		}
		else if (encodeType == PMXEncodeType_UTF8)
		{
			NewString = UTF8_TO_TCHAR(reinterpret_cast<const ANSICHAR*>(RawModData.GetData()));
		}
		*buffer += size;
		return NewString;
	}

	FString MMDImportHelper::ConvertMMDSJISToFString(
		const uint8 *buffer,
		const uint32 size
		)
	{
		FString NewString;
		//uint32 size = 0;
		{
			//FMemory::Memcpy(&size, *buffer, sizeof(uint32));
			//*buffer += sizeof(uint32);
			//size = 20;
			TArray<char> RawModData;
			RawModData.Empty(size);
			RawModData.AddUninitialized(size);
			FMemory::Memcpy(RawModData.GetData(), buffer, RawModData.Num());
			RawModData.Add(0); RawModData.Add(0);
			//NewString.Append((wchar_t*)saba::ConvertSjisToU16String(RawModData.GetData()).c_str());
			NewString.Append(reinterpret_cast<const TCHAR*>(saba::ConvertSjisToU16String(RawModData.GetData()).c_str()));
			//NewString.Append(UTF8_TO_TCHAR(encodeHelper.convert_encoding(RawModData.GetData(), "shift-jis", "utf-8").c_str()));
			//NewString.Append(RawModData.GetData());
		}
		return NewString;
	}

	uint32 MMDImportHelper::MMDExtendBufferSizeToUint32(
		const uint8 ** buffer,
		const uint8  blockSize
		)
	{
		uint32 retValue = 0;
		// Read up to 4 bytes little-endian and combine them
		uint8 bytesToRead = (blockSize < 4) ? blockSize : 4;
		for (uint8 i = 0; i < bytesToRead; ++i)
		{
			retValue |= (static_cast<uint32>((*buffer)[i]) << (8 * i));
		}
		*buffer += blockSize;
		return retValue;
	}
	int32 MMDImportHelper::MMDExtendBufferSizeToInt32(
		const uint8 ** buffer,
		const uint8  blockSize
		)
	{
		// Read up to 4 bytes little-endian
		uint8 bytesToRead = (blockSize < 4) ? blockSize : 4;
		uint32 tmp = 0;
		for (uint8 i = 0; i < bytesToRead; ++i)
		{
			tmp |= (static_cast<uint32>((*buffer)[i]) << (8 * i));
		}
		// Determine if the value is negative by checking the highest bit of the most-significant byte
		bool negative = (bytesToRead > 0) && (((*buffer)[bytesToRead - 1] & 0x80) != 0);
		*buffer += blockSize;
		if (negative)
		{
			// sign-extend to 32 bits
			uint32 mask = (~0u) << (bytesToRead * 8);
			tmp |= mask;
		}
		return static_cast<int32>(tmp);
	}
}