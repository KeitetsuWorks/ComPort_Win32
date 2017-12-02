/**
 * @file        ComPort.h
 * @brief       シリアルポート通信ライブラリ
 * @author      Keitetsu
 * @date        2011/11/11
 * @copyright   Copyright (c) 2011 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#ifndef __COMPORT_H__
#define __COMPORT_H__


/**
 * @struct  ComPort_st
 * @brief   シリアルポート情報構造体
 *
 * @typedef COMPORT_T
 * @brief   シリアルポート情報構造体
 */
typedef struct ComPort_st {
    HANDLE comHandle;           /*!< シリアルポートのハンドラ */
    DCB comDCB;                 /*!< シリアルポートの制御設定 */
    COMMTIMEOUTS comTimeouts;   /*!< シリアルポートのタイムアウト設定 */
    DWORD comRxBufSize;         /*!< 受信バッファサイズ */
    DWORD comTxBufSize;         /*!< 送信バッファサイズ */
    DWORD comBaudRate;          /*!< ボーレート */ 
    BYTE comByteSize;           /*!< データビット */
    BYTE comParity;             /*!< パリティビット */
    BYTE comStopBits;           /*!< ストップビット */
} COMPORT_T;


/**
 * @brief   シリアルポートを開く
 * @param[in,out]   comPort         シリアルポート情報構造体のポインタ
 * @param[in]       comName         シリアルポート名
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_openComPort(COMPORT_T *comPort, LPCTSTR comName);


/**
 * @brief   シリアルポートを閉じる
 * @param[in,out]   comPort         シリアルポート情報構造体のポインタ
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_closeComPort(COMPORT_T *comPort);


/**
 * @brief   シリアルポートから受信データを読込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[out]      comRxData       受信データバッファのポインタ
 * @param[in]       comRxBytes      受信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_readData(COMPORT_T *comPort, LPVOID comRxData, DWORD comRxBytes);


/**
 * @brief   シリアルポートに送信データを書込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[in]       comTxData       送信データバッファのポインタ
 * @param[in]       comTxBytes      送信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_writeData(COMPORT_T *comPort, LPVOID comTxData, DWORD comTxBytes);

#endif	/* __COMPORT_H__ */
