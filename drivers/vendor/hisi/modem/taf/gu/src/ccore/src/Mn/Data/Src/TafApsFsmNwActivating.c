
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : TafApsFsmNwActivating.c
  �� �� ��   : ����
  ��    ��   : S62952
  ��������   : 2011��12��14��
  ����޸�   :
  ��������   : NwActivating״̬����������
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2011��12��14��
    ��    ��   : S62952
    �޸�����   : �����ļ�

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafTypeDef.h"
#include "TafApsApi.h"
#include "Taf_Aps.h"
#include "MnApsComm.h"
#include "MmcApsInterface.h"
#include "TafLog.h"
#include "Taf_Aps.h"
#include "TafApsCtx.h"
#include "TafApsFsmMainTbl.h"
#include "TafApsFsmNwActivating.h"
#include "TafApsFsmNwActivatingTbl.h"
#include "TafApsFsmMsDeactivatingTbl.h"
#include "TafApsProcIpFilter.h"
#include "TafApsSndRabm.h"
#include "TafApsSndSm.h"
#include "TafApsSndAt.h"
#include "TafApsSndInternalMsg.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "SmEsmInterface.h"
#include "MnApsMultiMode.h"
#include "ApsL4aInterface.h"
#include "TafApsSndL4a.h"
#include "TafApsSndEsm.h"
#include "TafSmInterface.h"
#endif

#include "TafApsComFunc.h"
#include "ApsNdInterface.h"
#include "TafApsSndNd.h"

/*lint -e958*/

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_APS_FSM_NWACTIVATING_C

/******************************************************************************
   2 �ⲿ������������
******************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/


/******************************************************************************
   4 ȫ�ֱ�������
*****************************************************************************/


/******************************************************************************
   5 ����ʵ��
******************************************************************************/

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActivateInd_NwActivating_Init
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_INIT��״̬��
             �յ�D_PMC_SMREG_PDP_ACT_IND��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpActivateInd_NwActivating_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ������״̬���������Ϣ */
    TAF_APS_SaveCurrSubFsmEntryMsg(ulEventType, pstMsg);

    /*--------------------------------------------------------------------------
       ��ǰģΪG/W:  ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER
    --------------------------------------------------------------------------*/

    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_NW_ACTIVATING);
    TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode();

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvEsmPdpManageInd_NwActivating_Init
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_INIT��״̬��
             �յ�ID_APP_ESM_PDP_MANAGER_IND��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��12��7��
    ��    ��   : w00316404
    �޸�����   : Split L4A Project
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvEsmPdpManageInd_NwActivating_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ������״̬���������Ϣ */
    TAF_APS_SaveCurrSubFsmEntryMsg(ulEventType, pstMsg);

    /*--------------------------------------------------------------------------
       ��ǰģΪLTE:  ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER
    --------------------------------------------------------------------------*/

    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_NW_ACTIVATING);
    TAF_APS_RcvEsmPdpManageInd_NwActivating_LteMode();

    return VOS_TRUE;
}
#endif

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSetPdpContextStateReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSetPdpContextStateReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstPdpContextStateReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg               = (TAF_PS_MSG_STRU*)pstMsg;
    pstPdpContextStateReq   = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);

    /* �ȴ��û�Ӧ���ʱ���ϱ�PDP����/ȥ��������¼� */
    TAF_APS_SndSetPdpCtxStateCnf(&(pstPdpContextStateReq->stCtrl),
                                TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : S62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;
    TAF_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    enCurrRatType     = TAF_APS_GetCurrPdpEntityRatType();
    pstAppMsg         = (TAF_PS_MSG_STRU*)pstMsg;
    pstPppDialOrigReq = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* IP���͵����缤��Ӧ��AT^CGANSֻ֧��GUģ */
    switch (enCurrRatType)
    {
        case TAF_APS_RAT_TYPE_GSM:
        case TAF_APS_RAT_TYPE_WCDMA:
            TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitAppAnswer_GuMode(pstMsg);
            break;

        default:
            TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                                     TAF_PS_CAUSE_UNKNOWN);
            break;
    }



    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : S62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ORIG_REQ_STRU          *pstCallOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg       = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallOrigReq  = (TAF_PS_CALL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndCallOrigCnf(&(pstCallOrigReq->stCtrl),
                          pstCallOrigReq->stDialParaInfo.ucCid,
                          TAF_PS_CAUSE_CID_INVALID);
    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_END_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : S62952
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : S62952
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_MODIFY_REQ_STRU        *pstCallModifyReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallModifyReq                    = (TAF_PS_CALL_MODIFY_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �޸ĸ�APSʵ��, ����ERROR */
    TAF_APS_SndCallModifyCnf(&(pstCallModifyReq->stCtrl),
                            TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallAnswerReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_ANSWER_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_ANSWER_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��12��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallAnswerReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_RAT_TYPE_ENUM_UINT32            enCurrRatType;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    enCurrRatType                       = TAF_APS_GetCurrPdpEntityRatType();

    switch (enCurrRatType)
    {
        case TAF_APS_RAT_TYPE_GSM:
        case TAF_APS_RAT_TYPE_WCDMA:
            TAF_APS_RcvAtPsCallAnswerReq_NwActivating_GuMode(pstMsg);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case TAF_APS_RAT_TYPE_LTE:
            TAF_APS_RcvAtPsCallAnswerReq_NwActivating_LteMode(pstMsg);

            break;
#endif

        default:
            TAF_WARNING_LOG1(WUEPS_PID_TAF, "TAF_APS_RcvAtPsCallAnswerReq_NwActivating_WaitAppAnswer,enCurrRatType", enCurrRatType);
            break;
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallHangupReq_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_HANGUP_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_HANGUP_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : S62952
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallHangupReq_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_HANGUP_REQ_STRU        *pstCallHangupReq;
    TAF_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    enCurrRatType                       = TAF_APS_GetCurrPdpEntityRatType();
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallHangupReq                    = (TAF_PS_CALL_HANGUP_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڵȴ�Ӧ�𣬴�ʱ�йҶϲ���, ֱ�ӷ���TAF_ERR_NO_ERROR */
    TAF_APS_SndCallHangupCnf(&(pstCallHangupReq->stCtrl), TAF_ERR_NO_ERROR);

    switch (enCurrRatType)
    {
        case TAF_APS_RAT_TYPE_GSM:
        case TAF_APS_RAT_TYPE_WCDMA:
            TAF_APS_RcvAtPsCallHangUpReq_NwActivating_GuMode(pstMsg);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case TAF_APS_RAT_TYPE_LTE:
            TAF_APS_RcvAtPsCallHangUpReq_NwActivating_LteMode(pstMsg);

            break;
#endif

        default:
            TAF_WARNING_LOG1(WUEPS_PID_TAF, "TAF_APS_RcvAtPsCallHangupReq_NwActivating_WaitAppAnswer,enCurrRatType", enCurrRatType);
            break;
    }

    return VOS_TRUE;

}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitAppAnswer
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��25��
    ��    ��   : g00261581
    �޸�����   : �޸������Ϣ
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU           *pstServStaChangeInd;
    VOS_UINT8                                               ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32                            enLastRatType;                              /* ��ѯ���صĵ�ǰ���� */

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstServStaChangeInd                 = (TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU*)pstMsg;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��¼ǰһ�ε��������� */
    enLastRatType = pstServStaChangeInd->enLastRatType;

    /* ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬
       ���enRatTypeΪTAF_APS_RAT_TYPE_NULL��˵������������ϵͳ����ʱ������ֹ */
    if (TAF_APS_RAT_TYPE_NULL == pstServStaChangeInd->enRatType)
    {
        /* ��Ӧ�ûعҶ�ָʾ */
        TAF_APS_SndPdpDisconnectInd(ucPdpId);

        /* ��SM����Abort */
        if ((TAF_APS_RAT_TYPE_GSM == enLastRatType)
         || (TAF_APS_RAT_TYPE_WCDMA == enLastRatType))
        {
            TAF_APS_SndSmPdpAbortReq(ucPdpId);
        }

#if (FEATURE_ON == FEATURE_LTE)
        /* ��ESM����Abort */
        if (TAF_APS_RAT_TYPE_LTE == enLastRatType)
        {
            TAF_APS_SndL4aAbortReq(TAF_APS_GetPdpEntCurrCid(ucPdpId));
        }
#endif

        /* ͣ������ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

        /* �ͷ���Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        /* �˳���״̬�� */
        TAF_APS_QuitCurrSubFsm();
    }
    else
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitAppAnswer: Wrong Rat Type");
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitAppAnswer
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER��״̬��
             �յ�TI_TAF_APS_NET_ACTIVATING��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��25��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��Ӧ�ûعҶ�ָʾ */
    TAF_APS_SndPdpDisconnectInd(ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpDeactivateInd_NwActivating_WaitAppAnswer
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER��״̬��
             �յ�ID_SMREG_PDP_DEACTIVATE_IND��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��11��15��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpDeactivateInd_NwActivating_WaitAppAnswer(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* stop������ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

    /* ��Ӧ�ûعҶ�ָʾ */
    TAF_APS_SndPdpDisconnectInd(ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstPdpContextStateReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg               = (TAF_PS_MSG_STRU*)pstMsg;
    pstPdpContextStateReq   = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                 = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���ڼ����APSʵ��, �ټ����APSʵ�����õ�CID,�ϱ�ERROR�¼�,
       ����,����ȥ����״̬����Ǩ�� */
    if (TAF_CGACT_ACT == pstPdpContextStateReq->stCidListStateInfo.ucState)
    {
        /* �ϱ�PDP��������¼� */
        TAF_APS_SndSetPdpCtxStateCnf(&(pstPdpContextStateReq->stCtrl),
                                    TAF_PS_CAUSE_CID_INVALID);
    }
    else
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_CNF�¼� */
        TAF_APS_SndPdpDeActivateCnf(ucPdpId, TAF_APS_GetPdpEntCurrCid(ucPdpId));

        /* ��SM����ABORT����, ȡ����ǰ������� */
        TAF_APS_SndSmPdpAbortReq(ucPdpId);

        /* �ͷ�APS��Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        TAF_APS_QuitCurrSubFsm();
    }

    return VOS_TRUE;

}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg         = (TAF_PS_MSG_STRU*)pstMsg;
    pstPppDialOrigReq = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                             TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ORIG_REQ_STRU          *pstCallOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg       = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallOrigReq  = (TAF_PS_CALL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndCallOrigCnf(&(pstCallOrigReq->stCtrl),
                          pstCallOrigReq->stDialParaInfo.ucCid,
                          TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_MSG_TAF_PS_CALL_END_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_MODIFY_REQ_STRU        *pstCallModifyReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallModifyReq                    = (TAF_PS_CALL_MODIFY_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �޸ĸ�APSʵ��, ����ERROR */
    TAF_APS_SndCallModifyCnf(&(pstCallModifyReq->stCtrl),
                            TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpDeactivInd_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�D_PMC_SMREG_PDP_DEACTIV_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : D_PMC_SMREG_PDP_DEACTIV_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpDeactivInd_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    SMREG_PDP_DEACTIVATE_IND_STRU      *pstSmPdpDeactivateInd;


    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();
    pstSmPdpDeactivateInd = (SMREG_PDP_DEACTIVATE_IND_STRU*)pstMsg;

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ��״̬���յ�ID_PMC_SMREG_PDP_DEACTIV_INDΪ�쳣��Ϣ,�ϱ������¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstSmPdpDeactivateInd->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActRej_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�D_PMC_SMREG_PDP_ACT_REJ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : D_PMC_SMREG_PDP_ACT_REJ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpActRej_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    SMREG_PDP_ACTIVATE_REJ_STRU        *pstSmPdpActivateRej;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();
    pstSmPdpActivateRej = (SMREG_PDP_ACTIVATE_REJ_STRU *)pstMsg;

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ��״̬���յ�D_PMC_SMREG_PDP_ACT_REJΪ�쳣��Ϣ,�ϱ������¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstSmPdpActivateRej->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;

}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActSecRej_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�D_PMC_SMREG_PDP_ACT_SEC_REJ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : D_PMC_SMREG_PDP_ACT_SEC_REJ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpActSecRej_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    VOS_UINT8                           ucPdpId;
    SMREG_PDP_ACTIVATE_SEC_REJ_STRU    *pstSmSecPdpActivateRej;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();
    pstSmSecPdpActivateRej = (SMREG_PDP_ACTIVATE_SEC_REJ_STRU*)pstMsg;


    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ��״̬���յ�D_PMC_SMREG_PDP_ACT_REJΪ�쳣��Ϣ,�ϱ������¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstSmSecPdpActivateRej->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;

}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActSecCnf_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�D_PMC_SMREG_PDP_ACT_SEC_CNF��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : D_PMC_SMREG_PDP_ACT_SEC_CNF��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�

  3.��    ��   : 2013��01��21��
    ��    ��   : A00165503
    �޸�����   : DTS2013011803709: ����ND Client���ӽ��뼼���ж�, ��ֹ��NULL
                 ״̬�¼���ND Client����RS��������

  4.��    ��   : 2013��5��15��
    ��    ��   : w00167002
    �޸�����   : DTS2013051408623:V3R2 lint�����޸�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpActSecCnf_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;
    APS_ACTSECCNF_PARA_ST               stParam;

#if (FEATURE_ON == FEATURE_LTE)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif

    SMREG_PDP_ACTIVATE_SEC_CNF_STRU    *pstSmSecPdpActivateCnf;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstSmSecPdpActivateCnf = (SMREG_PDP_ACTIVATE_SEC_CNF_STRU *)pstMsg;
    ucPdpId                = TAF_APS_GetCurrFsmEntityPdpId();
    enCurrRatType          = TAF_APS_GetCurrPdpEntityRatType();

#if (FEATURE_ON == FEATURE_LTE)
    pstPdpEntity        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
#endif

    /* ֹͣ���缤�����̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

    /* �������Ĳ��� */
    ulRet = Aps_PdpActSecCnfParaCheck(pstSmSecPdpActivateCnf, &stParam);
    if (APS_PARA_VALID != ulRet)
    {
        if (APS_PDPTYPE_INVALID == ulRet)
        {
            /* �ϱ�����ʧ�� */
            TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER);

            /* �����ڲ���Ϣ, ȥ����PDP */
            TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER);
        }
        else
        {
            /* �ϱ�����ʧ�� */
            TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_INSUFFICIENT_RESOURCES);

            /* �����ڲ���Ϣ, ȥ����PDP */
            TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_INSUFFICIENT_RESOURCES);
        }

        return VOS_TRUE;
    }

    /* ���QOS�Ƿ�����MINҪ�� */
    if (APS_PARA_VALID  == Aps_CheckQosSatisify(ucPdpId, &stParam.PdpQos))
    {
        /* ����MINQOS */
        Aps_PdpActSecCnfQosSatisfy(ucPdpId, &stParam, pstSmSecPdpActivateCnf);

#if (FEATURE_ON == FEATURE_LTE)
        /* ͬ��PDP��Ϣ��ESM */
        MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_ACTIVATE);
#endif

        switch (enCurrRatType)
        {
            case TAF_APS_RAT_TYPE_GSM:
                /* ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP */
                TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP);
                break;

            case TAF_APS_RAT_TYPE_WCDMA:
                /* ����ɹ�����������ͳ�� */
                TAF_APS_StartDsFlowStats(pstSmSecPdpActivateCnf->ucNsapi);

                /* �����ǰAPSʵ���PDP����ΪIPv4, ��Ҫ����IPF */
                if (TAF_APS_CheckPdpAddrTypeIpv4(ucPdpId))
                {
                    /* ����IP������ */
                    TAF_APS_IpfConfigUlFilter(ucPdpId);
                }

                /* ��״̬Ǩ����TAF_APS_STA_ACTIVE, �˳���״̬״̬�� */
                TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_ACTIVE);
                TAF_APS_QuitCurrSubFsm();
                break;

            default:
                TAF_WARNING_LOG(WUEPS_PID_TAF,
                    "TAF_APS_RcvSmPdpActCnf_NwActivating_WaitSmActivateCnf: Wrong RAT type!");
                break;
        }
    }
    else
    {
        /* ������MINQOS */
        TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_QOS_NOT_ACCEPTED);

        /* �����ڲ���Ϣ, ȥ����PDP */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_QOS_NOT_ACCEPTED);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActCnf_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_PMC_SMREG_PDP_ACT_CNF��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_PMC_SMREG_PDP_ACT_CNF��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��12��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��10��
    ��    ��   : L00171473
    �޸�����   : DTS2012082204471, TQE����

  3.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�

  4.��    ��   : 2013��01��21��
    ��    ��   : A00165503
    �޸�����   : DTS2013011803709: ����ND Client���ӽ��뼼���ж�, ��ֹ��NULL
                 ״̬�¼���ND Client����RS��������
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpActCnf_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;
    APS_ACTCNF_PARA_ST                  stParam;
#if ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_IPV6))
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif
    SMREG_PDP_ACTIVATE_CNF_STRU        *pstSmPdpActivateCnf;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstSmPdpActivateCnf = (SMREG_PDP_ACTIVATE_CNF_STRU *)pstMsg;
    ucPdpId             = TAF_APS_GetCurrFsmEntityPdpId();
    enCurrRatType       = TAF_APS_GetCurrPdpEntityRatType();
#if ((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_IPV6))
    pstPdpEntity        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
#endif


    PS_MEM_SET(&stParam, 0x00, sizeof(stParam));


    /* ֹͣ���缤�����̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* �������Ĳ��� */
    ulRet = Aps_PdpActCnfParaCheck(pstSmPdpActivateCnf, &stParam);
    if (APS_PARA_VALID != ulRet)
    {
        if (APS_PDPTYPE_INVALID == ulRet)
        {
            /* �ϱ�����ʧ�� */
            TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER);

            /* �����ڲ���Ϣ, ȥ����PDP */
            TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER);
        }
        else
        {
            /* �ϱ�����ʧ�� */
            TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_INSUFFICIENT_RESOURCES);

            /* �����ڲ���Ϣ, ȥ����PDP */
            TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_INSUFFICIENT_RESOURCES);
        }

        return VOS_TRUE;
    }

    /* ���QOS�Ƿ�����MINҪ�� */
    if (APS_PARA_VALID  == Aps_CheckQosSatisify(ucPdpId, &stParam.PdpQos))
    {
        /* ����MINQOS */
        Aps_PdpActCnfQosSatisfy(ucPdpId, &stParam, pstSmPdpActivateCnf);

#if (FEATURE_ON == FEATURE_LTE)
        /* ͬ��PDP��Ϣ��ESM */
        MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_ACTIVATE);
#endif

#if (FEATURE_ON == FEATURE_IPV6)
        /* �����ַ������IPv6, ��Ҫͬ����ND Client */
        if (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId))
        {
            TAF_APS_SndNdPdpActInd(pstPdpEntity->ucNsapi,
                                   pstPdpEntity->PdpAddr.aucIpV6Addr);
        }
#endif

        switch (enCurrRatType)
        {
            case TAF_APS_RAT_TYPE_GSM:

                /* ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP */
                TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP);
                break;

            case TAF_APS_RAT_TYPE_WCDMA:

                /* ����ɹ�����������ͳ�� */
                TAF_APS_StartDsFlowStats(pstSmPdpActivateCnf->ucNsapi);

                /* �����ǰAPSʵ���PDP����ΪIPv4, ��Ҫ����IPF */
                if (TAF_APS_CheckPdpAddrTypeIpv4(ucPdpId))
                {
                    /* ����IP������ */
                    TAF_APS_IpfConfigUlFilter(ucPdpId);
                }

                /* ��״̬Ǩ����TAF_APS_STA_ACTIVE, �˳���״̬״̬�� */
                TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_ACTIVE);
                TAF_APS_QuitCurrSubFsm();
                break;

            default:
                TAF_WARNING_LOG(WUEPS_PID_TAF,
                    "TAF_APS_RcvSmPdpActCnf_NwActivating_WaitSmActivateCnf: Wrong RAT type!");
                break;
        }
    }
    else
    {
        /* ������MINQOS */
        TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_QOS_NOT_ACCEPTED);

        /* �����ڲ���Ϣ, ȥ����PDP */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_QOS_NOT_ACCEPTED);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvApsInterPdpDeactivateReq_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_APS_APS_INTER_PDP_DEACTIVATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvApsInterPdpDeactivateReq_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_APS_APS_INTERNAL_PDP_DEACTIVATE_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvApsLocalPdpDeactivateInd_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�ID_APS_APS_LOCAL_PDP_DEACTIVATE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_APS_APS_LOCAL_PDP_DEACTIVATE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��06��07��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvApsLocalPdpDeactivateInd_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU  *pstLocalMsg;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId     = TAF_APS_GetCurrFsmEntityPdpId();
    pstLocalMsg = (TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU *)pstMsg;

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ��״̬���յ�ID_APS_APS_LOCAL_PDP_DEACTIVATE_INDΪ�쳣��Ϣ,�ϱ������¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstLocalMsg->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitSmActivateCnf
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF״̬��
             �յ�ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��12��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��25��
    ��    ��   : g00261581
    �޸�����   : �޸������Ϣ
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU           *pstServStaChangeInd;
    VOS_UINT8                                               ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32                            enLastRatType;                              /* ��ѯ���صĵ�ǰ���� */

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstServStaChangeInd                 = (TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU*)pstMsg;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��¼ǰһ�ε��������� */
    enLastRatType = pstServStaChangeInd->enLastRatType;

    /* ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬
       ���enRatTypeΪMMC_APS_RAT_TYPE_NULL��
       ˵�����������л�����Ϊ���쳣��� */
    if (TAF_APS_RAT_TYPE_NULL == pstServStaChangeInd->enRatType)
    {
        /* ��Ӧ�ûعҶ�ָʾ */
        TAF_APS_SndPdpDisconnectInd(ucPdpId);

        /* ��SM����Abort */
        if ((TAF_APS_RAT_TYPE_GSM   == enLastRatType)
         || (TAF_APS_RAT_TYPE_WCDMA == enLastRatType))
        {
            TAF_APS_SndSmPdpAbortReq(ucPdpId);
        }

#if (FEATURE_ON == FEATURE_LTE)
        /* ��ESM����Abort */
        if (TAF_APS_RAT_TYPE_LTE == enLastRatType)
        {
            TAF_APS_SndL4aAbortReq(TAF_APS_GetPdpEntCurrCid(ucPdpId));
        }
#endif

        /* ͣ������ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

        /* �ͷ���Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        /* �˳���״̬�� */
        TAF_APS_QuitCurrSubFsm();
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitSmActivateCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF��״̬��
             �յ�TI_TAF_APS_NET_ACTIVATING��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��25��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitSmActivateCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ�¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_MAX_TIME_OUT);

    /* �ͷ�SM��Դ */
    TAF_APS_SndSmPdpAbortReq(ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitEsmCgansCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF��״̬��
             �յ�ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstPdpContextStateReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg               = (TAF_PS_MSG_STRU*)pstMsg;
    pstPdpContextStateReq   = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                 = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���ڼ����APSʵ��, �ټ����APSʵ�����õ�CID,�ϱ�ERROR�¼�,
       ����,����ȥ����״̬����Ǩ�� */
    if (TAF_CGACT_ACT == pstPdpContextStateReq->stCidListStateInfo.ucState)
    {
        /* �ϱ�PDP��������¼� */
        TAF_APS_SndSetPdpCtxStateCnf(&(pstPdpContextStateReq->stCtrl),
                                    TAF_PS_CAUSE_CID_INVALID);
    }
    else
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /*---------------------------------------------------------
           ����TAF_APS_STA_MS_DEACTIVATING״̬��
           ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

           ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
           ����ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
        ---------------------------------------------------------*/
        TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                           TAF_APS_GetMsDeactivatingFsmDescAddr(),
                           TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitEsmCgansCnf
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF״̬��
             �յ�ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg         = (TAF_PS_MSG_STRU*)pstMsg;
    pstPppDialOrigReq = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                             TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitEsmCgansCnf
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF״̬��
             �յ�ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ORIG_REQ_STRU          *pstCallOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg       = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallOrigReq  = (TAF_PS_CALL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndCallOrigCnf(&(pstCallOrigReq->stCtrl),
                          pstCallOrigReq->stDialParaInfo.ucCid,
                          TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitEsmCgansCnf
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF״̬��
             �յ�ID_MSG_TAF_PS_CALL_END_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitEsmCgansCnf
 ��������  : ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF״̬��
             �յ�ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_MODIFY_REQ_STRU        *pstCallModifyReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallModifyReq                    = (TAF_PS_CALL_MODIFY_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �޸ĸ�APSʵ��, ����ERROR */
    TAF_APS_SndCallModifyCnf(&(pstCallModifyReq->stCtrl),
                            TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvL4aSetCgansCnf_NwActivating_WaitEsmCgansCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF��״̬��
             �յ�ID_L4A_APS_SET_CGANS_CNF��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_L4A_APS_SET_CGANS_CNF��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvL4aSetCgansCnf_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId             = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ֹͣ���缤�����̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

#if (FEATURE_ON == FEATURE_IPV6)
    /* �����ַ������IPv6, ��Ҫͬ����ND Client */
    if (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId))
    {
        TAF_APS_SndNdPdpActInd(pstPdpEntity->ucNsapi,
                               pstPdpEntity->PdpAddr.aucIpV6Addr);
    }
#endif

#if (FEATURE_ON == FEATURE_IMS)
    /* ����IMSר�г��� */
    TAF_APS_ProcImsDedicateBearer(pstPdpEntity);
#endif

    /* ����ɹ�����������ͳ�� */
    TAF_APS_StartDsFlowStats(pstPdpEntity->ucNsapi);

    /* �����ǰAPSʵ���PDP����ΪIPv4, ��Ҫ����IPF */
    if (TAF_APS_CheckPdpAddrTypeIpv4(ucPdpId))
    {
        /* ����IP������ */
        TAF_APS_IpfConfigUlFilter(ucPdpId);
    }

    /* ��״̬Ǩ����TAF_APS_STA_ACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_ACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;

}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitEsmCgansCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF��״̬��
             �յ�ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��25��
    ��    ��   : g00261581
    �޸�����   : �޸������Ϣ
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU           *pstServStaChangeInd;
    VOS_UINT8                                               ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32                            enLastRatType;                              /* ��ѯ���صĵ�ǰ���� */

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstServStaChangeInd                 = (TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU*)pstMsg;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��¼ǰһ�ε��������� */
    enLastRatType = pstServStaChangeInd->enLastRatType;

    /* ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_L4A_CGANS_CNF״̬
       ���enRatTypeΪTAF_APS_RAT_TYPE_NULL��˵������������ϵͳ����ǰ������ֹ */
    if (TAF_APS_RAT_TYPE_NULL == pstServStaChangeInd->enRatType)
    {
        /* ��Ӧ�ûعҶ�ָʾ */
        TAF_APS_SndPdpDisconnectInd(ucPdpId);

        /* ��SM����Abort */
        if ((TAF_APS_RAT_TYPE_GSM   == enLastRatType)
         || (TAF_APS_RAT_TYPE_WCDMA == enLastRatType))
        {
            TAF_APS_SndSmPdpAbortReq(ucPdpId);
        }

#if (FEATURE_ON == FEATURE_LTE)
        /* ��ESM����Abort */
        if (TAF_APS_RAT_TYPE_LTE == enLastRatType)
        {
            TAF_APS_SndL4aAbortReq(TAF_APS_GetPdpEntCurrCid(ucPdpId));
        }
#endif

        /* ͣ������ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

        /* �ͷ���Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        /* �˳���״̬�� */
        TAF_APS_QuitCurrSubFsm();
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvEsmSmEpsBearerInfoInd_NwActivating_WaitEsmCgansCnf(
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF��״̬���յ�ID_ESM_SM_EPS_BEARER_INFO_IND��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��01��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2013��01��21��
    ��    ��   : A00165503
    �޸�����   : DTS2013011803709: ����ND Client���ӽ��뼼���ж�, ��ֹ��NULL
                 ״̬�¼���ND Client����RS��������

  3.��    ��   : 2013��4��23��
    ��    ��   : A00165503
    �޸�����   : DTS2013041204937: ר�ó��ؼ���ʱ����Ҫ����ND Client

  4.��    ��   : 2013��07��08��
    ��    ��   : Y00213812
    �޸�����   : VoLTE_PhaseI ��Ŀ����IMSר�г��صĴ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvEsmSmEpsBearerInfoInd_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    TAF_APS_EPS_BEARER_INFO_IND_STRU   *pstBearerInfo;

    pstBearerInfo = (TAF_APS_EPS_BEARER_INFO_IND_STRU*)pstMsg;
    ucPdpId       = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��ȡ��PDPID��PDPʵ�����ݵ�ַ */
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);


    /* ����EPS���صĲ�������(Activate, Deactivate, Modify),
       ������Ӧ�Ĵ������� */
    if (SM_ESM_PDP_OPT_ACTIVATE == pstBearerInfo->enPdpOption)
    {
        MN_APS_ProcEsmBearerInfoIndOptActivate(pstPdpEntity, pstBearerInfo);

#if (FEATURE_ON == FEATURE_IPV6)
        /* �����ַ������IPv6, ��Ҫͬ����ND Client */
        if ( (TAF_APS_CheckPrimaryPdp(ucPdpId))
          && (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId)) )
        {
            TAF_APS_SndNdPdpActInd(pstPdpEntity->ucNsapi,
                                   pstPdpEntity->PdpAddr.aucIpV6Addr);
        }
#endif
    }
    else
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,"TAF_APS_RcvEsmSmEpsBearerInfoInd_NwActivating_WaitEsmCgansCnf: Wrong option.");
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitEsmCgansCnf
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF��״̬��
             �յ�TI_TAF_APS_NET_ACTIVATING��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : TI_TAF_APS_NET_ACTIVATING��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : h44270
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitEsmCgansCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ�¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_MAX_TIME_OUT);

    /* ��ESM����Abort */
    TAF_APS_SndL4aAbortReq(TAF_APS_GetPdpEntCurrCid(ucPdpId));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}
#endif
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtSetPdpContextStateReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstPdpContextStateReq;
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstPdpContextStateReq               = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ����ȥ�����APSʵ��, �ټ����APSʵ�����õ�CID,�ϱ�ERROR�¼�,
       ����,����ȥ����״̬����Ǩ�� */
    if (TAF_CGACT_ACT == pstPdpContextStateReq->stCidListStateInfo.ucState)
    {
        /* �ϱ�PDP��������¼�*/
        TAF_APS_SndSetPdpCtxStateCnf(&(pstPdpContextStateReq->stCtrl),
                                    TAF_PS_CAUSE_CID_INVALID);
    }
    else
    {
        /* ֹͣ���༤�����̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /*---------------------------------------------------------
           ����TAF_APS_STA_MS_DEACTIVATING״̬��
           ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

           ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
           ����ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
        ---------------------------------------------------------*/
        TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                           TAF_APS_GetMsDeactivatingFsmDescAddr(),
                           TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg         = (TAF_PS_MSG_STRU*)pstMsg;
    pstPppDialOrigReq = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                             TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : s62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ORIG_REQ_STRU          *pstCallOrigReq;


    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg       = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallOrigReq  = (TAF_PS_CALL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /* ���ڼ����APSʵ��, �����APSʵ�����õ�CID, ����ERROR */
    TAF_APS_SndCallOrigCnf(&(pstCallOrigReq->stCtrl),
                          pstCallOrigReq->stDialParaInfo.ucCid,
                          TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_CALL_END_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallEndReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* ֹͣ���༤�����̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvAtPsCallModifyReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_MODIFY_REQ_STRU        *pstCallModifyReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallModifyReq                    = (TAF_PS_CALL_MODIFY_REQ_STRU*)(pstAppMsg->aucContent);

    /* ����ȥ�����APSʵ��, �޸ĸ�APSʵ��, ����ERROR */
    TAF_APS_SndCallModifyCnf(&(pstCallModifyReq->stCtrl),
                            TAF_PS_CAUSE_CID_INVALID);

    return VOS_TRUE;
}
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpDeactivInd_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_PMC_SMREG_PDP_DEACTIV_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_PMC_SMREG_PDP_DEACTIV_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSmPdpDeactivInd_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    SMREG_PDP_DEACTIVATE_IND_STRU      *pstSmPdpDeactivateInd;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId                 = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity            = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
    pstSmPdpDeactivateInd   = (SMREG_PDP_DEACTIVATE_IND_STRU *)pstMsg;

    /* ID_PMC_SMREG_PDP_DEACTIV_IND��Ϣ����˵��,��״̬���в���Ҫ��TEARDOWN
       �ٽ����жϴ���,APS���յ�ID_PMC_SMREG_PDP_DEACTIV_IND��Ϣ����Ԥ����,
       �����Ϣ�д���TEARDOWN,Ԥ�������ҳ�������ЩPDP��ȥ����,Ȼ��,��
       ID_PMC_SMREG_PDP_DEACTIV_IND�ַ�����Ӧ��ʵ��״̬��,����,��״̬���в���
       ��ID_PMC_SMREG_PDP_DEACTIV_IND�е�TEARDOWN�������� */

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ֹͣ����ͳ�� */
    TAF_APS_StopDsFlowStats(pstPdpEntity->ucNsapi);

#if (FEATURE_ON == FEATURE_LTE)
    /* ͬ��PDP��Ϣ��ESM */
    MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_DEACTIVATE);
#endif

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ�¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstSmPdpDeactivateInd->enCause));

    /* �ͷ�SNDCP��Դ */
    Aps_ReleaseSndcpResource(ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSndcpActivateRsp_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�APS_SN_ACT_RSP_MSG_TYPE��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSndcpActivateRsp_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    APS_SNDCP_ACTIVATE_RSP_ST          *pstSnActivateRsp;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ulResult                            = VOS_OK;
    pstSnActivateRsp                    = &((APS_SNDCP_ACTIVATE_RSP_MSG*)pstMsg)->ApsSnActRsp;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity                        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* �����Ϣ���� */
    ulResult = Aps_SnMsgModSnActRspParaCheck(pstSnActivateRsp);
    if (APS_PARA_VALID != ulResult)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvSndcpActivateRsp_NwActivating_WaitSndcpActivateRsp: Check para failed!");

        /* �ϱ�����ʧ�� */
        TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);

        return VOS_TRUE;
    }

    /* �յ�SN_ACT_RSP���޸�APSʵ�����, ����XID����, TRANSMODE */
    Aps_SnActRspChngEntity(pstSnActivateRsp, ucPdpId);

    /* ����RABM�Ĵ���ģʽ */
    TAF_APS_SndRabmSetTransModeMsg(pstSnActivateRsp->ucNsapi,
                                   pstPdpEntity->GprsPara.TransMode);

    /* �����ǰAPSʵ���PDP����ΪIPv4, ��Ҫ����IPF, Ŀǰֻ֧��Primary PDP */
    if ( (TAF_APS_CheckPrimaryPdp(ucPdpId))
      && (TAF_APS_CheckPdpAddrTypeIpv4(ucPdpId)) )
    {
        /* ����IP������ */
        TAF_APS_IpfConfigUlFilter(ucPdpId);
    }

    /* ����ɹ�����������ͳ�� */
    TAF_APS_StartDsFlowStats(pstSnActivateRsp->ucNsapi);

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF�¼� */
    TAF_APS_SndPdpActivateCnf(ucPdpId, TAF_APS_GetPdpEntCurrCid(ucPdpId));

    /* ��״̬Ǩ����TAF_APS_STA_ACTIVE, �˳���״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_ACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSndcpStatusReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�APS_SN_STATUS_REQ_MSG_TYPE��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : APS_SN_STATUS_REQ_MSG_TYPE��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvSndcpStatusReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucPdpId;
    APS_SNDCP_STATUS_REQ_ST            *pstSnStatusReq;

    pstSnStatusReq                      = &((APS_SNDCP_STATUS_REQ_MSG*)pstMsg)->ApsSnStatusReq;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    ulRet = TAF_APS_ValidatePdpForSnStatusReq(ucPdpId, pstSnStatusReq);
    if (VOS_TRUE == ulRet)
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /*�ϱ�����ʧ�ܲ�����Դ*/
        TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvApsInterPdpDeactivateReq_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             ID_APS_APS_INTERNAL_PDP_DEACTIVATE_REQ��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��13��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TAF_APS_RcvApsInterPdpDeactivateReq_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*---------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_APS_APS_INTERNAL_PDP_DEACTIVATE_REQ��Ϣ
    ---------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvApsLocalPdpDeactivateInd_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_APS_APS_LOCAL_PDP_DEACTIVATE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
             pstMsg     : ID_APS_APS_LOCAL_PDP_DEACTIVATE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
             VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 �޸���ʷ      :
  1.��    ��   : 2012��06��07��
    ��    ��   : f00179208
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvApsLocalPdpDeactivateInd_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU  *pstLocalMsg;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId      = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
    pstLocalMsg  = (TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU *)pstMsg;

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* ֹͣ����ͳ�� */
    TAF_APS_StopDsFlowStats(pstPdpEntity->ucNsapi);

#if (FEATURE_ON == FEATURE_LTE)
    /* ͬ��PDP��Ϣ��ESM */
    MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_DEACTIVATE);
#endif

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ�¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_APS_MapSmCause(pstLocalMsg->enCause));

    /* �ͷ�SNDCP��Դ */
    Aps_ReleaseSndcpResource(ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_MS_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ�Ĵ���
 �������  : ulEventType:��Ϣ����
              pstMsg     : ID_MSG_TAF_PS_INTER_SERVICE_STATUS_CHANGE_IND��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_FALSE:������Ϣʧ��
              VOS_TRUE:������Ϣ�ɹ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��25��
    ��    ��   : g00261581
    �޸�����   : �޸������Ϣ
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvInterServiceStatusChangeInd_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU           *pstServStaChangeInd;
    VOS_UINT8                                               ucPdpId;
    TAF_APS_RAT_TYPE_ENUM_UINT32                            enLastRatType;                              /* ��ѯ���صĵ�ǰ���� */

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstServStaChangeInd                 = (TAF_APS_INTER_SERVICE_STATUS_CHANGE_IND_STRU*)pstMsg;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��¼ǰһ�ε��������� */
    enLastRatType = pstServStaChangeInd->enLastRatType;

    /* ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬
       ���enRatTypeΪTAF_APS_RAT_TYPE_NULL��
       ˵�����������л�����Ϊ���쳣��� */
    if (TAF_APS_RAT_TYPE_NULL == pstServStaChangeInd->enRatType)
    {
        /* ��Ӧ�ûعҶ�ָʾ */
        TAF_APS_SndPdpDisconnectInd(ucPdpId);

        /* ��SM����Abort */
        if ((TAF_APS_RAT_TYPE_GSM   == enLastRatType)
         || (TAF_APS_RAT_TYPE_WCDMA == enLastRatType))
        {
            TAF_APS_SndSmPdpAbortReq(ucPdpId);
        }

#if (FEATURE_ON == FEATURE_LTE)
        /* ��ESM����Abort */
        if (TAF_APS_RAT_TYPE_LTE == enLastRatType)
        {
            TAF_APS_SndL4aAbortReq(TAF_APS_GetPdpEntCurrCid(ucPdpId));
        }
#endif

        /* ͣ������ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

        /* �ͷ���Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        /* �˳���״̬�� */
        TAF_APS_QuitCurrSubFsm();
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitSndcpActivateRsp
 ��������  : TAF_APS_MS_ACTIVATING_SUBSTA_WAIT_SNDCP_ACTIVATE_RSP��״̬��
             �յ�TI_TAF_APS_NET_ACTIVATING��Ϣ�Ĵ���
 �������  : ulEventType                - ��ϢID
             pstMsg                     - ��Ϣָ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE                   - ��Ϣ�����ɹ�
             VOS_FALSE                  - ��Ϣ����ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��25��
    ��    ��   : A00165503
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_UINT32 TAF_APS_RcvTiNwActivatingExpired_NwActivating_WaitSndcpActivateRsp(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ�¼� */
    TAF_APS_SndPdpActivateRej(ucPdpId, TAF_PS_CAUSE_SM_MAX_TIME_OUT);

    /* �����ڲ���Ϣ, ����PDPȥ�������� */
    TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_REGULAR_DEACTIVATION);

    return VOS_TRUE;
}
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_ManualAns
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_INIT��״̬���ֶ�Ӧ����
             �յ�D_PMC_SMREG_PDP_ACT_IND��Ϣ�Ĵ���
 �������  : VOS_UINT8                           ucPdpId
             SMREG_PDP_ACTIVATE_IND_STRU        *pstSmPdpActivateInd
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��10��
    ��    ��   : ³��/l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_ManualAns(
    VOS_UINT8                           ucPdpId,
    SMREG_PDP_ACTIVATE_IND_STRU        *pstSmPdpActivateInd
)
{
    VOS_UINT32                          ulRet;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;

    ulRet = TAF_APS_SndPdpManageInd(ucPdpId, pstSmPdpActivateInd);

    if (TAF_APS_SUCC == ulRet)
    {
        /* Ǩ����״̬����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER */
        TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER);

        /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
        enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                               ucPdpId);
        if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
        {
            /* �𱣻���ʱ�� */
            TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                               TI_TAF_APS_NET_ACTIVATING_LEN,
                               ucPdpId);
        }
    }
    else
    {
        /*�ֶ�Ӧ��ܾ�����*/
        Aps_ActIndRejRsp(ucPdpId);

        /* ͣ������ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

        /* �ͷ���Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

        /* �˳���״̬�� */
        TAF_APS_QuitCurrSubFsm();
    }

    return;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_AutoAns
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_INIT��״̬���Զ�Ӧ����
 �������  : VOS_UINT8                           ucAnswerType
             VOS_UINT8                           ucPdpId
             SMREG_PDP_ACTIVATE_IND_STRU        *pstSmPdpActivateInd
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��10��
    ��    ��   : ³��/l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_AutoAns(
    VOS_UINT8                           ucAnswerType,
    VOS_UINT8                           ucPdpId,
    SMREG_PDP_ACTIVATE_IND_STRU        *pstSmPdpActivateInd
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulActReqRslt;
    APS_PDP_ACT_REQ_ST                  stApsPdpActReq;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;

    PS_MEM_SET(&stApsPdpActReq, 0x00, sizeof(stApsPdpActReq));

    ulRet = Aps_ActIndAutoAns(ucAnswerType, ucPdpId, pstSmPdpActivateInd, &stApsPdpActReq);

    if (TAF_APS_SUCC == ulRet)
    {
        /* ���𼤻����� */
        ulActReqRslt = Aps_PdpActReq(&stApsPdpActReq);

        if (TAF_APS_SUCC == ulActReqRslt)
        {
            /* Ǩ����״̬����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF */
            TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF);

            /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
            enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                                   ucPdpId);
            if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
            {
                /* �𱣻���ʱ�� */
                TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                                   TI_TAF_APS_NET_ACTIVATING_LEN,
                                   ucPdpId);
            }

            return;
        }
    }

    /* �Զ�Ӧ��ܾ����� */
    Aps_ActIndRejRsp(ucPdpId);

    /* ͣ������ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

    /* �ͷ���Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    /* �˳���״̬�� */
    TAF_APS_QuitCurrSubFsm();

    return;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_INIT��״̬��
             �յ�D_PMC_SMREG_PDP_ACT_IND��Ϣ�Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���
  2.��    ��   : 2015��4��2��
    ��    ��   : w00316404
    �޸�����   : clean coverity
*****************************************************************************/
VOS_VOID TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode(VOS_VOID)
{
    VOS_UINT8                           ucPdpId;
    TAF_APS_ENTRY_MSG_STRU             *pstEntryMsg;
    SMREG_PDP_ACTIVATE_IND_STRU        *pstSmPdpActivateInd;
    TAF_PDP_ANSWER_MODE_ENUM_UINT8      ucAnswerMode;                           /*���Զ�Ӧ��, �����ֶ�Ӧ��*/
    TAF_PDP_ANSWER_TYPE_ENUM_UINT8      ucAnswerType;                           /*�Զ�Ӧ���ǽ���,���Ǿܾ�*/

    ucAnswerType                        = TAF_PDP_ANSWER_TYPE_BUTT;
    ucAnswerMode                        = TAF_PDP_ANSWER_MODE_BUTT;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstEntryMsg                         = TAF_APS_GetCurrSubFsmMsgAddr();
    pstSmPdpActivateInd                 = (SMREG_PDP_ACTIVATE_IND_STRU *)(pstEntryMsg->aucEntryMsgBuffer);
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /*��ѯ���Զ�Ӧ�����ֶ�Ӧ��, �Լ��Զ�Ӧ��Ļ�, ���Զ����ܻ��Ǿܾ� */
    Aps_GetPsAnswerMode(&ucAnswerMode, &ucAnswerType);

    if (TAF_PDP_ANSWER_MODE_MANUAL == ucAnswerMode)
    {
        TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_ManualAns(ucPdpId,
                                                                  pstSmPdpActivateInd);
    }
    else
    {
        TAF_APS_RcvSmPdpActivateInd_NwActivating_GuMode_AutoAns(ucAnswerType,
                                                                ucPdpId,
                                                                pstSmPdpActivateInd);
    }

    return;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallAnswerReq_NwActivating_GuMode
 ��������  : GUģ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER��״̬��
             �յ�ID_MSG_TAF_PS_CALL_ANSWER_REQ��Ϣ�Ĵ���
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : l60609
    �޸�����   : �����ɺ���
  2.��    ��   : 2012��8��10��
    ��    ��   : L00171473
    �޸�����   : DTS2012082204471, TQE����
  3.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�
*****************************************************************************/
VOS_VOID TAF_APS_RcvAtPsCallAnswerReq_NwActivating_GuMode(struct MsgCB *pstMsg)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ANSWER_REQ_STRU        *pstCallAnswerReq;
    APS_PDP_ACT_REQ_ST                  stApsActReq;
    VOS_UINT32                          ulRet;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;


    PS_MEM_SET(&stApsActReq, 0x00, sizeof(stApsActReq));


    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg            = (TAF_PS_MSG_STRU*)(pstMsg);
    pstCallAnswerReq     = (TAF_PS_CALL_ANSWER_REQ_STRU*)(pstAppMsg->aucContent);

    /*��ȡPDP ID*/
    ucPdpId              = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���PS��״̬, �����Ч, ֱ�ӷ���ERROR */
    if (VOS_FALSE == TAF_APS_GetCurrPdpEntitySimRegStatus())
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);
        /* �ϱ�PDP������� */
        TAF_APS_SndCallAnswerCnf(&(pstCallAnswerReq->stCtrl),
                                pstCallAnswerReq->stAnsInfo.ucCid,
                                TAF_PS_CAUSE_SIM_INVALID);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
        TAF_APS_QuitCurrSubFsm();

        return;
    }

    Aps_MakeAnsActReq(pstCallAnswerReq->stAnsInfo.ucCid, ucPdpId, &stApsActReq);

    /*����PDP����*/
    ulRet = Aps_PdpActReq(&stApsActReq);

    if (TAF_APS_FAIL == ulRet)
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /* ����PDP������� */
        TAF_APS_SndCallAnswerCnf(&(pstCallAnswerReq->stCtrl),
                                pstCallAnswerReq->stAnsInfo.ucCid,
                                TAF_ERR_UNSPECIFIED_ERROR);

        /* ���APS��Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
        TAF_APS_QuitCurrSubFsm();
    }
    else
    {
        /* ����PDP������� */
        TAF_APS_SndCallAnswerCnf(&(pstCallAnswerReq->stCtrl),
                                pstCallAnswerReq->stAnsInfo.ucCid,
                                TAF_ERR_NO_ERROR);

        /* ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF */
        TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF);

        /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
        enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                               ucPdpId);

        if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
        {
            /* �𱣻���ʱ�� */
            TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                               TI_TAF_APS_NET_ACTIVATING_LEN,
                               ucPdpId);
        }
    }

    return;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallHangUpReq_NwActivating_GuMode
 ��������  : GUģ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_HANGUP_REQ��Ϣ�Ĵ���
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��29��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TAF_APS_RcvAtPsCallHangUpReq_NwActivating_GuMode(struct MsgCB *pstMsg)
{
    VOS_UINT8                           ucPdpId;

    /*��ȡPDP ID*/
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���缤���ֶ�Ӧ����, �����Ϣʵ��Ϊ: APP�ܾ����緢��ļ��� */
    Aps_ActIndRejRsp(ucPdpId);

    /* ֹͣ�������̶�ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                      ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
    TAF_APS_QuitCurrSubFsm();

    return;
}


#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_RcvL4aPdpActivateInd_NwActivating_LteMode
 ��������  : TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER��״̬��
             �յ�ID_APP_ESM_PDP_MANAGER_IND��Ϣ�Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��12��17��
    ��    ��   : s62952
    �޸�����   : �����ɺ���
  2.��    ��   : 2013��07��08��
    ��    ��   : Y00213812
    �޸�����   : VoLTE_PhaseI ��Ŀ��EVENT�ṹ�滻
  3.��    ��   : 2015��12��7��
    ��    ��   : w00316404
    �޸�����   : Split L4A Project

*****************************************************************************/
VOS_VOID TAF_APS_RcvEsmPdpManageInd_NwActivating_LteMode(VOS_VOID)
{
    TAF_APS_ENTRY_MSG_STRU             *pstEntryMsg;
    APP_ESM_PDP_MANAGER_IND_STRU       *pstEsmPdpManageInd;
    TAF_PS_CALL_PDP_MANAGE_IND_STRU     stPdpActIndEvt;
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT8                           ucPdpId;
    TAF_PS_CALL_ANSWER_REQ_STRU         stCallAnswerReq;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;
#endif
    /*---------------------------------------------------------
       LTE�£����緢��ļ���������Զ�Ӧ�����ֶ�Ӧ����
       ESM��ʵ�֣�APSֻ����͸������aps�յ�ID_L4A_APS_PDP_ACTIVATE_IND
       ʱ��APS��ACTIVATE IND�¼��ϱ���AT��AT�ϱ�Ӧ��RING��CRING
       �������壬APS����Ҫ����PDP�����ġ�
    ---------------------------------------------------------*/

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstEntryMsg                         = TAF_APS_GetCurrSubFsmMsgAddr();
    pstEsmPdpManageInd                  = (APP_ESM_PDP_MANAGER_IND_STRU*)(pstEntryMsg->aucEntryMsgBuffer);

#if (FEATURE_ON == FEATURE_IMS)
    /* �жϵ�ǰ�Ƿ����IMSר�г��أ��ж�PDP��������Ϣ�е�ucLinkedRabid�Ƿ���IMSר�г�����ͬ */
    if (VOS_TRUE == pstEsmPdpManageInd->bitOpLinkCid)
    {
        if (VOS_TRUE == TAF_APS_CheckImsBearerByCid((VOS_UINT8)pstEsmPdpManageInd->ulLinkCid))
        {
            /*��ȡPDP ID*/
            ucPdpId       = TAF_APS_GetCurrFsmEntityPdpId();

            /* ����ID_APP_ESM_PDP_MANAGER_RSP��ESM */
            PS_MEM_SET(&stCallAnswerReq, 0, sizeof(TAF_PS_CALL_ANSWER_REQ_STRU));
            stCallAnswerReq.stCtrl.ucOpId       = (VOS_UINT8)pstEsmPdpManageInd->ulOpId;
            stCallAnswerReq.stAnsInfo.ucCid     = (VOS_UINT8)pstEsmPdpManageInd->ulCid;

            /*��Lģ����Ӧ������Lģ���ز���ܾ�*/
            TAF_APS_SndEsmPdpManagerRsp(APS_ESM_BEARER_ACT_ACCEPT, stCallAnswerReq.stAnsInfo.ucCid);

            /* Ǩ����״̬����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF */
            TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF);


            /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
            enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                                   ucPdpId);

            if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
            {
                /* �𱣻���ʱ�� */
                TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                                   TI_TAF_APS_NET_ACTIVATING_LEN,
                                   ucPdpId);
            }

            return;
        }
    }
#endif

    /* �����ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVE_IND�¼� */
    TAF_APS_FillCallEvtPdpAvtivateIndFromEsm(&stPdpActIndEvt,
                                             pstEsmPdpManageInd);

    /* �ϱ��¼� */
    /*����TAFM�ṩ���¼��ϱ����� */
    (VOS_VOID)TAF_APS_SndPsEvt(ID_EVT_TAF_PS_CALL_PDP_MANAGE_IND,
                     &stPdpActIndEvt,
                     sizeof(TAF_PS_CALL_PDP_MANAGE_IND_STRU));

    /* Ǩ����״̬����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER */
    TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER);

    return;
}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallAnswerReq_NwActivating_LteMode
 ��������  : Lģ��TAF_APS_MS_ACTIVATING_SUBSTA_INIT��״̬��
             �յ�ID_MSG_TAF_PS_CALL_ANSWER_REQ��Ϣ�Ĵ���
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��1��12��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2012��8��22��
    ��    ��   : Y00213812
    �޸�����   : �޸�PS��������ϱ�

  3.��    ��   : 2015��12��7��
    ��    ��   : w00316404
    �޸�����   : Split L4A Project
*****************************************************************************/
VOS_VOID TAF_APS_RcvAtPsCallAnswerReq_NwActivating_LteMode(struct MsgCB *pstMsg)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ANSWER_REQ_STRU        *pstCallAnswerReq;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg            = (TAF_PS_MSG_STRU*)(pstMsg);
    pstCallAnswerReq     = (TAF_PS_CALL_ANSWER_REQ_STRU*)(pstAppMsg->aucContent);

    /*��ȡPDP ID*/
    ucPdpId              = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���PS��״̬, �����Ч, ֱ�ӷ���ERROR */
    if (VOS_FALSE == TAF_APS_GetCurrPdpEntitySimRegStatus())
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /* �ϱ�PDP������� */
        TAF_APS_SndCallAnswerCnf(&(pstCallAnswerReq->stCtrl),
                                pstCallAnswerReq->stAnsInfo.ucCid,
                                TAF_PS_CAUSE_SIM_INVALID);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
        TAF_APS_QuitCurrSubFsm();

        return;
    }

    /* �û������CID��һ����LTEĿǰ��ʹ�õ�CID���������ˢ��һ��CID��
       Lģ��Ȼʹ���ϱ�ʱ��ʹ�õ�CIDֵ */
    pstCallAnswerReq->stAnsInfo.ucCid = TAF_APS_GetPdpEntCurrCid(ucPdpId);

    /*��Lģ����Ӧ������Lģ���ز���ܾ�*/
    TAF_APS_SndEsmPdpManagerRsp(APS_ESM_BEARER_ACT_ACCEPT, pstCallAnswerReq->stAnsInfo.ucCid);

    /* Ǩ����״̬����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF */
    TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_ESM_CGANS_CNF);

    /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
    enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                           ucPdpId);

    if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
    {
        /* �𱣻���ʱ�� */
        TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                           TI_TAF_APS_NET_ACTIVATING_LEN,
                           ucPdpId);
    }

    return;

}

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsCallHangUpReq_NwActivating_LteMode
 ��������  : Lģ��TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_CALL_HANGUP_REQ��Ϣ�Ĵ���
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��1��29��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��12��7��
    ��    ��   : w00316404
    �޸�����   : Split L4A Project

*****************************************************************************/
VOS_VOID TAF_APS_RcvAtPsCallHangUpReq_NwActivating_LteMode(struct MsgCB *pstMsg)
{
    VOS_UINT8                           ucPdpId;

    /*��ȡPDP ID*/
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /*��LTE���͹Ҷ�����Lģ��֤�Ҷϲ���ʧ��*/
    TAF_APS_SndEsmPdpManagerRsp(APS_ESM_BEARER_ACT_REJ, 0);

    /* ͣ������ʱ�� */
    TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING, ucPdpId);

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    /* �˳���״̬�� */
    TAF_APS_QuitCurrSubFsm();

    return;
}

#endif

/*****************************************************************************
 �� �� ��  : TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitAppAnswer_GuMode
 ��������  : GUģ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_APP_ANSWER״̬��
             �յ�ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ�Ĵ���
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��6��7��
    ��    ��   : l60609
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TAF_APS_RcvAtPsPppDialOrigReq_NwActivating_WaitAppAnswer_GuMode(struct MsgCB *pstMsg)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;
    APS_PDP_ACT_REQ_ST                  stApsActReq;
    VOS_UINT32                          ulRet;
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus;

    PS_MEM_SET(&stApsActReq, 0x00, sizeof(stApsActReq));

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg            = (TAF_PS_MSG_STRU*)(pstMsg);
    pstPppDialOrigReq    = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);

    /*��ȡPDP ID*/
    ucPdpId              = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���PS��״̬, �����Ч, ֱ�ӷ���ERROR */
    if (VOS_FALSE == TAF_APS_GetCurrPdpEntitySimRegStatus())
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /* �ϱ�PDP������� */
        TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                                 TAF_PS_CAUSE_SIM_INVALID);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
        TAF_APS_QuitCurrSubFsm();

        return;
    }

    TAF_APS_MakeIpTypeAnsExtActReq(ucPdpId, pstPppDialOrigReq, &stApsActReq);

    /*����PDP����*/
    ulRet = Aps_PdpActReq(&stApsActReq);

    if (TAF_APS_FAIL == ulRet)
    {
        /* ֹͣ�������̶�ʱ�� */
        TAF_APS_StopTimer(TI_TAF_APS_NET_ACTIVATING,
                          ucPdpId);

        /* ����PDP������� */
        TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                                 TAF_PS_CAUSE_UNKNOWN);

        /* ���APS��Դ */
        Aps_ReleaseApsResource(ucPdpId);

        /* ��״̬Ǩ����TAF_APS_STA_INACTIVE, �˳���״̬�� */
        TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);
        TAF_APS_QuitCurrSubFsm();
    }
    else
    {
        /* ����PDP������� */
        TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                                 TAF_PS_CAUSE_SUCCESS);

        /* ��״̬Ǩ����TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF */
        TAF_APS_SetCurrPdpEntitySubFsmState(TAF_APS_NW_ACTIVATING_SUBSTA_WAIT_SM_ACTIVATE_CNF);

        /* ��鶨ʱ���Ƿ���������, �����������, ����Ҫ�������� */
        enTimerStatus = TAF_APS_GetTimerStatus(TI_TAF_APS_NET_ACTIVATING,
                                               ucPdpId);

        if (TAF_APS_TIMER_STATUS_STOP == enTimerStatus)
        {
            /* �𱣻���ʱ�� */
            TAF_APS_StartTimer(TI_TAF_APS_NET_ACTIVATING,
                               TI_TAF_APS_NET_ACTIVATING_LEN,
                               ucPdpId);
        }
    }

    return;
}

/*lint +e958*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif









