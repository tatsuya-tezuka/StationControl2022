// ���� MFC �T���v�� �\�[�X �R�[�h�ł́AMFC Microsoft Office Fluent ���[�U�[ �C���^�[�t�F�C�X 
// ("Fluent UI") �̎g�p���@�������܂��B���̃R�[�h�́AMFC C++ ���C�u���� �\�t�g�E�F�A�� 
// ��������Ă��� Microsoft Foundation Class ���t�@�����X����ъ֘A�d�q�h�L�������g��
// �⊮���邽�߂̎Q�l�����Ƃ��Ē񋟂���܂��B
// Fluent UI �𕡐��A�g�p�A�܂��͔z�z���邽�߂̃��C�Z���X�����͌ʂɗp�ӂ���Ă��܂��B
// Fluent UI ���C�Z���X �v���O�����̏ڍׂɂ��ẮAWeb �T�C�g
// http://go.microsoft.com/fwlink/?LinkId=238214 ���Q�Ƃ��Ă��������B
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildFrm.cpp : CChildFrame �N���X�̎���
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
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�1"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�2"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�3"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�4"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},

	{
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�5"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Line,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�6"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Small,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			0,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�7"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Medium,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
	},
	{
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data1"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			blue,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data2"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			red,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data3"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			magenta,		// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data4"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			green,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data5"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			cyan,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data6"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			yellow,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data7"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			white,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data8"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			orange,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data9"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			brown,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
		},
		{
			1,				// wndIdx			// �E�B���h�E�ԍ�
			_T("�O���t�̈�8"),	// wndName;			// �E�B���h�[����
			_T("Data10"),	// itemName;		// �f�[�^����
			TRUE,			// isRialTiame;		// ���A���^�C���itrue�j/ �m�����A���^�C���ifalse�j
			TRUE,			// isViewMode;		// �\��On/Off
			Dots,			// drawMode;		// �`�惂�[�h�iLine�ADots�AL&D�j
			lime,			// color;			// �\���F
			Large,			// dotSize;			// �h�b�g�T�C�Y
			TRUE,			// isVAxisAutRange;	// �c���I�[�g�X�P�[��
			1.0f,			// VAxisScaleIntvl;	// �c���ڐ���̊Ԋu
			1.0f,			// VAxisLabelIntvl;	// �c�����x���̊Ԋu
			100.0f,			// VAxisMaxRange;	// �c���ő�l
			0.0f,			// VAxisMinRange;	// �c���ŏ��l
			TRUE,			// isHAxisAutRange;	// �����I�[�g�X�P�[��
			second,			// HAxisUnit;		// �����P�ʁi�b/��/���j
			1.0f,			// HAxisScale;		// �����\���X�P�[��
			1.0f,			// HAxisScaleIntvl;	// �����ڐ���̊Ԋu
			1.0f			// HAxisLabelIntvl;	// �������x���̊Ԋu
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

// CChildFrame �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame::CChildFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
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

// CChildFrame �f�f

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

// CChildFrame ���b�Z�[�W �n���h���[


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// �q�t���[���̃A�N�e�B�u��Ԃ����C���t���[���ɒʒm����
	theApp.m_pMainWnd->PostMessage(eMessage_ChildActivate, (WPARAM)bActivate, (LPARAM)this);
}


void CChildFrame::OnDestroy()
{
	// �q�t���[�����I���������Ƃ����C���t���[���ɒʒm����
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
		// �p�����[�^�X�V�̂��߁A�p�����[�^��ǂݍ��݂ɍs��
		{	
			CPropertiesData* pParam = &theApp.GetPropertiesData(m_nID);

			// �X�N���[���o�[��\���^��\��
//			((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ShowScroll(pParam->isRealTime ? FALSE : TRUE);
//			((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ShowScroll(pParam->isRealTime ? FALSE : TRUE);

			if( wParam )
			{
				// �ő�l�^�ŏ��l���쐬
				((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateMinMaxValue();
				((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateMinMaxValue();
			}

			// ���~�b�g�������폜
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

			// �Ď��f�[�^���X�V���ꂽ���߁A�r���[�ɍX�V��ʒm
			if( prop->isRealTime )
				UpdateDataCollection((wParam == 0) ? false : true);
		}
		break;
#endif

	case eMessage_UpdateZoomRectangle:
		// �Y�[����������
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ResetRectangle(theApp.GetDataCollection().GetTrendZoomStart(m_nID), theApp.GetDataCollection().GetTrendZoomEnd(m_nID));

		// �l����������
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ResetDiffer();

		if( wParam )
			UpdateDataCollection(true);

		break;

	case eMessage_UpdateCursorLine:
		// �J�[�\����������
		((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->ResetCursor(theApp.GetDataCollection().GetTrendAllCursor(m_nID));
		((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->ResetCursor(theApp.GetDataCollection().GetTrendZoomCursor(m_nID));

		if( wParam )
			UpdateDataCollection(true);

		// �o�̓E�B���h�E�ɒʒm
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
				tmpVScale.Auto = TRUE;	// ��͂Ȃ�AAUTO�c�X�P�[���Œ�
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

			// ��͒��Ȃ�L�[�C�x���g�ɂ���ăY�[���𑀍�
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
								tmpVScale.Auto = TRUE;	// ��͂Ȃ�AAUTO�c�X�P�[���Œ�
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
								tmpVScale.Auto = TRUE;	// ��͂Ȃ�AAUTO�c�X�P�[���Œ�
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

	// �E�B���h�E�ԍ����擾����
//	m_nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetChildWndListSize();
	m_nID = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();

	// �E�B���h�E�ԍ���8�ȏ�Ȃ�΃E�B���h�E�͊J���Ȃ�
	if (m_nID >= MAX_VIEW) {
		return FALSE;
	}

//	((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_All);
//	((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_Trend);
	((CRemoteGraphView*)m_wndSplitter.GetPane(0, 0))->CreateGraph(eViewID_1, eGraphType_All, m_nID);
	((CRemoteGraphView*)m_wndSplitter.GetPane(1, 0))->CreateGraph(eViewID_2, eGraphType_All, m_nID);

#if 0
	// �E�B���h�E�ԍ��ɑΉ�����p�����[�^�z����A�v������擾
	int pos = ((CMainFrame*)(theApp.m_pMainWnd))->GetCreateVewIdx();
//	CPropertiesData* pParam = &theApp.GetPropertiesData(m_nID);
	CPropertiesData* pParam = &theApp.GetPropertiesData(pos);
	// �r���[�Ƀp�����[�^�ǉ�
	AddViewParam(pParam);
#endif
#if 0
	// �_�~�[�p�����[�^���擾
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

	// �r���[�Ƀp�����[�^�ǉ�
	AddViewParam(pParam);

	delete pParam;
#endif

	// ����{�^��������
	CMenu*  pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	return TRUE;

//	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

/*============================================================================*/
/*! �q�E�B���h�E

-# �r���[�Ƀp�����[�^��ǉ�����

@param	pParam		�f�[�^�p�����[�^
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

			tmpVScale.Auto = TRUE;	// ��͂Ȃ�AAUTO�c�X�P�[���Œ�

			if( nView == eViewID_1 )
				ret = ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendAllTblNo(m_nID));
			else
				ret = ((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->AddAxis(tmpVScale, (HScale)theApp.GetDataCollection().GetTrendZoomTblNo(m_nID));
		}

		CString	strName;

		// �e�f�[�^�̃p�����[�^��ǉ�
		for (int i = 0; i < MAX_TDATA; ++i)
//		for (int i = 0; i < 1; ++i)
		{
			VScaleKind	tmpVScaleKind = pParam->item[i].vscaleKind;

			if( pParam->isRealTime )
				tmpVScaleKind = pParam->item[i].vscaleKind;
			else
				tmpVScaleKind = LScale;	// ��͂Ȃ�A���X�P�[���Œ�

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
				// �������Ȃ�
				break;
			}
		}

		// �O���t�X�V
		int nViewNo = ((CMainFrame*)(theApp.m_pMainWnd))->FindChildListPos(this);

		// �Y�[���^�J�[�\����������
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

		((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->UpdateGraphWindow(nView, true/*Min/Max���N���A*/);

//		SetActiveView(((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0)));
	}

	return ret;
}

#if 1
/*============================================================================*/
/*! �q�E�B���h�E

-# �r���[�ɊĎ��f�[�^�̍X�V��ʒm����

@param	�Ȃ�
@retval	�Ȃ�

*/
/*============================================================================*/
void CChildFrame::UpdateDataCollection(bool bClear)
{
	for (int nView = 0; nView < eViewID_Num; ++nView)
//	for (int nView = 0; nView < 1; ++nView)
	{
		int nViewNo = ((CMainFrame*)(theApp.m_pMainWnd))->FindChildListPos(this);

		// �O���t�X�V
		((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0))->UpdateGraphWindow(nView, bClear);
//		SetActiveView(((CRemoteGraphView*)m_wndSplitter.GetPane(nView, 0)));
	}
}
#endif

/*============================================================================*/
/*! �q�E�B���h�E

-# Min�^Max�l���擾

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
	// ����{�^�������Ȃ�
	if (nID == SC_CLOSE)
		return;

	CMDIChildWndEx::OnSysCommand(nID, lParam);
}
