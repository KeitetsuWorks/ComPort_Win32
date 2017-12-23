/**
 * @file        ComPort.c
 * @brief       �V���A���|�[�g�ʐM���C�u����
 * @author      Keitetsu
 * @date        2011/11/11
 * @copyright   Copyright (c) 2011 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <windows.h>

#include "ComPort.h"


/**
 * @name    ����M�o�b�t�@�T�C�Y
 */
/*! @{ */
#define COM_RX_BUF_SIZE         8192    /*!< ��M�o�b�t�@�T�C�Y */
#define COM_TX_BUF_SIZE         8192    /*!< ���M�o�b�t�@�T�C�Y */
/*! @} */


/**
 * @brief   �V���A���|�[�g�̃^�C���A�E�g�ݒ���s��
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 */
static VOID ComPort_setTimeouts(COMPORT comPort);


COMPORT ComPort_openComPort(LPCTSTR comName, DWORD comBaudRate)
{
    COMPORT comPort;
    BOOL retval;

    /* �C���X�^���X�̐��� */
    comPort = (COMPORT)malloc(sizeof(COMPORT_T));
    if (comPort == NULL) {
        return NULL;
    }
    ZeroMemory(comPort, sizeof(COMPORT_T));

    comPort->comHandle = CreateFile(
        comName,                        /* �V���A���|�[�g */
        GENERIC_READ | GENERIC_WRITE,   /* �A�N�Z�X���[�h */
        0,                              /* ���L���[�h */
        0,                              /* �Z�L�����e�B�L�q�q */
        OPEN_EXISTING,                  /* �쐬���@ */
        0,                              /* �t�@�C������ */
        0                               /* �e���v���[�g�t�@�C���̃n���h�� */
    );
    if (comPort->comHandle == INVALID_HANDLE_VALUE) {
        free(comPort);
        return NULL;
    }

    /* �V���A���|�[�g�̒ʐM�p�����[�^�̏����� */
    comPort->comBaudRate = comBaudRate;
    comPort->comByteSize = 8;
    comPort->comParity = NOPARITY;
    comPort->comStopBits = ONESTOPBIT;

    /* ����M�o�b�t�@�̐ݒ� */
    retval = SetupComm(
        comPort->comHandle,             /* �V���A���|�[�g�̃n���h�� */
        COM_RX_BUF_SIZE,                /* ��M�o�b�t�@�T�C�Y */
        COM_TX_BUF_SIZE                 /* ���M�o�b�t�@�T�C�Y */
    );
    if (retval == FALSE) {
        ComPort_closeComPort(comPort);
        return NULL;
    }

    /* ����M�o�b�t�@�̏����� */
    retval = PurgeComm(
        comPort->comHandle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retval == FALSE) {
        ComPort_closeComPort(comPort);
        return NULL;
    }

    /* �V���A���|�[�g�̐���ݒ� */
    ComPort_setDCB(comPort);

    /* �V���A���|�[�g�̃^�C���A�E�g�ݒ� */
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
    DWORD comErrors;        /* �G���[�R�[�h */
    COMSTAT comStat;        /* �ʐM��ԃo�b�t�@ */
    DWORD comReadBytes;     /* �V���A���|�[�g����Ǎ��񂾃o�C�g�� */
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
    DWORD comErrors;        /* �G���[�R�[�h */
    COMSTAT comStat;        /* �ʐM��ԃo�b�t�@ */
    DWORD comWrittenBytes;  /* �V���A���|�[�g�ɏ����񂾃o�C�g�� */
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
    DCB comDCB;                 /* �V���A���|�[�g�̐���ݒ� */

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* DCB�̎擾 */
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

        /* DCB�̐ݒ� */
        SetCommState(comPort->comHandle, &comDCB);
    }
    
    return;
}


static void ComPort_setTimeouts(COMPORT comPort)
{
    COMMTIMEOUTS comTimeouts;   /* �V���A���|�[�g�̃^�C���A�E�g�ݒ� */

    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* �^�C���A�E�g�p�����[�^�̎擾 */
        GetCommTimeouts(comPort->comHandle, &comTimeouts);

        comTimeouts.ReadIntervalTimeout = 100;
        comTimeouts.ReadTotalTimeoutMultiplier = 10;
        comTimeouts.ReadTotalTimeoutConstant = 100;
        comTimeouts.WriteTotalTimeoutMultiplier = 10;
        comTimeouts.WriteTotalTimeoutConstant = 100;

        /* �^�C���A�E�g�p�����[�^�̐ݒ� */
        SetCommTimeouts(comPort->comHandle, &comTimeouts);
    }

    return;
}
