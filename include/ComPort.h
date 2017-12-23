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
 *
 * @typedef COMPORT
 * @brief   シリアルポート情報構造体のポインタ
 */
typedef struct ComPort_st {
    HANDLE comHandle;           /*!< シリアルポートのハンドラ */
    DWORD comBaudRate;          /*!< ボーレート */ 
    BYTE comByteSize;           /*!< データビット */
    BYTE comParity;             /*!< パリティビット */
    BYTE comStopBits;           /*!< ストップビット */
} COMPORT_T, *COMPORT;


/**
 * @brief   シリアルポートを開く
 * @param[in]       comName         シリアルポート名
 * @param[in]       comBaudRate     ボーレート
 * @retval          NULL            異常終了
 * @retval          Others          シリアルポート情報構造体のポインタ
 */
COMPORT ComPort_openComPort(LPCTSTR comName, DWORD comBaudRate);


/**
 * @brief   シリアルポートを閉じる
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @retval          NULL            正常終了
 * @retval          Others          異常終了
 */
COMPORT ComPort_closeComPort(COMPORT comPort);


/**
 * @brief   シリアルポートから受信データを読込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[out]      comRxData       受信データバッファのポインタ
 * @param[in]       comRxBytes      受信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_readData(COMPORT comPort, LPVOID comRxData, DWORD comRxBytes);


/**
 * @brief   シリアルポートに送信データを書込む
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 * @param[in]       comTxData       送信データバッファのポインタ
 * @param[in]       comTxBytes      送信データのバイト数
 * @retval          TRUE            正常終了
 * @retval          FALSE           異常終了
 */
BOOL ComPort_writeData(COMPORT comPort, LPVOID comTxData, DWORD comTxBytes);


/**
 * @brief   シリアルポートの制御設定を行う
 * @param[in]       comPort         シリアルポート情報構造体のポインタ
 */
VOID ComPort_setDCB(COMPORT comPort);

#endif	/* __COMPORT_H__ */
