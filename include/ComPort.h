/**
 * @file        ComPort.h
 * @brief       �V���A���|�[�g�ʐM���C�u����
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
 * @brief   �V���A���|�[�g���\����
 *
 * @typedef COMPORT_T
 * @brief   �V���A���|�[�g���\����
 */
typedef struct ComPort_st {
    HANDLE comHandle;           /*!< �V���A���|�[�g�̃n���h�� */
    DCB comDCB;                 /*!< �V���A���|�[�g�̐���ݒ� */
    COMMTIMEOUTS comTimeouts;   /*!< �V���A���|�[�g�̃^�C���A�E�g�ݒ� */
    DWORD comRxBufSize;         /*!< ��M�o�b�t�@�T�C�Y */
    DWORD comTxBufSize;         /*!< ���M�o�b�t�@�T�C�Y */
    DWORD comBaudRate;          /*!< �{�[���[�g */ 
    BYTE comByteSize;           /*!< �f�[�^�r�b�g */
    BYTE comParity;             /*!< �p���e�B�r�b�g */
    BYTE comStopBits;           /*!< �X�g�b�v�r�b�g */
} COMPORT_T;


/**
 * @brief   �V���A���|�[�g���J��
 * @param[in,out]   comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @param[in]       comName         �V���A���|�[�g��
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_openComPort(COMPORT_T *comPort, LPCTSTR comName);


/**
 * @brief   �V���A���|�[�g�����
 * @param[in,out]   comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_closeComPort(COMPORT_T *comPort);


/**
 * @brief   �V���A���|�[�g�����M�f�[�^��Ǎ���
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @param[in]       comRxData       ��M�f�[�^�o�b�t�@�̃|�C���^
 * @param[in]       comRxBytes      ��M�f�[�^�̃o�C�g��
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_readData(COMPORT_T *comPort, LPVOID comRxData, DWORD comRxBytes);


/**
 * @brief   �V���A���|�[�g�ɑ��M�f�[�^��������
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @param[in]       comTxData       ���M�f�[�^�o�b�t�@�̃|�C���^
 * @param[in]       comTxBytes      ���M�f�[�^�̃o�C�g��
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_writeData(COMPORT_T *comPort, LPVOID comTxData, DWORD comTxBytes);

#endif	/* __COMPORT_H__ */
