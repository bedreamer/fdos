;������ FAT12 ���̵�ͷ
	BS_OEMName	DB 'Master  '	; OEM String, ���� 8 ���ֽ�
	BPB_BytsPerSec	DW 0x200		; ÿ�����ֽ���
	BPB_SecPerClus	DB 0x01		; ÿ�ض�������
	BPB_RsvdSecCnt	DW 0x01		; Boot ��¼ռ�ö�������
	BPB_NumFATs	DB 0x02		; ���ж��� FAT ��
	BPB_RootEntCnt	DW 224		; ��Ŀ¼�ļ������ֵ
	BPB_TotSec16	DW 2880		; �߼���������
	BPB_Media		DB 0xF0		; ý��������
	BPB_FATSz16	DW 9			; ÿFAT������
	BPB_SecPerTrk	DW 18		; ÿ�ŵ�������
	BPB_NumHeads	DW 2			; ��ͷ��(����)
	BPB_HiddSec	DD 0			; ����������
	BPB_TotSec32	DD 0			; ��� wTotalSectorCount �� 0 �����ֵ��¼������
	BS_DrvNum		DB 0			; �ж� 13 ����������
	BS_Reserved1	DB 0			; δʹ��
	BS_BootSig	DB 29h		; ��չ������� (29h)
	BS_VolID		DD 0			; �����к�
	BS_VolLab		DB 'Master     '	; ���, ���� 11 ���ֽ�
	BS_FileSysType	DB 'FAT12   '	; �ļ�ϵͳ����, ���� 8���ֽ�
