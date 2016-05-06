/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2011

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2014-09-03
 * @file
 * @brief
 * _ATTRIBUTE_
 * _ATTRIBUTE_
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef XSD_TAF_PIF_H
#define XSD_TAF_PIF_H

/*******************************************************************************
 1. Other files included
*******************************************************************************/
#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#pragma pack(4)

/*******************************************************************************
 2. Macro definitions
*******************************************************************************/

/*******************************************************************************
 3. Enumerations declarations
*******************************************************************************/
/** ****************************************************************************
 * Name        : XSD_TAF_NDSS_RESULT_ENUM_UINT8
 * Description :
 *******************************************************************************/
enum XSD_TAF_NDSS_RESULT_ENUM
{
    XSD_TAF_NDSS_RESULT_SUCCESS         = 0x00,
    XSD_TAF_NDSS_RESULT_FAILURE         = 0x01,

    XSD_TAF_NDSS_RESULT_BUTT            = 0x02
};
typedef VOS_UINT8 XSD_TAF_NDSS_RESULT_ENUM_UINT8;

/*******************************************************************************
 4. Message Header declaration
*******************************************************************************/

/*******************************************************************************
 5. Message declaration
*******************************************************************************/
/** ****************************************************************************
 * Name        : XSD_TAF_PIF_MSG_TYPE_ENUM
 * Description :
 *******************************************************************************/
enum XSD_TAF_PIF_MSG_TYPE_ENUM
{
    ID_XSD_TAF_NDSS_RESULT_IND        = 0x0000,      /* _H2ASN_MsgChoice XSD_TAF_NDSS_RESULT_IND_STRU*/ /**< @sa XSD_TAF_NDSS_RESULT_IND_STRU */

    ID_XSD_TAF_PIF_MSG_TYPE_ENUM_BUTT
};
typedef VOS_UINT16 XSD_TAF_PIF_MSG_TYPE_ENUM_UINT16;

/*******************************************************************************
 6. STRUCT and UNION declaration
*******************************************************************************/
/** ****************************************************************************
 * Name        : XSD_TAF_NDSS_RESULT_IND_STRU
 * Description : service redirection result ind
 *
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                   /* _H2ASN_Skip */
    XSD_TAF_PIF_MSG_TYPE_ENUM_UINT16    enMsgId;     /* _H2ASN_Skip */
    XSD_TAF_NDSS_RESULT_ENUM_UINT8      enNsddResult;
    VOS_UINT8                           aucReserved[1];
}XSD_TAF_NDSS_RESULT_IND_STRU;

/*******************************************************************************
 8. Global  declaration
*******************************************************************************/

/*******************************************************************************
 9. Function declarations
*******************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif