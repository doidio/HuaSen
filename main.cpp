#include <QtGui/QApplication>
#include <QtGui>

#include "ReconstruMainWindow.h"
#include "DealTime.h"
#include "DlgPassword.h"
#include "DlgAbout.h"

#include <windows.h>

#include <iostream>
#include "md5.h"

using namespace std;


HKEY hKey;
char content[256];			//所查询注册表键值的内容
struct HKEY__*RootKey;		//注册表主键名称
TCHAR *SubKey;				//欲打开注册表项的地址
//TCHAR *KeyName;				//欲设置项的名字
TCHAR *ValueName;			//欲设置值的名称
LPBYTE SetContent_S;		//字符串类型
//int SetContent_D[256];		//DWORD类型
//BYTE SetContent_B[256];		//二进制类型
DWORD dwType=REG_SZ;		//定义读取数据类型
DWORD dwLength=256;

//extern "C" LPCWSTR FAR __declspec(dllimport) HDSerialNumRead(); 

//typedef LPCWSTR FAR(*HDSerialNumRead)(); 
//
//typedef struct {
//  UINT4 state[4];                                   /* state (ABCD) */
//  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
//  unsigned char buffer[64];                         /* input buffer */
//} MD5_CTX;
//
//typedef void (*MD5Update) (MD5_CTX *context,unsigned char *input,unsigned int inputLen);
//typedef void (*MD5Init) (MD5_CTX *context);
//typedef void (*MD5Final) (unsigned char digest[16],MD5_CTX *context);

//以下是其中用到的某些结构及函数的定义:

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ( ( FILE_DEVICE_SCSI << 16 ) + 0x0501 )

#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

#define  IDENTIFY_BUFFER_SIZE  512
#define  SENDIDLENGTH  ( sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE )

#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

typedef struct _IDSECTOR
{
    USHORT  wGenConfig;
    USHORT  wNumCyls;
    USHORT  wReserved;
    USHORT  wNumHeads;
    USHORT  wBytesPerTrack;
    USHORT  wBytesPerSector;
    USHORT  wSectorsPerTrack;
    USHORT  wVendorUnique[3];
    CHAR    sSerialNumber[20];
    USHORT  wBufferType;
    USHORT  wBufferSize;
    USHORT  wECCSize;
    CHAR    sFirmwareRev[8];
    CHAR    sModelNumber[40];
    USHORT  wMoreVendorUnique;
    USHORT  wDoubleWordIO;
    USHORT  wCapabilities;
    USHORT  wReserved1;
    USHORT  wPIOTiming;
    USHORT  wDMATiming;
    USHORT  wBS;
    USHORT  wNumCurrentCyls;
    USHORT  wNumCurrentHeads;
    USHORT  wNumCurrentSectorsPerTrack;
    ULONG   ulCurrentSectorCapacity;
    USHORT  wMultSectorStuff;
    ULONG   ulTotalAddressableSectors;
    USHORT  wSingleWordDMA;
    USHORT  wMultiWordDMA;
    BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

typedef struct _DRIVERSTATUS

{
    BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
    BYTE  bIDEStatus;    //  Contents of IDE Error register.
    //  Only valid when bDriverError is SMART_IDE_ERROR.
    BYTE  bReserved[2];  //  Reserved for future expansion.
    DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS
{
    DWORD         cBufferSize;   //  Size of bBuffer in bytes
    DRIVERSTATUS  DriverStatus;  //  Driver status structure.
    BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _SRB_IO_CONTROL
{
    ULONG HeaderLength;
    UCHAR Signature[8];
    ULONG Timeout;
    ULONG ControlCode;
    ULONG ReturnCode;
    ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _IDEREGS
{
    BYTE bFeaturesReg;       // Used for specifying SMART "commands".
    BYTE bSectorCountReg;    // IDE sector count register
    BYTE bSectorNumberReg;   // IDE sector number register
    BYTE bCylLowReg;         // IDE low order cylinder value
    BYTE bCylHighReg;        // IDE high order cylinder value
    BYTE bDriveHeadReg;      // IDE drive/head register
    BYTE bCommandReg;        // Actual IDE command.
    BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS
{
    DWORD     cBufferSize;   //  Buffer size in bytes
    IDEREGS   irDriveRegs;   //  Structure with drive register values.
    BYTE bDriveNumber;       //  Physical drive number to send 
    //  command to (0,1,2,3).
    BYTE bReserved[3];       //  Reserved for future expansion.
    DWORD     dwReserved[4]; //  For future use.
    BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _GETVERSIONOUTPARAMS
{
    BYTE bVersion;      // Binary driver version.
    BYTE bRevision;     // Binary driver revision.
    BYTE bReserved;     // Not used.
    BYTE bIDEDeviceMap; // Bit map of IDE devices.
    DWORD fCapabilities; // Bit mask of driver capabilities.
    DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//////////////////////////////////////////////////////////////////////

//结构定义 
typedef struct _UNICODE_STRING 
{ 
    USHORT  Length;//长度 
    USHORT  MaximumLength;//最大长度 
    PWSTR  Buffer;//缓存指针 
} UNICODE_STRING,*PUNICODE_STRING; 

typedef struct _OBJECT_ATTRIBUTES 
{ 
    ULONG Length;//长度 18h 
    HANDLE RootDirectory;//  00000000 
    PUNICODE_STRING ObjectName;//指向对象名的指针 
    ULONG Attributes;//对象属性00000040h 
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR，0 
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE，0 
} OBJECT_ATTRIBUTES; 
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES; 

//函数指针变量类型
typedef DWORD  (__stdcall *ZWOS )( PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES); 
typedef DWORD  (__stdcall *ZWMV )( HANDLE,HANDLE,PVOID,ULONG,ULONG,PLARGE_INTEGER,PSIZE_T,DWORD,ULONG,ULONG); 
typedef DWORD  (__stdcall *ZWUMV )( HANDLE,PVOID); 


//查看函数
int  ShowContent (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName)
{
	int i=0;	//操作结果：0==succeed
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,ReValueName,NULL,&dwType,(unsigned char *) content,&dwLength)!=ERROR_SUCCESS) // content
		{
//			AfxMessageBox("错误：无法查询有关的注册表信息");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
//		AfxMessageBox("错误：无法打开有关的hKEY");
		i=1;
	}
	return i;
}

//设置字符串值函数
int  SetValue_S (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,LPBYTE ReSetContent_S)
{
	int i=0;	//操作结果：0==succeed
	//int StrLength;
	//StrLength=CString(SetContent_S).GetLength();

	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_SZ,ReSetContent_S,QString(QLatin1String((char*) SetContent_S)).length())!=ERROR_SUCCESS)
		{
//			AfxMessageBox("错误：无法设置有关的注册表信息");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
//		AfxMessageBox("错误：无法查询有关的注册表信息");
		i=1;
	}
	return i;
}

BOOL WinNTHDSerialNumAsScsiRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
    BOOL bInfoLoaded = FALSE;
    
    for( int iController = 0; iController < 2; ++ iController )
    {
        HANDLE hScsiDriveIOCTL = 0;
        char   szDriveName[256];
        
        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        sprintf( szDriveName, "\\\\.\\Scsi%d:", iController );

        //  Windows NT, Windows 2000, any rights should do
        hScsiDriveIOCTL = CreateFileA( szDriveName,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, 0, NULL);

        // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
        //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
        //            controller, GetLastError ());
        
        if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE )
        {
            int iDrive = 0;
            for( iDrive = 0; iDrive < 2; ++ iDrive )
            {
                char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 };

                SRB_IO_CONTROL* p = ( SRB_IO_CONTROL* )szBuffer;
                SENDCMDINPARAMS* pin = ( SENDCMDINPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
                DWORD dwResult;

                p->HeaderLength = sizeof( SRB_IO_CONTROL );
                p->Timeout = 10000;
                p->Length = SENDIDLENGTH;
                p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
                strncpy( ( char* )p->Signature, "SCSIDISK", 8 );

                pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
                pin->bDriveNumber = iDrive;
                
                if( DeviceIoControl( hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
                    szBuffer,
                    sizeof( SRB_IO_CONTROL ) + sizeof( SENDCMDINPARAMS ) - 1,
                    szBuffer,
                    sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH,
                    &dwResult, NULL ) )
                {
                    SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) );
                    IDSECTOR* pId = ( IDSECTOR* )( pOut->bBuffer );
                    if( pId->sModelNumber[0] )
                    {
                        if( * puSerialLen + 20U <= uMaxSerialLen )
                        {
                            // 序列号
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 10, 20 );

                            // Cut off the trailing blanks
							UINT i;
                            for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
                            {}
                            * puSerialLen += i;

                            // 型号
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 27, 40 );
                            // Cut off the trailing blanks
                            for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )
                            {}
                            * puSerialLen += i;

                            bInfoLoaded = TRUE;
                        }
                        else
                        {
                            ::CloseHandle( hScsiDriveIOCTL );
                            return bInfoLoaded;
                        }
                    }
                }
            }
            ::CloseHandle( hScsiDriveIOCTL );
        }
    }
    return bInfoLoaded;
}

BOOL DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned )
{
    // Set up data structures for IDENTIFY command.
    pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE;
    pSCIP->irDriveRegs.bFeaturesReg     = 0;
    pSCIP->irDriveRegs.bSectorCountReg  = 1;
    pSCIP->irDriveRegs.bSectorNumberReg = 1;
    pSCIP->irDriveRegs.bCylLowReg       = 0;
    pSCIP->irDriveRegs.bCylHighReg      = 0;
    
    // calc the drive number.
    pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 );

    // The command can either be IDE identify or ATAPI identify.
    pSCIP->irDriveRegs.bCommandReg = bIDCmd;
    pSCIP->bDriveNumber = bDriveNum;
    pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
    
    return DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
        ( LPVOID ) pSCIP,
        sizeof( SENDCMDINPARAMS ) - 1,
        ( LPVOID ) pSCOP,
        sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1,
        lpcbBytesReturned, NULL );
}

BOOL WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen )
{
#define  DFP_GET_VERSION          0x00074080
    BOOL bInfoLoaded = FALSE;

    for( UINT uDrive = 0; uDrive < 4; ++ uDrive )
    {
        HANDLE hPhysicalDriveIOCTL = 0;

        //  Try to get a handle to PhysicalDrive IOCTL, report failure
        //  and exit if can't.
        char szDriveName [256];
        sprintf( szDriveName, "\\\\.\\PhysicalDrive%d", uDrive );

        //  Windows NT, Windows 2000, must have admin rights
        hPhysicalDriveIOCTL = CreateFileA( szDriveName,
            GENERIC_READ | GENERIC_WRITE, 
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, 0, NULL);

        if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE )
        {
            GETVERSIONOUTPARAMS VersionParams = { 0 };
            DWORD               cbBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
            if( DeviceIoControl( hPhysicalDriveIOCTL, DFP_GET_VERSION,
                NULL, 
                0,
                &VersionParams,
                sizeof( GETVERSIONOUTPARAMS ),
                &cbBytesReturned, NULL ) )
            {
                // If there is a IDE device at number "i" issue commands
                // to the device
                if( VersionParams.bIDEDeviceMap != 0 )
                {
                    BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
                    SENDCMDINPARAMS  scip = { 0 };

                    // Now, get the ID sector for all IDE devices in the system.
                    // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
                    // otherwise use the IDE_ATA_IDENTIFY command
                    bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
                    BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 };

                    if( DoIdentify( hPhysicalDriveIOCTL, 
                        &scip, 
                        ( PSENDCMDOUTPARAMS )&IdOutCmd, 
                        ( BYTE )bIDCmd,
                        ( BYTE )uDrive,
                        &cbBytesReturned ) )
                    {
                        if( * puSerialLen + 20U <= uMaxSerialLen )
                        {
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 10, 20 );  // 序列号

                            // Cut off the trailing blanks
							UINT i;
                            for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
                            * puSerialLen += i;

                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 27, 40 ); // 型号

                            // Cut off the trailing blanks
                            for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {}
                            * puSerialLen += i;

                            bInfoLoaded = TRUE;
                        }
                        else
                        {
                            ::CloseHandle( hPhysicalDriveIOCTL );
                            return bInfoLoaded;
                        }
                    }
                }
            }
            CloseHandle( hPhysicalDriveIOCTL );
        }
    }
    return bInfoLoaded;
}

//UINT FindAwardBios( BYTE** ppBiosAddr )
//{
//    BYTE* pBiosAddr = * ppBiosAddr + 0xEC71;
//
//    BYTE szBiosData[128];
//    CopyMemory( szBiosData, pBiosAddr, 127 );
//    szBiosData[127] = 0;
//    
//    int iLen = lstrlenA( ( char* )szBiosData );
//    if( iLen > 0 && iLen < 128 )
//    {
//        //AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00 
//        //Phoenix-Award: 03/12/2002-sis645-p4s333
//        if( szBiosData[2] == '/' && szBiosData[5] == '/' )
//        {
//            BYTE* p = szBiosData;
//            while( * p )
//            {
//                if( * p < ' ' || * p >= 127 )
//                {
//                    break;
//                }
//                ++ p;
//            }
//            if( * p == 0 )
//            {
//                * ppBiosAddr = pBiosAddr;
//                return ( UINT )iLen;
//            }
//        }
//    }
//    return 0;
//}
//
//UINT FindAmiBios( BYTE** ppBiosAddr )
//{
//    BYTE* pBiosAddr = * ppBiosAddr + 0xF478;
//    
//    BYTE szBiosData[128];
//    CopyMemory( szBiosData, pBiosAddr, 127 );
//    szBiosData[127] = 0;
//    
//    int iLen = lstrlenA( ( char* )szBiosData );
//    if( iLen > 0 && iLen < 128 )
//    {
//        // Example: "AMI: 51-2300-000000-00101111-030199-"
//        if( szBiosData[2] == '-' && szBiosData[7] == '-' )
//        {
//            BYTE* p = szBiosData;
//            while( * p )
//            {
//                if( * p < ' ' || * p >= 127 )
//                {
//                    break;
//                }
//                ++ p;
//            }
//            if( * p == 0 )
//            {
//                * ppBiosAddr = pBiosAddr;
//                return ( UINT )iLen;
//            }
//        }
//    }
//    return 0;
//}
//
//UINT FindPhoenixBios( BYTE** ppBiosAddr )
//{
//    UINT uOffset[3] = { 0x6577, 0x7196, 0x7550 };
//    for( UINT i = 0; i < 3; ++ i )
//    {
//        BYTE* pBiosAddr = * ppBiosAddr + uOffset[i];
//
//        BYTE szBiosData[128];
//        CopyMemory( szBiosData, pBiosAddr, 127 );
//        szBiosData[127] = 0;
//
//        int iLen = lstrlenA( ( char* )szBiosData );
//        if( iLen > 0 && iLen < 128 )
//        {
//            // Example: Phoenix "NITELT0.86B.0044.P11.9910111055"
//            if( szBiosData[7] == '.' && szBiosData[11] == '.' )
//            {
//                BYTE* p = szBiosData;
//                while( * p )
//                {
//                    if( * p < ' ' || * p >= 127 )
//                    {
//                        break;
//                    }
//                    ++ p;
//                }
//                if( * p == 0 )
//                {
//                    * ppBiosAddr = pBiosAddr;
//                    return ( UINT )iLen;
//                }
//            }
//        }
//    }
//    return 0;
//}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString locale = QLocale::system().name();

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + locale);
	a.installTranslator(&qtTranslator);

	QTranslator translator;
    translator.load(QString("huasen_") + locale);
    a.installTranslator(&translator);

	BYTE szSystemInfo[4096]; // 在程序执行完毕后，此处存储取得的系统特征码
	UINT uSystemInfoLen = 0; // 在程序执行完毕后，此处存储取得的系统特征码的长度

	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &ovi );

	if( ovi.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{	
		if( !WinNTHDSerialNumAsPhysicalRead( szSystemInfo, &uSystemInfoLen, 1024 ) )
		{
			WinNTHDSerialNumAsScsiRead( szSystemInfo, &uSystemInfoLen, 1024 );
		}
	}

	QString sApplyingID;
	for(   int   i=0;   i<uSystemInfoLen;   i++   )   
	{   
//		QString     strItem;   
//		strItem.sprintf("%X",   szSystemInfo[i]);   
//		str   +=   strItem;  
		sApplyingID   +=   szSystemInfo[i];
	}  

	sApplyingID   +="WASTON2014";//UEGCXJ2010 for the complete version; UEGCXJ2010LTD for the limited version;4 places for modified

	QByteArray ba = sApplyingID.toLatin1();
	const char *c_str2 = ba.data();
	sApplyingID=QString(QLatin1String(md5(c_str2).c_str()));

	RootKey=HKEY_CURRENT_USER;     //注册表主键名称
	SubKey=TEXT("Software");  //欲打开注册表值的地址
	ValueName=TEXT("WastonPassword");               //欲设置值的名称

	if ((ShowContent(RootKey,SubKey,ValueName))==0)
	{
		QString sPassword;
		//sPassword.sprintf("%s",content);
		sPassword=QString(QLatin1String(content));
		
		QString sPasswordFrHW;

		sApplyingID+="WASTON2014";
		ba = sApplyingID.toLatin1();
		c_str2 = ba.data();
		sPasswordFrHW=QString(QLatin1String(md5(c_str2).c_str())); 

		if(sPassword!=sPasswordFrHW)
		{
			QMessageBox::information(0, "Information","The software is not authorized,please contact Waston Medical Co.LTD!  http://www.wastonmed.com/");
			return true;
			exit(0);
		}
	}
	else
	{
		CDlgPassword *dialog=new CDlgPassword;
		//dialog->setParent(Qt::Dialog);

		dialog->ui.lineEdit_ApplyingID->setText(sApplyingID);

		dialog->show();

		if (dialog->exec() == QDialog::Accepted) 
		{
			QString sPassword=dialog->ui.lineEdit_Password->text();
			QString sPasswordFrHW;

			sApplyingID+="WASTON2014";
			ba = sApplyingID.toLatin1();
			c_str2 = ba.data();
			sPasswordFrHW=QString(QLatin1String(md5(c_str2).c_str())); 

			if(sPassword!=sPasswordFrHW)
			{
				QMessageBox::information(0, "Information","The software is not authorized,please contact Waston Medical Co.LTD!  http://www.wastonmed.com/");
				return true;
				exit(0);
			}
			else
			{
				ba = sPassword.toLatin1();
				c_str2 = ba.data();
				SetContent_S=LPBYTE(c_str2);
				SetValue_S(RootKey,SubKey,ValueName,SetContent_S);
			}
		}
		else
		{
			//QMessageBox::information(0, "Information","The software is not authorized,please contact Shanghai UEG Software Co.LTD! http://www.ueg.com.cn/");
			//return true;
			exit(0);
		}
		delete dialog;
	}

////anti-piracy
	//CDealTime tm = CDealTime::GetLocalTime();	//	

	//int year = tm.GetYear();
	//int month = tm.GetMonth();
	//int day = tm.GetDay();

	//QString sLocalDate;
	//sLocalDate.sprintf("%04u-%02u-%02u",year,month,day);

	//RootKey=HKEY_CURRENT_USER;     //注册表主键名称
	//SubKey=TEXT("Software\\Microsoft");  //欲打开注册表值的地址
	//ValueName=TEXT("Microsoft");               //欲设置值的名称

	//if ((ShowContent(RootKey,SubKey,ValueName))==0)
	//{
	//	QString sExpire;
	//	sExpire.sprintf("%s",content);

	//	if(sExpire=="1")
	//	{
	//		QMessageBox::information(0, "Information","The software is expired,please contact Shanghai UEG Software Co.LTD! http://www.ueg.com.cn/");
	//		return TRUE;
	//		exit(0);
	//	}

	//	if(sLocalDate>"2012-08-01")
	//	{
	//		SetContent_S=LPBYTE("1");
	//		SetValue_S(RootKey,SubKey,ValueName,SetContent_S);

	//		QMessageBox::information(0, "Information","The software is expired,please contact Shanghai UEG Software Co.LTD! http://www.ueg.com.cn/");
	//		return TRUE;
	//		exit(0);
	//	}
	//}
	//else
	//{
	//	SetContent_S=LPBYTE("0"); 
	//	
	//	if((SetValue_S(RootKey,SubKey,ValueName,SetContent_S))!=0)
	//	{
	//		QMessageBox::information(0, "Information","The software is expired,please contact Shanghai UEG Software Co.LTD! http://www.ueg.com.cn/");
	//		return TRUE;
	//		exit(0);
	//	}
	//}

	QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("SplashScreen.png"));
    splash->show();
    //Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    //splash->showMessage(QObject::tr("Setting up the main window..."),
    //                    topRight, Qt::white);
	QTime t; 
	t.start(); 
	while(t.elapsed()<1000) 
		QCoreApplication::processEvents(); 

	ReconstruMainWindow w;
	w.showMaximized();	
	splash->finish(&w);
    delete splash;

	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}


