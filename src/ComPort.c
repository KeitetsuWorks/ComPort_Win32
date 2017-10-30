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
 * @brief   �V���A���|�[�g�̐���ݒ���s��
 * @param[in,out]   comPort         �V���A���|�[�g���\����
 */
static VOID ComPort_setDCB(COMPORT_T *comPort);


/**
 * @brief   �V���A���|�[�g�̃^�C���A�E�g�ݒ���s��
 * @param[in,out]   comPort         �V���A���|�[�g���\����
 */
static VOID ComPort_setTimeouts(COMPORT_T *comPort);


BOOL ComPort_openComPort(COMPORT_T *comPort, LPCTSTR comName)
{
    BOOL retval;

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
        return FALSE;
    }

    /* �V���A���|�[�g�̒ʐM�p�����[�^�̏������i����M�o�b�t�@�̐ݒ�j */
    retval = SetupComm(
        comPort->comHandle,             /* �V���A���|�[�g�̃n���h�� */
        comPort->comRxBufSize,          /* ��M�o�b�t�@�T�C�Y */
        comPort->comTxBufSize           /* ���M�o�b�t�@�T�C�Y */
    );
    if (retval == FALSE) {
        return FALSE;
    }

    /* ����M�o�b�t�@�̏����� */
    retval = PurgeComm(
        comPort->comHandle,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
    );
    if (retval == FALSE) {
        return FALSE;
    }

    /* �V���A���|�[�g�̐���ݒ� */
    ComPort_setDCB(comPort);

    /* �V���A���|�[�g�̃^�C���A�E�g�ݒ� */
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
    DWORD comErrors;        /* �G���[�R�[�h */
    COMSTAT comStat;        /* �ʐM��ԃo�b�t�@ */
    DWORD comReadBytes;     /* �V���A���|�[�g�ɓǍ��񂾃o�C�g�� */
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


static void ComPort_setDCB(COMPORT_T *comPort)
{
    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* DCB�̎擾 */
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

        /* DCB�̐ݒ� */
        SetCommState(comPort->comHandle, &(comPort->comDCB));
    }
    
    return;
}


static void ComPort_setTimeouts(COMPORT_T *comPort)
{
    if (comPort->comHandle != INVALID_HANDLE_VALUE) {
        /* �^�C���A�E�g�p�����[�^�̎擾 */
        GetCommTimeouts(comPort->comHandle, &(comPort->comTimeouts));

        comPort->comTimeouts.ReadIntervalTimeout = 100;
        comPort->comTimeouts.ReadTotalTimeoutMultiplier = 10;
        comPort->comTimeouts.ReadTotalTimeoutConstant = 100;
        comPort->comTimeouts.WriteTotalTimeoutMultiplier = 10;
        comPort->comTimeouts.WriteTotalTimeoutConstant = 100;

        /* �^�C���A�E�g�p�����[�^�̐ݒ� */
        SetCommTimeouts(comPort->comHandle, &(comPort->comTimeouts));
    }

    return;
}
