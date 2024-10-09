
//---------------------------------------------
// �H�w�l�ϊ��p�N���X
//---------------------------------------------
#pragma once
#include <tchar.h>
#include <vector>
#include <map>
#include <deque>
#include <WinSock2.h>

using namespace std;

/* ### �o�c�t��� ### */
#define DATA_PDU         0x010030b0      /* �f�[�^�o�c�t */

#define VEQU_ID   0xf


#define TLMDATAINFSIZE (12)      /*�u���t�B�[���h�v�̃o�C�g��*/
#define TLMDATAHEDSIZE (12+64)   /*�u���t�B�[���h�v�{�u�t�����v�̃o�C�g��*/

#define  SHMFLG          0666
#define  SIZEINF_SIZE    8
#define  EOFINF_SIZE     4

#define  LOG             0x0001
#define  START_LOG       2801
#define  END_LOG         2802
#define  FILESIZEERR_LOG 2803

#define  VMAX            1024


#define    U_LIB_PRIVATE  uc_valchk

/*
���[�J�� define�錾��
*/
#define   LX         0x5830                /* 0X */
#define   SX         0x7830                /* 0x */
#define   LB         0x4230                /* 0B */
#define   SB         0x6230                /* 0b */
#define   LT         0x5430                /* 0T */
#define   ST         0x7430                /* 0t */

/* DB���r�b�N�G���f�B�A�����ۂ� 1:�r�b�N 0:���g�� */
#define IS_BIG_ENDIA 0

#define EQCTL_DB_SHM_ID		1	/* �ݔ�������f�[�^�x�[�X */
#define EQMON_DB_SHM_ID		2	/* �ݔ�������f�[�^�x�[�X */
#define EQSAT_DB_SHM_ID		3	/* �q���ŗL���f�[�^�x�[�X */
#define EQDSP_DB_SHM_ID		4	/* ��ʏ���`�f�[�^�x�[�X */
#define EQINF_DB_SHM_ID		5	/* �ݔ�����`�f�[�^�x�[�X */
#define EQINI_DB_SHM_ID		6	/* �ݔ����������f�[�^�x�[�X */

/*
�ݔ�������c�a
*/

#define	CTLNAME_LEN	28	/* �ݔ����䖼�̍ő咷+1 *//* [Ver1.02] 20->28byte */
#define	EQNAME_LEN	12	/* �ݔ����̍ő咷+1 */
#define	CTLID_LEN	( CTLNAME_LEN + EQNAME_LEN )	/* ���䍀��ID�ő咷+1 */
#define	CHKVALDBL_NUM	2	/* ���ؒl�F�����^�̏ꍇ�̔z�� */
#define	CHKVALSTR_LEN	( CHKVALDBL_NUM * ( sizeof(double) / sizeof(char)  ))
//#define CHKVALSTR_LEN   ( 6 * CHKVALDBL_NUM * ( sizeof(double) /sizeof(char)  ))

#define	OBSNAME_ID		0x0101		/* ������`�h�c */
#define	IF_ID			0x0102		/* ������`�h�c */
#define	ELSE_ID			0x010d		/* ������`�h�c */
#define	ELSEIF_ID		0x010e		/* ������`�h�c */
#define	ENDIF_ID		0x010f		/* ������`�h�c */
#define	EXT_ID			0x0103		/* �o�C�i�����o�i���������j�h�c */
#define	EXTS_ID			0x0104		/* �o�C�i�����o�i�����L��j�h�c */
#define	EXTBCD_ID		0x0105		/* �a�b�c���o�i���������j�h�c */
#define	EXTBCDS_ID		0x0106		/* �a�b�c���o�i�����L��j�h�c */
#define	EXTAVG_ID		0x0107		/* ���o�f�[�^���ρi���������j�h�c */
#define	EXTAVGS_ID		0x0108		/* ���o�f�[�^���ρi�����L��j�h�c */
#define	EXTCHAR_ID		0x0110		/* �����񒊏o�h�c */
#define	EXTCHARDEC_ID	0x0113		/* ������^���l���o�h�c */
#define	EXTDBL_ID		0x0111		/* double���o�h�c */
#define	EXTFLT_ID		0x0114		/* float���o�h�c */
#define POL_ID			0x0109		/* �������ϊ���`�h�c */
#define CARVTBL_ID		0x010a		/* �J�[�u�ϊ���`�h�c */
#define TIMECVT_ID		0x010b		/* �����ϊ���`�h�c */
#define OBSSTS_ID		0x010c		/* �X�e�[�^�X��`�h�c */
#define CAL_ID			0x0112		/* �����ڌv�Z��`�h�c */

#define _GREAT_ENGVAL
#ifdef _GREAT_ENGVAL
/* GREAT �H�w�l�ϊ��̉��C */
#define LOG10_ID		0x0115		/* ��p�ΐ�(log10)�ϊ���`�h�c */
#define EXP10_ID		0x0116		/* ��p�ΐ�(log10)�t�ϊ���`�h�c */
#define ARCTAN_ID		0x0117		/* �t����(arctan)�ϊ���`�h�c */
#endif /* _GREAT_ENGVAL */

#define	EQMON_INF_ID	0x0121		/* "inf":�t������`���Ӗ�����h�c */
#define ENDOFOBS_ID 	-1			/* �X�̃R�}���h�̒�`�̏I���h�c */
#define L_OFFSET        6

#define CARV_MAX        32          /* �J�[�u�e�[�u���̍ő吔 */

#define ON         1     /*���[�h(�L��)*/
#define OFF        0     /*���[�h(�Ȃ�)*/
#define OK         0

#define iNULL 0  /* integer��NULL: NULL�̓|�C���^�^�Ȃ̂Œ�`*/ 

#define OBS_MAX    8192
#define MAX_SDTP  (32*1024)
#define STR_BUF_SIZE 256

#define YYYY_MAX 2069 /* 2038�N�������̍ő�l:���̏I��� -> 2069�N�ɕύX */

#define LOW3BIT  0x0007  /* ����3�ޯĎ�肾�� */

#define PCTIME_SPAN100		(60 * 60 * 24 * 100)	// PC���Ԃ̍���100��
#define PCTIME_SPAN365		(60 * 60 * 24 * 365)	// PC���Ԃ̍���365��

enum{
	eTimeDirect,
	eTimeRange,
};

#pragma pack(1)

#ifdef _ORG
struct TL_TLMDATA {  /*�u���t�B�[���h�{�t�����{�e�����g���t���[���v */
	/* ���t�B�[���h */
	unsigned char uc_recid;         /* ���R�[�h�h�c %1 */
	unsigned char uca_dataid[7];    /* �f�[�^�h�c
									�ȉ��̂Q�o�C�g��ue_tlmmsgsnd
									->ue_msgrcv�Őݒ肵�Ă���B
									dataid[0]:CPN�T�[�r�X��:=0x08
									dataid[1]:VCID�l
									*/
	long          l_reclen;         /* �f�[�^�t�B�[���h�̃f�[�^���A�܂�A
									�u�t�����v�̃o�C�g���{�e�����g����M
									�f�[�^�̃o�C�g�� %1 */
	/* �t����� */
	unsigned char uc_ver;           /* ���u�Ǘ��ԍ��E�o�[�W�����ԍ� %3 */
	unsigned char uc_scno;          /* �q���ԍ� %3 */

	unsigned char uca_bcdyyyy[2];   /*BCD�t�H�[�}�b�g�̔N(4��):1994..2001..%2*/
	unsigned char uca_bcddate[2];   /*BCD�t�H�[�}�b�g�̒ʎZ��:0001..0366 %2*/
	unsigned char uca_bcdtime[5];   /*BCD�t�H�[�}�b�g�̎���  :HHMMSSmmmu %2*/
	unsigned char uc_bcdtime_uu;    /*BCD�t�H�[�}�b�g�̎���  :uu %2*/

	unsigned char uc_bnd;           /* ��M�o���h�� %3 */
	unsigned char uc_fsts1;         /* �������u�X�e�[�^�X�i���̂P�j%1 */
	unsigned char uc_fsts2;         /* �������u�X�e�[�^�X�i���̂Q�j%1 */
	unsigned char uc_locksts;       /* ���b�N�X�e�[�^�X %1 */
	unsigned char uc_bitrate;       /* �r�b�g���[�g %2 */
	unsigned char uca_psno[5];      /* �p�X�ԍ� (BCD�t�H�[�}�b�g) %3 */
	unsigned char uca_psno_yy;      /* �p�X�ԍ� (BCD�t�H�[�}�b�g):
									00:�]���`���p�X�ԍ��ł��邱�Ƃ�����
									19..20:�N�̏�ʂQ�� %3 */
	unsigned char uc_antid;         /* ��M�A���e�i�h�c %3 */
	unsigned char uca_space[40];    /* �\�� */

	/* �e�����g����M�f�[�^�i�ő�2048�o�C�g�j*/
	// �ȉ��̔z��Ǝ�荇����100k�m�ۂ���
	unsigned char uca_tlmfrm[1024 * 100];
	//unsigned char uca_tlmfrm[1024 * 2];
	/*�q����M�f�[�^�i�e�����g���f�[�^�j
	���ӁF���̕����͉q�����̉ϒ��f�[�^�ł��B�b�R���p�C���̓s����
	�ϒ��f�[�^�̒�`���o���Ȃ��̂ŃT�C�Y2048�o�C�g�ɂȂ���
	���܂����A���ۂ́A128,512,2048�o�C�g���ɂȂ�܂��B
	�il_reclen����u�t�����v�̃o�C�g���������������ɂȂ�܂��B�j

	�i���������āAtlm1rec_t/TL_TLMDATA�`���̃f�[�^��z��\����
	����ꍇ�ɂb����̃R�[�f�B���O��͔z��̎Q�ƕ��@�ŃA�N�Z�X
	����̂͊ԈႢ�ł��i���̊ԈႢ�̓R���p�C�����ɂ��G���[��
	�Ȃ�Ȃ��j�̂Œ��ӂ��ĉ������j
	*/
	/*
	���ݒ茳�F
	%1:�e�����g�����̓h���C�o(/dev/tlmdem)+HW����̓��̓f�[�^���̂���
	%2:�e�����g����荞�݃v���Z�X(tirtproc)
	%3:�e�����g�����͕��z�v���Z�X(tiinproc)
	%4:SDTP�T�[�o�v���Z�X(sdsvproc)
	*/
};
#else
typedef struct { // ���t�B�[���h
	unsigned char uc_recid;			// ���R�[�h�h�c
	unsigned char uca_dataid[7];	// �f�[�^�h�c
	/* �ȉ��̂Q�o�C�g��ue_tlmmsgsnd
	->ue_msgrcv�Őݒ肵�Ă���B
	dataid[0]:CPN�T�[�r�X��:=0x08
	dataid[1]:VCID�l
	*/
	long          l_reclen;			// �f�[�^�t�B�[���h�̃f�[�^��
	/*
	�u�t�����v�̃o�C�g���{�e�����g����M�f�[�^�̃o�C�g��
	*/
} xTL_INFO;

typedef struct { // �t�����
	unsigned char uc_ver;			// ���u�Ǘ��ԍ��E�o�[�W�����ԍ� %3
	unsigned char uc_scno;			// �q���ԍ� %3

	unsigned char uca_bcdyyyy[2];	// BCD�t�H�[�}�b�g�̔N(4��):1994..2001..%2
	unsigned char uca_bcddate[2];	// BCD�t�H�[�}�b�g�̒ʎZ��:0001..0366 %2
	unsigned char uca_bcdtime[5];	// BCD�t�H�[�}�b�g�̎���  :HHMMSSmmmu %2
	unsigned char uc_bcdtime_uu;	// BCD�t�H�[�}�b�g�̎���  :uu %2

	unsigned char uc_bnd;			// ��M�o���h�� %3
	unsigned char uc_fsts1;			// �������u�X�e�[�^�X�i���̂P�j%1
	unsigned char uc_fsts2;			// �������u�X�e�[�^�X�i���̂Q�j%1
	unsigned char uc_locksts;		// ���b�N�X�e�[�^�X %1
	unsigned char uc_bitrate;		// �r�b�g���[�g %2
	unsigned char uca_psno[5];		// �p�X�ԍ� (BCD�t�H�[�}�b�g) %3
	unsigned char uca_psno_yy;		// �p�X�ԍ� (BCD�t�H�[�}�b�g)
	/*
	00:�]���`���p�X�ԍ��ł��邱�Ƃ�����
	19..20:�N�̏�ʂQ�� %3 */
	unsigned char uc_antid;			// ��M�A���e�i�h�c %3

	//unsigned char uca_space[40];	// �\��
	unsigned char uca_space[22];	// �\��
	unsigned char uc_op_rec_id;		// ���t�B�[���h�E���R�[�h����
	unsigned char uca_op_data_id[7]; // ���t�B�[���h�E�f�[�^����
	unsigned char uca_op_rec_len[4]; // �ǉ^�p�F���t�B�[���h�E���R�[�h��
	unsigned char uca_timetag[6];	// �^�C���^�O: 0D DD HH MM  SS ss
} xTL_ADDINFO;

typedef struct { // �e�����g����M�f�[�^�i�ρj
	unsigned char uca_tlmfrm[1];
	/*�q����M�f�[�^�i�e�����g���f�[�^�j
	���ӁF���̕����͉q�����̉ϒ��f�[�^�ł��B�b�R���p�C���̓s����
	�ϒ��f�[�^�̒�`���o���Ȃ��̂ŃT�C�Y2048�o�C�g�ɂȂ���
	���܂����A���ۂ́A128,512,2048�o�C�g���ɂȂ�܂��B
	�il_reclen����u�t�����v�̃o�C�g���������������ɂȂ�܂��B�j

	�i���������āAtlm1rec_t/TL_TLMDATA�`���̃f�[�^��z��\����
	����ꍇ�ɂb����̃R�[�f�B���O��͔z��̎Q�ƕ��@�ŃA�N�Z�X
	����̂͊ԈႢ�ł��i���̊ԈႢ�̓R���p�C�����ɂ��G���[��
	�Ȃ�Ȃ��j�̂Œ��ӂ��ĉ������j
	*/
	/*
	���ݒ茳�F
	%1:�e�����g�����̓h���C�o(/dev/tlmdem)+HW����̓��̓f�[�^���̂���
	%2:�e�����g����荞�݃v���Z�X(tirtproc)
	%3:�e�����g�����͕��z�v���Z�X(tiinproc)
	%4:SDTP�T�[�o�v���Z�X(sdsvproc)
	*/
} xTL_RECDATA;

typedef struct {  /*�u���t�B�[���h�{�t�����{�e�����g���t���[���v */
	xTL_INFO		info;		// ���t�B�[���h
	xTL_ADDINFO	addinfo;	// �t�����
	xTL_RECDATA	recvdata;	// �e�����g����M�f�[�^�i�ρj
} xTL_TLMDATA;

typedef struct {
	BYTE	bMode;		// eTimeDirect:���t�̒��ڎw��	eTimeRange�F�����͈͎̔w��
	WORD	wYear;		// eTimeDirect���̔N�i1970-3000�j
	WORD	wMonth;		// eTimeDirect���̌��i0-12 0�̏ꍇwYear��1�`12���Ώہj
	WORD	wDay;		// eTimeDirect���̓��i0-31 0�̏ꍇwMonth��1�`31���Ώہj
	time_t	sttime;		// �J�n����
	time_t	sptime;		// �I������
} xTL_TIME;
#endif

/*
*******************************************************************************
* �ݔ��Ď����̏��
*******************************************************************************
*/
struct xobsname_t {	/* ���̃e�[�u���͑S�Ď����ڂɂ��đ��݂��� */
	long   l_id;	/* �f�[�^����: �萔=OBSNAME_ID�l */
	long   l_len;	/* �f�[�^��  : sizeof( struct obsname_t ) */
	long   l_no;	/* �Ď����ڔԍ��i�R���p�C�����V�[�P���V�����ɕt������B�j */
	char   sz_obsname[CTLID_LEN]; 	/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
	long   l_inflg; /* �f�[�^�L���t���O 1=�L��  ���P */
	long   l_grmd;  /* �X�e�[�^�X�t���O 1=�L��,2=���������� ���P */
	long   l_data;  /* �ϊ����ʁi�����j         ���P */
	double d_data;  /* �ϊ����ʁi�����j         ���P */
	char   sz_sts_name[64]; /* �ϊ����ʃX�e�[�^�X������ */

	/* �ȉ���64bit�������x�Ή��g���̈� */

	long double ld_data;/* �H�w�l�f�[�^64bit�������i�[�ł��鐸�x ���P */

	long   l_bitinfo;   /* �����r�b�g��:�������o�f�[�^�̃r�b�g��:1..64
						==0:�������o�ł͂Ȃ�   ���P */
	long   l__rfu__;    /* ���g�p�Fpadding�o�C�g */

	long   l_cnvResType;	/* �ϊ����ʃ^�C�v�i�r�b�g���сj */
};

/*
*******************************************************************************
* �X�e�[�^�X�ƍ���`
*******************************************************************************
*/
typedef struct xchksts_tbl_t {	/* �X�e�[�^�X�ƍ���`�e�[�u�� */
	long l_val;	/* ���ڒl */
	long l_rsv;	/* �T�C�Y���� */
	char sz_sts[CHKVALSTR_LEN];/* �X�e�[�^�X������ */
} xC_STS;

/*
*******************************************************************************
* �X�e�[�^�X���
*******************************************************************************
*/
struct xobssts_tbl_t {	/* �X�e�[�^�X���e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=OBSSTS_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct obssts_tbl_t ) */
	long l_sts_cnt; /* �X�e�[�^�X���ڐ� */
	long l_rsv;	/* �T�C�Y���� */
	char sz_i_sts[CHKVALSTR_LEN];/* �����l�X�e�[�^�X������ */
	xchksts_tbl_t chk_t[32]; /* �X�e�[�^�X�ϊ����� */
};

/*
*******************************************************************************
* ���o��`���
*******************************************************************************
*/
typedef struct xpwb_tbl_t {	/* ���o��`�e�[�u����ɑ��݂���B */
	long l_pid;	/* ���u�ԍ� */
	long l_word;	/* ���[�h�ʒu */
	long l_s_bit;  /* �J�n�r�b�g�ʒu */
	long l_e_bit;  /* �I���r�b�g�ʒu */
} xPWB_T;

/*
*******************************************************************************
* �e���o��`���
*******************************************************************************
*/
struct xext_tbl_t{	/* �e���o��`��� */
	long l_id;	/* �f�[�^����:
				EXT_ID or EXTS_ID or
				EXTBCD_ID or EXTBCDS_ID or
				EXTAVG_ID or EXTAVGS_ID or
				EXTCHAR_ID or EXTCHARDEC or
				EXTDBL or EXTFLT */
	long l_len;	/* �f�[�^��  : sizeof( struct ext_tbl_t ) */
	long l_pw_cnt;	/* ���o��`�� */
	long l_rsv;	/* �T�C�Y���� */
	xpwb_tbl_t s_pwb[10];/* ���o��`��� */
};

/*
*******************************************************************************
* ������`���(if)
*******************************************************************************
*/
struct xif_tbl_t {	/* ������`�e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=IF_ID , CATDATA_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct if_tbl_t ) */
	char sz_obsname[CTLID_LEN]; 	/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
	long l_flag;   /* ������ 0:'=' 1:'!' 2:'<' 3:'>' CATDATA_ID�̎��͖��� */
	long l_offset;	/* �Ď����ڃI�t�Z�b�g */
	double d_val;  /* �l */
};

/*
*******************************************************************************
* �����ڌv�Z���
*******************************************************************************
*/
struct xcal_tbl_t {	/* �����ڌv�Z��`�e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=CAL_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct cal_tbl_t ) */
	long l_flag;	/* ���Z�� 0:'+' 1:'-' 2:'*' 3:'/' */
	long l_offset;	/* �Ď����ڃI�t�Z�b�g */
	char sz_obsname[CTLID_LEN];/* �Ď����ڂh�c: �ݔ���.�Ď����ږ� */
};

/*
*******************************************************************************
* ��������`���
*******************************************************************************
*/
struct xpol_tbl_t {	/* ��������`��`�e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=pol_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct pol_tbl_t ) */
	long l_val_cnt; /* d_val�̌� */
	long l_rsv;	/* �T�C�Y���� */
	double d_val[5];/* �l */
};

/*
*******************************************************************************
* �J�[�u��`���
*******************************************************************************
*/
struct xcarv_tbl_t {	/* �J�[�u��`�e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=CARV_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct carv_tbl_t ) */
	long l_carv_cnt; /* d_carv�̌� */
	long l_rsv;	/* �T�C�Y���� */
	double d_carv[CARV_MAX][2]; /* AGC�Z���_�e�[�u��[][1] */
};

/*
*******************************************************************************
* �����ϊ����
*******************************************************************************
*/
struct xtimecvt_tbl_t {	/* �����ϊ�����`�e�[�u�� */
	long l_id;	/* �f�[�^����: �萔=TIMECVT_ID�l */
	long l_len;	/* �f�[�^��  : sizeof( struct timecvt_tbl_t ) */
	long l_cvtno;  /* �ϊ��ԍ� */
	long l_rsv;	/* �T�C�Y���� */
};

struct  xsdtp_msg_t {               /* sdtp ���b�Z�[�W�e�[�u��*/
	long long   l_unix_msgtype;    /* ���b�Z�[�W�^�C�v(64) (8 byte -> 16 byte)*/
	long        l_len;             /* ���M�o�C�g�� */
	long        l_objtype;         /* �v���h�c */
	long        l_blkcnt;          /* �u���b�N�J�E���g */
	long        l_send_ipc;        /* ���M�����b�Z�[�W�L���[�h�c */
	long        l_recv_ipc;        /* ��M�����b�Z�[�W�L���[�h�c */
	unsigned char uca_SdtpDummy[26];/* ��M�����b�Z�[�W�L���[�h�c */
	unsigned char uca_SdtpPDULen[4];/* PDU�� */
	unsigned char uca_SdtpRecNum[2];/* ���R�[�h�� */
	unsigned char uca_buff[MAX_SDTP];/* �f�[�^�o�b�t�@ */
};

struct  xsdtp_msg_ex_t {               /* sdtp ���b�Z�[�W�e�[�u�� 2017�N�� */
	unsigned char uca_SdtpDummy[26];/* ��M�����b�Z�[�W�L���[�h�c */
	unsigned char uca_SdtpPDULen[4];/* PDU�� */
	unsigned char uca_SdtpRecNum[2];/* ���R�[�h�� */
	unsigned char uca_buff[MAX_SDTP];/* �f�[�^�o�b�t�@ */
};

union   xumoji {                         /* ������擪2�����̃o�b�t�@ */
	char   c_moji[2];
	short  s_moji;
};


struct xobs_tbl_t {
	struct xobsname_t *obs_adr;     /* �Ď����ڒ�`��� */
	long             l_dataid;     /* ���o�f�[�^�h�c */
	long             l_offset;     /* �Ď����ڂ̃I�t�Z�b�g */
	long             l_grp_cnt;    /* �g���F�O���[�v�J�E���g */
};


/*
*******************************************************************************
* �H�w�l�ϊ�����
*******************************************************************************
*/
struct xobs_tbl_res_t {
	struct xobsname_t obs_adr;      /* �Ď����ڒ�`��� */
	long             l_dataid;     /* ���o�f�[�^�h�c */
	long             l_offset;     /* �Ď����ڂ̃I�t�Z�b�g */
	long double      ld_time_val;  /* �g���F���Ԓl */
	long             l_grp_cnt;    /* �g���F�O���[�v�J�E���g */
};

/*
*******************************************************************************
* �H�w�l�ϊ����ʂ̊i�[�^�C�v
*******************************************************************************
*/
enum eCnvResType
{
	eCnvResType_Long = 0,				/* �r�b�g0: long */
	eCnvResType_Double,					/* �r�b�g1: double */
	eCnvResType_LongDouble,				/* �r�b�g2: long double */
	eCnvResType_StatusStr,				/* �r�b�g3: �X�e�[�^�X������ */
	eCnvResType_TimeStr,				/* �r�b�g4: ���������� */
	eCnvResType_TimeLeapSec,			/* �r�b�g5: ���邤�b */

	eCnvResType_Max,
};
#pragma pack()


#pragma warning(disable:4251)

typedef void (CALLBACK *NOTIFYDATACALLBACK)(TCHAR* monName, time_t monTime, xobs_tbl_res_t* monData, bool bLeapSec);

class CEngValCnvDll
{
public:
	CEngValCnvDll(char* db_file_name);
	CEngValCnvDll();
	~CEngValCnvDll();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	char		mEqMonDBFileName[_MAX_PATH];
	TCHAR		mMonDataPathName[_MAX_PATH];
	CString		mDbName;
	map< CString, vector<xobs_tbl_res_t> >	mOutDataList;			// �H�w�l�ϊ��p�Ď����}�b�v
	vector<CString>							mMonDataFileList;		// �H�w�l�ϊ��Ώۂ̃t�@�C�����X�g
	std::vector<xobs_tbl_res_t> data_list;

protected:

	long g_l_ub_dtime_timezoneflg = 0; /*1:timezone�͗L��*/
	int db_status; /* read��� */

	int mFileSize;
	char *db_data;// [BUFF_SIZE];
	char *db_data2;// [BUFF_SIZE];
	struct xobs_tbl_t obs_tbl[OBS_MAX];

	long m_lastCnvAfterType;

	map<int, vector<xobs_tbl_t>> obs_map;

	map<CString, long double>	mMonTimeLDData;			// �ݔ����Ƃ�MON_TIME�̎���

	NOTIFYDATACALLBACK	mCallBack;

	/* ------------------------------------------------------------------------------------ */
	/*	�����o�֐�
	/* ------------------------------------------------------------------------------------ */
public:
	void SetEqMonDbFileName(TCHAR* val);
	void SetMonDataPath(TCHAR* val);
	bool GetMonitorData(xTL_TIME& timeTarget, map< CString, vector<xobs_tbl_res_t> >& outDataList);
	bool GetMonitorDataEx(xTL_TIME* timeTarget);
	void ClearMonitorName();
	void AddMonitorName(TCHAR* name);
	void SetCallBack(NOTIFYDATACALLBACK fCallBack) { mCallBack = fCallBack; }

	map< CString, vector<xobs_tbl_res_t> >&	GetOutDataList() { return mOutDataList; }
	vector<CString>&						GetMonDataFileList() { return mMonDataFileList; }
	void GetDataFileList(CString& path, vector<CString>& outFileList);
	void GetTimeRangePath(time_t sttime, time_t sptime, vector<CString>& fileList);
	void ClearDataFileList();
	void AddDataFileList(TCHAR* val);
protected:
	bool ConvertDirectRangeTime(xTL_TIME& dtime, xTL_TIME& rtime, bool timezero=true);
	bool ConvertStatusChar(char* stsname, int len);
	CStringA UTF8_3CharsToSJis(char *pUtf8Str);
	CStringA EUCToSJis(char *pEucStr);
	bool IsObsName(char* obsname, map< CString, vector<xobs_tbl_res_t> >& outDataList);

	long double GetAndUpdateMonTime(CString strWatch, long double fTimeVal = 0.0)
	{
		long double	res_time = 0.0;

		if( mMonTimeLDData.count(strWatch) != 0 )
		{
			if( fTimeVal != 0.0 )
				mMonTimeLDData[strWatch] = fTimeVal;
			res_time = mMonTimeLDData[strWatch];
		}
		else
			mMonTimeLDData.insert(pair<CString, long double>(strWatch, fTimeVal));

		return res_time;
	}

public:
	int  CalcFromFileEx(char *cp_loadadr_p, map< CString, vector<xobs_tbl_res_t> >& outDataList);
	int  CalcFromFile(char *cp_loadadr_p);
	int  CalcFromMsg(xsdtp_msg_ex_t* msg);
	//vector<obs_tbl_res_t>& GetAllVal() { return data_list; }
	vector<xobs_tbl_res_t>& GetAllVal() { return data_list; }

	long GetCnvResType()	{ return m_lastCnvAfterType; }
	static BOOL IsCnvResLong(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_Long)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResDouble(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_Double)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResLongDouble(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_LongDouble)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResStatusStr(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_StatusStr)) != 0)
			return TRUE;
		else
			return FALSE;
	}
	static BOOL IsCnvResTimeStr(struct xobsname_t& obs_adr)
	{
		if ((obs_adr.l_cnvResType & (1 << eCnvResType_TimeStr)) != 0)
			return TRUE;
		else
			return FALSE;
	}

protected:
	int DbRead(char* db_file_nam, char *cp_loadadr_p);
	void* MonRead(FILE **fp);

	int ud_montbl_data64(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[], long double *ldp_data, long *lp_bitinfo);
	void ua_ldbitfieldll(unsigned long long *pull_outbuf, unsigned char inputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg);
	void ua_ldbitfield(unsigned long *pul_outbuf, unsigned char inputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen, long l_signflg);
	void ua_insbitfield(unsigned char  *tgtadr, long l_inspos, long l_inslen, char  *suadr, long l_extpos);
	void ub_bcdext(double *d_data, unsigned char ca_data[], long l_word, long l_bit, long l_len);
	int ud_montbl_data(char *shmadr, int l_dataid, unsigned char ca_data[], int l_offset, int *l_inflg, int *l_grmd, double *d_data, char sz_sts_name[]);
	void ub_dtime_to_asc(double d_uttime, char *sz_time);
	void ub_bcd6_to_bcd(unsigned char achBCD6[], unsigned char achYYYY[], unsigned char achTime[]);
	double ub_bcd_to_dtime(unsigned char uca_yyyy[], unsigned char uca_time[]);
	double ua_ddd_to_dtime(char *cap_ddd, char *cap_hh, char *cap_mm, char *cap_ss);
	double ub_asc_to_dtime(char *sz_time);
	void ua_stbitfield(unsigned long ul_input, unsigned char outputbuf[], long l_wordno, unsigned long ul_bitno, unsigned long ul_bitlen);
	int ud_ext_adr(char sz_watch[], xobsname_t *obs_adr, xext_tbl_t *ext_adr[]);
	double dswap(double d);
	double dswap2(double d);
	float fswap(float d);
	long ds_htonl(long val);
	int uc_valchk(char *s, int *i_p, double *d_p, int l_opt);
	int uc_valchkll(char *s, long *i_p, long double *d_p, long l_opt);
	int ud_sts_flg(char sz_watch[], char *sz_path, char *shmadr, long l_n, int *l_grmd, long *l_offset, char *sz_sts_tbl[]);
	int ud_obsname_adr(char sz_watch[], char *shmadr, struct xobsname_t *obs_adr[]);
	int CreateObsTbl();

};
