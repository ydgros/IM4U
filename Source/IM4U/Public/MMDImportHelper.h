// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"

namespace MMD4UE4
{
	//ToDo:MMD（PMX、PMD、VMD）的共同函数重构
	//现状，PMX系、PMD/VMD系分别安装。

	///////////////
	// encode type 
	///////////////
	enum PMXEncodeType
	{
		PMXEncodeType_UTF16LE = 0,	//for PMX
		PMXEncodeType_UTF8,			//for PMX
		PMXEncodeType_SJIS,			//for PMD, VMD
		PMXEncodeType_ERROR
	};
	class MMDImportHelper
	{
	public:
		// Prevent instantiation and copying: this class provides only static helpers
		//MMDImportHelper() = delete;
		MMDImportHelper() = default; //基MMDImportHelper未在此构造函数中初始化
		MMDImportHelper(const MMDImportHelper&) = delete;
		MMDImportHelper& operator=(const MMDImportHelper&) = delete;
		//////////////////////////////////////
		// from MMD Asix To UE4 Asix 
		// param  : vec , in vector
		// return : convert vec ,out
		//////////////////////////////////////
		static FVector3f ConvertVectorAsixToUE4FromMMD(
			FVector3f vec
			);
		//////////////////////////////////////
		// from PMX Binary Buffer To String @ TextBuf
		// 4 + n: TexBuf
		// buf : top string (top data)
		// encodeType : 0 utf-16, 1 utf-8
		//////////////////////////////////////
		static FString PMXTexBufferToFString(
			const uint8 ** buffer,
			PMXEncodeType encodeType
			);
		//////////////////////////////////////
		// from MMD char (SJIS) To FString 
		// 4 + n: TexBuf
		// buf : top string (top data)
		// encodeType : SJIS 
		// for Pmd , Vmd format
		//////////////////////////////////////
		static FString ConvertMMDSJISToFString(
			const uint8 * buffer,
			const uint32 size
			);

		/////////////////////////////////////
		// import uint
		//////////////////////////////////////
		static uint32 MMDExtendBufferSizeToUint32(
			const uint8 ** buffer,
			const uint8  blockSize
			);
		/////////////////////////////////////
		// import int
		//////////////////////////////////////
		static int32 MMDExtendBufferSizeToInt32(
			const uint8 ** buffer,
			const uint8  blockSize
			);

		/////////////////////////////////////
		// 带边界检查的读取：越界时置bOutOfRange并返回0，不再推进指针
		// 供PMX/PMD/VMD二进制解析使用，避免截断文件导致越界读取崩溃
		//////////////////////////////////////
		static uint32 MMDExtendBufferSizeToUint32Safe(
			const uint8 ** buffer,
			const uint8 *  bufferEnd,
			const uint8    blockSize,
			bool &         bOutOfRange
			);
		static int32 MMDExtendBufferSizeToInt32Safe(
			const uint8 ** buffer,
			const uint8 *  bufferEnd,
			const uint8    blockSize,
			bool &         bOutOfRange
			);
		/////////////////////////////////////
		// 带边界检查的原始拷贝：越界时置bOutOfRange并返回false
		//////////////////////////////////////
		static bool MemcpyFromBufferSafe(
			void *         destination,
			const uint8 ** buffer,
			const uint8 *  bufferEnd,
			const uint64   size,
			bool &         bOutOfRange
			);
		/////////////////////////////////////
		// 带边界检查的字符串读取：先读长度再读数据，任何一步越界都置bOutOfRange
		// 避免损坏文件中的巨大长度导致巨额分配或越界读取崩溃
		//////////////////////////////////////
		static FString PMXTexBufferToFStringSafe(
			const uint8 ** buffer,
			const uint8 *  bufferEnd,
			PMXEncodeType  encodeType,
			bool &         bOutOfRange
			);
	};
}