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
 *
 * @typedef COMPORT
 * @brief   �V���A���|�[�g���\���̂̃|�C���^
 */
typedef struct ComPort_st {
    HANDLE comHandle;           /*!< �V���A���|�[�g�̃n���h�� */
    DWORD comBaudRate;          /*!< �{�[���[�g */ 
    BYTE comByteSize;           /*!< �f�[�^�r�b�g */
    BYTE comParity;             /*!< �p���e�B�r�b�g */
    BYTE comStopBits;           /*!< �X�g�b�v�r�b�g */
} COMPORT_T, *COMPORT;


/**
 * @brief   �V���A���|�[�g���J��
 * @param[in]       comName         �V���A���|�[�g��
 * @param[in]       comBaudRate     �{�[���[�g
 * @retval          NULL            �ُ�I��
 * @retval          Others          �V���A���|�[�g���\���̂̃|�C���^
 */
COMPORT ComPort_openComPort(LPCTSTR comName, DWORD comBaudRate);


/**
 * @brief   �V���A���|�[�g�����
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @retval          NULL            ����I��
 * @retval          Others          �ُ�I��
 */
COMPORT ComPort_closeComPort(COMPORT comPort);


/**
 * @brief   �V���A���|�[�g�����M�f�[�^��Ǎ���
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @param[out]      comRxData       ��M�f�[�^�o�b�t�@�̃|�C���^
 * @param[in]       comRxBytes      ��M�f�[�^�̃o�C�g��
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_readData(COMPORT comPort, LPVOID comRxData, DWORD comRxBytes);


/**
 * @brief   �V���A���|�[�g�ɑ��M�f�[�^��������
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 * @param[in]       comTxData       ���M�f�[�^�o�b�t�@�̃|�C���^
 * @param[in]       comTxBytes      ���M�f�[�^�̃o�C�g��
 * @retval          TRUE            ����I��
 * @retval          FALSE           �ُ�I��
 */
BOOL ComPort_writeData(COMPORT comPort, LPVOID comTxData, DWORD comTxBytes);


/**
 * @brief   �V���A���|�[�g�̐���ݒ���s��
 * @param[in]       comPort         �V���A���|�[�g���\���̂̃|�C���^
 */
VOID ComPort_setDCB(COMPORT comPort);

#endif	/* __COMPORT_H__ */
