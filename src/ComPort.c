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
 * @brief   シリアルポートの制御設定を行う
 * @param[in,out]   comPort         シリアルポート情報構造体
 */
static VOID ComPort_setDCB(COMPORT_T *comPort);


/**
 * @brief   シリアルポートのタイムアウト設定を行う
 * @param[in,out]   comPort         シリアルポート情報構造体
 */
static VOID ComPort_setTimeouts(COMPORT_T *comPort);


BOOL ComPort_openComPort(COMPORT_T *comPort, LPCTSTR comName)
{
    BOOL retval;

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
        return FALSE;
    }

    /* シリアルポートの通信パラメータの初期化（送受信バッファの設定） */
    retval = SetupComm(
        comPort->comHandle,             /* シリアルポートのハンドラ */
        comPort->comRxBufSize,          /* 受信バッファサイズ */
        comPort->comTxBufSize           /* 送信バッファサイズ */
    );
    if (retval == FALSE) {
        return FALSE;
    }

    /* 送受信バッファの初期化 */
    retval = PurgeComm(
        comPort->comHandle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retval == FALSE) {
        return FALSE;
    }

    /* シリアルポートの制御設定 */
    ComPort_setDCB(comPort);

    /* シリアルポートのタイムアウト設定 */
    ComPort_setTimeouts(comPort);

    return TRUE;
}


BOOL ComPort_closeComPort(COMPORT_T *comPort)
{
    BOOL result;

    result = TRUE;

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(comPort->comHandle);
        comPort->comHandle = INVALID_HANDLE_VALUE;
    }

    return result;
}


BOOL ComPort_readData(COMPORT_T *comPort, LPVOID comRxData, DWORD comRxBytes)
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


BOOL ComPort_writeData(COMPORT_T *comPort, LPVOID comTxData, DWORD comTxBytes)
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


static void ComPort_setDCB(COMPORT_T *comPort)
{
    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* DCBの取得 */
        GetCommState(comPort->comHandle, &(comPort->comDCB));

        comPort->comDCB.BaudRate = comPort->comBaudRate;
        comPort->comDCB.ByteSize = comPort->comByteSize;
        comPort->comDCB.Parity = comPort->comParity;
        comPort->comDCB.StopBits = comPort->comStopBits;

        comPort->comDCB.fBinary = TRUE;
        if (comPort->comParity == NOPARITY) {
            comPort->comDCB.fParity = FALSE;
        }
        else {
            comPort->comDCB.fParity = TRUE;
        }
        comPort->comDCB.fOutxCtsFlow = FALSE;
        comPort->comDCB.fOutxDsrFlow = FALSE;
        comPort->comDCB.fDtrControl = DTR_CONTROL_DISABLE;
        comPort->comDCB.fDsrSensitivity = FALSE;
        comPort->comDCB.fRtsControl = RTS_CONTROL_DISABLE;
        comPort->comDCB.fAbortOnError = TRUE;

        /* DCBの設定 */
        SetCommState(comPort->comHandle, &(comPort->comDCB));
    }
    
    return;
}


static void ComPort_setTimeouts(COMPORT_T *comPort)
{
    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* タイムアウトパラメータの取得 */
        GetCommTimeouts(comPort->comHandle, &(comPort->comTimeouts));

        comPort->comTimeouts.ReadIntervalTimeout = 100;
        comPort->comTimeouts.ReadTotalTimeoutMultiplier = 10;
        comPort->comTimeouts.ReadTotalTimeoutConstant = 100;
        comPort->comTimeouts.WriteTotalTimeoutMultiplier = 10;
        comPort->comTimeouts.WriteTotalTimeoutConstant = 100;

        /* タイムアウトパラメータの設定 */
        SetCommTimeouts(comPort->comHandle, &(comPort->comTimeouts));
    }

    return;
}
