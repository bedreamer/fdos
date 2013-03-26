	BS_OEMName	    DB 'Master  '
	BPB_BytsPerSec	DW 0x200
	BPB_SecPerClus	DB 0x01
	BPB_RsvdSecCnt	DW 0x01
	BPB_NumFATs	    DB 0x02
	BPB_RootEntCnt	DW 224
	BPB_TotSec16	DW 2880
	BPB_Media		DB 0xF0
	BPB_FATSz16	    DW 9
	BPB_SecPerTrk	DW 18
	BPB_NumHeads	DW 2
	BPB_HiddSec	    DD 0
	BPB_TotSec32	DD 0
	BS_DrvNum		DB 0
	BS_Reserved1	DB 0
	BS_BootSig	    DB 29h
	BS_VolID		DD 0
	BS_VolLab		DB 'Master     '
	BS_FileSysType	DB 'FAT12   '
