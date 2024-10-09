// この MFC サンプル ソース コードでは、MFC Microsoft Office Fluent ユーザー インターフェイス 
// ("Fluent UI") の使用方法を示します。このコードは、MFC C++ ライブラリ ソフトウェアに 
// 同梱されている Microsoft Foundation Class リファレンスおよび関連電子ドキュメントを
// 補完するための参考資料として提供されます。
// Fluent UI を複製、使用、または配布するためのライセンス条項は個別に用意されています。
// Fluent UI ライセンス プログラムの詳細については、Web サイト
// http://go.microsoft.com/fwlink/?LinkId=238214 を参照してください。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildFrm.cpp : CChildFrame クラスの実装
//

#include "stdafx.h"
#include "RemoteGraph.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "RemoteGraphDoc.h"
#include "RemoteGraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if 0
enum {
	blue = 0x0000ff,
	red = 0xff0000,
	magenta = 0xff00ff,
	green = 0x008000,
	cyan = 0x00ffff,
	yellow = 0xffff00,
	white = 0xffffff,
	orange = 0xffa500,
	brown = 0xa52a2a,
	lime = 0x00ff00,
} eColor;


extern const dataParam sampleParam[8][10] = {
	{
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域1"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域2"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域3"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域4"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},

	{
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域5"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Line,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域6"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Small,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			0,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域7"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Medium,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},
	{
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data1"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			blue,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data2"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			red,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data3"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			magenta,		// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data4"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			green,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data5"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			cyan,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data6"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			yellow,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data7"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			white,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data8"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			orange,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data9"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			brown,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
		{
			1,				// wndIdx			// ウィンドウ番号
			_T("グラフ領域8"),	// wndName;			// ウィンドー名称
			_T("Data10"),	// itemName;		// データ名称
			TRUE,			// isRialTiame;		// リアルタイム（true）/ ノンリアルタイム（false）
			TRUE,			// isViewMode;		// 表示On/Off
			Dots,			// drawMode;		// 描画モード（Line、Dots、L&D）
			lime,			// color;			// 表示色
			Large,			// dotSize;			// ドットサイズ
			TRUE,			// isVAxisAutRange;	// 縦軸オートスケール
			1.0f,			// VAxisScaleIntvl;	// 縦軸目盛りの間隔
			1.0f,			// VAxisLabelIntvl;	// 縦軸ラベルの間隔
			100.0f,			// VAxisMaxRange;	// 縦軸最大値
			0.0f,			// VAxisMinRange;	// 縦軸最小値
			TRUE,			// isHAxisAutRange;	// 横軸オートスケール
			second,			// HAxisUnit;		// 横軸単位（秒/分/時）
			1.0f,			// HAxisScale;		// 横軸表示スケール
			1.0f,			// HAxisScaleIntvl;	// 横軸目盛りの間隔
			1.0f			// HAxisLabelIntvl;	// 横軸ラベルの間隔
		},
	},

};
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CChildFrame コンストラクション/デストラクション

CChildFrame::CChildFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 診断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame メッセージ ハンドラー


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// 子フレームのアクティブ状態をメインフレームに通知する
	theApp.m_pMainWnd->PostMessage(eMessage_ChildActivate, (WPARAM)bActivate, (LPARAM)this);
}


void CChildFrame::OnDestroy()
{
	// 子フレームが終了したことをメインフレームに通知する
	theApp.m_pMainWnd->PostMessage(eMessage_ChildDestroy, 0, (LPARAM)this);

	((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->DeleteGraph();
	((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->DeleteGraph();

	CMDIChildWndEx::OnDestroy();

}


LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case	mMessageStartInitialize:
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->DeleteGraph();
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->DeleteGraph();
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_All, m_nID);
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_All, m_nID);
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->SetViewType(eGraphType_All);
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->SetViewType(eGraphType_All);
		break;
	case	mMessageOpenFile:
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->DeleteGraph();
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->DeleteGraph();
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_OffAll, m_nID);
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_OffAll, m_nID);
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->SetViewType(eGraphType_OffAll);
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->SetViewType(eGraphType_OffAll);
		break;
	case	eMessage_PropetyChange:
		// パラメータ更新のため、パラメータを読み込みに行く
		{	
			CPropertiesData* pParam = &theApp.GetPropertiesData(m_nID);

			// スクロールバーを表示／非表示
//			((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ShowScroll(pParam->isRealTime ? FALSE : TRUE);
//			((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ShowScroll(pParam->isRealTime ? FALSE : TRUE);

			if( wParam )
			{
				// 最大値／最小値を作成
				((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateMinMaxValue();
				((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateMinMaxValue();
			}

			// リミット履歴を削除
			for( int i = 0; i < MAX_TDATA; i++ )
				theApp.GetDataCollection().ClearLimitState(m_nID, i);

			AddViewParam(pParam);

//			Invalidate();
//			UpdateWindow();
		}
		break;
#if 1
	case	eMessage_ThreadNormal:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData(m_nID);

			// 監視データが更新されたため、ビューに更新を通知
			if( prop->isRealTime )
				UpdateDataCollection((wParam == 0) ? false : true);
		}
		break;
#endif

	case eMessage_UpdateZoomRectangle:
		// ズームを初期化
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ResetRectangle(theApp.GetDataCollection().GetTrendZoomStart(m_nID), theApp.GetDataCollection().GetTrendZoomEnd(m_nID));

		// 値差を初期化
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ResetDiffer();

		if( wParam )
			UpdateDataCollection(true);

		break;

	case eMessage_UpdateCursorLine:
		// カーソルを初期化
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ResetCursor(theApp.GetDataCollection().GetTrendAllCursor(m_nID));
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ResetCursor(theApp.GetDataCollection().GetTrendZoomCursor(m_nID));

		if( wParam )
			UpdateDataCollection(true);

		// 出力ウィンドウに通知
		((CMainFrame *)theApp.m_pMainWnd)->PostMessageToOutput(eMessage_UpdateCursorLine, m_nID, 0);

		break;

	case	eMessage_UpdateGraph:
		UpdateDataCollection(true);

		break;

	case eMessage_AutoClear:
		if( wParam == 0 )
		{
			CPropertiesData* pParam = &theApp.GetPropertiesData(m_nID);
			AddViewParam(pParam);
		}
		break;

	case eMessage_MoveZoomRectangle:
		{
			theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, m_nZoomStart);
			theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
			PostMessage(eMessage_UpdateZoomRectangle, 0);
			PostMessage(eMessage_UpdateCursorLine, 1);
		}
		break;

	case eMessage_CreateZoomRectangle:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData(m_nID);

			theApp.GetDataCollection().UpdateTrendZoomTblNo(m_nID, m_nZoomTblNo);
			theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, m_nZoomStart);
			theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
			{
				stVScale	tmpVScale = prop->trandVScale;
				tmpVScale.Auto = TRUE;	// 解析なら、AUTO縦スケール固定
				((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendZoomTblNo(m_nID));
			}
			PostMessage(eMessage_UpdateZoomRectangle, 0);
			PostMessage(eMessage_UpdateCursorLine, 1);
		}
		break;

	case eMessage_MoveCursorLine:
		theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, m_nCursor);
		theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
		PostMessage(eMessage_UpdateCursorLine, 1);
		break;

	case eMessage_CreateCursorLine:
		theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, m_nCursor);
		theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
		PostMessage(eMessage_UpdateCursorLine, 1);
		break;

	case eMessage_AnalysisKeyEvent:
		{
			CPropertiesData	*prop = &theApp.GetPropertiesData(m_nID);

			// 解析中ならキーイベントによってズームを操作
			if( !prop->isRealTime )
			{
				int	nTbl = theApp.GetDataCollection().GetTrendZoomTblNo(m_nID);
				unsigned __int64	nStart = theApp.GetDataCollection().GetTrendZoomStart(m_nID);
				unsigned __int64	nPos = theApp.GetDataCollection().GetTrendAllCursor(m_nID);

				switch( wParam )
				{
					case VK_UP:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomTblNo(m_nID, nTbl - 1);
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, nStart);
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							{
								stVScale	tmpVScale = prop->trandVScale;
								tmpVScale.Auto = TRUE;	// 解析なら、AUTO縦スケール固定
								((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendZoomTblNo(m_nID));
							}
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					case VK_DOWN:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomTblNo(m_nID, nTbl + 1);
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, nStart);
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							{
								stVScale	tmpVScale = prop->trandVScale;
								tmpVScale.Auto = TRUE;	// 解析なら、AUTO縦スケール固定
								((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendZoomTblNo(m_nID));
							}
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					case VK_LEFT:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, (nStart == 0) ? 0 : (nStart - 1));
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						else
						{
							theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, (nPos == 0) ? 0 : (nPos - 1));
							theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					case VK_RIGHT:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, nStart + 1);
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						else
						{
							theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, nPos + 1);
							theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					case VK_PRIOR:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, (nStart < tblRealScaleInfo[nTbl].dsp_data) ? 0 : (nStart - tblRealScaleInfo[nTbl].dsp_data));
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						else
						{
							theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, (nPos < tblRealScaleInfo[nTbl].dsp_data) ? 0 : (nPos - tblRealScaleInfo[nTbl].dsp_data));
							theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					case VK_NEXT:
						if( GetKeyState(VK_CONTROL) < 0 )
						{
							theApp.GetDataCollection().UpdateTrendZoomPos(m_nID, nStart + tblRealScaleInfo[nTbl].dsp_data);
							theApp.GetDataCollection().UpdateTrendZoomData(m_nID);
							PostMessage(eMessage_UpdateZoomRectangle, 0);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						else
						{
							theApp.GetDataCollection().UpdateTrendCursorPos(m_nID, nPos + tblRealScaleInfo[nTbl].dsp_data);
							theApp.GetDataCollection().UpdateTrendCursorData(m_nID);
							PostMessage(eMessage_UpdateCursorLine, 1);
						}
						break;

					default:
						break;
				}
			}
		}

		break;

	default:
		return CMDIChildWndEx::WindowProc(message, wParam, lParam);
	}

	return 1;
}


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect rectClient;
	GetClientRect(rectClient);

	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, CSize(0, rectClient.Height() / 2), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CRemoteGraphView), CSize(0, rectClient.Height() / 2), pContext);

	// ウィンドウ番号を取得する
//	m_nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetChildWndListSize();
	m_nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();

	// ウィンドウ番号が8以上ならばウィンドウは開けない
	if (m_nID >= MAX_VIEW) {
		return FALSE;
	}

//	((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_All);
//	((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_Trend);
	((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_All, m_nID);
	((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_All, m_nID);

#if 0
	// ウィンドウ番号に対応するパラメータ配列をアプリから取得
	int pos = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();
//	CPropertiesData* pParam = &theApp.GetPropertiesData(m_nID);
	CPropertiesData* pParam = &theApp.GetPropertiesData(pos);
	// ビューにパラメータ追加
	AddViewParam(pParam);
#endif
#if 0
	// ダミーパラメータを取得
	CPropertiesData* pParam = new CPropertiesData();
	pParam->wndName = sampleParam[m_nID][0].wndName;
	for (int i = 0; i < MAX_TDATA; ++i) {
		pParam->itemName[i] = sampleParam[m_nID][i].itemName;
		pParam->isRealTime[i] = sampleParam[m_nID][i].isRialTiame;
		pParam->isViewMode[i] = sampleParam[m_nID][i].isViewMode;
		pParam->drawMode[i] = sampleParam[m_nID][i].drawMode;
		pParam->color[i] = sampleParam[m_nID][i].color;
		pParam->dotSize[i] = sampleParam[m_nID][i].dotSize;
		pParam->isVAxisAutRange[i] = sampleParam[m_nID][i].isVAxisAutRange;
		pParam->VAxisScaleIntvl[i] = sampleParam[m_nID][i].VAxisScaleIntvl;
		pParam->VAxisLabelIntvl[i] = sampleParam[m_nID][i].VAxisLabelIntvl;
		pParam->VAxisMaxRange[i] = sampleParam[m_nID][i].VAxisMaxRange;
		pParam->isHAxisAutRange[i] = sampleParam[m_nID][i].isHAxisAutRange;
		pParam->HAxisUnit[i] = sampleParam[m_nID][i].HAxisUnit;
		pParam->HAxisScale[i] = sampleParam[m_nID][i].HAxisScale;
		pParam->HAxisScaleIntvl[i] = sampleParam[m_nID][i].HAxisScaleIntvl;
		pParam->HAxisLabelIntvl[i] = sampleParam[m_nID][i].HAxisLabelIntvl;
	}

	// ビューにパラメータ追加
	AddViewParam(pParam);

	delete pParam;
#endif

	// 閉じるボタン無効化
	CMenu*  pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	return TRUE;

//	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

/*============================================================================*/
/*! 子ウィンドウ

-# ビューにパラメータを追加する

@param	pParam		データパラメータ
@retval

*/
/*============================================================================*/
BOOL CChildFrame::AddViewParam(CPropertiesData* pParam)
{
	BOOL ret = FALSE;

	ASSERT(pParam);

	for (int nView = 0; nView < eViewID_Num; ++nView)
//	for (int nView = 0; nView < 1; ++nView)
	{
		((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->SetRealTime(pParam->isRealTime);

		if( pParam->isRealTime )
			ret = ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddAxis(pParam->trandVScale, pParam->trandHScale[nView]);
		else
		{
			stVScale	tmpVScale = pParam->trandVScale;

			tmpVScale.Auto = TRUE;	// 解析なら、AUTO縦スケール固定

			if( nView == eViewID_1 )
				ret = ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendAllTblNo(m_nID));
			else
				ret = ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendZoomTblNo(m_nID));
		}

		CString	strName;

		// 各データのパラメータを追加
		for (int i = 0; i < MAX_TDATA; ++i)
//		for (int i = 0; i < 1; ++i)
		{
			VScaleKind	tmpVScaleKind = pParam->item[i].vscaleKind;

			if( pParam->isRealTime )
				tmpVScaleKind = pParam->item[i].vscaleKind;
			else
				tmpVScaleKind = LScale;	// 解析なら、左スケール固定

			switch (pParam->item[i].drawMode)
			{
			case Line:
				ret &= ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddLine(i, pParam->item[i].dataName, pParam->item[i].dataGName, 0, pParam->item[i].color, PS_SOLID, 1, tmpVScaleKind, pParam->item[i].isViewMode);
				break;
			case Dots:
				ret &= ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddDot(i, pParam->item[i].dataName, pParam->item[i].dataGName, 0, pParam->item[i].color, pParam->item[i].dotSize, tmpVScaleKind, pParam->item[i].isViewMode);
				break;
			case LineAndDots:
				ret &= ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddLineAndDot(i, pParam->item[i].dataName, pParam->item[i].dataGName, 0, pParam->item[i].color, PS_SOLID, 1, pParam->item[i].dotSize, tmpVScaleKind, pParam->item[i].isViewMode);
				break;
			default:
				// 何もしない
				break;
			}
		}

		// グラフ更新
		int nViewNo = ((CMainFrame*)(theApp.m_pMainWnd))->FindChildListPos(this);

		// ズーム／カーソルを初期化
		if( !pParam->isRealTime )
		{
			if( nView == eViewID_1 )
			{
				((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->ResetRectangle(theApp.GetDataCollection().GetTrendZoomStart(m_nID), theApp.GetDataCollection().GetTrendZoomEnd(m_nID));
				((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->ResetCursor(theApp.GetDataCollection().GetTrendAllCursor(m_nID));
			}
			else
				((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->ResetCursor(theApp.GetDataCollection().GetTrendZoomCursor(m_nID));

			((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->ResetDiffer();
		}

		((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->UpdateGraphWindow(nView, true/*Min/Maxをクリア*/);

//		SetActiveView(((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0)));
	}

	return ret;
}

#if 1
/*============================================================================*/
/*! 子ウィンドウ

-# ビューに監視データの更新を通知する

@param	なし
@retval	なし

*/
/*============================================================================*/
void CChildFrame::UpdateDataCollection(bool bClear)
{
	for (int nView = 0; nView < eViewID_Num; ++nView)
//	for (int nView = 0; nView < 1; ++nView)
	{
		int nViewNo = ((CMainFrame*)(theApp.m_pMainWnd))->FindChildListPos(this);

		// グラフ更新
		((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->UpdateGraphWindow(nView, bClear);
//		SetActiveView(((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0)));
	}
}
#endif

/*============================================================================*/
/*! 子ウィンドウ

-# Min／Max値を取得

@param
@retval

*/
/*============================================================================*/
void CChildFrame::GetMinMaxValue(double& min, double& max)
{
	((CRemoteGraphView *)m_wndSplitter.GetPane(1, 0))->GetMinMaxValue(min, max);
}
void CChildFrame::GetMinMaxValueEx(double& min, double& max)
{
	((CRemoteGraphView *)m_wndSplitter.GetPane(1, 0))->GetMinMaxValueEx(min, max);
}

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//lpMMI->ptMinTrackSize.x = 300;
	//lpMMI->ptMinTrackSize.y = 200;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// 閉じるボタン処理なし
	if (nID == SC_CLOSE)
		return;

	CMDIChildWndEx::OnSysCommand(nID, lParam);
}
