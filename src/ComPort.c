/**
 * @file        ComPort.c
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2011/11/11
 * @copyright   Copyright (c) 2011 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <windows.h>

#include "ComPort.h"


/**
 * @name    送受信バッファサイズ
 */
/*! @{ */
#define COM_RX_BUF_SIZE         8192    /*!< 受信バッファサイズ */
#define COM_TX_BUF_SIZE         8192    /*!< 送信バッファサイズ */
/*! @} */


/**
 * @brief   シリアルポートのタイムアウト設定を行う
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 */
static VOID ComPort_setTimeouts(COMPORT comPort);


COMPORT ComPort_openComPort(LPCTSTR comName, DWORD comBaudRate)
{
    COMPORT comPort;
    BOOL retval;

    /* インスタンスの生成 */
    comPort = (COMPORT)malloc(sizeof(COMPORT_T));
    if (comPort == NULL) {
        return NULL;
    }
    ZeroMemory(comPort, sizeof(COMPORT_T));

    comPort->comHandle = CreateFile(
        comName,                        /* シリアルポート */
        GENERIC_READ | GENERIC_WRITE,   /* アクセスモード */
        0,                              /* 共有モード */
        0,                              /* セキュリティ記述子 */
        OPEN_EXISTING,                  /* 作成方法 */
        0,                              /* ファイル属性 */
        0                               /* テンプレートファイルのハンドル */
    );
    if (comPort->comHandle == INVALID_HANDLE_VALUE) {
        free(comPort);
        return NULL;
    }

    /* シリアルポートの通信パラメータの初期化 */
    comPort->comBaudRate = comBaudRate;
    comPort->comByteSize = 8;
    comPort->comParity = NOPARITY;
    comPort->comStopBits = ONESTOPBIT;

    /* 送受信バッファの設定 */
    retval = SetupComm(
        comPort->comHandle,             /* シリアルポートのハンドラ */
        COM_RX_BUF_SIZE,                /* 受信バッファサイズ */
        COM_TX_BUF_SIZE                 /* 送信バッファサイズ */
    );
    if (retval == FALSE) {
        ComPort_closeComPort(comPort);
        return NULL;
    }

    /* 送受信バッファの初期化 */
    retval = PurgeComm(
        comPort->comHandle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retval == FALSE) {
        ComPort_closeComPort(comPort);
        return NULL;
    }

    /* シリアルポートの制御設定 */
    ComPort_setDCB(comPort);

    /* シリアルポートのタイムアウト設定 */
    ComPort_setTimeouts(comPort);

    return comPort;
}


COMPORT ComPort_closeComPort(COMPORT comPort)
{
    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(comPort->comHandle);
        free(comPort);
        comPort = NULL;
    }

    return comPort;
}


BOOL ComPort_readData(COMPORT comPort, LPVOID comRxData, DWORD comRxBytes)
{
    DWORD comErrors;        /* エラーコード */
    COMSTAT comStat;        /* 通信状態バッファ */
    DWORD comReadBytes;     /* シリアルポートから読込んだバイト数 */
    BOOL retval;
    BOOL result;

    result = TRUE;
    comReadBytes = 0;

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        ClearCommError(comPort->comHandle, &comErrors, &comStat);
        retval = ReadFile(comPort->comHandle, comRxData, comRxBytes, &comReadBytes, NULL);
        if ((retval == FALSE) || (comReadBytes != comRxBytes)) {
            result = FALSE;
        }
    }
    else {
        result = FALSE;
    }

    return result;
}


BOOL ComPort_writeData(COMPORT comPort, LPVOID comTxData, DWORD comTxBytes)
{
    DWORD comErrors;        /* エラーコード */
    COMSTAT comStat;        /* 通信状態バッファ */
    DWORD comWrittenBytes;  /* シリアルポートに書込んだバイト数 */
    BOOL retval;
    BOOL result;

    result = TRUE;
    comWrittenBytes = 0;

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        ClearCommError(comPort->comHandle, &comErrors, &comStat);
        retval = WriteFile(comPort->comHandle, comTxData, comTxBytes, &comWrittenBytes, NULL);
        if ((retval == FALSE) || (comWrittenBytes != comTxBytes)) {
            result = FALSE;
        }
    }
    else {
        result = FALSE;
    }

    return result;
}


void ComPort_setDCB(COMPORT comPort)
{
    DCB comDCB;                 /* シリアルポートの制御設定 */

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* DCBの取得 */
        GetCommState(comPort->comHandle, &comDCB);

        comDCB.BaudRate = comPort->comBaudRate;
        comDCB.ByteSize = comPort->comByteSize;
        comDCB.Parity = comPort->comParity;
        comDCB.StopBits = comPort->comStopBits;

        comDCB.fBinary = TRUE;
        if (comPort->comParity == NOPARITY) {
            comDCB.fParity = FALSE;
        }
        else {
            comDCB.fParity = TRUE;
        }
        comDCB.fOutxCtsFlow = FALSE;
        comDCB.fOutxDsrFlow = FALSE;
        comDCB.fDtrControl = DTR_CONTROL_DISABLE;
        comDCB.fDsrSensitivity = FALSE;
        comDCB.fRtsControl = RTS_CONTROL_DISABLE;
        comDCB.fAbortOnError = TRUE;

        /* DCBの設定 */
        SetCommState(comPort->comHandle, &comDCB);
    }
    
    return;
}


static void ComPort_setTimeouts(COMPORT comPort)
{
    COMMTIMEOUTS comTimeouts;   /* シリアルポートのタイムアウト設定 */

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* タイムアウトパラメータの取得 */
        GetCommTimeouts(comPort->comHandle, &comTimeouts);

        comTimeouts.ReadIntervalTimeout = 100;
        comTimeouts.ReadTotalTimeoutMultiplier = 10;
        comTimeouts.ReadTotalTimeoutConstant = 100;
        comTimeouts.WriteTotalTimeoutMultiplier = 10;
        comTimeouts.WriteTotalTimeoutConstant = 100;

        /* タイムアウトパラメータの設定 */
        SetCommTimeouts(comPort->comHandle, &comTimeouts);
    }

    return;
}
