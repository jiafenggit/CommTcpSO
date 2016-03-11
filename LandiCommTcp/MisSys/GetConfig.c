/***********************************************************************************
 * �� �� ��   : GetConfig.c
 * �� �� ��   : harry
 * ��������   : 2016��3��11��
 * �ļ�����   : ��ȡ������Ϣ
 * ��Ȩ˵��   : Copyright (c) 2008-2016   xx xx xx xx �������޹�˾
 * ��    ��   : 
 * �޸���־   : 
***********************************************************************************/
#include "GetConfig.h"

#define CONFIG_PATH "./config/config.ini"




/**********************************************************************
* ���������� ��ȡ������ַ���ֵ
* ��������� fp-�����ļ�ָ��
             pszSectionName-����, ��: GENERAL
             pszKeyName-��������, ��: EmployeeName
             iOutputLen-������泤��
* ��������� pszOutput-�������
* �� �� ֵ�� ��
* ����˵���� ��
* �޸�����         �汾��        �޸���       �޸�����
* ------------------------------------------------------------------
* 20150507       V1.0     Zhou Zhaoxiong     ����
********************************************************************/
void GetStringContentValue(FILE *fp, UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pszOutput, UINT32 iOutputLen)
{
    UINT8  szSectionName[100] = {0};
    UINT8  szKeyName[100] = {0};
    UINT8  szContentLine[256] = {0};
    UINT8  szContentLineBak[256] = {0};
    UINT32 iContentLineLen = 0;
    UINT32 iPositionFlag = 0;

    // �ȶ�������������쳣�ж�
    if (fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
    {
        printf("GetStringContentValue: input parameter(s) is NULL!\n");
        return;
    }

    sprintf(szSectionName, "[%s]", pszSectionName);
    strcpy(szKeyName, pszKeyName);

    while (feof(fp) == 0)
    {
        memset(szContentLine, 0x00, sizeof(szContentLine));
        fgets(szContentLine, sizeof(szContentLine), fp);      // ��ȡ����

        // �ж��Ƿ���ע����(��;��ͷ���о���ע����)�������������ַ���ͷ����
        if (szContentLine[0] == ';' || szContentLine[0] == '\r' || szContentLine[0] == '\n' || szContentLine[0] == '\0')
        {
            continue;
        }

        // ƥ�����
        if (strncasecmp(szSectionName, szContentLine, strlen(szSectionName)) == 0)     
        {
            while (feof(fp) == 0)
            {
                memset(szContentLine,    0x00, sizeof(szContentLine));
                memset(szContentLineBak, 0x00, sizeof(szContentLineBak));
                fgets(szContentLine, sizeof(szContentLine), fp);     // ��ȡ�ֶ�ֵ

                // �ж��Ƿ���ע����(��;��ͷ���о���ע����)
                if (szContentLine[0] == ';')
                {
                    continue;
                }

                memcpy(szContentLineBak, szContentLine, strlen(szContentLine));

                // ƥ����������
                if (strncasecmp(szKeyName, szContentLineBak, strlen(szKeyName)) == 0)     
                {
                    iContentLineLen = strlen(szContentLine);
                    for (iPositionFlag = strlen(szKeyName); iPositionFlag <= iContentLineLen; iPositionFlag ++)
                    {
                        if (szContentLine[iPositionFlag] == ' ')
                        {
                            continue;
                        }
                        if (szContentLine[iPositionFlag] == '=')
                        {
                            break;
                        }

                        iPositionFlag = iContentLineLen + 1;
                        break;
                    }

                    iPositionFlag = iPositionFlag + 1;    // ����=��λ��

                    if (iPositionFlag > iContentLineLen)
                    {
                        continue;
                    }

                    memset(szContentLine, 0x00, sizeof(szContentLine));
                    strcpy(szContentLine, szContentLineBak + iPositionFlag);

                    // ȥ�������е��޹��ַ�
                    for (iPositionFlag = 0; iPositionFlag < strlen(szContentLine); iPositionFlag ++)
                    {
                        if (szContentLine[iPositionFlag] == '\r' || szContentLine[iPositionFlag] == '\n' || szContentLine[iPositionFlag] == '\0')
                        {
                            szContentLine[iPositionFlag] = '\0';
                            break;
                        }
                    }

                    // �����������ݿ��������������
                    strncpy(pszOutput, szContentLine, iOutputLen-1);
                    break;
                }
                else if (szContentLine[0] == '[')
                {
                    break;
                }
            }
            break;
        }
    }
}


/**********************************************************************
* ���������� �������ļ��л�ȡ�ַ���
* ��������� pszSectionName-����, ��: GENERAL
             pszKeyName-��������, ��: EmployeeName
             pDefaultVal-Ĭ��ֵ
             iOutputLen-������泤��
* ��������� pszOutput-�������
* �� �� ֵ�� ��
* ����˵���� ��
* �޸�����        �汾��         �޸���       �޸�����
* ------------------------------------------------------------------
* 20150507       V1.0     Zhou Zhaoxiong     ����
********************************************************************/  
void GetConfigFileStringValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pDefaultVal, UINT8 *pszOutput, UINT32 iOutputLen)
{
    FILE  *fp                    = NULL;

    // �ȶ�������������쳣�ж�
    if (pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
    {
        printf("GetConfigFileStringValue: input parameter(s) is NULL!\n");
        return;
    }

    // ��ȡĬ��ֵ
    if (pDefaultVal == NULL)
    {
        strcpy(pszOutput, "");
    }
    else
    {
        strcpy(pszOutput, pDefaultVal);
    }

    // �������ļ�
    fp = fopen(CONFIG_PATH, "r");
    if (fp == NULL)
    {
        printf("GetConfigFileStringValue: open %s failed!\n", szWholePath);
        return;
    }

    // ���ú������ڻ�ȡ�����������ֵ
    GetStringContentValue(fp, pszSectionName, pszKeyName, pszOutput, iOutputLen);

    // �ر��ļ�
    fclose(fp);
    fp = NULL;
}


/**********************************************************************
* ���������� �������ļ��л�ȡ���ͱ���
* ��������� pszSectionName-����, ��: GENERAL
             pszKeyName-��������, ��: EmployeeName
             iDefaultVal-Ĭ��ֵ
* ��������� ��
* �� �� ֵ�� iGetValue-��ȡ��������ֵ   -1-��ȡʧ��
* ����˵���� ��
* �޸�����         �汾��       �޸���        �޸�����
* ------------------------------------------------------------------
* 20150507       V1.0     Zhou Zhaoxiong     ����
********************************************************************/  
INT32 GetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 iDefaultVal)
{
    UINT8  szGetValue[512] = {0};
    INT32  iGetValue       = 0;

    // �ȶ�������������쳣�ж�
    if (pszSectionName == NULL || pszKeyName == NULL)
    {
        printf("GetConfigFileIntValue: input parameter(s) is NULL!\n");
        return -1;
    }

    GetConfigFileStringValue(pszSectionName, pszKeyName, NULL, szGetValue, 512-1);    // �Ƚ���ȡ��ֵ������ַ��ͻ�����

    if (szGetValue[0] == '\0' || szGetValue[0] == ';')    // ����ǽ�������ֺ�, ��ʹ��Ĭ��ֵ
    {
        iGetValue = iDefaultVal;
    }
    else
    {
        iGetValue = atoi(szGetValue);
    }

    return iGetValue;
}


