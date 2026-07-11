//fake function signatures
void OSDumpStopwatch(void);
void PSVECDotProduct(void);
void PSVECDistance(void);
void PSVECSquareDistance(void);
void VIGetDTVStatus(void);
void OSGetProgressiveMode(void);
void OSSetProgressiveMode(void);
void PSVECSubtract(void);
void PSMTXTranspose(void);
void C_VECReflect(void);
void PSMTXInvXpose(void);
void PSMTXRotRad(void);
void PSMTXRotAxisRad(void);
void PSVECMag(void);
void PSVECScale(void);
void PSMTXMultVecSR(void);
void C_QUATRotAxisRad(void);
void C_QUATMtx(void);
void C_QUATAdd(void);
void C_QUATSlerp(void);
void PSQUATDotProduct(void);
void PSQUATAdd(void);
void PSMTXQuat(void);
void PSQUATMultiply(void);
void PSQUATInverse(void);
void PSQUATNormalize(void);
void PSQUATScale(void);
void PSQUATSubtract(void);
void OSTicksToCalendarTime(void);
void PSMTXTransApply(void);
void PSMTXScaleApply(void);
void OSGetLanguage(void);
void PSMTXMultVecArraySR(void);
void PSMTX44MultVec(void);
void SIGetTypeString(void);
void OSCheckActiveThreads(void);
void OSGetThreadPriority(void);
void OSInitSemaphore(void);
void OSIsThreadTerminated(void);
void OSSignalSemaphore(void);
void OSWaitSemaphore(void);
void log(void);
void exp(void);
void GXWaitDrawDone(void);
void GXProject(void);
void GXGetProjectionv(void);
void GXGetViewportv(void);
void GXPixModeSync(void);
void GXSetIndTexOrder(void);
void GXSetIndTexMtx(void);
void GXSetTevIndWarp(void);
void GXSetTexCoordScaleManually(void);
void GXSetTevIndTile(void);
void GXNtsc480Prog(void);
void GXNtsc480Int(void);
void GXNtsc240Int(void);
void GXSetVtxDescv(void);
void GXDrawSphere(void);
void GXDrawCylinder(void);
void omSetStatBit(void);
void GameMesKillCheck(void);
void WipeCheck(void);
void WipeTypeGet(void);
void omMakeGroupEx(void);
void omGetGroupMemberListEx(void);
void GameMesDispSet(void);
void GameMesKill(void);
void ClusterAdjustObject(void);
void printWin(void);
void print8(void);
void HuSysVWaitSet(void);
void GameMesGet(void);
void omObjManPause(void);
void GameMesInit(void);
void GameMesCreate(void);
void omDelObjEx(void);
void GameMesPosSet(void);
void atan2(void);
void atan2f(void);
void cos(void);
void GameMesStatGet(void);
void Center(void);
void CRotM(void);
void CenterM(void);
void CZoomM(void);
void omOutViewMulti(void);
void omOutView(void);
void omAddObjEx(void);
void omInitObjMan(void);
void rand8(void);
void frandmod(void);
void frandom(void);
void SetDefLight(void);
void HuSetVecF(void);
void HuSubVecF(void);
void frandf(void);
void GameMesSprKill(void);
void mtxRotCat(void);
void MgScoreBoxInit(void);
void MgScoreBoxCreate(void);
void MgScoreBoxCreateChar(void);
void MgScoreBoxCreateCharMulti(void);
void MgScoreBoxColorSet(void);
void MgScoreBoxTPLvlSet(void);
void MgScoreBoxPosSet(void);
void MgScoreBoxDispSet(void);
void GameMesStrWinCreate(void);
void Hu3DCameraLayerHookSet(void);
void Hu3DCameraLayerHookReset(void);
void Hu3DLayerHookSet(void);
void Hu3DLayerHookReset(void);
void Hu3DNoSyncSet(void);
void Hu3DModelCreate(void);
void Hu3DModelLink(void);
void Hu3DHookFuncCreate(void);
void Hu3DModelPosSet(void);
void Hu3DModelPosSetV(void);
void Hu3DModelRotSet(void);
void Hu3DModelRotSetV(void);
void Hu3DModelScaleSet(void);
void Hu3DModelScaleSetV(void);
void Hu3DModelPosGet(void);
void Hu3DModelRotGet(void);
void Hu3DModelScaleGet(void);
void Hu3DModelKill(void);
void Hu3DModelAllKill(void);
void Hu3DModelAttrSet(void);
void Hu3DModelAttrReset(void);
void Hu3DModelAttrGet(void);
void Hu3DModelMotionAttrGet(void);
void Hu3DBGColorSet(void);
void Hu3DModelCameraSet(void);
void Hu3DModelLayerSet(void);
void Hu3DModelTPLvlSet(void);
void Hu3DModelHookSet(void);
void Hu3DModelHookReset(void);
void Hu3DModelHookObjReset(void);
void Hu3DModelAmbSet(void);
void Hu3DModelShadowDispOn(void);
void Hu3DModelShadowDispOff(void);
void Hu3DModelProjectionSet(void);
void Hu3DModelProjectionReset(void);
void Hu3DModelObjPosGet(void);
void Hu3DModelObjMtxGet(void);
void Hu3DModelHiliteTypeSet(void);
void Hu3DModelReflectTypeSet(void);
void Hu3DModelHiliteMapSet(void);
void Hu3DReflectModelSet(void);
void Hu3DModelMatHookSet(void);
void Hu3DModelDebug(void);
void Hu3DModelMtxSet(void);
void Hu3DModelMtxGet(void);
void Hu3DModelShadowMapTPLvlSet(void);
void Hu3DModelShadowMapObjTPLvlSet(void);
void Hu3DModelDieCreate(void);
void Hu3DModelLinkDieCreate(void);
void HuPadInit(void);
void HuPadRead(void);
void HuPadRumbleSet(void);
void HuPadRumbleStop(void);
void HuPadRumbleAllStop(void);
void HuPadStatGet(void);
void HuPadRumbleGet(void);
void HuPadStkDirGet(void);
void HuPadSubStkDirGet(void);
void Hu3DCameraCreate(void);
void Hu3DCameraPerspectiveSet(void);
void Hu3DCameraViewportSet(void);
void Hu3DCameraScissorSet(void);
void Hu3DCameraPosSet(void);
void Hu3DCameraKill(void);
void Hu3DCameraPosGet(void);
void Hu3DCameraPosSetV(void);
void Hu3DCameraAllKill(void);
void Hu3DCameraSet(void);
void Hu3DCameraMotionOn(void);
void Hu3DCameraMotionStart(void);
void Hu3DCameraMotionOff(void);
void Hu3DModelCameraCreate(void);
void Hu3DModelCameraInfoSet(void);
void Hu3DCameraPerspectiveGet(void);
void Hu3DLighInit(void);
void Hu3DGLightCreate(void);
void Hu3DGLightCreateV(void);
void Hu3DGLightKill(void);
void Hu3DLightAllKill(void);
void Hu3DLightSet(void);
void Hu3DGLightSpotSet(void);
void Hu3DGLightInfinitytSet(void);
void Hu3DGLightPointSet(void);
void Hu3DGLightColorSet(void);
void Hu3DGLightPosSetV(void);
void Hu3DGLightPosSet(void);
void Hu3DGLightPosAimSetV(void);
void Hu3DGLightPosAimSet(void);
void Hu3DGLightStaticSet(void);
void Hu3DLLightParamGet(void);
void Hu3DGLightParamGet(void);
void Hu3DGLightPosAngleSet(void);
void Hu3DModelLightBitSet(void);
void Hu3DModelLightBitReset(void);
void Hu3DModelLightInfoSet(void);
void Hu3DModelLightIdGet(void);
void Hu3DReflectMapSet(void);
void Hu3DReflectNoSet(void);
void Hu3DFogSet(void);
void Hu3DFogClear(void);
void Hu3DModelObjPtrGet(void);
void Hu3DModelShadowSet(void);
void Hu3DModelShadowReset(void);
void Hu3DShadowCreate(void);
void Hu3DShadowPosSet(void);
void Hu3DShadowTPLvlSet(void);
void Hu3DShadowColSet(void);
void Hu3DShadowMultiCreate(void);
void Hu3DShadowMultiPosSet(void);
void Hu3DShadowMultiSizeSet(void);
void Hu3DShadowMultiColSet(void);
void Hu3DShadowMultiTPLvlSet(void);
void Hu3DModelShadowMapSet(void);
void Hu3DModelShadowMapObjSet(void);
void Hu3DModelShadowMapObjReset(void);
void Hu3DModelShadowMapReset(void);
void Hu3DShadowSizeSet(void);
void Hu3DMipMapSet(void);
void Hu3DModelClusterAttrSet(void);
void Hu3DModelClusterAttrReset(void);
void Hu3DLLightCreate(void);
void Hu3DLLightCreateV(void);
void Hu3DLLightSpotSet(void);
void Hu3DLLightInfinitytSet(void);
void Hu3DLLightPointSet(void);
void Hu3DLLightKill(void);
void Hu3DLLightColorSet(void);
void Hu3DLLightPosSetV(void);
void Hu3DLLightPosSet(void);
void Hu3DLLightPosAimSetV(void);
void Hu3DLLightPosAimSet(void);
void Hu3DLLightStaticSet(void);
void Hu3DLLightPosAngleSet(void);
void Hu3DShineSet(void);
void Hu3DObjHookSet(void);
void Hu3DAnimInit(void);
void Hu3DAnimCreate(void);
void Hu3DAnimAnimSet(void);
void Hu3DAnimLink(void);
void Hu3DAnimKill(void);
void Hu3DAnimAllKill(void);
void Hu3DAnimSet(void);
void Hu3DAnimExec(void);
void Hu3DAnimAttrSet(void);
void Hu3DAnimAttrReset(void);
void Hu3DAnimSpeedSet(void);
void Hu3DAnimBankSet(void);
void Hu3DAnmNoSet(void);
void Hu3DTexScrollCreate(void);
void Hu3DTexScrollPosSet(void);
void Hu3DTexScrollPosMoveSet(void);
void Hu3DTexScrollRotSet(void);
void Hu3DTexScrollRotMoveSet(void);
void Hu3DTexScrollKill(void);
void Hu3DTexScrollAllKill(void);
void Hu3DTexScrollPauseDisableSet(void);
void Hu3DParticleCreate(void);
void Hu3DParticleScaleSet(void);
void Hu3DParticleZRotSet(void);
void Hu3DParticleColSet(void);
void Hu3DParticleTPLvlSet(void);
void Hu3DParticleBlendModeSet(void);
void Hu3DParticleHookSet(void);
void Hu3DParticleCntSet(void);
void Hu3DParticleAttrSet(void);
void Hu3DParticleAttrReset(void);
void Hu3DParticleAnimModeSet(void);
void Hu3DParManInit(void);
void Hu3DParManCreate(void);
void Hu3DParManKill(void);
void Hu3DParManAllKill(void);
void Hu3DParManPtrGet(void);
void Hu3DParManAttrSet(void);
void Hu3DParManAttrReset(void);
void Hu3DParManPosSet(void);
void Hu3DParManVecSet(void);
void Hu3DParManRotSet(void);
void Hu3DParManModelIDGet(void);
void Hu3DParManLink(void);
void Hu3DParManTimeLimitSet(void);
void Hu3DParManColorSet(void);
void Hu3DParManLayerSet(void);
void Hu3DWaterCreate(void);
void Hu3DWaterTexPosSet(void);
void Hu3DWaterTexScaleSet(void);
void Hu3DWaterPadYSet(void);
void Hu3DWaterGlowSet(void);
void Hu3DWaterIndTexMtxSet(void);
void Hu3DWaterHiliteSet(void);
void Hu3DWaterHiliteTPLvlSet(void);
void Hu3DWaterCameraSet(void);
void Hu3DWaterWaveCreate(void);
void Hu3DMotionCreate(void);
void Hu3DMotionInit(void);
void Hu3DMotionModelCreate(void);
void Hu3DMotionKill(void);
void Hu3DMotionAllKill(void);
void Hu3DMotionSet(void);
void Hu3DMotionExec(void);
void Hu3DSubMotionExec(void);
void Hu3DMotionNext(void);
void Hu3DMotionTimeSet(void);
void Hu3DMotionTimeGet(void);
void Hu3DMotionMaxTimeGet(void);
void Hu3DMotionStartEndSet(void);
void Hu3DMotionMotionMaxTimeGet(void);
void Hu3DMotionEndCheck(void);
void Hu3DMotionShiftSet(void);
void Hu3DMotionIDGet(void);
void Hu3DMotionShiftIDGet(void);
void Hu3DMotionShiftTimeGet(void);
void Hu3DMotionShiftMaxTimeGet(void);
void Hu3DMotionShiftStartEndSet(void);
void Hu3DMotionShiftSpeedSet(void);
void Hu3DMotionSpeedSet(void);
void Hu3DMotionShapeSpeedSet(void);
void Hu3DMotionShapeTimeSet(void);
void Hu3DMotionShapeTimeGet(void);
void Hu3DMotionNoMotSet(void);
void Hu3DMotionNoMotReset(void);
void Hu3DMotionForceSet(void);
void Hu3DMotionShapeSet(void);
void Hu3DMotionShapeIDGet(void);
void Hu3DMotionShapeMaxTimeGet(void);
void Hu3DMotionClusterSet(void);
void Hu3DMotionClusterNoSet(void);
void Hu3DMotionShapeReset(void);
void Hu3DMotionClusterReset(void);
void Hu3DJointMotion(void);
void Hu3DMotionOverlaySet(void);
void Hu3DMotionOverlayReset(void);
void Hu3DMotionOverlayTimeGet(void);
void Hu3DMotionOverlayTimeSet(void);
void Hu3DMotionOverlaySpeedSet(void);
void Hu3DMotionOverlayMaxTimeGet(void);
void Hu3DMotionOverlayEndCheck(void);
void Hu3DModelObjDrawInit(void);
void Hu3DModelObjDraw(void);
void Hu3DModelObjPtrDraw(void);
void Hu3DMotionCalc(void);
void Hu3DMotionShapeStartEndSet(void);
void Hu3DSubMotionSet(void);
void Hu3DSubMotionReset(void);
void Hu3DSubMotionTimeSet(void);
void Hu3DMotionShiftTimeSet(void);
void Hu3DMotionClusterSpeedSet(void);
void Hu3DMotionClusterTimeSet(void);
void Hu3DMotionClusterMaxTimeGet(void);
void Hu3DMotionAttrSet(void);
void Hu3DMotionAttrReset(void);
void Hu3DMotionTimingHookSet(void);
void Hu3DMotionTimingHookReset(void);
void Hu3DMatLightSet(void);
void Hu3DFbCopyLayerSet(void);
void Hu3DFbCopyExec(void);
void Hu3DZClear(void);
void Hu3DZClearLayerSet(void);
void CamMotionEx(void);
void CamMotionEx2(void);
void CamMotionExPathGet(void);
void CamMotionExPath(void);
void Hu3D2Dto3D(void);
void Hu3D3Dto2D(void);
void Hu3DMtxRotGet(void);
void Hu3DMtxTransGet(void);
void Hu3DMtxScaleGet(void);
void Hu3DProjectionCreate(void);
void Hu3DProjectionPosSet(void);
void Hu3DProjectionTPLvlSet(void);
void Hu3DProjectionKill(void);
void HuAudInit(void);
void HuAudStreamPlay(void);
void HuAudAllStop(void);
void HuAudFadeOut(void);
void HuAudStreamPauseOn(void);
void HuAudStreamPauseOff(void);
void HuAudStreamFadeOut(void);
void HuAudStreamVolSet(void);
void HuAudFXPlay(void);
void HuAudFXPlayVol(void);
void HuAudFXPlayVolPan(void);
void HuAudFXPlayPan(void);
void HuAudFXPlayDelay(void);
void HuAudFXPlayDelayVol(void);
void HuAudFXPlayDelayPan(void);
void HuAudFXPlayDelayVolPan(void);
void HuAudFXStop(void);
void HuAudFXFadeOut(void);
void HuAudFXPanning(void);
void HuAudSeqPlay(void);
void HuAudSeqStop(void);
void HuAudSeqFadeOut(void);
void HuAudSeqMidiCtrlGet(void);
void HuAudFXListnerSet(void);
void HuAudFXListnerUpdate(void);
void HuAudFXEmiterPlay(void);
void HuAudFXEmiterUpDate(void);
void HuAudFXListnerKill(void);
void HuAudFXStatusGet(void);
void HuAudFXPitchSet(void);
void HuAudFXVolSet(void);
void HuAudDllSndGrpSet(void);
void HuAudSndGrpSetSet(void);
void HuAudSndGrpSet(void);
void HuAudSndCommonGrpSet(void);
void HuAudSStreamPlay(void);
void HuAudSStreamStop(void);
void HuAudSStreamFadeOut(void);
void HuAudSStreamAllFadeOut(void);
void HuAudSStreamAllStop(void);
void HuAudSStreamStatGet(void);
void HuAudSStreamPause(void);
void HuAudSStreamPauseFadeOut(void);
void HuAudSStreamParamSet(void);
void PlayerFXPlay(void);
void CharFXPlay(void);
void CharFXPlayVol(void);
void CharFXPlayPan(void);
void CharFXPlayVolPan(void);
void CharFXPlayDelayVolPan(void);
void CharFXPlayDelay(void);
void CharFXPlayDelayVol(void);
void CharFXPlayDelayPan(void);
void PlayerFXPlayPos(void);
void CharFXPlayPos(void);
void PlayerFXStop(void);
void CharFXStop(void);
void HuAudAUXSet(void);
void HuAudSStreamChanPlay(void);
void HuAudBGMPlay(void);
void HuAudJinglePlay(void);
void msmMusPlay(void);
void espInit(void);
void espEntry(void);
void espKill(void);
void espGrpIDGet(void);
void espDispOn(void);
void espDispOff(void);
void espAttrSet(void);
void espAttrReset(void);
void espPosSet(void);
void espScaleSet(void);
void espZRotSet(void);
void espTPLvlSet(void);
void espColorSet(void);
void espSpeedSet(void);
void espBankSet(void);
void espDrawNoSet(void);
void espPriSet(void);
void HuPrcInit(void);
void HuPrcCall(void);
void HuPrcCreate(void);
void HuPrcKill(void);
void HuPrcEnd(void);
void HuPrcWakeup(void);
void HuPrcSleep(void);
void HuPrcVSleep(void);
void HuPrcCurrentGet(void);
void HuPrcDestructorSet(void);
void HuPrcDestructorSet2(void);
void HuPrcChildCreate(void);
void HuPrcChildLink(void);
void HuPrcChildUnlink(void);
void HuPrcChildKill(void);
void HuPrcMemAlloc(void);
void HuPrcMemFree(void);
void Hu3DParManVacumeSet(void);
void HuDataInit(void);
void HuDataDirRead(void);
void HuDataRead(void);
void HuDataReadNum(void);
void HuDataReadMulti(void);
void HuDataDirReadAsync(void);
void HuDataDirReadNumAsync(void);
void HuDataSelHeapRead(void);
void HuDataSelHeapReadNum(void);
void HuDataGetSize(void);
void HuDataDirClose(void);
void HuDataDirCloseNum(void);
void HuDataDirCloseAll(void);
void HuDataClose(void);
void HuDataCloseMulti(void);
void HuDataReadNumHeapShortForce(void);
void HuDataDirCancel(void);
void ARRegisterDMACallback(void);
void ARGetDMAStatus(void);
void ARStartDMA(void);
void ARInit(void);
void ARAlloc(void);
void ARFree(void);
void ARGetSize(void);
void ARSetSize(void);
void HuSprBegin(void);
void HuSprCall(void);
void HuSprFinish(void);
void HuSprAnimRead(void);
void HuSprCreate(void);
void HuSprGrpCreate(void);
void HuSprGrpMemberSet(void);
void HuSprGrpMemberKill(void);
void HuSprGrpKill(void);
void HuSprKill(void);
void HuSprAnimKill(void);
void HuSprAttrSet(void);
void HuSprAttrReset(void);
void HuSprAttrGet(void);
void HuSprPosSet(void);
void HuSprScaleSet(void);
void HuSprZRotSet(void);
void HuSprGrpPosSet(void);
void HuSprGrpZRotSet(void);
void HuSprGrpScaleSet(void);
void HuSprTPLvlSet(void);
void HuSprColorSet(void);
void HuSprSpeedSet(void);
void HuSprBankSet(void);
void HuSprGrpDrawNoSet(void);
void HuSprDrawNoSet(void);
void HuSprPriSet(void);
void HuSprFuncCreate(void);
void HuSprAnimMake(void);
void HuSprGrpCenterSet(void);
void HuSprAnimLock(void);
void AnimDebug(void);
void HuSprExecLayerSet(void);
void HuSprSprBGSet(void);
void HuSprBGSet(void);
void HuSprGrpScissorSet(void);
void HuSprScissorSet(void);
void HuSprGrpTPLvlSet(void);
void HuSprExecLayerCameraSet(void);
void HuSprAnimNoSet(void);
void HuSprAnimNoSetPause(void);
void HuSpr3DHookSet(void);
void HuSpr3DDataCreate(void);
void HuSpr3DSet(void);
void HuSpr3DRotSet(void);
void HuSpr3DFovSet(void);
void HuSprVtxColorSet(void);
void HuSprVtxColorReset(void);
void HuMemMemoryNumSet(void);
void HuARMalloc(void);
void HuARFree(void);
void HuAR_DVDtoARAM(void);
void HuAR_MRAMtoARAM(void);
void HuAR_MRAMtoARAM2(void);
void HuAR_ARAMtoMRAM(void);
void HuAR_ARAMtoMRAMNum(void);
void HuARDMACheck(void);
void HuARDirCheck(void);
void HuARDirFree(void);
void HuAR_ARAMtoMRAMFileRead(void);
void HuAudFXListnerSetEX(void);
void HuWindowInit(void);
void HuWinInit(void);
void HuWinCreate(void);
void HuWinKill(void);
void HuWinAllKill(void);
void HuWinMesSet(void);
void HuWinInsertMesSet(void);
void HuWinChoiceGet(void);
void HuWinChoiceSet(void);
void HuWinChoiceNumGet(void);
void HuWinMesWait(void);
void HuWinSprKill(void);
void HuWinMesRead(void);
void HuWinAnimSet(void);
void HuWinSprSet(void);
void HuWinSprPosSet(void);
void HuWinSprIDGet(void);
void HuWinMesMaxSizeGet(void);
void HuWinInsertMesSizeGet(void);
void HuWinMesMaxSizeBetGet(void);
void HuWinExCreate(void);
void HuWinDispOn(void);
void HuWinDispOff(void);
void HuWinExOpen(void);
void HuWinExClose(void);
void HuWinExKill(void);
void HuWinExSpeakerSet(void);
void HuWinComKeyWait(void);
void HuWinComKeySet(void);
void HuWinComKeyReset(void);
void HuWinDrawNoSet(void);
void HuWinAttrSet(void);
void HuWinAttrReset(void);
void HuWinBGTPLvlSet(void);
void HuWinMesSpeedSet(void);
void HuWinBGColSet(void);
void HuWinChoiceDisable(void);
void HuWinKeyWaitEntry(void);
void HuWinCenterPosSet(void);
void HuWinPosSet(void);
void HuWinZRotSet(void);
void HuWinScaleSet(void);
void HuWinChoiceNowGet(void);
void HuWinStatGet(void);
void HuWinMesColSet(void);
void HuWinScissorSet(void);
void HuWinExCreateFrame(void);
void HuWinPriSet(void);
void HuWinSprPriSet(void);
void HuWinKeyWaitNumGet(void);
void HuWinMesSizeCancelCRSet(void);
void HuWinMesPalSet(void);
void HuWinHomeClear(void);
void HuWinPushKeySet(void);
void HuWinDisablePlayerSet(void);
void HuWinDisablePlayerReset(void);
void HuWinMesPtrGet(void);
void HuWinMesMaxDirGet(void);
void HuWinMesMaxNumGet(void);
void HuWinMesLanguageSet(void);
void HuWinWarningCreate(void);
void HuWinWarningKill(void);
void HuWinWarningOpen(void);
void HuWinWarningClose(void);
void HuWinFrameSet(void);
void HuWinCallbackStub(void);
void HuWinCallbackSet(void);
void HuWinPadMaskSet(void);
void CharInit(void);
void CharMotionInit(void);
void CharMotionInitAsync(void);
void CharDataClose(void);
void CharModelKill(void);
void CharClose(void);
void CharModelCreate(void);
void CharMotionCreate(void);
void CharMotionNoSet(void);
void CharMotionKill(void);
void CharMotionDataClose(void);
void CharModelDataClose(void);
void CharMotionAMemPGet(void);
void CharMotionSet(void);
void CharMotionTimeSet(void);
void CharMotionTimeGet(void);
void CharMotionMaxTimeGet(void);
void CharMotionEndCheck(void);
void CharMotionShiftIDGet(void);
void CharMotionShiftSet(void);
void CharMotionShiftTimeGet(void);
void CharMotionSpeedSet(void);
void CharModelHookDustCreate(void);
void CharEffectSmokeCreate(void);
void CharEffectSmokeCreateScale(void);
void CharEffectCoinGlowCreate(void);
void CharModelHitCreate(void);
void CharEffectHitCreate(void);
void CharEffectShoeHitCreate(void);
void CharModelShoeHitCreate(void);
void GWCharColorGet(void);
void CharEffectLayerSet(void);
void CharEyeBmpNameTbl(void);
void CharModelEyeBmpGet(void);
void CharMotionVoiceOnSet(void);
void CharNpcDustVoiceOffSet(void);
void CharModelStepSet(void);
void CharModelVoicePanAutoSet(void);
void CharModelVoiceFlagSet(void);
void CharModelLandDustCreate(void);
void CharModelAttrSet(void);
void CharModelAttrReset(void);
void CharMotionStartEndSet(void);
void CharModelHeightGet(void);
void CharModelMotListCreate(void);
void CharEffectDustCreate(void);
void CharEffectCryCreate(void);
void CharModelCryCreate(void);
void CharModelTimingHookNoGet(void);
void CharMotionUpdateSet(void);
void CharModelVoicePanSet(void);
void CharModelVoiceVolSet(void);
void CharWinLoseVoicePlay(void);
void CharLoseVoicePlay(void);
void CharMotionTotalTimeGet(void);
void CharMotionExtraTimeGet(void);
void CharLightCreate(void);
void CharLightCreateV(void);
void CharLightSpotSet(void);
void CharLightInfinitytSet(void);
void CharLightPointSet(void);
void CharLightColorSet(void);
void CharLightPosAimSetV(void);
void CharLightPosAimSet(void);
void CharLightStaticSet(void);
void CharEffectWarnCreate(void);
void CharMotionLoad(void);
void CharNpcDustSet(void);
void HuPrcSetStat(void);
void HuPrcResetStat(void);
void omGameSysInit(void);
void omResetStatBit(void);
void omCurrentOvlGet(void);
void omPauseChk(void);
void omOvlHisChg(void);
void omOvlHisGet(void);
void omovlevtno(void);
void omovlstat(void);
void AddX(void);
void AddZ(void);
void MapObject(void);
void CharObject(void);
void nMap(void);
void nChar(void);
void MapWall(void);
void MapPos(void);
void HitFaceVec(void);
void HitFace(void);
void HitFaceCount(void);
void GwPlayer(void);
void GwSystem(void);
void GWSinglePrizeFlagSet(void);
void GWSinglePrizeFlagGet(void);
void GWSingleDataInit(void);
void GWSinglePrizeSaveFlagSet(void);
void GWSinglePrizeSaveFlagGet(void);
void GWBankStarAdd(void);
void GWBankStarSub(void);
void GWBankStarGet(void);
void GWBankStarAwardGet(void);
void GWBankStarAwardReset(void);
void GWBankFlagSet(void);
void GWBankFlagReset(void);
void GWBankFlagGet(void);
void GWSingleMgFlagGet(void);
void GWSingleMgWinNumGet(void);
void GWSingleMgWinNumSet(void);
void GWSingleMgRecordNumGet(void);
void GWSingleMgRecordNumSet(void);
void HuTHPSprCreateVol(void);
void HuTHPSprCreate(void);
void HuTHP3DCreateVol(void);
void HuTHP3DCreate(void);
void HuTHPStop(void);
void HuTHPClose(void);
void HuTHPRestart(void);
void HuTHPEndCheck(void);
void HuTHPFrameGet(void);
void HuTHPTotalFrameGet(void);
void HuTHPSetVolume(void);
void THPSimpleGetFrameRate(void);
void HuTHPMatHook(void);
void HuTHPStartCheck(void);
void HuCardCheck(void);
void HuCardMount(void);
void HuCardFormat(void);
void HuCardOpen(void);
void HuCardRead(void);
void HuCardCreate(void);
void HuCardClose(void);
void HuCardWrite(void);
void HuCardDelete(void);
void HuCardSectorSizeGet(void);
void HuCardFreeSpaceGet(void);
void HuCardSlotCheck(void);
void HuCardWriteAsync(void);
void HuCardWriteCheck(void);
void HuCardWriteIdle(void);
void SLFileOpen(void);
void SLFileCreate(void);
void SLFileWrite(void);
void SLFileClose(void);
void SLFileRead(void);
void SLSaveEmptySet(void);
void SLCurSlotNoSet(void);
void SLCurSlotNoGet(void);
void SLSaveDataMake(void);
void SLSaveDataSlotMake(void);
void SLCommonSet(void);
void SLBoardSave(void);
void SLSave(void);
void SLCommonLoad(void);
void SLBoardLoad(void);
void SLLoad(void);
void SLCurBoxNoSet(void);
void SLCurBoxNoGet(void);
void SLSaveFlagSet(void);
void SLSaveFlagGet(void);
void SLCheckSumCheck(void);
void SLCheckSumBoxSlotCheck(void);
void SLCheckSumGet(void);
void SLCheckSumSlotGet(void);
void SLSerialNoGet(void);
void SLSerialNoCheck(void);
void saveExecF(void);
void SLWinIdSet(void);
void SLBoxBackupLoad(void);
void SLBoxBackupSlotLoad(void);
void SLCheckSumBoxSet(void);
void SLBoxDataOffsetGet(void);
void SLSaveBoardTurnExec(void);
void SLSaveBoardEndExec(void);
void SLSaveModeExec(void);
void curSlotNo(void);
void UnMountCnt(void);
void SLCommonSaveCopy(void);
void SLCommonLoadCopy(void);
void _CheckFlag(void);
void _SetFlag(void);
void _ClearFlag(void);
void MgDataTbl(void);
void MgModeSubMode(void);
void MgModeScore(void);
void MgModePlayNum(void);
void MgModeWork(void);
void GwCommonBackup(void);
void lbl_802C0438(void);
void MgExitReq(void);
void MgPauseExitF(void);
void MgBoard2Force(void);
void MgNoGet(void);
void MiracleBookEvtNo(void);
void MgSubMode(void);
void MgSubModeGet(void);
void MgBattleOrder(void);
void MgDecaScoreCalc(void);
void MgInstExitF(void);
void GWRecordSet(void);
void GWRecordGet(void);
void GWBoardPlayNumAdd(void);
void GWBoardPlayNumSet(void);
void GWBoardPlayNumGet(void);
void GWCharPlayNumInc(void);
void GWCharPlayNumGet(void);
void GWCharPlayNumSet(void);
void GWBoardMaxStarSet(void);
void GWBoardMaxStarGet(void);
void GWBoardMaxCoinSet(void);
void GWBoardMaxCoinGet(void);
void GWPlayerStarAdd(void);
void GWPlayerStarGet(void);
void GWPlayerStarSet(void);
void GWPlayerCoinAdd(void);
void GWPlayerCoinSet(void);
void GWPlayerCoinGet(void);
void GWMgCustomReset(void);
void GWMgCustomSet(void);
void GWMgCustomGet(void);
void GWMgUnlockGet(void);
void GWMgPlayNumSet(void);
void GWMgPlayNumGet(void);
void GWMgPlayNumAdd(void);
void GWMiracleBookFlagSet(void);
void GWMiracleBookFlagGet(void);
void GWMikeActRecordSet(void);
void GWMikeActRecordGet(void);
void GWSaveDebugSet(void);
void MgModeWorkInt(void);
void MgModeWorkFloat(void);
void GwMgDecaScore(void);
void GwLanguage(void);
void fmod(void);
void _savegpr_15(void);
void _restgpr_15(void);
void _savegpr_16(void);
void _restgpr_16(void);
void _savegpr_14(void);
void _restgpr_14(void);
void omDBGMenuButton(void);
void omDBGSystemKeyCheckSetup(void);
void msmSeSetListener(void);
void msmSeUpdataListener(void);
void msmSeGetIndexPtr(void);
void msmMusSetMasterVolume(void);
void msmSysGetOutputMode(void);
void msmSysSetGroupLoadMode(void);
void WipeCreate(void);
void WipeColorSet(void);
void omOvlReturnEx(void);
void HuDataGetAsyncStat(void);
void omVibrate(void);
void GameMesOvlPrev(void);
void MgTimerCreate(void);
void MgTimerKill(void);
void MgTimerModeGet(void);
void MgTimerModeOnSet(void);
void MgTimerModeOffSet(void);
void MgTimerParamSet(void);
void MgTimerValueGet(void);
void MgTimerDoneCheck(void);
void MgTimerPosSet(void);
void MgTimerPosGet(void);
void MgTimerBackColorSet(void);
void MgTimerColorSet(void);
void MgTimerRecordSet(void);
void MgTimerRecordDispOn(void);
void MgTimerRecordDispOff(void);
void MgScoreInit(void);
void MgScoreKill(void);
void MgScoreModeGet(void);
void MgScoreValueSet(void);
void MgScoreValueGet(void);
void MgScoreMaxDigitSet(void);
void MgScoreMaxDigitGet(void);
void MgScorePosSet(void);
void MgScorePosGet(void);
void MgScoreScaleSet(void);
void MgScoreScaleGet(void);
void MgScoreZRotSet(void);
void MgScoreZRotGet(void);
void MgScoreDigitWidthSet(void);
void MgScoreDigitWidthGet(void);
void MgScoreDispOn(void);
void MgScoreDispOff(void);
void MgScoreTPLvlSet(void);
void MgScoreTPLvlGet(void);
void MgScoreModeDefaultSet(void);
void MgScoreCreate(void);
void MgScoreColorSet(void);
void MgScoreDigitScaleSet(void);
void MgScorePriSet(void);
void MgScoreUnitBankSet(void);
void MgSeqCreate(void);
void MgSeqCreatePrio(void);
void MgSeqKill(void);
void MgSeqModeGet(void);
void MgSeqModeNext(void);
void MgSeqModeSet(void);
void MgSeqRecordSet(void);
void MgSeqModeChangeOff(void);
void MgSeqModeChangeOn(void);
void MgSeqStatGet(void);
void MgSeqWinnerSet(void);
void MgSeqDrawSet(void);
void fn_80071CCC(void);
void MgSeqModeDelaySet(void);
void MgSeqModeHookAdd(void);
void MgSeqModeHookReset(void);
void MgSeqTimerValueGet(void);
void MgSeqFrameNoGet(void);
void MgSeqFrameNoSet(void);
void MgSeqGameMesIdGet(void);
void MgSeqTimerKill(void);
void MgSeqStatBitSet(void);
void MgSeqStatBitReset(void);
void omCameraViewInit(void);
void omCameraViewSetMulti(void);
void omCameraViewSet(void);
void omCameraViewMoveSimpleMulti(void);
void omCameraViewMoveSimple(void);
void omCameraViewMoveMulti(void);
void omCameraViewMove(void);
void omCameraViewCheck(void);
void MgActorObjectSetup(void);
void MgActorColMapInit(void);
void MgPlayerPadSet(void);
void MgPlayerModeFuncSet(void);
void MgPlayerCreate(void);
void MgPlayerCreateJumpAlt(void);
void MgActorCreate(void);
void MgActorKill(void);
void MgActorExec(void);
void MgPlayerModeLandSet(void);
void MgPlayerModeSet(void);
void MgPlayerComStkOn(void);
void MgPlayerComStkOff(void);
void MgPlayerVecChase(void);
void MgActorVecChase(void);
void MgActorColAttrSet(void);
void MgActorColAttrReset(void);
void MgActorColAttrGet(void);
void MgActorColMapPolyGet(void);
void MgActorPosSet(void);
void MgActorPosSetRaw(void);
void MgActorPosGet(void);
void MgPlayerDespawn(void);
void MgPlayerSpawn(void);
void MgPlayerColDisable(void);
void MgPlayerColEnable(void);
void MgPlayerVibrateCreate(void);
void MgPlayerVibrateKill(void);
void MgActorColAttrParamSet(void);
void MgActorColAttrParamGet(void);
void MgActorColMapMaskSet(void);
void MgActorColMapMaskGet(void);
void MgActorColMaskSet(void);
void MgActorColMaskGet(void);
void MgPlayerAttrCheck(void);
void MgPlayerAttrSet(void);
void MgPlayerAttrReset(void);
void MgPlayerModeAttrCheck(void);
void MgPlayerModeAttrReset(void);
void MgPlayerModeAttrSet(void);
void MgPlayerVibAttrCheck(void);
void MgPlayerVibAttrSet(void);
void MgPlayerStunSet(void);
void MgActorColCylReset(void);
void MgActorColCylSet(void);
void MgActorColCodeGet(void);
void MgActorColNormalGet(void);
void MgActorRotYSet(void);
void MgActorRotYGet(void);
void MgActorPushSet(void);
void MgPlayerPosSet(void);
void MgActorGravitySet(void);
void MgActorVelYSet(void);
void MgActorColMeshGet(void);
void MgActorVelSet(void);
void MgActorColBounceSet(void);
void CharModelFileNumGet(void);
void CharMotionFileNumGet(void);
void lbl_802BF860(void);
void lbl_802BF864(void);
void lbl_802BF868(void);
void _savefpr_22(void);
void _restfpr_22(void);
void _savefpr_24(void);
void _restfpr_24(void);
void _savefpr_27(void);
void _restfpr_27(void);
void _savefpr_28(void);
void _restfpr_28(void);
void __construct_array(void);
void __destroy_arr(void);
void __msl_assertion_failed(void);
void __nw__FUl(void);
void __nwa__FUl(void);
void __ptmf_test(void);
void __ptmf_scall(void);
void __dla__FPv(void);
void lbl_802166D8(void);
void __ptmf_cmpr(void);
void HuDataDirCancelAsync(void);
void HuDataDirCloseAsync(void);
void HuTHPProcCheck(void);
void mbSaveInit(void);
void mbSavePartyInit(void);
void mbSaveStoryInit(void);
void fn_80146BA0(void);
void fn_80146C88(void);
void HuLoadProcStart(void);
void GwMgNightF(void);
void HuMCSysInit(void);
void HuMCInit(void);
void HuMCClose(void);
void HuMCContextSet(void);
void HuMCResponseGet2(void);
void HuMCStatGet(void);
void HuMCContextCallbackSet(void);
void HuMCResponseGet(void);
void HuMCContextCreate(void);
void HuMCContextKill(void);
void HuMCThresholdSet(void);
void HuMCCurResponseGet(void);
void HuMCButtonGet(void);
void HuMCButtonDownGet(void);
void HuMCProbe(void);
void HuMCMount(void);
void HuMCMicSprCreate(void);
void HuMCMicSprKill(void);
void HuMCMicGet(void);
void HuMCMicSet(void);
void HuMCSelWinCreate(void);
void HuMCSelWinKill(void);
void HuMCSelWinItemSet(void);
void HuMCSelWinItemRandSet(void);
void HuMCSelWinCheck(void);
void HuMCSelModeGet(void);
void HuMCSelWinContextSet(void);
void HuMCSelWinChoiceGet(void);
void HuMCSelWinContextKill(void);
void HuMCSelWinMaxTimeSet(void);
void HuMCSelWinMaxTimeGet(void);
void HuMCListenerCreate(void);
void HuMCListenerKill(void);
void HuMCSessionSet(void);
void HuMCSessionKill(void);
void HuMCSessionClose(void);
void HuMCUnkResponseCheck(void);
void HuMCNewResponseGet(void);
void HuMCVolSampleCreate(void);
void HuMCVolGet(void);
void HuMCShiftsSet(void);
void fn_8014C818(void);
void fn_8014C81C(void);
void mbBranchInit(void);
void mbev_Branch(void);
void mbev_BranchDebug(void);
void mbBranchAttrSet(void);
void mbBranchAttrReset(void);
void mbBranchAttrGet(void);
void mbBranchMAttrSet(void);
void mbBranchMAttrReset(void);
void mbBranchMAttrGet(void);
void mbBranchAttrCheck(void);
void mbBranchComStarHookSet(void);
void mbCameraInit(void);
void mbCameraZoomSet(void);
void mbCameraZoomGet(void);
void mbCameraRotSetV(void);
void mbCameraRotSet(void);
void mbCameraRotGet(void);
void mbCameraCenterSetV(void);
void mbCameraCenterSet(void);
void mbCameraCenterGet(void);
void mbCameraEyeSetV(void);
void mbCameraEyeSet(void);
void mbCameraEyeGet(void);
void mbCameraOffsetSetV(void);
void mbCameraOffsetSet(void);
void mbCameraOffsetGet(void);
void mbCameraDirGet(void);
void mbCameraPosDirGet(void);
void mbCameraLookAtFuncSet(void);
void mbCameraBitSet(void);
void mbCameraFovSet(void);
void mbCameraScissorSet(void);
void mbCameraNearFarSet(void);
void mbCameraNearFarGet(void);
void mbCameraGet(void);
void mbCameraStackPush(void);
void mbCameraStackIdxSet(void);
void mbCameraStackPop(void);
void mbCameraLookAtGet(void);
void mbCameraLookAtInvGet(void);
void mbCameraCurveTypeSet(void);
void mbCameraCurveTypeGet(void);
void mbCameraCullCheck(void);
void mbCameraPlayerViewSet(void);
void mbCameraPlayerViewSetFast(void);
void mbCameraPlayerViewGet(void);
void mbCameraPlayerViewNoGet(void);
void mbCameraPlayerViewZoomGet(void);
void mbCameraPlayerViewRotXGet(void);
void mbCameraFocusReset(void);
void mbCameraFocusPosSet(void);
void mbCameraFocusPosAdd(void);
void mbCameraFocusPlayerSet(void);
void mbCameraFocusPlayerAdd(void);
void mbCameraFocusObjSet(void);
void mbCameraFocusObjAdd(void);
void mbCameraFocusMasuSet(void);
void mbCameraFocusMasuAdd(void);
void mbCameraMoveOnSet(void);
void mbCameraSpeedSet(void);
void mbCameraMovePos(void);
void mbCameraMovePlayer(void);
void mbCameraMoveObj(void);
void mbCameraMoveMasu(void);
void mbCameraShakeSet(void);
void mbCameraShakeReset(void);
void mbCameraMoveStop(void);
void mbCameraMoveCheck(void);
void mbCameraMoveWait(void);
void mbCapSelect(void);
void mbCapDelete(void);
void fn_8019A618(void);
void fn_8019A61C(void);
void fn_8019A620(void);
void fn_8019A624(void);
void fn_8019A628(void);
void fn_8019A62C(void);
void mbCapSelectGrow(void);
void mbCapSelectShrinkCheck(void);
void mbCapSelectResultSet(void);
void mbCapSelectResultGet(void);
void mbCapSelectResultReset(void);
void mbCapMasuExec(void);
void mbCapMasuObjInit(void);
void mbCapMasuObjClose(void);
void mbCapMasuObjCreateAll(void);
void mbCapMasuObjCreate(void);
void mbCapMasuDispSet(void);
void mbCapCapsuleGet(void);
void mbev_CapKinoko(void);
void mbev_CapKinokoKill(void);
void mbev_CapSKinoko(void);
void mbev_CapSKinokoKill(void);
void mbev_CapPKinoko(void);
void mbev_CapPKinokoKill(void);
void mbev_CapMKinoko(void);
void mbev_CapMKinokoKill(void);
void mbev_CapKiller(void);
void mbev_CapKillerKill(void);
void mbev_CapDokan(void);
void mbev_CapDokanKill(void);
void mbev_CapHanachan(void);
void mbev_CapHanachanKill(void);
void mbev_CapNKinoko(void);
void mbev_CapNKinokoKill(void);
void mbev_CapKillerMove(void);
void mbev_CapKillerMoveKill(void);
void mbev_CapTogezo(void);
void mbev_CapTogezoKill(void);
void mbev_CapKuribo(void);
void mbev_CapKuriboKill(void);
void mbev_CapPakkun(void);
void mbev_CapPakkunKill(void);
void mbev_CapJango(void);
void mbev_CapJangoKill(void);
void mbev_CapPatapata(void);
void mbev_CapPatapataKill(void);
void mbev_CapKokamekku(void);
void mbev_CapKokamekkuKill(void);
void mbev_CapKamekku(void);
void mbev_CapKamekkuKill(void);
void mbev_CapThrowman(void);
void mbev_CapThrowmanKill(void);
void mbev_CapBoble(void);
void mbev_CapBobleKill(void);
void mbev_CapBobleTrap(void);
void mbev_CapBobleMove(void);
void mbev_CapBiriQ(void);
void mbev_CapBiriQKill(void);
void mbev_CapBiriQTrap(void);
void mbev_CapBiriQMetalShock(void);
void mbev_CapTumujikun(void);
void mbev_CapTumujikunKill(void);
void mbev_CapTumujikunTrap(void);
void mbev_CapDossun(void);
void mbev_CapDossunKill(void);
void mbev_CapDossunTrap(void);
void mbev_CapBomhei(void);
void mbev_CapBomheiKill(void);
void mbev_CapBomheiTrap(void);
void mbev_CapBomheiMove(void);
void mbev_CapTeresa(void);
void mbev_CapTeresaKill(void);
void mbev_CapTeresaFadeCreate(void);
void mbev_CapTeresaFadeKill(void);
void mbev_CapTeresaFadeSet(void);
void mbev_CapMiracle(void);
void mbev_CapMiracleKill(void);
void mbev_CapKettou(void);
void mbev_CapKettouKill(void);
void mbev_CapDonkey(void);
void mbev_CapDonkeyKill(void);
void mbev_CapKoopa(void);
void mbev_CapKoopaKill(void);
void mbev_CapCall(void);
void mbev_CapCallTrap(void);
void mbev_CapCapGet(void);
void mbev_CapWait(void);
void mbev_CapCallKettou(void);
void mbev_CapCircuitCallKettou(void);
void mbev_CapCallDonkey(void);
void mbev_CapCallKoopa(void);
void MBCapsuleStub5(void);
void MBCapsuleStub6(void);
void MBCapsuleStub7(void);
void mbev_CapCallTeresa(void);
void mbev_CapCallMiracle(void);
void mbev_CapBiriQShockCreate(void);
void mbev_CapBiriQShockDelayGet(void);
void mbev_CapBiriQMetalShockCreate(void);
void mbev_CapRandomBonusCoin(void);
void mbev_CapBonusCoinCall(void);
void mbev_CapBonusCoin(void);
void mbev_CapBonusCoinCheck(void);
void mbev_CapMoveMasuSet(void);
void mbev_CapStopMasuSet(void);
void mbev_CapKettouEndCall(void);
void mbev_CapDonkeyEndCall(void);
void mbev_CapKoopaEndCall(void);
void mbev_CapVsEndCall(void);
void mbev_CapKillerMoveCall(void);
void MBCapsuleStub11(void);
void MBCapsuleStub12(void);
void mbev_CapKillerMoveCheck(void);
void mbev_CapKillerMoveCheckAll(void);
void mbev_CapBankCoinInit(void);
void mbev_CapInit(void);
void mbev_CapKillerCall(void);
void mbev_CapKillerMultiCall(void);
void mbev_CapOpeningAdd(void);
void mbev_CapKoopaAdd(void);
void mbev_CapBankCoinGet(void);
void mbev_CapBubbleHookSet(void);
void mbev_CapBubbleHookCall(void);
void mbev_CapBubbleHookCallStory(void);
void mbev_CapKettouCoinLoseGet(void);
void mbev_CapKettouOppCoinLoseGet(void);
void mbev_CapKettouCoinLoseGet2(void);
void mbev_CapKettouOppCoinLoseGet2(void);
void mbev_CapPlayerMotionCreate(void);
void mbev_CapObjCreate(void);
void mbev_CapObjPosSet(void);
void mbev_CapPlayerPosSet(void);
void mbev_CapSprCreate(void);
void mbev_CapMalloc(void);
void mbev_CapObjClose(void);
void mbev_CapSprClose(void);
void mbev_CapMallocClose(void);
void mbev_CapEffExplodeCreate(void);
void mbev_CapEffExhaustCreate(void);
void mbev_CapEffExplodeOMExec(void);
void mbev_CapEffExplodeKill(void);
void mbev_CapEffExplodeAnimGet(void);
void mbev_CapEffExplodeAnimSet(void);
void mbev_CapEffExplodeAdd(void);
void mbev_CapEffExplodeKillerAdd(void);
void mbev_CapEffExplodeCircleAdd(void);
void mbev_CapEffDustCloudAdd(void);
void mbev_CapEffDustExplodeAdd(void);
void mbev_CapEffDustHeavyAdd(void);
void mbev_CapEffDustMultiAdd(void);
void mbev_CapEffBoostCreate(void);
void mbev_CapEffBoostOMExec(void);
void mbev_CapEffBoostKill(void);
void mbev_CapEffBoostTimeGet(void);
void mbev_CapEffBoostBlendModeSet(void);
void mbev_CapEffBoostAdd(void);
void mbev_CapEffSnowCreate(void);
void mbev_CapEffSnowOMExec(void);
void mbev_CapEffSnowKill(void);
void mbev_CapEffSnowDispGet(void);
void mbev_CapEffSnowAdd(void);
void mbev_CapEffGlowCreate(void);
void mbev_CapEffGlowFireCreate(void);
void mbev_CapEffGlowOMExec(void);
void mbev_CapEffGlowKill(void);
void mbev_CapEffGlowDispGet(void);
void mbev_CapEffGlowPatSet(void);
void mbev_CapEffGlowBlendModeSet(void);
void mbev_CapEffGlowAnimSet(void);
void mbev_CapEffGlowAdd(void);
void mbev_CapEffGlowKinokoAdd(void);
void mbev_CapEffGlowKinokoAddAlt(void);
void mbev_CapEffGlowKinokoTimeSet(void);
void mbev_CapEffGlowCoinAdd(void);
void mbev_CapEffRingCreate(void);
void mbev_CapEffRingHitCreate(void);
void mbev_CapEffRingOMExec(void);
void mbev_CapEffRingKill(void);
void mbev_CapEffRingDispGet(void);
void mbev_CapEffRingAdd(void);
void mbev_CapEffRingHitAdd(void);
void mbev_CapEffRingAnimSet(void);
void mbev_CapEffElectricCreate(void);
void mbev_CapEffElectricOMExec(void);
void mbev_CapEffElectricKill(void);
void mbev_CapEffElectricDispGet(void);
void mbev_CapEffElectricAdd(void);
void mbev_CapEffElectricModelSet(void);
void mbev_CapEffRayCreate(void);
void mbev_CapEffRayOMExec(void);
void mbev_CapEffRayKill(void);
void mbev_CapEffRayDraw(void);
void mbev_CapEffRayAdd(void);
void mbev_CapEffRayAlphaSet(void);
void mbev_CapEffRayTransformSet(void);
void mbev_CapEffMasuHitCreate(void);
void mbev_CapEffMasuHitOMExec(void);
void mbev_CapEffMasuHitKill(void);
void mbev_CapEffMasuHitAdd(void);
void mbev_CapEffMasuHitTransformSet(void);
void mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinOMExec(void);
void mbev_CapEffCoinKill(void);
void mbev_CapEffCoinNumGet(void);
void mbev_CapEffCoinAdd(void);
void mbev_CapEffCoinMaxYSet(void);
void mbev_CapEffCoinMultiAdd(void);
void mbev_CapEffCoinGlowSet(void);
void mbev_CapCoinAdd(void);
void mbev_CapEffCapLoseCreate(void);
void mbev_CapEffCapLoseOMExec(void);
void mbev_CapEffCapLoseKill(void);
void mbev_CapEffCapLoseNumGet(void);
void mbev_CapEffCapLoseObjAdd(void);
void mbev_CapEffCapLoseAdd(void);
void mbev_CapCoinManCreate(void);
void mbev_CapCoinManOMExec(void);
void mbev_CapCoinManKill(void);
void mbev_CapCoinManNumGet(void);
void mbev_CapCoinManObjAdd(void);
void mbev_CapCoinManAdd2(void);
void mbev_CapCoinManAdd(void);
void mbev_CapStarManCreate(void);
void mbev_CapStarManOMExec(void);
void mbev_CapStarManKill(void);
void mbev_CapStarManNumGet(void);
void mbev_CapStarManObjAdd(void);
void mbev_CapStarManAdd2(void);
void mbev_CapStarManAdd(void);
void mbev_CapPlayerMoveObjInit(void);
void mbev_CapPlayerMoveHitCreate(void);
void mbev_CapPlayerMoveEjectCreate(void);
void mbev_CapPlayerMoveIdleCreate(void);
void mbev_CapPlayerMoveMinYSet(void);
void mbev_CapPlayerMoveVelSet(void);
void mbev_CapPlayerMoveObjExec(void);
void mbev_CapPlayerMoveObjCheck(void);
void mbev_CapPlayerMoveObjClose(void);
void mbev_CapPlayerMoveObjKill(void);
void mbev_CapPlayerSquishSet(void);
void mbev_CapPlayerSquishVoiceSet(void);
void mbev_CapPlayerStunSet(void);
void mbev_CapPlayerShockSet(void);
void mbev_CapPlayerCheck(void);
void mbev_CapCullPlayerCheck(void);
void mbev_CapCullCheck(void);
void mbev_CapPointCullCheck(void);
void mbev_CapPlayerMasuNumGet(void);
void mbev_CapPlayerNoSearch(void);
void mbev_CapPlayerOrderGet(void);
void mbev_CapMasuLinkNextGet(void);
void mbev_CapMasuLinkNextRandomGet(void);
void mbev_CapMasuValidPrevGet(void);
void mbev_CapMasuPrevGet(void);
void mbev_CapPlayerComSelGet(void);
void mbev_CapPlayerComSelSameGet(void);
void mbev_CapPlayerComSelRandomGet(void);
void mbev_CapPlayerComSelKettouGet(void);
void mbev_CapEffColorSet(void);
void mbev_CapPlayerMotShiftCheck(void);
void mbev_CapPlayerMotShiftSet(void);
void mbev_CapPlayerMotShiftWait(void);
void mbev_CapObjMotionSet(void);
void mbev_CapObjMotionOMExec(void);
void mbev_CapPlayerMotionSet(void);
void mbev_CapPlayerMotionOMExec(void);
void mbev_CapPlayerRotate(void);
void mbev_CapPlayerIdleWait(void);
void mbev_CapCoinDisp(void);
void mbev_CapMasuMoveCheck(void);
void mbev_CapVibrate(void);
void mbev_CapStatusDispSet(void);
void mbev_CapDuelStatusOffSet(void);
void mbev_CapDuelStatusOnSet(void);
void mbev_CapDuelStatusDispSet(void);
void mbev_CapStatusDispSetAll(void);
void mbev_CapStatusDispCheck(void);
void mbev_CapChoiceSet(void);
void mbev_CapAngleWrap(void);
void mbev_CapAngleLerp(void);
void mbev_CapAngleSumLerp(void);
void mbev_CapVecChase(void);
void mbev_CapVecRotGet(void);
void mbev_CapColorLerp(void);
void mbev_CapHermiteConstGet(void);
void mbev_CapHermiteConstGet2(void);
void mbev_CapHermiteGetV(void);
void mbev_CapBezierGet(void);
void mbev_CapBezierGetV(void);
void mbev_CapBezierSlopeGet(void);
void mbev_CapBezierNormGetV(void);
void mbev_ShopEnableSet(void);
void mbev_ShopInit(void);
void mbev_ShopCreate(void);
void mbev_ShopExInit(void);
void mbev_Shop(void);
void mbev_ShopExObjHookSet(void);
void mbev_ShopBackCreate(void);
void mbev_ShopBackMotCreate(void);
void mbCapUse(void);
void MBCapsuleStub1(void);
void MBCapsuleStub2(void);
void mbCapEffUseCreate(void);
void mbCapEffUseModeGet(void);
void mbCapEffUsePosGet(void);
void mbCapEffUseWanWanCreate(void);
void mbCapPlayerThrow(void);
void mbCapPlayerThrowCheck(void);
void mbCapThrowColCreate(void);
void mbCapThrowColCheck(void);
void mbCapEffThrowCreate(void);
void mbCapEffThrowCheck(void);
void mbCapAutoThrow(void);
void mbCapThrowHookSet(void);
void mbCapSelectMasu(void);
void mbCapSelectMasuInit(void);
void mbCapListInit(void);
void mbCapListRead(void);
void mbCapListCopy(void);
void mbCapListDebug(void);
void mbCapNumInc(void);
void mbCapNumDebug(void);
void mbCapValueTypeGet(void);
void mbCapMasuTypeGet(void);
void mbCapValuePlayerGet(void);
void mbCapMasuPlayerGet2(void);
void mbCapMasuPlayerSet(void);
void mbCapMasuPlayerTypeSet(void);
void mbCapMasuPlayerGet(void);
void mbCapUseModeGet(void);
void mbCapMasuDispTypeGet(void);
void mbCapUseTrapCheck(void);
void mbCapBuyCostGet(void);
void mbCapSellCostGet(void);
void mbCapCostGet(void);
void mbCapThrowMasuCheck(void);
void mbCapComChanceGet(void);
void mbCapSelectComGet(void);
void mbCapSelectDeleteComGet(void);
void mbCapRandomThrowAdd(void);
void mbCapRandomTrapAdd(void);
void mbCapMasuCapsuleSet(void);
void mbCapFileGet(void);
void mbCapDescMesGet(void);
void mbCapUseMesGet(void);
void mbCapDebugNameGet(void);
void mbCapMasuPatGet(void);
void mbCapColorGet(void);
void mbCapUseCostGet(void);
void mbCapUseCheck(void);
void mbCapValidCheck(void);
void mbCapSelectMasuNum(void);
void mbCapSelectMasuFrontNum(void);
void mbCapSelectMasuBackNum(void);
void mbCapValidListGet(void);
void mbCapNextGet(void);
void mbCapMasuNextGet(void);
void mbCapShopListGet(void);
void mbCapRandomListGet(void);
void mbCapBonusCoinNumGet(void);
void mbCapDescWinCreate(void);
void mbCapInit(void);
void mbCapObjCreate(void);
void mbCapObjBorderCreate(void);
void mbCapObjKill(void);
void mbCapObjBorderKill(void);
void mbCapObjColorCreate(void);
void mbCapObjColorKill(void);
void mbCapObjColorPosSet(void);
void mbCapObjColorRotSet(void);
void mbCapObjColorScaleSet(void);
void mbCapObjColorPosSetV(void);
void mbCapObjColorRotSetV(void);
void mbCapObjColorScaleSetV(void);
void mbCapObjColorPosGet(void);
void mbCapObjColorRotGet(void);
void mbCapObjColorScaleGet(void);
void mbCapObjColorLayerSet(void);
void mbCapObjColorLayerGet(void);
void mbCapObjColorAttrSet(void);
void mbCapObjColorAttrReset(void);
void mbCapObjColorDispSet(void);
void mbCapObjColorAlphaSet(void);
void mbCapObjColorMtxSet(void);
void mbCapObjColorMtxGet(void);
void mbCoinInit(void);
void mbCoinClose(void);
void mbCoinEffObjCreate(void);
void mbCoinEffCreate(void);
void mbCoinDispKill(void);
void mbCoinDispKillCheck(void);
void mbCoinDispCapsuleCreate(void);
void mbCoinDispMasuCreate(void);
void mbCoinDispCreate(void);
void mbCoinAddProcExec(void);
void mbCoinAddDispExec(void);
void mbCoinAddExec(void);
void mbCoinAddAllProcExecV(void);
void mbCoinAddAllProcExec(void);
void mbCoinAddAllExec(void);
void mbStatTeamMinValGet(void);
void mbCoinCreate(void);
void mbCoinCreate2(void);
void mbCoinObjCreate(void);
void mbCoinObjCreate2(void);
void mbCoinObjNumDec(void);
void mbCoinObjKill(void);
void mbCoinObjGet(void);
void mbCoinObjPosSet(void);
void mbCoinObjPosSetV(void);
void mbCoinObjRotSet(void);
void mbCoinObjRotSetV(void);
void mbCoinObjScaleSet(void);
void mbCoinObjScaleSetV(void);
void mbCoinObjAlphaSet(void);
void mbCoinObjDispSet(void);
void mbCoinObjLayerSet(void);
void mbCoinObjMotSet(void);
void mbCoinObjPosGet(void);
void mbCoinObjRotGet(void);
void mbCoinObjScaleGet(void);
void mbCoinObjAlphaGet(void);
void mbCoinObjDispGet(void);
void mbComChoiceLeftSet(void);
void mbComChoiceUpSet(void);
void mbComChoiceRightSet(void);
void mbComChoiceDownSet(void);
void mbComChoiceListDownSet(void);
void mbComChoiceListRightSet(void);
void mbComChoiceListSet(void);
void mbConfigExec(void);
void mbPauseDispCopyKill(void);
void mbPauseDispCopyCreate(void);
void mbConfigPadDisableSet(void);
void mbPauseGuideCreate(void);
void mbPauseGuideKill(void);
void mbPausePanelCreate(void);
void mbPausePanelKill(void);
void mbPausePanelPosSet(void);
void mbPausePanelRotSet(void);
void mbPausePanelScaleSet(void);
void mbPausePanelBankSet(void);
void mbPausePanelBatsuSet(void);
void mbPausePanelPosGet(void);
void mbPausePanelScaleGet(void);
void mbPausePanelFreezeGet(void);
void mbPausePanelUnlockSet(void);
void mbPausePanelSlideSet(void);
void mbPausePanelSizeSet(void);
void mbPausePanelAnmNoSet(void);
void mbPausePanelGrowSet(void);
void mbPausePanelShrinkSet(void);
void mbPauseGuideMoveSet(void);
void mbPadDStkRepGetAll(void);
void mbPadBtnDownGetAll(void);
void mbev_Last5(void);
void mbDiceInit(void);
void mbDicePlayerExec(void);
void mbDiceExec(void);
void mbDiceProcExec(void);
void mbDiceChanceTradeExec(void);
void mbDiceChanceCharExec(void);
void mbDiceFadeSet(void);
void mbDiceKill(void);
void mbDiceClose(void);
void mbDiceKillCheck(void);
void mbDiceKillCheckAll(void);
void mbDiceResultGet(void);
void mbDiceHitHookSet(void);
void mbDiceTypeGet(void);
void mbDiceTutorialNumSet(void);
void mbDiceTutorialNumGet(void);
void mbDiceValueNoGet(void);
void mbDiceNumKill(void);
void mbDiceNumShrinkSet(void);
void mbDiceNumStopCheck(void);
void mbDiceStub(void);
void mbDiceSNpcNumCreate(void);
void mbDiceNumObjCreate(void);
void mbDiceMaxGet(void);
void mbDiceValueMaxGet(void);
void mbDicePadBtnHookSet(void);
void mbDiceMotHookSet(void);
void mbDiceObjHit(void);
void mbDiceSNpcNumObjCreate(void);
void mbDiceSNpcNumKill(void);
void mbDiceSNpcNumDispSet(void);
void mbDiceSNpcNumSet(void);
void mbDiceSNpcNumPosSet(void);
void mbDiceSNpcNumOfsSet(void);
void mbDiceSNpcNumOfsGet(void);
void mbEffInit(void);
void mbEffClose(void);
void mbEffConfettiCreate(void);
void mbEffConfettiKill(void);
void mbEffConfettiReset(void);
void mbEffFadeOutSet(void);
void mbEffFadeCreate(void);
void mbEffFadeDoneCheck(void);
void mbEffFadeCheck(void);
void mbEffFadeCameraSet(void);
void mbParticleCreate(void);
void mbParticleKill(void);
void mbParticleHookSet(void);
void mbParticleAttrSet(void);
void mbParticleAttrReset(void);
void mbParticleUnkTotalGet(void);
void mbParticleDataCreate(void);
void mbParticleColorCreate(void);
void mbParManCreate(void);
void mbParManKill(void);
void mbParManPosSet(void);
void mbParManVecSet(void);
void mbParManRotSet(void);
void mbParManAttrSet(void);
void mbParManAttrReset(void);
void mbParticleBlendModeSet(void);
void mbGateInit(void);
void mbGateClose(void);
void mbGateCreate(void);
void mbev_GateMasu(void);
void mbev_GateBattanMasuFind(void);
void mbev_GateBattan(void);
void mbGateBattanKill(void);
void mbGuideInit(void);
void mbGuideCreateIn(void);
void mbGuideCreateFlag(void);
void mbGuideCreate(void);
void mbGuideKill(void);
void mbGuideEnd(void);
void mbGuideFadeIn(void);
void mbGuideFadeOut(void);
void mbGuideModelGet(void);
void mbGuideIdleCheck(void);
void mbGuideScreenSet(void);
void mbGuideMotionNextSet(void);
void mbGuideMotionSet(void);
void mbGuideMotionShiftSet(void);
void mbGuideMotionStop(void);
void mbGuideMotionCheck(void);
void mbGuideNoGet(void);
void mbGuideSpeakerNoGet(void);
void mbExitWatchCreate(void);
void mbExitReq(void);
void mbExitCheck(void);
void mbExitWatchProcGet(void);
void mbChangeTimeSet(void);
void mbChangeTime(void);
void mbev_NextTimeSet(void);
void mbev_LoadTimeSet(void);
void mbObjectSetup(void);
void mbPauseEnableSet(void);
void mbPauseEnableReset(void);
void mbPauseEnableCheck(void);
void mbLightSet(void);
void mbLightReset(void);
void mbLightFuncSet(void);
void mbBGRead(void);
void mbBGReadWait(void);
void mbReturnMgCheck(void);
void mbOvlCall(void);
void mbDirClose(void);
void mbev_TurnStartSet(void);
void mbev_TurnEndSet(void);
void fn_8014C3B4(void);
void fn_8014C3BC(void);
void fn_8014C3C4(void);
void fn_8014C3CC(void);
void mbScrollInit(void);
void mbScrollClose(void);
void mbev_Scroll(void);
void mbev_ScrollCapsule(void);
void mbScrollStarFindFuncSet(void);
void mbScrollHookSet(void);
void mbMapCameraSet(void);
void mbMapHookSet(void);
void mbMapSprAdd(void);
void mbev_StarScroll(void);
void mbMasuInit(void);
void mbMasuClose(void);
void mbev_MasuStop(void);
void mbMasuDataRead(void);
void mbev_MasuMasuEnd(void);
void mbev_MasuMasuStart(void);
void mbev_MasuCapStop(void);
void mbev_MasuMove(void);
void mbMasuNextSet(void);
void mbMasuNextDispSet(void);
void mbMasuNumGet(void);
void mbMasuRawNumGet(void);
void mbMasuGet(void);
void mbMasuLayerSet(void);
void mbMasuLayerGet(void);
void mbMasuAttrGet(void);
void mbMasuAttrSet(void);
void mbMasuMAttrGet(void);
void mbMasuMAttrSet(void);
void mbev_MasuBitGet(void);
void mbev_MasuAttrGet(void);
void mbMasuTypeGet(void);
void mbMasuTypeSet(void);
void mbMasuTypeChange(void);
void mbMasuDispCheck(void);
void mbMasuCapsuleReset(void);
void mbMasuCapsuleSet(void);
void mbMasuCapsuleGet(void);
void mbMasuPosGet(void);
void mbMasuPosSet(void);
void mbMasuPosSetV(void);
void mbMasuCornerRotPosGet(void);
void mbMasuCornerPosGet(void);
void mbMasuRotGet(void);
void mbMasuRotSet(void);
void mbMasuRotSetV(void);
void mbMasuMtxGet(void);
void mbMasuMtxSet(void);
void mbMasuDispGet(void);
void mbMasuDispSet(void);
void mbMasuDispAttrReset(void);
void mbMasuDispAttrSet(void);
void mbMasuDispAttrGet(void);
void mbMasuDispMAttrReset(void);
void mbMasuDispMAttrSet(void);
void mbMasuDispMAttrGet(void);
void mbMasuModelDispSet(void);
void mbMasuLinkGet(void);
void mbMasuLinkNumGet(void);
void mbMasuLinkTblGet(void);
void mbMasuLinkTblGet2(void);
void mbMasuAttrFindLink(void);
void mbMasuAttrMatchFindLink(void);
void mbMasuMAttrFindLink(void);
void mbMasuMAttrMatchFindLink(void);
void mbMasuTypeFindLink(void);
void mbMasuLinkParentGet(void);
void mbMasuFind_TypeStepGet(void);
void mbMasuFind_TypeStepGet2(void);
void mbMasuFind_AttrStepGet(void);
void mbMasuFind_AttrMatchStepGet(void);
void mbMasuFind_AttrStepGet2(void);
void mbMasuFind_MAttrStepGet(void);
void mbMasuFind_MAttrMatchStepGet(void);
void mbMasuFind_MAttrStepGet2(void);
void mbMasuFind_IdStepGet(void);
void mbMasuFind_IdStepGet2(void);
void mbMasuFind_TypeSearch(void);
void mbMasuFind_TypeIdGet(void);
void mbMasuFind_AttrIdGet(void);
void mbMasuFind_AttrMatchIdGet(void);
void mbMasuFind_AttrMatchIdGet2(void);
void mbMasuFind_MAttrIdGet(void);
void mbMasuFind_MAttrMatchIdGet(void);
void mbMasuFind_MAttrMatchIdGet2(void);
void mbMasuTypeListGet(void);
void mbMasuAttrListGet(void);
void mbMasuAttrMatchListGet(void);
void mbMasuMAttrListGet(void);
void mbMasuMAttrMatchTblGet(void);
void mbMasuFind_TypeListGet(void);
void mbMasuFind_TypeListGet2(void);
void mbMasuFind_AttrNumGet(void);
void mbMasuFind_AttrMatchNumGet(void);
void mbMasuFind_AttrMatchNumGet2(void);
void mbMasuFind_MAttrNumGet(void);
void mbMasuFind_MAttrMatchListGet(void);
void mbMasuFind_MAttrMatchListGet2(void);
void mbMasuFind_IdListGet(void);
void mbMasuFind_IdListGet2(void);
void mbev_MasuMoveEndSet(void);
void mbev_MasuHatenaSet(void);
void mbev_MasuMoveStartSet(void);
void mbev_MasuLinkTblHookSet(void);
void mbMasuPlayerDispSet(void);
void mbMasuPlayerFadeSet(void);
void mbMasuPlayerCapMoveCheck(void);
void mbMasuPlayerCapStopCheck(void);
void mbMasuStub(void);
void mbMasuPlayerColorSet(void);
void mbMasuPlayerPrizeReset(void);
void mbMasuPKinokoValueGet(void);
void mbMathInit(void);
void mbMathClose(void);
void mbRandMod(void);
void mbVecMagCheck(void);
void mbAngleAdd(void);
void mbAngleMoveTo(void);
void mbAngleWrap2(void);
void mbVecMagXZ(void);
void mbMtxLookAtCalc(void);
void mbAngleWrapV(void);
void mbAngleWrap(void);
void mbVecMagXZCheck(void);
void mbBezierCalc(void);
void mbBezierCalcV(void);
void mbBezierCalcVList(void);
void mbBezierCalcSlope(void);
void mbBezierCalcSlopeV(void);
void mbHermiteCalc(void);
void mbHermiteCalcV(void);
void mbHermiteCalcSlope(void);
void mbPos3Dto2D(void);
void mbPos2Dto3D(void);
void mbPos3DtoNorm(void);
void mbNormPosto3D(void);
void mbNormPosto2D(void);
void mbAngleLerp(void);
void mbAngleEaseOut(void);
void mbAngleEaseIn(void);
void mbMathDistScale(void);
void mbCosDeg(void);
void mbSinDeg(void);
void mbCosRad(void);
void mbSinRad(void);
void mbMtxRot(void);
void mbMtxRotAxisDeg(void);
void mbMtxRotAxisRad(void);
void mbMtxRotXDeg(void);
void mbMtxRotXRad(void);
void mbMtxRotYDeg(void);
void mbMtxRotYRad(void);
void mbMtxRotZDeg(void);
void mbMtxRotZRad(void);
void mbMtxScaleRotXDeg(void);
void mbMtxScaleRotYDeg(void);
void mbMtxScaleRotZDeg(void);
void mbMtxRotTrigX(void);
void mbMtxRotTrigY(void);
void mbMtxRotTrigZ(void);
void mbMtxRotTrigScaleX(void);
void mbMtxRotTrigScaleY(void);
void mbMtxRotTrigScaleZ(void);
void mbMtxTransCat(void);
void mbObjCullInit(void);
void mbTelopPlayerCreate(void);
void mbTelopPlayerSkipCreate(void);
void mbTelopCreate(void);
void mbTelopCheck(void);
void mbTelopLastTurnCreate(void);
void mbPadStkXGet(void);
void mbPadStkYGet(void);
void mbTauntInit(void);
void mbTauntClose(void);
void mbLanguageGet(void);
void mbLanguageSet(void);
void mbBoardDataNumGet(void);
void mbBoardDataDirRead(void);
void mbTelopTimeChangeCreate(void);
void mbTelopTimeChangeCheck(void);
void mbTelopTimeChangeKill(void);
void mbTelopTimeCreate(void);
void mbTelopTimeSprCreate(void);
void mbTelopTimeSprKill(void);
void mbTelopTimeDispSet(void);
void mbTelopTimeSprRotSet(void);
void mbTelopTimeStarSet(void);
void mbTelopTimeTPLvlSet(void);
void mbObjInit(void);
void mbObjClose(void);
void mbObjKill(void);
void mbObjDirSet(void);
void mbObjDataNumGet(void);
void mbObjCreate(void);
void mbObjCharCreate(void);
void mbObjGet(void);
void mbObjDispGet(void);
void mbObjDispSet(void);
void mbObjCameraSet(void);
void mbObjLayerSet(void);
void mbObjLayerGet(void);
void mbObjLayerSetAll(void);
void mbObjModelIDGet(void);
void mbObjPosGet(void);
void mbObjPosSetV(void);
void mbObjPosSet(void);
void mbObjOffsetGet(void);
void mbObjOffsetSetV(void);
void mbObjOffsetSet(void);
void mbObjRotGet(void);
void mbObjRotSetV(void);
void mbObjRotSet(void);
void mbObjRotYGet(void);
void mbObjRotYSet(void);
void mbObjScaleGet(void);
void mbObjScaleSetV(void);
void mbObjScaleSet(void);
void mbObjMtxGet(void);
void mbObjMtxSet(void);
void mbObjAttrSet(void);
void mbObjAttrReset(void);
void mbObjAmbSet(void);
void mbObjAmbSetAll(void);
void mbObjAlphaGet(void);
void mbObjAlphaSet(void);
void mbObjFadeSet(void);
void mbObjZWriteOffSet(void);
void mbObjSePlay(void);
void mbObjMotionVoiceOnSet(void);
void mbObjCullRadiusSet(void);
void mbObjHookSet(void);
void mbObjHookReset(void);
void mbObjHookObjReset(void);
void mbObjMotionGet(void);
void mbObjMotionSet(void);
void mbObjMotionIDCurGet(void);
void mbObjMotionIDGet(void);
void mbObjMotionShiftSet(void);
void mbObjMotionShiftIDGet(void);
void mbObjMotionNoCreate(void);
void mbObjMotionCreate(void);
void mbObjMotionKill(void);
void mbObjMotionTimeGet(void);
void mbObjMotionTimeSet(void);
void mbObjMotionMaxTimeGet(void);
void mbObjMotionSpeedGet(void);
void mbObjMotionSpeedSet(void);
void mbObjMotionEndCheck(void);
void mbObjMotionStartEndSet(void);
void mbObjMotionLoopSet(void);
void mbObjMotionShapeSet(void);
void mbObjMotionShapeTimeGet(void);
void mbObjMotionShapeTimeSet(void);
void mbObjMotionShapeMaxTimeGet(void);
void mbObjMotionShapeSpeedGet(void);
void mbObjMotionShapeSpeedSet(void);
void mbObjMotionShapeEndCheck(void);
void mbObjMotionShapeStartEndSet(void);
void mbObjShadowSet(void);
void mbObjShadowReset(void);
void mbObjShadowMapSet(void);
void mbObjShadowMapReset(void);
void mbObjStub0(void);
void mbev_OpeningParty(void);
void mbev_OpeningSingle(void);
void mbev_Opening(void);
void mbOpeningViewSet(void);
void mbOpeningZoomGet(void);
void mbOpeningRotGet(void);
void mbOpeningPosGet(void);
void mbOpeningInstHookSet(void);
void mbOpeningStarInstHookSet(void);
void mbOpeningGuideObjIdGet(void);
void mbOpeningGuidePosRestore(void);
void mbOpeningCameraPosRestore(void);
void mbPauseInit(void);
void mbPauseCreate(void);
void mbPauseStartCheck(void);
void mbPauseProcCheck(void);
void mbPauseDisableSet(void);
void mbPauseDisableGet(void);
void mbPauseHookPush(void);
void mbPauseHookPop(void);
void mbPauseSet(void);
void mbPauseDataDirGet(void);
void mbPauseDataNumGet(void);
void mbPlayerMetalSet(void);
void mbPlayerEffectSet(void);
void mbPlayerMetalColorSet(void);
void mbPlayerBiriQSet(void);
void mbPlayerInit(void);
void mbPlayerClose(void);
void mbTurnExec(void);
void mbSingleTurnExec(void);
void mbPlayerSwap(void);
void mbPlayerNameMesGet(void);
void mbPlayerNameGet(void);
void mbPlayerTagNameMesGet(void);
void mbPlayerColorGet(void);
void mbPlayerAmbSet(void);
void mbPlayerWorkGet(void);
void mbPlayerAllComCheck(void);
void mbPlayerObjIDGet(void);
void mbPlayerModelIDGet(void);
void mbPlayerDispSet(void);
void mbPlayerDispGet(void);
void mbPlayerLayerSet(void);
void mbPlayerCameraSet(void);
void mbPlayerCullRadiusSet(void);
void mbPlayerStubValSet(void);
void mbPlayerMtxSet(void);
void mbPlayerMtxGet(void);
void mbPlayerPosSet(void);
void mbPlayerPosSetV(void);
void mbPlayerPosGet(void);
void mbPlayerRotSet(void);
void mbPlayerRotSetV(void);
void mbPlayerRotGet(void);
void mbPlayerRotYSet(void);
void mbPlayerRotYGet(void);
void mbPlayerScaleSet(void);
void mbPlayerScaleSetV(void);
void mbPlayerScaleGet(void);
void mbPlayerMotionGet(void);
void mbPlayerMotionEndCheck(void);
void mbPlayerMotionEndCheckAll(void);
void mbPlayerMotionEndWait(void);
void mbPlayerMotionMaxTimeGet(void);
void mbPlayerMotionTimeGet(void);
void mbPlayerMotionSet(void);
void mbPlayerMotionVoiceOnSet(void);
void mbPlayerMotionShiftSet(void);
void mbPlayerMotionCreate(void);
void mbPlayerMotionKill(void);
void mbPlayerMotionSpeedSet(void);
void mbPlayerMotionTimeSet(void);
void mbPlayerMotionStartEndSet(void);
void mbPlayerAttrSet(void);
void mbPlayerAttrReset(void);
void mbPlayerMotIdleSet(void);
void mbPlayerRotateStart(void);
void mbPlayerRotateCheck(void);
void mbPlayerRotateCheckAll(void);
void mbPlayerEyeMatDarkSet(void);
void mbPlayerRankGet(void);
void mbPlayerCapsuleUseGet(void);
void mbPlayerCapsuleUseSet(void);
void mbPlayerCapsuleRemove(void);
void mbPlayerCapsuleFind(void);
void mbPlayerCapsuleAdd(void);
void mbPlayerCapsuleNumGet(void);
void mbPlayerCapsuleGet(void);
void mbPlayerCapsuleMaxGet(void);
void mbPlayerCoinAdd(void);
void mbPlayerCoinSet(void);
void mbPlayerCoinGet(void);
void mbPlayerMaxCoinGet(void);
void mbPlayerStarAdd(void);
void mbPlayerStarSet(void);
void mbPlayerStarGet(void);
void mbPlayerTeamFind(void);
void mbPlayerTeamFindOther(void);
void mbPlayerTeamCoinSet(void);
void mbPlayerTeamCoinGet(void);
void mbPlayerGrpStarSet(void);
void mbPlayerGrpStarGet(void);
void mbPlayerTeamRankGet(void);
void mbPlayerTeamFindPlayer(void);
void mbPlayerTeamCapsuleGet(void);
void mbPlayerTeamCapsuleNumGet(void);
void mbPlayerTeamFindOpp(void);
void mbPlayerTeamGet(void);
void mbPlayerTeamCheckSame(void);
void mbPlayerWalkSpeedGet(void);
void mbPlayerMoveExec(void);
void mbPlayerMoveMain(void);
void mbev_PlayerColReserve(void);
void mbev_PlayerColSet(void);
void mbMoveNumCreateColor(void);
void mbMoveNumCreate(void);
void mbMoveNumKill(void);
void mbMoveNumDispSet(void);
void mbPlayerMasuMoveTo(void);
void mbPlayerMasuMove(void);
void mbPlayerMasuMovePos(void);
void mbPlayerMasuMoveSpeed(void);
void mbPlayerColSnapSet(void);
void mbev_PlayerColMasuSet(void);
void mbev_PlayerColMasuAllSet(void);
void mbev_PlayerColMasuAdd(void);
void mbev_PlayerColMasu(void);
void mbev_PlayerColCircleAdd(void);
void mbev_PlayerColBall(void);
void mbPlayerColSnapPlayerSet(void);
void mbPlayerColSnapGet(void);
void mbPlayerColRestSet(void);
void mbPlayerColCheck(void);
void mbPlayerBestPathGet(void);
void mbPlayerPosReset(void);
void mbPlayerPosResetAll(void);
void mbPlayerMasuCornerSet(void);
void mbPlayerMasuCornerGet(void);
void mbPlayerDiceMotExec(void);
void mbPlayerKettouCheck(void);
void mbPlayerBlackoutSet(void);
void mbPlayerBlackoutGet(void);
void mbPlayerWinLoseVoicePlay(void);
void mbPlayerVoicePlay(void);
void mbPlayerVoicePanPlay(void);
void mbPlayerStartTurnHookSet(void);
void mbPlayerEndTurnHookSet(void);
void mbPlayerMoveHookSet(void);
void mbPlayerDiceTypeGet(void);
void mbPlayerTurnInitHookSet(void);
void mbPlayerTurnCloseHookSet(void);
void mbPlayerColFirstSet(void);
void mbPlayerColOrderReset(void);
void mbPlayerPlusMasuExec(void);
void mbPlayerMinusMasuExec(void);
void mbPlayerCapCoinMasuExec(void);
void mbRouletteCreate(void);
void mbRouletteKaneCreate(void);
void mbRouletteCheck(void);
void mbRouletteWait(void);
void mbRouletteResultGet(void);
void mbRouletteMaxSpeedSet(void);
void mbRouletteComValueSet(void);
void mbSingleInit(void);
void mbSingleClose(void);
void mbSingleReturn(void);
void mbSingleReturnWrite(void);
void mbSingleGameEnd(void);
void mbSingleSaveInit(void);
void mbSingleMgUnlockWrite(void);
void mbSingleMgUnlockInit(void);
void mbSingleMgUnlockSet(void);
void mbSingleMgUnlockReset(void);
void mbSingleMgUnlockGet(void);
void mbSingleMgUnlockCheckAny(void);
void mbSingleMgUnlockNumGet(void);
void mbev_SingleMg(void);
void mbev_SingleMgEnd(void);
void mbSingleStepGet(void);
void mbSingleCall(void);
void mbSingleOppCharGet(void);
void mbSingleTeamCharSet(void);
void mbSingleTeamCharGet(void);
void mbSingleSaveFlush(void);
void mbSingleMgUnlockCheck(void);
void mbMusPlay(void);
void mbMusFadeOut(void);
void mbMusFadeOutSpeed(void);
void mbMusStub(void);
void mbMusParamSet(void);
void mbMusPauseFadeOut(void);
void mbMusStatGet(void);
void mbAudClose(void);
void mbAudInit(void);
void mbAudStub1(void);
void mbAudStub2(void);
void mbMusBoardPlay(void);
void mbMusCheck(void);
void mbMusEndCheck(void);
void mbMusStopCheck(void);
void mbMusFadeCheck(void);
void mbMusPauseSet(void);
void mbMusJinglePlay(void);
void mbMusJingleWait(void);
void mbMusJingleStatGet(void);
void mbAudFXObjSet(void);
void mbAudFXObjCreate(void);
void mbAudFXObjVolSet(void);
void mbAudFXObjKill(void);
void mbAudFXObjClose(void);
void mbAudFXObjEmitterSet(void);
void mbAudFXObjEmitterCreate(void);
void mbAudFXObjEmitterUpdate(void);
void mbAudFXObjEmiterPosSet(void);
void mbAudFXObjRefSet(void);
void mbAudFXPlay(void);
void mbAudFXStop(void);
void mbAudFXStopAll(void);
void mbAudFXPosPlay(void);
void mbAudFXPosPanGet(void);
void mbAudFXEmitterPlay(void);
void mbAudFXVolSet(void);
void mbAudFXPanning(void);
void mbAudFXPosPanning(void);
void mbAudFXDelaySet(void);
void mbAudGuidePlay(void);
void mbev_StarMasu(void);
void mbev_StarFreeMasu(void);
void mbStarGetMain(void);
void mbStarGetPosExec(void);
void mbStarMapViewExec(void);
void mbStarStub2(void);
void mbStarInit(void);
void mbStarClose(void);
void mbStarStub(void);
void mbStarNumSet(void);
void mbStarNumGet(void);
void mbStarFlagInit(void);
void mbStarFlagSet(void);
void mbStarFlagReset(void);
void mbStarFlagGet(void);
void mbStarNextNoSet(void);
void mbStarNextNoGet(void);
void mbStarNoSet(void);
void mbStarNoRandGet(void);
void mbStarReset(void);
void mbStarNoDispSet(void);
void mbStarMasuDispSet(void);
void mbStarDispSetAll(void);
void mbStarObjDispSetAll(void);
void mbStarMasuNextSet(void);
void mbStarGetExec(void);
void mbStarMoveHookSet(void);
void mbStarMasuFuncSet(void);
void mbStarAddNumSet(void);
void mbStarDispCreate(void);
void mbStarDispPlayerCreate(void);
void mbStarDispCheck(void);
void mbStarObjCreate(void);
void mbStarObjPosSet(void);
void mbStarObjRotSet(void);
void mbStarObjScaleSet(void);
void mbStarObjPosSetV(void);
void mbStarObjRotSetV(void);
void mbStarObjScaleSetV(void);
void mbStarObjPosGet(void);
void mbStarObjRotGet(void);
void mbStarObjScaleGet(void);
void mbStarObjDispSet(void);
void mbStarObjDispFlagSet(void);
void mbStarObjKill(void);
void mbStarChestCreate(void);
void mbStarAddProcExec(void);
void mbStarAddDispExec(void);
void mbStarAddExec(void);
void mbStarAddAllProcExecV(void);
void mbStarAddAllProcExec(void);
void mbStarAddAllExec(void);
void mbZtarObjInit(void);
void mbZtarObjClose(void);
void mbZtarMasuNextSet(void);
void mbZtarGetExec(void);
void mbZtarGetMain(void);
void mbZtarObjDispFlagSet(void);
void mbStarMapViewProcExec(void);
void mbStatusInit(void);
void mbStatusClose(void);
void mbStatusReset(void);
void mbStatusDispGet(void);
void mbStatusDispSet(void);
void mbStatusDispFocusSet(void);
void mbStatusDispSetAll(void);
void mbStatusDispForceSet(void);
void mbStatusDispForceSetAll(void);
void mbStatusMoveCheck(void);
void mbStatusOffCheckAll(void);
void mbSprCreate(void);
void mbStatusPosGet(void);
void mbStatusNoPosGet(void);
void mbStatusCapsuleDispSet(void);
void mbStatusKaoNoSet(void);
void mbSprNumSet(void);
void mbStatusMoveTo(void);
void mbStatusMoveSet(void);
void mbStatusPosOnGet(void);
void mbStatusPosOffGet(void);
void mbStatusRainbowSet(void);
void mbStatusColorSet(void);
void mbStatusColorAllSet(void);
void mbStatusColorGet(void);
void mbStatusLayoutSet(void);
void mbStatusCapsuleFocusSet(void);
void mbStatusNoGet(void);
void mbStatusNoMoveSet(void);
void mbStatusDispBackup(void);
void mbStatusDispRestoreForce(void);
void mbStatusDispRestore(void);
void mbStatusMasuDispSet(void);
void mbSNpcInit(void);
void mbSNpcCreate(void);
void mbSNpcKill(void);
void mbSNpcMotReset(void);
void mbSNpcPlayerWalkSet(void);
void mbSNpcStarExec(void);
void mbSNpcMasuGet(void);
void mbSNpcMotWinSet(void);
void mbSNpcMotIdleSet(void);
void mbSNpcDispSet(void);
void mbMasuChanceCreate(void);
void mbMasuChanceTypeSet(void);
void mbMasuChanceFlagSet(void);
void mbMasuChancePlayerSet(void);
void mbMasuChanceSet(void);
void mbMasuChanceKill(void);
void mbSNpcMasuStarNextGet(void);
void mbObjStarTevStageSet(void);
void mbObjFadeCreate(void);
void mbObjFadeKill(void);
void mbObjFadeTexRotSet(void);
void mbObjFadeTexColorSet(void);
void mbObjMetalCreate(void);
void mbObjMetalKill(void);
void mbObjMetalTPLvlSet(void);
void mbObjMetalColorSet(void);
void mbObjBiriQCreate(void);
void mbObjBiriQKill(void);
void mbObjBiriQColorSet(void);
void mbTutorialInit(void);
void mbTutorialCreate(void);
void mbTutorialExitReqGet(void);
void mbTutorialExitSet(void);
void mbTutorialExitOnSet(void);
void mbTutorialCall(void);
void mbTutorialMainFuncSet(void);
void mbTutorialSprCreate(void);
void mbTutorialSprDispOn(void);
void mbTutorialSprDispOff(void);
void mbTutorialSprKill(void);
void mbTutorialSprGrpSet(void);
void mbTutorialSprGrpKill(void);
void mbTutorialModelCreate(void);
void mbTutorialModelKill(void);
void mbTutorialGuideCreate(void);
void mbTutorialGuideGet(void);
void mbTutorialGuideClose(void);
void mbTutorialWinWait(void);
void mbTutorialWinClose(void);
void mbTutorialWinMesExec(void);
void mbTutorialWinCreate(void);
void mbTutorialWinMesMasuExec(void);
void mbTutorialWinKeyWait(void);
void mbTutorialViewSet(void);
void mbTutorialViewMasuSet(void);
void mbTutorialVSleep(void);
void mbTutorialCallEnd(void);
void mbTutorialCallWait(void);
void mbTutorialMultiCall(void);
void mbTutorialCallResult(void);
void mbTutorialMesCall(void);
void mbTutorialMesMasuCall(void);
void mbTutorialModeSet(void);
void mbTutorialModeGet(void);
void mbTutorialResultSet(void);
void mbTutorialSceneGet(void);
void mbTutorialTurnCall(void);
void mbTutorialGuideCall(void);
void mbTutorialCapsuleUseCall(void);
void mbTutorialMgCallExec(void);
void mbTutorialMgCallClose(void);
void mbWinInit(void);
void mbWinClose(void);
void mbWinCreate(void);
void mbWinCreateChoice(void);
void mbWinCreateHelp(void);
void mbWinCreateFrame(void);
void mbWinCreateTime(void);
void mbWinCreateBlank(void);
void mbWinCreateBlankFrame(void);
void mbWinKillAll(void);
void mbWinKill(void);
void mbWinTopKill(void);
void mbWinPosSet(void);
void mbWinTopPosSet(void);
void mbWinPosGet(void);
void mbWinTopPosGet(void);
void mbWinSizeSet(void);
void mbWinTopSizeSet(void);
void mbWinMesMaxSizeGet(void);
void mbWinTopMesMaxSizeGet(void);
void mbWinScaleSet(void);
void mbWinTopScaleSet(void);
void mbWinScaleGet(void);
void mbWinTopScaleGet(void);
void mbWinChoiceGet(void);
void mbWinTopChoiceGet(void);
void mbWinPause(void);
void mbWinTopPause(void);
void mbWinInsertMesSet(void);
void mbWinTopInsertMesSet(void);
void mbWinDoneCheck(void);
void mbWinTopDoneCheck(void);
void mbWinWait(void);
void mbWinTopWait(void);
void mbWinAttrSet(void);
void mbWinTopAttrSet(void);
void mbWinAttrReset(void);
void mbWinTopAttrReset(void);
void mbWinChoiceDisable(void);
void mbWinTopChoiceDisable(void);
void mbWinMesSpeedSet(void);
void mbWinTopMesSpeedSet(void);
void mbWinMesColSet(void);
void mbWinTopMesColSet(void);
void mbWinChoiceNowGet(void);
void mbWinTopChoiceNowGet(void);
void mbWinPriSet(void);
void mbWinTopPriSet(void);
void mbWinChoiceNowGet2(void);
void mbWinTopChoiceNowGet2(void);
void mbWinPlayerDisable(void);
void mbWinTopPlayerDisable(void);
void mbWinIDGet(void);
void mbWinTopIDGet(void);
void mbWinDispSet(void);
void mbWinTopDispSet(void);
void mbWinPauseHook(void);
void mbWinGet(void);
void mbWinCenterGet(void);
void mbWinCenterInsertGet(void);
void mbWinTopCenterInsertGet(void);
void mbWipeSpecialInit(void);
void mbWipeSpecialClose(void);
void mbWipeSpecialKill(void);
void mbWipeWait(void);
void mbWipeCreate(void);
void mbWipeFadeOut(void);
void mbWipeFadeIn(void);
void mbWipeWhiteFadeOut(void);
void mbWipeWhiteFadeIn(void);
void mbWipeDissolveFadeOut(void);
void mbWipeDissolveFadeIn(void);
void mbWipeFadeOutTime(void);
void mbWipeFadeInTime(void);
void mbWipeWhiteFadeOutTime(void);
void mbWipeWhiteFadeInTime(void);
void mbWipeDissolveFadeOutTime(void);
void mbWipeDissolveFadeInTime(void);
void mbWipeSpecialCreate(void);
void mbWipeSpecialWait(void);
void mbWipeSpecialFadeOutCreate(void);
void mbWipeSpecialFadeInCreate(void);
void mbWipeSpecialStatGet(void);
void mbMgCallInit(void);
void mbev_MgCall(void);
void mbev_MgCallKettou(void);
void mbev_MgCallDonkey(void);
void mbev_MgCallKoopa(void);
void mbev_MgCallSingle(void);
void mbev_MgCallSingleKoopa(void);
void mbMgCallDataClose(void);
void mbev_MgCallTutorial(void);
void mbMgRouletteFocusKill(void);
void mbMgRouletteNumGet(void);
void mbMgCallSingleOnCheck(void);
void mbMgCallVsEffCreate(void);
extern void _kerjmp_OSDumpStopwatch(void);
extern void _kerjmp_PSVECDotProduct(void);
extern void _kerjmp_PSVECDistance(void);
extern void _kerjmp_PSVECSquareDistance(void);
extern void _kerjmp_VIGetDTVStatus(void);
extern void _kerjmp_OSGetProgressiveMode(void);
extern void _kerjmp_OSSetProgressiveMode(void);
extern void _kerjmp_PSVECSubtract(void);
extern void _kerjmp_PSMTXTranspose(void);
extern void _kerjmp_C_VECReflect(void);
extern void _kerjmp_PSMTXInvXpose(void);
extern void _kerjmp_PSMTXRotRad(void);
extern void _kerjmp_PSMTXRotAxisRad(void);
extern void _kerjmp_PSVECMag(void);
extern void _kerjmp_PSVECScale(void);
extern void _kerjmp_PSMTXMultVecSR(void);
extern void _kerjmp_C_QUATRotAxisRad(void);
extern void _kerjmp_C_QUATMtx(void);
extern void _kerjmp_C_QUATAdd(void);
extern void _kerjmp_C_QUATSlerp(void);
extern void _kerjmp_PSQUATDotProduct(void);
extern void _kerjmp_PSQUATAdd(void);
extern void _kerjmp_PSMTXQuat(void);
extern void _kerjmp_PSQUATMultiply(void);
extern void _kerjmp_PSQUATInverse(void);
extern void _kerjmp_PSQUATNormalize(void);
extern void _kerjmp_PSQUATScale(void);
extern void _kerjmp_PSQUATSubtract(void);
extern void _kerjmp_OSTicksToCalendarTime(void);
extern void _kerjmp_PSMTXTransApply(void);
extern void _kerjmp_PSMTXScaleApply(void);
extern void _kerjmp_OSGetLanguage(void);
extern void _kerjmp_PSMTXMultVecArraySR(void);
extern void _kerjmp_PSMTX44MultVec(void);
extern void _kerjmp_SIGetTypeString(void);
extern void _kerjmp_OSCheckActiveThreads(void);
extern void _kerjmp_OSGetThreadPriority(void);
extern void _kerjmp_OSInitSemaphore(void);
extern void _kerjmp_OSIsThreadTerminated(void);
extern void _kerjmp_OSSignalSemaphore(void);
extern void _kerjmp_OSWaitSemaphore(void);
extern void _kerjmp_log(void);
extern void _kerjmp_exp(void);
extern void _kerjmp_GXWaitDrawDone(void);
extern void _kerjmp_GXProject(void);
extern void _kerjmp_GXGetProjectionv(void);
extern void _kerjmp_GXGetViewportv(void);
extern void _kerjmp_GXPixModeSync(void);
extern void _kerjmp_GXSetIndTexOrder(void);
extern void _kerjmp_GXSetIndTexMtx(void);
extern void _kerjmp_GXSetTevIndWarp(void);
extern void _kerjmp_GXSetTexCoordScaleManually(void);
extern void _kerjmp_GXSetTevIndTile(void);
extern void _kerjmp_GXNtsc480Prog(void);
extern void _kerjmp_GXNtsc480Int(void);
extern void _kerjmp_GXNtsc240Int(void);
extern void _kerjmp_GXSetVtxDescv(void);
extern void _kerjmp_GXDrawSphere(void);
extern void _kerjmp_GXDrawCylinder(void);
extern void _kerjmp_omSetStatBit(void);
extern void _kerjmp_GameMesKillCheck(void);
extern void _kerjmp_WipeCheck(void);
extern void _kerjmp_WipeTypeGet(void);
extern void _kerjmp_omMakeGroupEx(void);
extern void _kerjmp_omGetGroupMemberListEx(void);
extern void _kerjmp_GameMesDispSet(void);
extern void _kerjmp_GameMesKill(void);
extern void _kerjmp_ClusterAdjustObject(void);
extern void _kerjmp_printWin(void);
extern void _kerjmp_print8(void);
extern void _kerjmp_HuSysVWaitSet(void);
extern void _kerjmp_GameMesGet(void);
extern void _kerjmp_omObjManPause(void);
extern void _kerjmp_GameMesInit(void);
extern void _kerjmp_GameMesCreate(void);
extern void _kerjmp_omDelObjEx(void);
extern void _kerjmp_GameMesPosSet(void);
extern void _kerjmp_atan2(void);
extern void _kerjmp_atan2f(void);
extern void _kerjmp_cos(void);
extern void _kerjmp_GameMesStatGet(void);
extern void _kerjmp_Center(void);
extern void _kerjmp_CRotM(void);
extern void _kerjmp_CenterM(void);
extern void _kerjmp_CZoomM(void);
extern void _kerjmp_omOutViewMulti(void);
extern void _kerjmp_omOutView(void);
extern void _kerjmp_omAddObjEx(void);
extern void _kerjmp_omInitObjMan(void);
extern void _kerjmp_rand8(void);
extern void _kerjmp_frandmod(void);
extern void _kerjmp_frandom(void);
extern void _kerjmp_SetDefLight(void);
extern void _kerjmp_HuSetVecF(void);
extern void _kerjmp_HuSubVecF(void);
extern void _kerjmp_frandf(void);
extern void _kerjmp_GameMesSprKill(void);
extern void _kerjmp_mtxRotCat(void);
extern void _kerjmp_MgScoreBoxInit(void);
extern void _kerjmp_MgScoreBoxCreate(void);
extern void _kerjmp_MgScoreBoxCreateChar(void);
extern void _kerjmp_MgScoreBoxCreateCharMulti(void);
extern void _kerjmp_MgScoreBoxColorSet(void);
extern void _kerjmp_MgScoreBoxTPLvlSet(void);
extern void _kerjmp_MgScoreBoxPosSet(void);
extern void _kerjmp_MgScoreBoxDispSet(void);
extern void _kerjmp_GameMesStrWinCreate(void);
extern void _kerjmp_Hu3DCameraLayerHookSet(void);
extern void _kerjmp_Hu3DCameraLayerHookReset(void);
extern void _kerjmp_Hu3DLayerHookSet(void);
extern void _kerjmp_Hu3DLayerHookReset(void);
extern void _kerjmp_Hu3DNoSyncSet(void);
extern void _kerjmp_Hu3DModelCreate(void);
extern void _kerjmp_Hu3DModelLink(void);
extern void _kerjmp_Hu3DHookFuncCreate(void);
extern void _kerjmp_Hu3DModelPosSet(void);
extern void _kerjmp_Hu3DModelPosSetV(void);
extern void _kerjmp_Hu3DModelRotSet(void);
extern void _kerjmp_Hu3DModelRotSetV(void);
extern void _kerjmp_Hu3DModelScaleSet(void);
extern void _kerjmp_Hu3DModelScaleSetV(void);
extern void _kerjmp_Hu3DModelPosGet(void);
extern void _kerjmp_Hu3DModelRotGet(void);
extern void _kerjmp_Hu3DModelScaleGet(void);
extern void _kerjmp_Hu3DModelKill(void);
extern void _kerjmp_Hu3DModelAllKill(void);
extern void _kerjmp_Hu3DModelAttrSet(void);
extern void _kerjmp_Hu3DModelAttrReset(void);
extern void _kerjmp_Hu3DModelAttrGet(void);
extern void _kerjmp_Hu3DModelMotionAttrGet(void);
extern void _kerjmp_Hu3DBGColorSet(void);
extern void _kerjmp_Hu3DModelCameraSet(void);
extern void _kerjmp_Hu3DModelLayerSet(void);
extern void _kerjmp_Hu3DModelTPLvlSet(void);
extern void _kerjmp_Hu3DModelHookSet(void);
extern void _kerjmp_Hu3DModelHookReset(void);
extern void _kerjmp_Hu3DModelHookObjReset(void);
extern void _kerjmp_Hu3DModelAmbSet(void);
extern void _kerjmp_Hu3DModelShadowDispOn(void);
extern void _kerjmp_Hu3DModelShadowDispOff(void);
extern void _kerjmp_Hu3DModelProjectionSet(void);
extern void _kerjmp_Hu3DModelProjectionReset(void);
extern void _kerjmp_Hu3DModelObjPosGet(void);
extern void _kerjmp_Hu3DModelObjMtxGet(void);
extern void _kerjmp_Hu3DModelHiliteTypeSet(void);
extern void _kerjmp_Hu3DModelReflectTypeSet(void);
extern void _kerjmp_Hu3DModelHiliteMapSet(void);
extern void _kerjmp_Hu3DReflectModelSet(void);
extern void _kerjmp_Hu3DModelMatHookSet(void);
extern void _kerjmp_Hu3DModelDebug(void);
extern void _kerjmp_Hu3DModelMtxSet(void);
extern void _kerjmp_Hu3DModelMtxGet(void);
extern void _kerjmp_Hu3DModelShadowMapTPLvlSet(void);
extern void _kerjmp_Hu3DModelShadowMapObjTPLvlSet(void);
extern void _kerjmp_Hu3DModelDieCreate(void);
extern void _kerjmp_Hu3DModelLinkDieCreate(void);
extern void _kerjmp_HuPadInit(void);
extern void _kerjmp_HuPadRead(void);
extern void _kerjmp_HuPadRumbleSet(void);
extern void _kerjmp_HuPadRumbleStop(void);
extern void _kerjmp_HuPadRumbleAllStop(void);
extern void _kerjmp_HuPadStatGet(void);
extern void _kerjmp_HuPadRumbleGet(void);
extern void _kerjmp_HuPadStkDirGet(void);
extern void _kerjmp_HuPadSubStkDirGet(void);
extern void _kerjmp_Hu3DCameraCreate(void);
extern void _kerjmp_Hu3DCameraPerspectiveSet(void);
extern void _kerjmp_Hu3DCameraViewportSet(void);
extern void _kerjmp_Hu3DCameraScissorSet(void);
extern void _kerjmp_Hu3DCameraPosSet(void);
extern void _kerjmp_Hu3DCameraKill(void);
extern void _kerjmp_Hu3DCameraPosGet(void);
extern void _kerjmp_Hu3DCameraPosSetV(void);
extern void _kerjmp_Hu3DCameraAllKill(void);
extern void _kerjmp_Hu3DCameraSet(void);
extern void _kerjmp_Hu3DCameraMotionOn(void);
extern void _kerjmp_Hu3DCameraMotionStart(void);
extern void _kerjmp_Hu3DCameraMotionOff(void);
extern void _kerjmp_Hu3DModelCameraCreate(void);
extern void _kerjmp_Hu3DModelCameraInfoSet(void);
extern void _kerjmp_Hu3DCameraPerspectiveGet(void);
extern void _kerjmp_Hu3DLighInit(void);
extern void _kerjmp_Hu3DGLightCreate(void);
extern void _kerjmp_Hu3DGLightCreateV(void);
extern void _kerjmp_Hu3DGLightKill(void);
extern void _kerjmp_Hu3DLightAllKill(void);
extern void _kerjmp_Hu3DLightSet(void);
extern void _kerjmp_Hu3DGLightSpotSet(void);
extern void _kerjmp_Hu3DGLightInfinitytSet(void);
extern void _kerjmp_Hu3DGLightPointSet(void);
extern void _kerjmp_Hu3DGLightColorSet(void);
extern void _kerjmp_Hu3DGLightPosSetV(void);
extern void _kerjmp_Hu3DGLightPosSet(void);
extern void _kerjmp_Hu3DGLightPosAimSetV(void);
extern void _kerjmp_Hu3DGLightPosAimSet(void);
extern void _kerjmp_Hu3DGLightStaticSet(void);
extern void _kerjmp_Hu3DLLightParamGet(void);
extern void _kerjmp_Hu3DGLightParamGet(void);
extern void _kerjmp_Hu3DGLightPosAngleSet(void);
extern void _kerjmp_Hu3DModelLightBitSet(void);
extern void _kerjmp_Hu3DModelLightBitReset(void);
extern void _kerjmp_Hu3DModelLightInfoSet(void);
extern void _kerjmp_Hu3DModelLightIdGet(void);
extern void _kerjmp_Hu3DReflectMapSet(void);
extern void _kerjmp_Hu3DReflectNoSet(void);
extern void _kerjmp_Hu3DFogSet(void);
extern void _kerjmp_Hu3DFogClear(void);
extern void _kerjmp_Hu3DModelObjPtrGet(void);
extern void _kerjmp_Hu3DModelShadowSet(void);
extern void _kerjmp_Hu3DModelShadowReset(void);
extern void _kerjmp_Hu3DShadowCreate(void);
extern void _kerjmp_Hu3DShadowPosSet(void);
extern void _kerjmp_Hu3DShadowTPLvlSet(void);
extern void _kerjmp_Hu3DShadowColSet(void);
extern void _kerjmp_Hu3DShadowMultiCreate(void);
extern void _kerjmp_Hu3DShadowMultiPosSet(void);
extern void _kerjmp_Hu3DShadowMultiSizeSet(void);
extern void _kerjmp_Hu3DShadowMultiColSet(void);
extern void _kerjmp_Hu3DShadowMultiTPLvlSet(void);
extern void _kerjmp_Hu3DModelShadowMapSet(void);
extern void _kerjmp_Hu3DModelShadowMapObjSet(void);
extern void _kerjmp_Hu3DModelShadowMapObjReset(void);
extern void _kerjmp_Hu3DModelShadowMapReset(void);
extern void _kerjmp_Hu3DShadowSizeSet(void);
extern void _kerjmp_Hu3DMipMapSet(void);
extern void _kerjmp_Hu3DModelClusterAttrSet(void);
extern void _kerjmp_Hu3DModelClusterAttrReset(void);
extern void _kerjmp_Hu3DLLightCreate(void);
extern void _kerjmp_Hu3DLLightCreateV(void);
extern void _kerjmp_Hu3DLLightSpotSet(void);
extern void _kerjmp_Hu3DLLightInfinitytSet(void);
extern void _kerjmp_Hu3DLLightPointSet(void);
extern void _kerjmp_Hu3DLLightKill(void);
extern void _kerjmp_Hu3DLLightColorSet(void);
extern void _kerjmp_Hu3DLLightPosSetV(void);
extern void _kerjmp_Hu3DLLightPosSet(void);
extern void _kerjmp_Hu3DLLightPosAimSetV(void);
extern void _kerjmp_Hu3DLLightPosAimSet(void);
extern void _kerjmp_Hu3DLLightStaticSet(void);
extern void _kerjmp_Hu3DLLightPosAngleSet(void);
extern void _kerjmp_Hu3DShineSet(void);
extern void _kerjmp_Hu3DObjHookSet(void);
extern void _kerjmp_Hu3DAnimInit(void);
extern void _kerjmp_Hu3DAnimCreate(void);
extern void _kerjmp_Hu3DAnimAnimSet(void);
extern void _kerjmp_Hu3DAnimLink(void);
extern void _kerjmp_Hu3DAnimKill(void);
extern void _kerjmp_Hu3DAnimAllKill(void);
extern void _kerjmp_Hu3DAnimSet(void);
extern void _kerjmp_Hu3DAnimExec(void);
extern void _kerjmp_Hu3DAnimAttrSet(void);
extern void _kerjmp_Hu3DAnimAttrReset(void);
extern void _kerjmp_Hu3DAnimSpeedSet(void);
extern void _kerjmp_Hu3DAnimBankSet(void);
extern void _kerjmp_Hu3DAnmNoSet(void);
extern void _kerjmp_Hu3DTexScrollCreate(void);
extern void _kerjmp_Hu3DTexScrollPosSet(void);
extern void _kerjmp_Hu3DTexScrollPosMoveSet(void);
extern void _kerjmp_Hu3DTexScrollRotSet(void);
extern void _kerjmp_Hu3DTexScrollRotMoveSet(void);
extern void _kerjmp_Hu3DTexScrollKill(void);
extern void _kerjmp_Hu3DTexScrollAllKill(void);
extern void _kerjmp_Hu3DTexScrollPauseDisableSet(void);
extern void _kerjmp_Hu3DParticleCreate(void);
extern void _kerjmp_Hu3DParticleScaleSet(void);
extern void _kerjmp_Hu3DParticleZRotSet(void);
extern void _kerjmp_Hu3DParticleColSet(void);
extern void _kerjmp_Hu3DParticleTPLvlSet(void);
extern void _kerjmp_Hu3DParticleBlendModeSet(void);
extern void _kerjmp_Hu3DParticleHookSet(void);
extern void _kerjmp_Hu3DParticleCntSet(void);
extern void _kerjmp_Hu3DParticleAttrSet(void);
extern void _kerjmp_Hu3DParticleAttrReset(void);
extern void _kerjmp_Hu3DParticleAnimModeSet(void);
extern void _kerjmp_Hu3DParManInit(void);
extern void _kerjmp_Hu3DParManCreate(void);
extern void _kerjmp_Hu3DParManKill(void);
extern void _kerjmp_Hu3DParManAllKill(void);
extern void _kerjmp_Hu3DParManPtrGet(void);
extern void _kerjmp_Hu3DParManAttrSet(void);
extern void _kerjmp_Hu3DParManAttrReset(void);
extern void _kerjmp_Hu3DParManPosSet(void);
extern void _kerjmp_Hu3DParManVecSet(void);
extern void _kerjmp_Hu3DParManRotSet(void);
extern void _kerjmp_Hu3DParManModelIDGet(void);
extern void _kerjmp_Hu3DParManLink(void);
extern void _kerjmp_Hu3DParManTimeLimitSet(void);
extern void _kerjmp_Hu3DParManColorSet(void);
extern void _kerjmp_Hu3DParManLayerSet(void);
extern void _kerjmp_Hu3DWaterCreate(void);
extern void _kerjmp_Hu3DWaterTexPosSet(void);
extern void _kerjmp_Hu3DWaterTexScaleSet(void);
extern void _kerjmp_Hu3DWaterPadYSet(void);
extern void _kerjmp_Hu3DWaterGlowSet(void);
extern void _kerjmp_Hu3DWaterIndTexMtxSet(void);
extern void _kerjmp_Hu3DWaterHiliteSet(void);
extern void _kerjmp_Hu3DWaterHiliteTPLvlSet(void);
extern void _kerjmp_Hu3DWaterCameraSet(void);
extern void _kerjmp_Hu3DWaterWaveCreate(void);
extern void _kerjmp_Hu3DMotionCreate(void);
extern void _kerjmp_Hu3DMotionInit(void);
extern void _kerjmp_Hu3DMotionModelCreate(void);
extern void _kerjmp_Hu3DMotionKill(void);
extern void _kerjmp_Hu3DMotionAllKill(void);
extern void _kerjmp_Hu3DMotionSet(void);
extern void _kerjmp_Hu3DMotionExec(void);
extern void _kerjmp_Hu3DSubMotionExec(void);
extern void _kerjmp_Hu3DMotionNext(void);
extern void _kerjmp_Hu3DMotionTimeSet(void);
extern void _kerjmp_Hu3DMotionTimeGet(void);
extern void _kerjmp_Hu3DMotionMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionStartEndSet(void);
extern void _kerjmp_Hu3DMotionMotionMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionEndCheck(void);
extern void _kerjmp_Hu3DMotionShiftSet(void);
extern void _kerjmp_Hu3DMotionIDGet(void);
extern void _kerjmp_Hu3DMotionShiftIDGet(void);
extern void _kerjmp_Hu3DMotionShiftTimeGet(void);
extern void _kerjmp_Hu3DMotionShiftMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionShiftStartEndSet(void);
extern void _kerjmp_Hu3DMotionShiftSpeedSet(void);
extern void _kerjmp_Hu3DMotionSpeedSet(void);
extern void _kerjmp_Hu3DMotionShapeSpeedSet(void);
extern void _kerjmp_Hu3DMotionShapeTimeSet(void);
extern void _kerjmp_Hu3DMotionShapeTimeGet(void);
extern void _kerjmp_Hu3DMotionNoMotSet(void);
extern void _kerjmp_Hu3DMotionNoMotReset(void);
extern void _kerjmp_Hu3DMotionForceSet(void);
extern void _kerjmp_Hu3DMotionShapeSet(void);
extern void _kerjmp_Hu3DMotionShapeIDGet(void);
extern void _kerjmp_Hu3DMotionShapeMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionClusterSet(void);
extern void _kerjmp_Hu3DMotionClusterNoSet(void);
extern void _kerjmp_Hu3DMotionShapeReset(void);
extern void _kerjmp_Hu3DMotionClusterReset(void);
extern void _kerjmp_Hu3DJointMotion(void);
extern void _kerjmp_Hu3DMotionOverlaySet(void);
extern void _kerjmp_Hu3DMotionOverlayReset(void);
extern void _kerjmp_Hu3DMotionOverlayTimeGet(void);
extern void _kerjmp_Hu3DMotionOverlayTimeSet(void);
extern void _kerjmp_Hu3DMotionOverlaySpeedSet(void);
extern void _kerjmp_Hu3DMotionOverlayMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionOverlayEndCheck(void);
extern void _kerjmp_Hu3DModelObjDrawInit(void);
extern void _kerjmp_Hu3DModelObjDraw(void);
extern void _kerjmp_Hu3DModelObjPtrDraw(void);
extern void _kerjmp_Hu3DMotionCalc(void);
extern void _kerjmp_Hu3DMotionShapeStartEndSet(void);
extern void _kerjmp_Hu3DSubMotionSet(void);
extern void _kerjmp_Hu3DSubMotionReset(void);
extern void _kerjmp_Hu3DSubMotionTimeSet(void);
extern void _kerjmp_Hu3DMotionShiftTimeSet(void);
extern void _kerjmp_Hu3DMotionClusterSpeedSet(void);
extern void _kerjmp_Hu3DMotionClusterTimeSet(void);
extern void _kerjmp_Hu3DMotionClusterMaxTimeGet(void);
extern void _kerjmp_Hu3DMotionAttrSet(void);
extern void _kerjmp_Hu3DMotionAttrReset(void);
extern void _kerjmp_Hu3DMotionTimingHookSet(void);
extern void _kerjmp_Hu3DMotionTimingHookReset(void);
extern void _kerjmp_Hu3DMatLightSet(void);
extern void _kerjmp_Hu3DFbCopyLayerSet(void);
extern void _kerjmp_Hu3DFbCopyExec(void);
extern void _kerjmp_Hu3DZClear(void);
extern void _kerjmp_Hu3DZClearLayerSet(void);
extern void _kerjmp_CamMotionEx(void);
extern void _kerjmp_CamMotionEx2(void);
extern void _kerjmp_CamMotionExPathGet(void);
extern void _kerjmp_CamMotionExPath(void);
extern void _kerjmp_Hu3D2Dto3D(void);
extern void _kerjmp_Hu3D3Dto2D(void);
extern void _kerjmp_Hu3DMtxRotGet(void);
extern void _kerjmp_Hu3DMtxTransGet(void);
extern void _kerjmp_Hu3DMtxScaleGet(void);
extern void _kerjmp_Hu3DProjectionCreate(void);
extern void _kerjmp_Hu3DProjectionPosSet(void);
extern void _kerjmp_Hu3DProjectionTPLvlSet(void);
extern void _kerjmp_Hu3DProjectionKill(void);
extern void _kerjmp_HuAudInit(void);
extern void _kerjmp_HuAudStreamPlay(void);
extern void _kerjmp_HuAudAllStop(void);
extern void _kerjmp_HuAudFadeOut(void);
extern void _kerjmp_HuAudStreamPauseOn(void);
extern void _kerjmp_HuAudStreamPauseOff(void);
extern void _kerjmp_HuAudStreamFadeOut(void);
extern void _kerjmp_HuAudStreamVolSet(void);
extern void _kerjmp_HuAudFXPlay(void);
extern void _kerjmp_HuAudFXPlayVol(void);
extern void _kerjmp_HuAudFXPlayVolPan(void);
extern void _kerjmp_HuAudFXPlayPan(void);
extern void _kerjmp_HuAudFXPlayDelay(void);
extern void _kerjmp_HuAudFXPlayDelayVol(void);
extern void _kerjmp_HuAudFXPlayDelayPan(void);
extern void _kerjmp_HuAudFXPlayDelayVolPan(void);
extern void _kerjmp_HuAudFXStop(void);
extern void _kerjmp_HuAudFXFadeOut(void);
extern void _kerjmp_HuAudFXPanning(void);
extern void _kerjmp_HuAudSeqPlay(void);
extern void _kerjmp_HuAudSeqStop(void);
extern void _kerjmp_HuAudSeqFadeOut(void);
extern void _kerjmp_HuAudSeqMidiCtrlGet(void);
extern void _kerjmp_HuAudFXListnerSet(void);
extern void _kerjmp_HuAudFXListnerUpdate(void);
extern void _kerjmp_HuAudFXEmiterPlay(void);
extern void _kerjmp_HuAudFXEmiterUpDate(void);
extern void _kerjmp_HuAudFXListnerKill(void);
extern void _kerjmp_HuAudFXStatusGet(void);
extern void _kerjmp_HuAudFXPitchSet(void);
extern void _kerjmp_HuAudFXVolSet(void);
extern void _kerjmp_HuAudDllSndGrpSet(void);
extern void _kerjmp_HuAudSndGrpSetSet(void);
extern void _kerjmp_HuAudSndGrpSet(void);
extern void _kerjmp_HuAudSndCommonGrpSet(void);
extern void _kerjmp_HuAudSStreamPlay(void);
extern void _kerjmp_HuAudSStreamStop(void);
extern void _kerjmp_HuAudSStreamFadeOut(void);
extern void _kerjmp_HuAudSStreamAllFadeOut(void);
extern void _kerjmp_HuAudSStreamAllStop(void);
extern void _kerjmp_HuAudSStreamStatGet(void);
extern void _kerjmp_HuAudSStreamPause(void);
extern void _kerjmp_HuAudSStreamPauseFadeOut(void);
extern void _kerjmp_HuAudSStreamParamSet(void);
extern void _kerjmp_PlayerFXPlay(void);
extern void _kerjmp_CharFXPlay(void);
extern void _kerjmp_CharFXPlayVol(void);
extern void _kerjmp_CharFXPlayPan(void);
extern void _kerjmp_CharFXPlayVolPan(void);
extern void _kerjmp_CharFXPlayDelayVolPan(void);
extern void _kerjmp_CharFXPlayDelay(void);
extern void _kerjmp_CharFXPlayDelayVol(void);
extern void _kerjmp_CharFXPlayDelayPan(void);
extern void _kerjmp_PlayerFXPlayPos(void);
extern void _kerjmp_CharFXPlayPos(void);
extern void _kerjmp_PlayerFXStop(void);
extern void _kerjmp_CharFXStop(void);
extern void _kerjmp_HuAudAUXSet(void);
extern void _kerjmp_HuAudSStreamChanPlay(void);
extern void _kerjmp_HuAudBGMPlay(void);
extern void _kerjmp_HuAudJinglePlay(void);
extern void _kerjmp_msmMusPlay(void);
extern void _kerjmp_espInit(void);
extern void _kerjmp_espEntry(void);
extern void _kerjmp_espKill(void);
extern void _kerjmp_espGrpIDGet(void);
extern void _kerjmp_espDispOn(void);
extern void _kerjmp_espDispOff(void);
extern void _kerjmp_espAttrSet(void);
extern void _kerjmp_espAttrReset(void);
extern void _kerjmp_espPosSet(void);
extern void _kerjmp_espScaleSet(void);
extern void _kerjmp_espZRotSet(void);
extern void _kerjmp_espTPLvlSet(void);
extern void _kerjmp_espColorSet(void);
extern void _kerjmp_espSpeedSet(void);
extern void _kerjmp_espBankSet(void);
extern void _kerjmp_espDrawNoSet(void);
extern void _kerjmp_espPriSet(void);
extern void _kerjmp_HuPrcInit(void);
extern void _kerjmp_HuPrcCall(void);
extern void _kerjmp_HuPrcCreate(void);
extern void _kerjmp_HuPrcKill(void);
extern void _kerjmp_HuPrcEnd(void);
extern void _kerjmp_HuPrcWakeup(void);
extern void _kerjmp_HuPrcSleep(void);
extern void _kerjmp_HuPrcVSleep(void);
extern void _kerjmp_HuPrcCurrentGet(void);
extern void _kerjmp_HuPrcDestructorSet(void);
extern void _kerjmp_HuPrcDestructorSet2(void);
extern void _kerjmp_HuPrcChildCreate(void);
extern void _kerjmp_HuPrcChildLink(void);
extern void _kerjmp_HuPrcChildUnlink(void);
extern void _kerjmp_HuPrcChildKill(void);
extern void _kerjmp_HuPrcMemAlloc(void);
extern void _kerjmp_HuPrcMemFree(void);
extern void _kerjmp_Hu3DParManVacumeSet(void);
extern void _kerjmp_HuDataInit(void);
extern void _kerjmp_HuDataDirRead(void);
extern void _kerjmp_HuDataRead(void);
extern void _kerjmp_HuDataReadNum(void);
extern void _kerjmp_HuDataReadMulti(void);
extern void _kerjmp_HuDataDirReadAsync(void);
extern void _kerjmp_HuDataDirReadNumAsync(void);
extern void _kerjmp_HuDataSelHeapRead(void);
extern void _kerjmp_HuDataSelHeapReadNum(void);
extern void _kerjmp_HuDataGetSize(void);
extern void _kerjmp_HuDataDirClose(void);
extern void _kerjmp_HuDataDirCloseNum(void);
extern void _kerjmp_HuDataDirCloseAll(void);
extern void _kerjmp_HuDataClose(void);
extern void _kerjmp_HuDataCloseMulti(void);
extern void _kerjmp_HuDataReadNumHeapShortForce(void);
extern void _kerjmp_HuDataDirCancel(void);
extern void _kerjmp_ARRegisterDMACallback(void);
extern void _kerjmp_ARGetDMAStatus(void);
extern void _kerjmp_ARStartDMA(void);
extern void _kerjmp_ARInit(void);
extern void _kerjmp_ARAlloc(void);
extern void _kerjmp_ARFree(void);
extern void _kerjmp_ARGetSize(void);
extern void _kerjmp_ARSetSize(void);
extern void _kerjmp_HuSprBegin(void);
extern void _kerjmp_HuSprCall(void);
extern void _kerjmp_HuSprFinish(void);
extern void _kerjmp_HuSprAnimRead(void);
extern void _kerjmp_HuSprCreate(void);
extern void _kerjmp_HuSprGrpCreate(void);
extern void _kerjmp_HuSprGrpMemberSet(void);
extern void _kerjmp_HuSprGrpMemberKill(void);
extern void _kerjmp_HuSprGrpKill(void);
extern void _kerjmp_HuSprKill(void);
extern void _kerjmp_HuSprAnimKill(void);
extern void _kerjmp_HuSprAttrSet(void);
extern void _kerjmp_HuSprAttrReset(void);
extern void _kerjmp_HuSprAttrGet(void);
extern void _kerjmp_HuSprPosSet(void);
extern void _kerjmp_HuSprScaleSet(void);
extern void _kerjmp_HuSprZRotSet(void);
extern void _kerjmp_HuSprGrpPosSet(void);
extern void _kerjmp_HuSprGrpZRotSet(void);
extern void _kerjmp_HuSprGrpScaleSet(void);
extern void _kerjmp_HuSprTPLvlSet(void);
extern void _kerjmp_HuSprColorSet(void);
extern void _kerjmp_HuSprSpeedSet(void);
extern void _kerjmp_HuSprBankSet(void);
extern void _kerjmp_HuSprGrpDrawNoSet(void);
extern void _kerjmp_HuSprDrawNoSet(void);
extern void _kerjmp_HuSprPriSet(void);
extern void _kerjmp_HuSprFuncCreate(void);
extern void _kerjmp_HuSprAnimMake(void);
extern void _kerjmp_HuSprGrpCenterSet(void);
extern void _kerjmp_HuSprAnimLock(void);
extern void _kerjmp_AnimDebug(void);
extern void _kerjmp_HuSprExecLayerSet(void);
extern void _kerjmp_HuSprSprBGSet(void);
extern void _kerjmp_HuSprBGSet(void);
extern void _kerjmp_HuSprGrpScissorSet(void);
extern void _kerjmp_HuSprScissorSet(void);
extern void _kerjmp_HuSprGrpTPLvlSet(void);
extern void _kerjmp_HuSprExecLayerCameraSet(void);
extern void _kerjmp_HuSprAnimNoSet(void);
extern void _kerjmp_HuSprAnimNoSetPause(void);
extern void _kerjmp_HuSpr3DHookSet(void);
extern void _kerjmp_HuSpr3DDataCreate(void);
extern void _kerjmp_HuSpr3DSet(void);
extern void _kerjmp_HuSpr3DRotSet(void);
extern void _kerjmp_HuSpr3DFovSet(void);
extern void _kerjmp_HuSprVtxColorSet(void);
extern void _kerjmp_HuSprVtxColorReset(void);
extern void _kerjmp_HuMemMemoryNumSet(void);
extern void _kerjmp_HuARMalloc(void);
extern void _kerjmp_HuARFree(void);
extern void _kerjmp_HuAR_DVDtoARAM(void);
extern void _kerjmp_HuAR_MRAMtoARAM(void);
extern void _kerjmp_HuAR_MRAMtoARAM2(void);
extern void _kerjmp_HuAR_ARAMtoMRAM(void);
extern void _kerjmp_HuAR_ARAMtoMRAMNum(void);
extern void _kerjmp_HuARDMACheck(void);
extern void _kerjmp_HuARDirCheck(void);
extern void _kerjmp_HuARDirFree(void);
extern void _kerjmp_HuAR_ARAMtoMRAMFileRead(void);
extern void _kerjmp_HuAudFXListnerSetEX(void);
extern void _kerjmp_HuWindowInit(void);
extern void _kerjmp_HuWinInit(void);
extern void _kerjmp_HuWinCreate(void);
extern void _kerjmp_HuWinKill(void);
extern void _kerjmp_HuWinAllKill(void);
extern void _kerjmp_HuWinMesSet(void);
extern void _kerjmp_HuWinInsertMesSet(void);
extern void _kerjmp_HuWinChoiceGet(void);
extern void _kerjmp_HuWinChoiceSet(void);
extern void _kerjmp_HuWinChoiceNumGet(void);
extern void _kerjmp_HuWinMesWait(void);
extern void _kerjmp_HuWinSprKill(void);
extern void _kerjmp_HuWinMesRead(void);
extern void _kerjmp_HuWinAnimSet(void);
extern void _kerjmp_HuWinSprSet(void);
extern void _kerjmp_HuWinSprPosSet(void);
extern void _kerjmp_HuWinSprIDGet(void);
extern void _kerjmp_HuWinMesMaxSizeGet(void);
extern void _kerjmp_HuWinInsertMesSizeGet(void);
extern void _kerjmp_HuWinMesMaxSizeBetGet(void);
extern void _kerjmp_HuWinExCreate(void);
extern void _kerjmp_HuWinDispOn(void);
extern void _kerjmp_HuWinDispOff(void);
extern void _kerjmp_HuWinExOpen(void);
extern void _kerjmp_HuWinExClose(void);
extern void _kerjmp_HuWinExKill(void);
extern void _kerjmp_HuWinExSpeakerSet(void);
extern void _kerjmp_HuWinComKeyWait(void);
extern void _kerjmp_HuWinComKeySet(void);
extern void _kerjmp_HuWinComKeyReset(void);
extern void _kerjmp_HuWinDrawNoSet(void);
extern void _kerjmp_HuWinAttrSet(void);
extern void _kerjmp_HuWinAttrReset(void);
extern void _kerjmp_HuWinBGTPLvlSet(void);
extern void _kerjmp_HuWinMesSpeedSet(void);
extern void _kerjmp_HuWinBGColSet(void);
extern void _kerjmp_HuWinChoiceDisable(void);
extern void _kerjmp_HuWinKeyWaitEntry(void);
extern void _kerjmp_HuWinCenterPosSet(void);
extern void _kerjmp_HuWinPosSet(void);
extern void _kerjmp_HuWinZRotSet(void);
extern void _kerjmp_HuWinScaleSet(void);
extern void _kerjmp_HuWinChoiceNowGet(void);
extern void _kerjmp_HuWinStatGet(void);
extern void _kerjmp_HuWinMesColSet(void);
extern void _kerjmp_HuWinScissorSet(void);
extern void _kerjmp_HuWinExCreateFrame(void);
extern void _kerjmp_HuWinPriSet(void);
extern void _kerjmp_HuWinSprPriSet(void);
extern void _kerjmp_HuWinKeyWaitNumGet(void);
extern void _kerjmp_HuWinMesSizeCancelCRSet(void);
extern void _kerjmp_HuWinMesPalSet(void);
extern void _kerjmp_HuWinHomeClear(void);
extern void _kerjmp_HuWinPushKeySet(void);
extern void _kerjmp_HuWinDisablePlayerSet(void);
extern void _kerjmp_HuWinDisablePlayerReset(void);
extern void _kerjmp_HuWinMesPtrGet(void);
extern void _kerjmp_HuWinMesMaxDirGet(void);
extern void _kerjmp_HuWinMesMaxNumGet(void);
extern void _kerjmp_HuWinMesLanguageSet(void);
extern void _kerjmp_HuWinWarningCreate(void);
extern void _kerjmp_HuWinWarningKill(void);
extern void _kerjmp_HuWinWarningOpen(void);
extern void _kerjmp_HuWinWarningClose(void);
extern void _kerjmp_HuWinFrameSet(void);
extern void _kerjmp_HuWinCallbackStub(void);
extern void _kerjmp_HuWinCallbackSet(void);
extern void _kerjmp_HuWinPadMaskSet(void);
extern void _kerjmp_CharInit(void);
extern void _kerjmp_CharMotionInit(void);
extern void _kerjmp_CharMotionInitAsync(void);
extern void _kerjmp_CharDataClose(void);
extern void _kerjmp_CharModelKill(void);
extern void _kerjmp_CharClose(void);
extern void _kerjmp_CharModelCreate(void);
extern void _kerjmp_CharMotionCreate(void);
extern void _kerjmp_CharMotionNoSet(void);
extern void _kerjmp_CharMotionKill(void);
extern void _kerjmp_CharMotionDataClose(void);
extern void _kerjmp_CharModelDataClose(void);
extern void _kerjmp_CharMotionAMemPGet(void);
extern void _kerjmp_CharMotionSet(void);
extern void _kerjmp_CharMotionTimeSet(void);
extern void _kerjmp_CharMotionTimeGet(void);
extern void _kerjmp_CharMotionMaxTimeGet(void);
extern void _kerjmp_CharMotionEndCheck(void);
extern void _kerjmp_CharMotionShiftIDGet(void);
extern void _kerjmp_CharMotionShiftSet(void);
extern void _kerjmp_CharMotionShiftTimeGet(void);
extern void _kerjmp_CharMotionSpeedSet(void);
extern void _kerjmp_CharModelHookDustCreate(void);
extern void _kerjmp_CharEffectSmokeCreate(void);
extern void _kerjmp_CharEffectSmokeCreateScale(void);
extern void _kerjmp_CharEffectCoinGlowCreate(void);
extern void _kerjmp_CharModelHitCreate(void);
extern void _kerjmp_CharEffectHitCreate(void);
extern void _kerjmp_CharEffectShoeHitCreate(void);
extern void _kerjmp_CharModelShoeHitCreate(void);
extern void _kerjmp_GWCharColorGet(void);
extern void _kerjmp_CharEffectLayerSet(void);
extern void _kerjmp_CharEyeBmpNameTbl(void);
extern void _kerjmp_CharModelEyeBmpGet(void);
extern void _kerjmp_CharMotionVoiceOnSet(void);
extern void _kerjmp_CharNpcDustVoiceOffSet(void);
extern void _kerjmp_CharModelStepSet(void);
extern void _kerjmp_CharModelVoicePanAutoSet(void);
extern void _kerjmp_CharModelVoiceFlagSet(void);
extern void _kerjmp_CharModelLandDustCreate(void);
extern void _kerjmp_CharModelAttrSet(void);
extern void _kerjmp_CharModelAttrReset(void);
extern void _kerjmp_CharMotionStartEndSet(void);
extern void _kerjmp_CharModelHeightGet(void);
extern void _kerjmp_CharModelMotListCreate(void);
extern void _kerjmp_CharEffectDustCreate(void);
extern void _kerjmp_CharEffectCryCreate(void);
extern void _kerjmp_CharModelCryCreate(void);
extern void _kerjmp_CharModelTimingHookNoGet(void);
extern void _kerjmp_CharMotionUpdateSet(void);
extern void _kerjmp_CharModelVoicePanSet(void);
extern void _kerjmp_CharModelVoiceVolSet(void);
extern void _kerjmp_CharWinLoseVoicePlay(void);
extern void _kerjmp_CharLoseVoicePlay(void);
extern void _kerjmp_CharMotionTotalTimeGet(void);
extern void _kerjmp_CharMotionExtraTimeGet(void);
extern void _kerjmp_CharLightCreate(void);
extern void _kerjmp_CharLightCreateV(void);
extern void _kerjmp_CharLightSpotSet(void);
extern void _kerjmp_CharLightInfinitytSet(void);
extern void _kerjmp_CharLightPointSet(void);
extern void _kerjmp_CharLightColorSet(void);
extern void _kerjmp_CharLightPosAimSetV(void);
extern void _kerjmp_CharLightPosAimSet(void);
extern void _kerjmp_CharLightStaticSet(void);
extern void _kerjmp_CharEffectWarnCreate(void);
extern void _kerjmp_CharMotionLoad(void);
extern void _kerjmp_CharNpcDustSet(void);
extern void _kerjmp_HuPrcSetStat(void);
extern void _kerjmp_HuPrcResetStat(void);
extern void _kerjmp_omGameSysInit(void);
extern void _kerjmp_omResetStatBit(void);
extern void _kerjmp_omCurrentOvlGet(void);
extern void _kerjmp_omPauseChk(void);
extern void _kerjmp_omOvlHisChg(void);
extern void _kerjmp_omOvlHisGet(void);
extern void _kerjmp_omovlevtno(void);
extern void _kerjmp_omovlstat(void);
extern void _kerjmp_AddX(void);
extern void _kerjmp_AddZ(void);
extern void _kerjmp_MapObject(void);
extern void _kerjmp_CharObject(void);
extern void _kerjmp_nMap(void);
extern void _kerjmp_nChar(void);
extern void _kerjmp_MapWall(void);
extern void _kerjmp_MapPos(void);
extern void _kerjmp_HitFaceVec(void);
extern void _kerjmp_HitFace(void);
extern void _kerjmp_HitFaceCount(void);
extern void _kerjmp_GwPlayer(void);
extern void _kerjmp_GwSystem(void);
extern void _kerjmp_GWSinglePrizeFlagSet(void);
extern void _kerjmp_GWSinglePrizeFlagGet(void);
extern void _kerjmp_GWSingleDataInit(void);
extern void _kerjmp_GWSinglePrizeSaveFlagSet(void);
extern void _kerjmp_GWSinglePrizeSaveFlagGet(void);
extern void _kerjmp_GWBankStarAdd(void);
extern void _kerjmp_GWBankStarSub(void);
extern void _kerjmp_GWBankStarGet(void);
extern void _kerjmp_GWBankStarAwardGet(void);
extern void _kerjmp_GWBankStarAwardReset(void);
extern void _kerjmp_GWBankFlagSet(void);
extern void _kerjmp_GWBankFlagReset(void);
extern void _kerjmp_GWBankFlagGet(void);
extern void _kerjmp_GWSingleMgFlagGet(void);
extern void _kerjmp_GWSingleMgWinNumGet(void);
extern void _kerjmp_GWSingleMgWinNumSet(void);
extern void _kerjmp_GWSingleMgRecordNumGet(void);
extern void _kerjmp_GWSingleMgRecordNumSet(void);
extern void _kerjmp_HuTHPSprCreateVol(void);
extern void _kerjmp_HuTHPSprCreate(void);
extern void _kerjmp_HuTHP3DCreateVol(void);
extern void _kerjmp_HuTHP3DCreate(void);
extern void _kerjmp_HuTHPStop(void);
extern void _kerjmp_HuTHPClose(void);
extern void _kerjmp_HuTHPRestart(void);
extern void _kerjmp_HuTHPEndCheck(void);
extern void _kerjmp_HuTHPFrameGet(void);
extern void _kerjmp_HuTHPTotalFrameGet(void);
extern void _kerjmp_HuTHPSetVolume(void);
extern void _kerjmp_THPSimpleGetFrameRate(void);
extern void _kerjmp_HuTHPMatHook(void);
extern void _kerjmp_HuTHPStartCheck(void);
extern void _kerjmp_HuCardCheck(void);
extern void _kerjmp_HuCardMount(void);
extern void _kerjmp_HuCardFormat(void);
extern void _kerjmp_HuCardOpen(void);
extern void _kerjmp_HuCardRead(void);
extern void _kerjmp_HuCardCreate(void);
extern void _kerjmp_HuCardClose(void);
extern void _kerjmp_HuCardWrite(void);
extern void _kerjmp_HuCardDelete(void);
extern void _kerjmp_HuCardSectorSizeGet(void);
extern void _kerjmp_HuCardFreeSpaceGet(void);
extern void _kerjmp_HuCardSlotCheck(void);
extern void _kerjmp_HuCardWriteAsync(void);
extern void _kerjmp_HuCardWriteCheck(void);
extern void _kerjmp_HuCardWriteIdle(void);
extern void _kerjmp_SLFileOpen(void);
extern void _kerjmp_SLFileCreate(void);
extern void _kerjmp_SLFileWrite(void);
extern void _kerjmp_SLFileClose(void);
extern void _kerjmp_SLFileRead(void);
extern void _kerjmp_SLSaveEmptySet(void);
extern void _kerjmp_SLCurSlotNoSet(void);
extern void _kerjmp_SLCurSlotNoGet(void);
extern void _kerjmp_SLSaveDataMake(void);
extern void _kerjmp_SLSaveDataSlotMake(void);
extern void _kerjmp_SLCommonSet(void);
extern void _kerjmp_SLBoardSave(void);
extern void _kerjmp_SLSave(void);
extern void _kerjmp_SLCommonLoad(void);
extern void _kerjmp_SLBoardLoad(void);
extern void _kerjmp_SLLoad(void);
extern void _kerjmp_SLCurBoxNoSet(void);
extern void _kerjmp_SLCurBoxNoGet(void);
extern void _kerjmp_SLSaveFlagSet(void);
extern void _kerjmp_SLSaveFlagGet(void);
extern void _kerjmp_SLCheckSumCheck(void);
extern void _kerjmp_SLCheckSumBoxSlotCheck(void);
extern void _kerjmp_SLCheckSumGet(void);
extern void _kerjmp_SLCheckSumSlotGet(void);
extern void _kerjmp_SLSerialNoGet(void);
extern void _kerjmp_SLSerialNoCheck(void);
extern void _kerjmp_saveExecF(void);
extern void _kerjmp_SLWinIdSet(void);
extern void _kerjmp_SLBoxBackupLoad(void);
extern void _kerjmp_SLBoxBackupSlotLoad(void);
extern void _kerjmp_SLCheckSumBoxSet(void);
extern void _kerjmp_SLBoxDataOffsetGet(void);
extern void _kerjmp_SLSaveBoardTurnExec(void);
extern void _kerjmp_SLSaveBoardEndExec(void);
extern void _kerjmp_SLSaveModeExec(void);
extern void _kerjmp_curSlotNo(void);
extern void _kerjmp_UnMountCnt(void);
extern void _kerjmp_SLCommonSaveCopy(void);
extern void _kerjmp_SLCommonLoadCopy(void);
extern void _kerjmp__CheckFlag(void);
extern void _kerjmp__SetFlag(void);
extern void _kerjmp__ClearFlag(void);
extern void _kerjmp_MgDataTbl(void);
extern void _kerjmp_MgModeSubMode(void);
extern void _kerjmp_MgModeScore(void);
extern void _kerjmp_MgModePlayNum_0(void);
extern void _kerjmp_MgModeWork(void);
extern void _kerjmp_MgModePlayNum_1(void);
extern void _kerjmp_GwCommonBackup(void);
extern void _kerjmp_lbl_802C0438(void);
extern void _kerjmp_MgExitReq(void);
extern void _kerjmp_MgPauseExitF(void);
extern void _kerjmp_MgBoard2Force(void);
extern void _kerjmp_MgNoGet(void);
extern void _kerjmp_MiracleBookEvtNo(void);
extern void _kerjmp_MgSubMode(void);
extern void _kerjmp_MgSubModeGet(void);
extern void _kerjmp_MgBattleOrder(void);
extern void _kerjmp_MgDecaScoreCalc(void);
extern void _kerjmp_MgInstExitF(void);
extern void _kerjmp_GWRecordSet(void);
extern void _kerjmp_GWRecordGet(void);
extern void _kerjmp_GWBoardPlayNumAdd(void);
extern void _kerjmp_GWBoardPlayNumSet(void);
extern void _kerjmp_GWBoardPlayNumGet(void);
extern void _kerjmp_GWCharPlayNumInc(void);
extern void _kerjmp_GWCharPlayNumGet(void);
extern void _kerjmp_GWCharPlayNumSet(void);
extern void _kerjmp_GWBoardMaxStarSet(void);
extern void _kerjmp_GWBoardMaxStarGet(void);
extern void _kerjmp_GWBoardMaxCoinSet(void);
extern void _kerjmp_GWBoardMaxCoinGet(void);
extern void _kerjmp_GWPlayerStarAdd(void);
extern void _kerjmp_GWPlayerStarGet(void);
extern void _kerjmp_GWPlayerStarSet(void);
extern void _kerjmp_GWPlayerCoinAdd(void);
extern void _kerjmp_GWPlayerCoinSet(void);
extern void _kerjmp_GWPlayerCoinGet(void);
extern void _kerjmp_GWMgCustomReset(void);
extern void _kerjmp_GWMgCustomSet(void);
extern void _kerjmp_GWMgCustomGet(void);
extern void _kerjmp_GWMgUnlockGet(void);
extern void _kerjmp_GWMgPlayNumSet(void);
extern void _kerjmp_GWMgPlayNumGet(void);
extern void _kerjmp_GWMgPlayNumAdd(void);
extern void _kerjmp_GWMiracleBookFlagSet(void);
extern void _kerjmp_GWMiracleBookFlagGet(void);
extern void _kerjmp_GWMikeActRecordSet(void);
extern void _kerjmp_GWMikeActRecordGet(void);
extern void _kerjmp_GWSaveDebugSet(void);
extern void _kerjmp_MgModeWorkInt(void);
extern void _kerjmp_MgModeWorkFloat(void);
extern void _kerjmp_GwMgDecaScore(void);
extern void _kerjmp_GwLanguage(void);
extern void _kerjmp_fmod(void);
extern void _kerjmp__savegpr_15(void);
extern void _kerjmp__restgpr_15(void);
extern void _kerjmp__savegpr_16(void);
extern void _kerjmp__restgpr_16(void);
extern void _kerjmp__savegpr_14(void);
extern void _kerjmp__restgpr_14(void);
extern void _kerjmp_omDBGMenuButton(void);
extern void _kerjmp_omDBGSystemKeyCheckSetup(void);
extern void _kerjmp_msmSeSetListener(void);
extern void _kerjmp_msmSeUpdataListener(void);
extern void _kerjmp_msmSeGetIndexPtr(void);
extern void _kerjmp_msmMusSetMasterVolume(void);
extern void _kerjmp_msmSysGetOutputMode(void);
extern void _kerjmp_msmSysSetGroupLoadMode(void);
extern void _kerjmp_WipeCreate(void);
extern void _kerjmp_WipeColorSet(void);
extern void _kerjmp_omOvlReturnEx(void);
extern void _kerjmp_HuDataGetAsyncStat(void);
extern void _kerjmp_omVibrate(void);
extern void _kerjmp_GameMesOvlPrev(void);
extern void _kerjmp_MgTimerCreate(void);
extern void _kerjmp_MgTimerKill(void);
extern void _kerjmp_MgTimerModeGet(void);
extern void _kerjmp_MgTimerModeOnSet(void);
extern void _kerjmp_MgTimerModeOffSet(void);
extern void _kerjmp_MgTimerParamSet(void);
extern void _kerjmp_MgTimerValueGet(void);
extern void _kerjmp_MgTimerDoneCheck(void);
extern void _kerjmp_MgTimerPosSet(void);
extern void _kerjmp_MgTimerPosGet(void);
extern void _kerjmp_MgTimerBackColorSet(void);
extern void _kerjmp_MgTimerColorSet(void);
extern void _kerjmp_MgTimerRecordSet(void);
extern void _kerjmp_MgTimerRecordDispOn(void);
extern void _kerjmp_MgTimerRecordDispOff(void);
extern void _kerjmp_MgScoreInit(void);
extern void _kerjmp_MgScoreKill(void);
extern void _kerjmp_MgScoreModeGet(void);
extern void _kerjmp_MgScoreValueSet(void);
extern void _kerjmp_MgScoreValueGet(void);
extern void _kerjmp_MgScoreMaxDigitSet(void);
extern void _kerjmp_MgScoreMaxDigitGet(void);
extern void _kerjmp_MgScorePosSet(void);
extern void _kerjmp_MgScorePosGet(void);
extern void _kerjmp_MgScoreScaleSet(void);
extern void _kerjmp_MgScoreScaleGet(void);
extern void _kerjmp_MgScoreZRotSet(void);
extern void _kerjmp_MgScoreZRotGet(void);
extern void _kerjmp_MgScoreDigitWidthSet(void);
extern void _kerjmp_MgScoreDigitWidthGet(void);
extern void _kerjmp_MgScoreDispOn(void);
extern void _kerjmp_MgScoreDispOff(void);
extern void _kerjmp_MgScoreTPLvlSet(void);
extern void _kerjmp_MgScoreTPLvlGet(void);
extern void _kerjmp_MgScoreModeDefaultSet(void);
extern void _kerjmp_MgScoreCreate(void);
extern void _kerjmp_MgScoreColorSet(void);
extern void _kerjmp_MgScoreDigitScaleSet(void);
extern void _kerjmp_MgScorePriSet(void);
extern void _kerjmp_MgScoreUnitBankSet(void);
extern void _kerjmp_MgSeqCreate(void);
extern void _kerjmp_MgSeqCreatePrio(void);
extern void _kerjmp_MgSeqKill(void);
extern void _kerjmp_MgSeqModeGet(void);
extern void _kerjmp_MgSeqModeNext(void);
extern void _kerjmp_MgSeqModeSet(void);
extern void _kerjmp_MgSeqRecordSet(void);
extern void _kerjmp_MgSeqModeChangeOff(void);
extern void _kerjmp_MgSeqModeChangeOn(void);
extern void _kerjmp_MgSeqStatGet(void);
extern void _kerjmp_MgSeqWinnerSet(void);
extern void _kerjmp_MgSeqDrawSet(void);
extern void _kerjmp_fn_80071CCC(void);
extern void _kerjmp_MgSeqModeDelaySet(void);
extern void _kerjmp_MgSeqModeHookAdd(void);
extern void _kerjmp_MgSeqModeHookReset(void);
extern void _kerjmp_MgSeqTimerValueGet(void);
extern void _kerjmp_MgSeqFrameNoGet(void);
extern void _kerjmp_MgSeqFrameNoSet(void);
extern void _kerjmp_MgSeqGameMesIdGet(void);
extern void _kerjmp_MgSeqTimerKill(void);
extern void _kerjmp_MgSeqStatBitSet(void);
extern void _kerjmp_MgSeqStatBitReset(void);
extern void _kerjmp_omCameraViewInit(void);
extern void _kerjmp_omCameraViewSetMulti(void);
extern void _kerjmp_omCameraViewSet(void);
extern void _kerjmp_omCameraViewMoveSimpleMulti(void);
extern void _kerjmp_omCameraViewMoveSimple(void);
extern void _kerjmp_omCameraViewMoveMulti(void);
extern void _kerjmp_omCameraViewMove(void);
extern void _kerjmp_omCameraViewCheck(void);
extern void _kerjmp_MgActorObjectSetup(void);
extern void _kerjmp_MgActorColMapInit(void);
extern void _kerjmp_MgPlayerPadSet(void);
extern void _kerjmp_MgPlayerModeFuncSet(void);
extern void _kerjmp_MgPlayerCreate(void);
extern void _kerjmp_MgPlayerCreateJumpAlt(void);
extern void _kerjmp_MgActorCreate(void);
extern void _kerjmp_MgActorKill(void);
extern void _kerjmp_MgActorExec(void);
extern void _kerjmp_MgPlayerModeLandSet(void);
extern void _kerjmp_MgPlayerModeSet(void);
extern void _kerjmp_MgPlayerComStkOn(void);
extern void _kerjmp_MgPlayerComStkOff(void);
extern void _kerjmp_MgPlayerVecChase(void);
extern void _kerjmp_MgActorVecChase(void);
extern void _kerjmp_MgActorColAttrSet(void);
extern void _kerjmp_MgActorColAttrReset(void);
extern void _kerjmp_MgActorColAttrGet(void);
extern void _kerjmp_MgActorColMapPolyGet(void);
extern void _kerjmp_MgActorPosSet(void);
extern void _kerjmp_MgActorPosSetRaw(void);
extern void _kerjmp_MgActorPosGet(void);
extern void _kerjmp_MgPlayerDespawn(void);
extern void _kerjmp_MgPlayerSpawn(void);
extern void _kerjmp_MgPlayerColDisable(void);
extern void _kerjmp_MgPlayerColEnable(void);
extern void _kerjmp_MgPlayerVibrateCreate(void);
extern void _kerjmp_MgPlayerVibrateKill(void);
extern void _kerjmp_MgActorColAttrParamSet(void);
extern void _kerjmp_MgActorColAttrParamGet(void);
extern void _kerjmp_MgActorColMapMaskSet(void);
extern void _kerjmp_MgActorColMapMaskGet(void);
extern void _kerjmp_MgActorColMaskSet(void);
extern void _kerjmp_MgActorColMaskGet(void);
extern void _kerjmp_MgPlayerAttrCheck(void);
extern void _kerjmp_MgPlayerAttrSet(void);
extern void _kerjmp_MgPlayerAttrReset(void);
extern void _kerjmp_MgPlayerModeAttrCheck(void);
extern void _kerjmp_MgPlayerModeAttrReset(void);
extern void _kerjmp_MgPlayerModeAttrSet(void);
extern void _kerjmp_MgPlayerVibAttrCheck(void);
extern void _kerjmp_MgPlayerVibAttrSet(void);
extern void _kerjmp_MgPlayerStunSet(void);
extern void _kerjmp_MgActorColCylReset(void);
extern void _kerjmp_MgActorColCylSet(void);
extern void _kerjmp_MgActorColCodeGet(void);
extern void _kerjmp_MgActorColNormalGet(void);
extern void _kerjmp_MgActorRotYSet(void);
extern void _kerjmp_MgActorRotYGet(void);
extern void _kerjmp_MgActorPushSet(void);
extern void _kerjmp_MgPlayerPosSet(void);
extern void _kerjmp_MgActorGravitySet(void);
extern void _kerjmp_MgActorVelYSet(void);
extern void _kerjmp_MgActorColMeshGet(void);
extern void _kerjmp_MgActorVelSet(void);
extern void _kerjmp_MgActorColBounceSet(void);
extern void _kerjmp_CharModelFileNumGet(void);
extern void _kerjmp_CharMotionFileNumGet(void);
extern void _kerjmp_lbl_802BF860(void);
extern void _kerjmp_lbl_802BF864(void);
extern void _kerjmp_lbl_802BF868(void);
extern void _kerjmp__savefpr_22(void);
extern void _kerjmp__restfpr_22(void);
extern void _kerjmp__savefpr_24(void);
extern void _kerjmp__restfpr_24(void);
extern void _kerjmp__savefpr_27(void);
extern void _kerjmp__restfpr_27(void);
extern void _kerjmp__savefpr_28(void);
extern void _kerjmp__restfpr_28(void);
extern void _kerjmp___construct_array(void);
extern void _kerjmp___destroy_arr(void);
extern void _kerjmp___msl_assertion_failed(void);
extern void _kerjmp___nw__FUl(void);
extern void _kerjmp___nwa__FUl(void);
extern void _kerjmp___ptmf_test(void);
extern void _kerjmp___ptmf_scall(void);
extern void _kerjmp___dla__FPv(void);
extern void _kerjmp_lbl_802166D8(void);
extern void _kerjmp___ptmf_cmpr(void);
extern void _kerjmp_HuDataDirCancelAsync(void);
extern void _kerjmp_HuDataDirCloseAsync(void);
extern void _kerjmp_HuTHPProcCheck(void);
extern void _kerjmp_mbSaveInit(void);
extern void _kerjmp_mbSavePartyInit(void);
extern void _kerjmp_mbSaveStoryInit(void);
extern void _kerjmp_fn_80146BA0(void);
extern void _kerjmp_fn_80146C88(void);
extern void _kerjmp_HuLoadProcStart(void);
extern void _kerjmp_GwMgNightF(void);
extern void _kerjmp_HuMCSysInit(void);
extern void _kerjmp_HuMCInit(void);
extern void _kerjmp_HuMCClose(void);
extern void _kerjmp_HuMCContextSet(void);
extern void _kerjmp_HuMCResponseGet2(void);
extern void _kerjmp_HuMCStatGet(void);
extern void _kerjmp_HuMCContextCallbackSet(void);
extern void _kerjmp_HuMCResponseGet(void);
extern void _kerjmp_HuMCContextCreate(void);
extern void _kerjmp_HuMCContextKill(void);
extern void _kerjmp_HuMCThresholdSet(void);
extern void _kerjmp_HuMCCurResponseGet(void);
extern void _kerjmp_HuMCButtonGet(void);
extern void _kerjmp_HuMCButtonDownGet(void);
extern void _kerjmp_HuMCProbe(void);
extern void _kerjmp_HuMCMount(void);
extern void _kerjmp_HuMCMicSprCreate(void);
extern void _kerjmp_HuMCMicSprKill(void);
extern void _kerjmp_HuMCMicGet(void);
extern void _kerjmp_HuMCMicSet(void);
extern void _kerjmp_HuMCSelWinCreate(void);
extern void _kerjmp_HuMCSelWinKill(void);
extern void _kerjmp_HuMCSelWinItemSet(void);
extern void _kerjmp_HuMCSelWinItemRandSet(void);
extern void _kerjmp_HuMCSelWinCheck(void);
extern void _kerjmp_HuMCSelModeGet(void);
extern void _kerjmp_HuMCSelWinContextSet(void);
extern void _kerjmp_HuMCSelWinChoiceGet(void);
extern void _kerjmp_HuMCSelWinContextKill(void);
extern void _kerjmp_HuMCSelWinMaxTimeSet(void);
extern void _kerjmp_HuMCSelWinMaxTimeGet(void);
extern void _kerjmp_HuMCListenerCreate(void);
extern void _kerjmp_HuMCListenerKill(void);
extern void _kerjmp_HuMCSessionSet(void);
extern void _kerjmp_HuMCSessionKill(void);
extern void _kerjmp_HuMCSessionClose(void);
extern void _kerjmp_HuMCUnkResponseCheck(void);
extern void _kerjmp_HuMCNewResponseGet(void);
extern void _kerjmp_HuMCVolSampleCreate(void);
extern void _kerjmp_HuMCVolGet(void);
extern void _kerjmp_HuMCShiftsSet(void);
extern void _kerjmp_fn_8014C818(void);
extern void _kerjmp_fn_8014C81C(void);
extern void _kerjmp_mbBranchInit(void);
extern void _kerjmp_mbev_Branch(void);
extern void _kerjmp_mbev_BranchDebug(void);
extern void _kerjmp_mbBranchAttrSet(void);
extern void _kerjmp_mbBranchAttrReset(void);
extern void _kerjmp_mbBranchAttrGet(void);
extern void _kerjmp_mbBranchMAttrSet(void);
extern void _kerjmp_mbBranchMAttrReset(void);
extern void _kerjmp_mbBranchMAttrGet(void);
extern void _kerjmp_mbBranchAttrCheck(void);
extern void _kerjmp_mbBranchComStarHookSet(void);
extern void _kerjmp_mbCameraInit(void);
extern void _kerjmp_mbCameraZoomSet(void);
extern void _kerjmp_mbCameraZoomGet(void);
extern void _kerjmp_mbCameraRotSetV(void);
extern void _kerjmp_mbCameraRotSet(void);
extern void _kerjmp_mbCameraRotGet(void);
extern void _kerjmp_mbCameraCenterSetV(void);
extern void _kerjmp_mbCameraCenterSet(void);
extern void _kerjmp_mbCameraCenterGet(void);
extern void _kerjmp_mbCameraEyeSetV(void);
extern void _kerjmp_mbCameraEyeSet(void);
extern void _kerjmp_mbCameraEyeGet(void);
extern void _kerjmp_mbCameraOffsetSetV(void);
extern void _kerjmp_mbCameraOffsetSet(void);
extern void _kerjmp_mbCameraOffsetGet(void);
extern void _kerjmp_mbCameraDirGet(void);
extern void _kerjmp_mbCameraPosDirGet(void);
extern void _kerjmp_mbCameraLookAtFuncSet(void);
extern void _kerjmp_mbCameraBitSet(void);
extern void _kerjmp_mbCameraFovSet(void);
extern void _kerjmp_mbCameraScissorSet(void);
extern void _kerjmp_mbCameraNearFarSet(void);
extern void _kerjmp_mbCameraNearFarGet(void);
extern void _kerjmp_mbCameraGet(void);
extern void _kerjmp_mbCameraStackPush(void);
extern void _kerjmp_mbCameraStackIdxSet(void);
extern void _kerjmp_mbCameraStackPop(void);
extern void _kerjmp_mbCameraLookAtGet(void);
extern void _kerjmp_mbCameraLookAtInvGet(void);
extern void _kerjmp_mbCameraCurveTypeSet(void);
extern void _kerjmp_mbCameraCurveTypeGet(void);
extern void _kerjmp_mbCameraCullCheck(void);
extern void _kerjmp_mbCameraPlayerViewSet(void);
extern void _kerjmp_mbCameraPlayerViewSetFast(void);
extern void _kerjmp_mbCameraPlayerViewGet(void);
extern void _kerjmp_mbCameraPlayerViewNoGet(void);
extern void _kerjmp_mbCameraPlayerViewZoomGet(void);
extern void _kerjmp_mbCameraPlayerViewRotXGet(void);
extern void _kerjmp_mbCameraFocusReset(void);
extern void _kerjmp_mbCameraFocusPosSet(void);
extern void _kerjmp_mbCameraFocusPosAdd(void);
extern void _kerjmp_mbCameraFocusPlayerSet(void);
extern void _kerjmp_mbCameraFocusPlayerAdd(void);
extern void _kerjmp_mbCameraFocusObjSet(void);
extern void _kerjmp_mbCameraFocusObjAdd(void);
extern void _kerjmp_mbCameraFocusMasuSet(void);
extern void _kerjmp_mbCameraFocusMasuAdd(void);
extern void _kerjmp_mbCameraMoveOnSet(void);
extern void _kerjmp_mbCameraSpeedSet(void);
extern void _kerjmp_mbCameraMovePos(void);
extern void _kerjmp_mbCameraMovePlayer(void);
extern void _kerjmp_mbCameraMoveObj(void);
extern void _kerjmp_mbCameraMoveMasu(void);
extern void _kerjmp_mbCameraShakeSet(void);
extern void _kerjmp_mbCameraShakeReset(void);
extern void _kerjmp_mbCameraMoveStop(void);
extern void _kerjmp_mbCameraMoveCheck(void);
extern void _kerjmp_mbCameraMoveWait(void);
extern void _kerjmp_mbCapSelect(void);
extern void _kerjmp_mbCapDelete(void);
extern void _kerjmp_fn_8019A618(void);
extern void _kerjmp_fn_8019A61C(void);
extern void _kerjmp_fn_8019A620(void);
extern void _kerjmp_fn_8019A624(void);
extern void _kerjmp_fn_8019A628(void);
extern void _kerjmp_fn_8019A62C(void);
extern void _kerjmp_mbCapSelectGrow(void);
extern void _kerjmp_mbCapSelectShrinkCheck(void);
extern void _kerjmp_mbCapSelectResultSet(void);
extern void _kerjmp_mbCapSelectResultGet(void);
extern void _kerjmp_mbCapSelectResultReset(void);
extern void _kerjmp_mbCapMasuExec(void);
extern void _kerjmp_mbCapMasuObjInit(void);
extern void _kerjmp_mbCapMasuObjClose(void);
extern void _kerjmp_mbCapMasuObjCreateAll(void);
extern void _kerjmp_mbCapMasuObjCreate(void);
extern void _kerjmp_mbCapMasuDispSet(void);
extern void _kerjmp_mbCapCapsuleGet(void);
extern void _kerjmp_mbev_CapKinoko(void);
extern void _kerjmp_mbev_CapKinokoKill(void);
extern void _kerjmp_mbev_CapSKinoko(void);
extern void _kerjmp_mbev_CapSKinokoKill(void);
extern void _kerjmp_mbev_CapPKinoko(void);
extern void _kerjmp_mbev_CapPKinokoKill(void);
extern void _kerjmp_mbev_CapMKinoko(void);
extern void _kerjmp_mbev_CapMKinokoKill(void);
extern void _kerjmp_mbev_CapKiller(void);
extern void _kerjmp_mbev_CapKillerKill(void);
extern void _kerjmp_mbev_CapDokan(void);
extern void _kerjmp_mbev_CapDokanKill(void);
extern void _kerjmp_mbev_CapHanachan(void);
extern void _kerjmp_mbev_CapHanachanKill(void);
extern void _kerjmp_mbev_CapNKinoko(void);
extern void _kerjmp_mbev_CapNKinokoKill(void);
extern void _kerjmp_mbev_CapKillerMove(void);
extern void _kerjmp_mbev_CapKillerMoveKill(void);
extern void _kerjmp_mbev_CapTogezo(void);
extern void _kerjmp_mbev_CapTogezoKill(void);
extern void _kerjmp_mbev_CapKuribo(void);
extern void _kerjmp_mbev_CapKuriboKill(void);
extern void _kerjmp_mbev_CapPakkun(void);
extern void _kerjmp_mbev_CapPakkunKill(void);
extern void _kerjmp_mbev_CapJango(void);
extern void _kerjmp_mbev_CapJangoKill(void);
extern void _kerjmp_mbev_CapPatapata(void);
extern void _kerjmp_mbev_CapPatapataKill(void);
extern void _kerjmp_mbev_CapKokamekku(void);
extern void _kerjmp_mbev_CapKokamekkuKill(void);
extern void _kerjmp_mbev_CapKamekku(void);
extern void _kerjmp_mbev_CapKamekkuKill(void);
extern void _kerjmp_mbev_CapThrowman(void);
extern void _kerjmp_mbev_CapThrowmanKill(void);
extern void _kerjmp_mbev_CapBoble(void);
extern void _kerjmp_mbev_CapBobleKill(void);
extern void _kerjmp_mbev_CapBobleTrap(void);
extern void _kerjmp_mbev_CapBobleMove(void);
extern void _kerjmp_mbev_CapBiriQ(void);
extern void _kerjmp_mbev_CapBiriQKill(void);
extern void _kerjmp_mbev_CapBiriQTrap(void);
extern void _kerjmp_mbev_CapBiriQMetalShock(void);
extern void _kerjmp_mbev_CapTumujikun(void);
extern void _kerjmp_mbev_CapTumujikunKill(void);
extern void _kerjmp_mbev_CapTumujikunTrap(void);
extern void _kerjmp_mbev_CapDossun(void);
extern void _kerjmp_mbev_CapDossunKill(void);
extern void _kerjmp_mbev_CapDossunTrap(void);
extern void _kerjmp_mbev_CapBomhei(void);
extern void _kerjmp_mbev_CapBomheiKill(void);
extern void _kerjmp_mbev_CapBomheiTrap(void);
extern void _kerjmp_mbev_CapBomheiMove(void);
extern void _kerjmp_mbev_CapTeresa(void);
extern void _kerjmp_mbev_CapTeresaKill(void);
extern void _kerjmp_mbev_CapTeresaFadeCreate(void);
extern void _kerjmp_mbev_CapTeresaFadeKill(void);
extern void _kerjmp_mbev_CapTeresaFadeSet(void);
extern void _kerjmp_mbev_CapMiracle(void);
extern void _kerjmp_mbev_CapMiracleKill(void);
extern void _kerjmp_mbev_CapKettou(void);
extern void _kerjmp_mbev_CapKettouKill(void);
extern void _kerjmp_mbev_CapDonkey(void);
extern void _kerjmp_mbev_CapDonkeyKill(void);
extern void _kerjmp_mbev_CapKoopa(void);
extern void _kerjmp_mbev_CapKoopaKill(void);
extern void _kerjmp_mbev_CapCall(void);
extern void _kerjmp_mbev_CapCallTrap(void);
extern void _kerjmp_mbev_CapCapGet(void);
extern void _kerjmp_mbev_CapWait(void);
extern void _kerjmp_mbev_CapCallKettou(void);
extern void _kerjmp_mbev_CapCircuitCallKettou(void);
extern void _kerjmp_mbev_CapCallDonkey(void);
extern void _kerjmp_mbev_CapCallKoopa(void);
extern void _kerjmp_MBCapsuleStub5(void);
extern void _kerjmp_MBCapsuleStub6(void);
extern void _kerjmp_MBCapsuleStub7(void);
extern void _kerjmp_mbev_CapCallTeresa(void);
extern void _kerjmp_mbev_CapCallMiracle(void);
extern void _kerjmp_mbev_CapBiriQShockCreate(void);
extern void _kerjmp_mbev_CapBiriQShockDelayGet(void);
extern void _kerjmp_mbev_CapBiriQMetalShockCreate(void);
extern void _kerjmp_mbev_CapRandomBonusCoin(void);
extern void _kerjmp_mbev_CapBonusCoinCall(void);
extern void _kerjmp_mbev_CapBonusCoin(void);
extern void _kerjmp_mbev_CapBonusCoinCheck(void);
extern void _kerjmp_mbev_CapMoveMasuSet(void);
extern void _kerjmp_mbev_CapStopMasuSet(void);
extern void _kerjmp_mbev_CapKettouEndCall(void);
extern void _kerjmp_mbev_CapDonkeyEndCall(void);
extern void _kerjmp_mbev_CapKoopaEndCall(void);
extern void _kerjmp_mbev_CapVsEndCall(void);
extern void _kerjmp_mbev_CapKillerMoveCall(void);
extern void _kerjmp_MBCapsuleStub11(void);
extern void _kerjmp_MBCapsuleStub12(void);
extern void _kerjmp_mbev_CapKillerMoveCheck(void);
extern void _kerjmp_mbev_CapKillerMoveCheckAll(void);
extern void _kerjmp_mbev_CapBankCoinInit(void);
extern void _kerjmp_mbev_CapInit(void);
extern void _kerjmp_mbev_CapKillerCall(void);
extern void _kerjmp_mbev_CapKillerMultiCall(void);
extern void _kerjmp_mbev_CapOpeningAdd(void);
extern void _kerjmp_mbev_CapKoopaAdd(void);
extern void _kerjmp_mbev_CapBankCoinGet(void);
extern void _kerjmp_mbev_CapBubbleHookSet(void);
extern void _kerjmp_mbev_CapBubbleHookCall(void);
extern void _kerjmp_mbev_CapBubbleHookCallStory(void);
extern void _kerjmp_mbev_CapKettouCoinLoseGet(void);
extern void _kerjmp_mbev_CapKettouOppCoinLoseGet(void);
extern void _kerjmp_mbev_CapKettouCoinLoseGet2(void);
extern void _kerjmp_mbev_CapKettouOppCoinLoseGet2(void);
extern void _kerjmp_mbev_CapPlayerMotionCreate(void);
extern void _kerjmp_mbev_CapObjCreate(void);
extern void _kerjmp_mbev_CapObjPosSet(void);
extern void _kerjmp_mbev_CapPlayerPosSet(void);
extern void _kerjmp_mbev_CapSprCreate(void);
extern void _kerjmp_mbev_CapMalloc(void);
extern void _kerjmp_mbev_CapObjClose(void);
extern void _kerjmp_mbev_CapSprClose(void);
extern void _kerjmp_mbev_CapMallocClose(void);
extern void _kerjmp_mbev_CapEffExplodeCreate(void);
extern void _kerjmp_mbev_CapEffExhaustCreate(void);
extern void _kerjmp_mbev_CapEffExplodeOMExec(void);
extern void _kerjmp_mbev_CapEffExplodeKill(void);
extern void _kerjmp_mbev_CapEffExplodeAnimGet(void);
extern void _kerjmp_mbev_CapEffExplodeAnimSet(void);
extern void _kerjmp_mbev_CapEffExplodeAdd(void);
extern void _kerjmp_mbev_CapEffExplodeKillerAdd(void);
extern void _kerjmp_mbev_CapEffExplodeCircleAdd(void);
extern void _kerjmp_mbev_CapEffDustCloudAdd(void);
extern void _kerjmp_mbev_CapEffDustExplodeAdd(void);
extern void _kerjmp_mbev_CapEffDustHeavyAdd(void);
extern void _kerjmp_mbev_CapEffDustMultiAdd(void);
extern void _kerjmp_mbev_CapEffBoostCreate(void);
extern void _kerjmp_mbev_CapEffBoostOMExec(void);
extern void _kerjmp_mbev_CapEffBoostKill(void);
extern void _kerjmp_mbev_CapEffBoostTimeGet(void);
extern void _kerjmp_mbev_CapEffBoostBlendModeSet(void);
extern void _kerjmp_mbev_CapEffBoostAdd(void);
extern void _kerjmp_mbev_CapEffSnowCreate(void);
extern void _kerjmp_mbev_CapEffSnowOMExec(void);
extern void _kerjmp_mbev_CapEffSnowKill(void);
extern void _kerjmp_mbev_CapEffSnowDispGet(void);
extern void _kerjmp_mbev_CapEffSnowAdd(void);
extern void _kerjmp_mbev_CapEffGlowCreate(void);
extern void _kerjmp_mbev_CapEffGlowFireCreate(void);
extern void _kerjmp_mbev_CapEffGlowOMExec(void);
extern void _kerjmp_mbev_CapEffGlowKill(void);
extern void _kerjmp_mbev_CapEffGlowDispGet(void);
extern void _kerjmp_mbev_CapEffGlowPatSet(void);
extern void _kerjmp_mbev_CapEffGlowBlendModeSet(void);
extern void _kerjmp_mbev_CapEffGlowAnimSet(void);
extern void _kerjmp_mbev_CapEffGlowAdd(void);
extern void _kerjmp_mbev_CapEffGlowKinokoAdd(void);
extern void _kerjmp_mbev_CapEffGlowKinokoAddAlt(void);
extern void _kerjmp_mbev_CapEffGlowKinokoTimeSet(void);
extern void _kerjmp_mbev_CapEffGlowCoinAdd(void);
extern void _kerjmp_mbev_CapEffRingCreate(void);
extern void _kerjmp_mbev_CapEffRingHitCreate(void);
extern void _kerjmp_mbev_CapEffRingOMExec(void);
extern void _kerjmp_mbev_CapEffRingKill(void);
extern void _kerjmp_mbev_CapEffRingDispGet(void);
extern void _kerjmp_mbev_CapEffRingAdd(void);
extern void _kerjmp_mbev_CapEffRingHitAdd(void);
extern void _kerjmp_mbev_CapEffRingAnimSet(void);
extern void _kerjmp_mbev_CapEffElectricCreate(void);
extern void _kerjmp_mbev_CapEffElectricOMExec(void);
extern void _kerjmp_mbev_CapEffElectricKill(void);
extern void _kerjmp_mbev_CapEffElectricDispGet(void);
extern void _kerjmp_mbev_CapEffElectricAdd(void);
extern void _kerjmp_mbev_CapEffElectricModelSet(void);
extern void _kerjmp_mbev_CapEffRayCreate(void);
extern void _kerjmp_mbev_CapEffRayOMExec(void);
extern void _kerjmp_mbev_CapEffRayKill(void);
extern void _kerjmp_mbev_CapEffRayDraw(void);
extern void _kerjmp_mbev_CapEffRayAdd(void);
extern void _kerjmp_mbev_CapEffRayAlphaSet(void);
extern void _kerjmp_mbev_CapEffRayTransformSet(void);
extern void _kerjmp_mbev_CapEffMasuHitCreate(void);
extern void _kerjmp_mbev_CapEffMasuHitOMExec(void);
extern void _kerjmp_mbev_CapEffMasuHitKill(void);
extern void _kerjmp_mbev_CapEffMasuHitAdd(void);
extern void _kerjmp_mbev_CapEffMasuHitTransformSet(void);
extern void _kerjmp_mbev_CapEffCoinCreate(void);
extern void _kerjmp_mbev_CapEffCoinOMExec(void);
extern void _kerjmp_mbev_CapEffCoinKill(void);
extern void _kerjmp_mbev_CapEffCoinNumGet(void);
extern void _kerjmp_mbev_CapEffCoinAdd(void);
extern void _kerjmp_mbev_CapEffCoinMaxYSet(void);
extern void _kerjmp_mbev_CapEffCoinMultiAdd(void);
extern void _kerjmp_mbev_CapEffCoinGlowSet(void);
extern void _kerjmp_mbev_CapCoinAdd(void);
extern void _kerjmp_mbev_CapEffCapLoseCreate(void);
extern void _kerjmp_mbev_CapEffCapLoseOMExec(void);
extern void _kerjmp_mbev_CapEffCapLoseKill(void);
extern void _kerjmp_mbev_CapEffCapLoseNumGet(void);
extern void _kerjmp_mbev_CapEffCapLoseObjAdd(void);
extern void _kerjmp_mbev_CapEffCapLoseAdd(void);
extern void _kerjmp_mbev_CapCoinManCreate(void);
extern void _kerjmp_mbev_CapCoinManOMExec(void);
extern void _kerjmp_mbev_CapCoinManKill(void);
extern void _kerjmp_mbev_CapCoinManNumGet(void);
extern void _kerjmp_mbev_CapCoinManObjAdd(void);
extern void _kerjmp_mbev_CapCoinManAdd2(void);
extern void _kerjmp_mbev_CapCoinManAdd(void);
extern void _kerjmp_mbev_CapStarManCreate(void);
extern void _kerjmp_mbev_CapStarManOMExec(void);
extern void _kerjmp_mbev_CapStarManKill(void);
extern void _kerjmp_mbev_CapStarManNumGet(void);
extern void _kerjmp_mbev_CapStarManObjAdd(void);
extern void _kerjmp_mbev_CapStarManAdd2(void);
extern void _kerjmp_mbev_CapStarManAdd(void);
extern void _kerjmp_mbev_CapPlayerMoveObjInit(void);
extern void _kerjmp_mbev_CapPlayerMoveHitCreate(void);
extern void _kerjmp_mbev_CapPlayerMoveEjectCreate(void);
extern void _kerjmp_mbev_CapPlayerMoveIdleCreate(void);
extern void _kerjmp_mbev_CapPlayerMoveMinYSet(void);
extern void _kerjmp_mbev_CapPlayerMoveVelSet(void);
extern void _kerjmp_mbev_CapPlayerMoveObjExec(void);
extern void _kerjmp_mbev_CapPlayerMoveObjCheck(void);
extern void _kerjmp_mbev_CapPlayerMoveObjClose(void);
extern void _kerjmp_mbev_CapPlayerMoveObjKill(void);
extern void _kerjmp_mbev_CapPlayerSquishSet(void);
extern void _kerjmp_mbev_CapPlayerSquishVoiceSet(void);
extern void _kerjmp_mbev_CapPlayerStunSet(void);
extern void _kerjmp_mbev_CapPlayerShockSet(void);
extern void _kerjmp_mbev_CapPlayerCheck(void);
extern void _kerjmp_mbev_CapCullPlayerCheck(void);
extern void _kerjmp_mbev_CapCullCheck(void);
extern void _kerjmp_mbev_CapPointCullCheck(void);
extern void _kerjmp_mbev_CapPlayerMasuNumGet(void);
extern void _kerjmp_mbev_CapPlayerNoSearch(void);
extern void _kerjmp_mbev_CapPlayerOrderGet(void);
extern void _kerjmp_mbev_CapMasuLinkNextGet(void);
extern void _kerjmp_mbev_CapMasuLinkNextRandomGet(void);
extern void _kerjmp_mbev_CapMasuValidPrevGet(void);
extern void _kerjmp_mbev_CapMasuPrevGet(void);
extern void _kerjmp_mbev_CapPlayerComSelGet(void);
extern void _kerjmp_mbev_CapPlayerComSelSameGet(void);
extern void _kerjmp_mbev_CapPlayerComSelRandomGet(void);
extern void _kerjmp_mbev_CapPlayerComSelKettouGet(void);
extern void _kerjmp_mbev_CapEffColorSet(void);
extern void _kerjmp_mbev_CapPlayerMotShiftCheck(void);
extern void _kerjmp_mbev_CapPlayerMotShiftSet(void);
extern void _kerjmp_mbev_CapPlayerMotShiftWait(void);
extern void _kerjmp_mbev_CapObjMotionSet(void);
extern void _kerjmp_mbev_CapObjMotionOMExec(void);
extern void _kerjmp_mbev_CapPlayerMotionSet(void);
extern void _kerjmp_mbev_CapPlayerMotionOMExec(void);
extern void _kerjmp_mbev_CapPlayerRotate(void);
extern void _kerjmp_mbev_CapPlayerIdleWait(void);
extern void _kerjmp_mbev_CapCoinDisp(void);
extern void _kerjmp_mbev_CapMasuMoveCheck(void);
extern void _kerjmp_mbev_CapVibrate(void);
extern void _kerjmp_mbev_CapStatusDispSet(void);
extern void _kerjmp_mbev_CapDuelStatusOffSet(void);
extern void _kerjmp_mbev_CapDuelStatusOnSet(void);
extern void _kerjmp_mbev_CapDuelStatusDispSet(void);
extern void _kerjmp_mbev_CapStatusDispSetAll(void);
extern void _kerjmp_mbev_CapStatusDispCheck(void);
extern void _kerjmp_mbev_CapChoiceSet(void);
extern void _kerjmp_mbev_CapAngleWrap(void);
extern void _kerjmp_mbev_CapAngleLerp(void);
extern void _kerjmp_mbev_CapAngleSumLerp(void);
extern void _kerjmp_mbev_CapVecChase(void);
extern void _kerjmp_mbev_CapVecRotGet(void);
extern void _kerjmp_mbev_CapColorLerp(void);
extern void _kerjmp_mbev_CapHermiteConstGet(void);
extern void _kerjmp_mbev_CapHermiteConstGet2(void);
extern void _kerjmp_mbev_CapHermiteGetV(void);
extern void _kerjmp_mbev_CapBezierGet(void);
extern void _kerjmp_mbev_CapBezierGetV(void);
extern void _kerjmp_mbev_CapBezierSlopeGet(void);
extern void _kerjmp_mbev_CapBezierNormGetV(void);
extern void _kerjmp_mbev_ShopEnableSet(void);
extern void _kerjmp_mbev_ShopInit(void);
extern void _kerjmp_mbev_ShopCreate(void);
extern void _kerjmp_mbev_ShopExInit(void);
extern void _kerjmp_mbev_Shop(void);
extern void _kerjmp_mbev_ShopExObjHookSet(void);
extern void _kerjmp_mbev_ShopBackCreate(void);
extern void _kerjmp_mbev_ShopBackMotCreate(void);
extern void _kerjmp_mbCapUse(void);
extern void _kerjmp_MBCapsuleStub1(void);
extern void _kerjmp_MBCapsuleStub2(void);
extern void _kerjmp_mbCapEffUseCreate(void);
extern void _kerjmp_mbCapEffUseModeGet(void);
extern void _kerjmp_mbCapEffUsePosGet(void);
extern void _kerjmp_mbCapEffUseWanWanCreate(void);
extern void _kerjmp_mbCapPlayerThrow(void);
extern void _kerjmp_mbCapPlayerThrowCheck(void);
extern void _kerjmp_mbCapThrowColCreate(void);
extern void _kerjmp_mbCapThrowColCheck(void);
extern void _kerjmp_mbCapEffThrowCreate(void);
extern void _kerjmp_mbCapEffThrowCheck(void);
extern void _kerjmp_mbCapAutoThrow(void);
extern void _kerjmp_mbCapThrowHookSet(void);
extern void _kerjmp_mbCapSelectMasu(void);
extern void _kerjmp_mbCapSelectMasuInit(void);
extern void _kerjmp_mbCapListInit(void);
extern void _kerjmp_mbCapListRead(void);
extern void _kerjmp_mbCapListCopy(void);
extern void _kerjmp_mbCapListDebug(void);
extern void _kerjmp_mbCapNumInc(void);
extern void _kerjmp_mbCapNumDebug(void);
extern void _kerjmp_mbCapValueTypeGet(void);
extern void _kerjmp_mbCapMasuTypeGet(void);
extern void _kerjmp_mbCapValuePlayerGet(void);
extern void _kerjmp_mbCapMasuPlayerGet2(void);
extern void _kerjmp_mbCapMasuPlayerSet(void);
extern void _kerjmp_mbCapMasuPlayerTypeSet(void);
extern void _kerjmp_mbCapMasuPlayerGet(void);
extern void _kerjmp_mbCapUseModeGet(void);
extern void _kerjmp_mbCapMasuDispTypeGet(void);
extern void _kerjmp_mbCapUseTrapCheck(void);
extern void _kerjmp_mbCapBuyCostGet(void);
extern void _kerjmp_mbCapSellCostGet(void);
extern void _kerjmp_mbCapCostGet(void);
extern void _kerjmp_mbCapThrowMasuCheck(void);
extern void _kerjmp_mbCapComChanceGet(void);
extern void _kerjmp_mbCapSelectComGet(void);
extern void _kerjmp_mbCapSelectDeleteComGet(void);
extern void _kerjmp_mbCapRandomThrowAdd(void);
extern void _kerjmp_mbCapRandomTrapAdd(void);
extern void _kerjmp_mbCapMasuCapsuleSet(void);
extern void _kerjmp_mbCapFileGet(void);
extern void _kerjmp_mbCapDescMesGet(void);
extern void _kerjmp_mbCapUseMesGet(void);
extern void _kerjmp_mbCapDebugNameGet(void);
extern void _kerjmp_mbCapMasuPatGet(void);
extern void _kerjmp_mbCapColorGet(void);
extern void _kerjmp_mbCapUseCostGet(void);
extern void _kerjmp_mbCapUseCheck(void);
extern void _kerjmp_mbCapValidCheck(void);
extern void _kerjmp_mbCapSelectMasuNum(void);
extern void _kerjmp_mbCapSelectMasuFrontNum(void);
extern void _kerjmp_mbCapSelectMasuBackNum(void);
extern void _kerjmp_mbCapValidListGet(void);
extern void _kerjmp_mbCapNextGet(void);
extern void _kerjmp_mbCapMasuNextGet(void);
extern void _kerjmp_mbCapShopListGet(void);
extern void _kerjmp_mbCapRandomListGet(void);
extern void _kerjmp_mbCapBonusCoinNumGet(void);
extern void _kerjmp_mbCapDescWinCreate(void);
extern void _kerjmp_mbCapInit(void);
extern void _kerjmp_mbCapObjCreate(void);
extern void _kerjmp_mbCapObjBorderCreate(void);
extern void _kerjmp_mbCapObjKill(void);
extern void _kerjmp_mbCapObjBorderKill(void);
extern void _kerjmp_mbCapObjColorCreate(void);
extern void _kerjmp_mbCapObjColorKill(void);
extern void _kerjmp_mbCapObjColorPosSet(void);
extern void _kerjmp_mbCapObjColorRotSet(void);
extern void _kerjmp_mbCapObjColorScaleSet(void);
extern void _kerjmp_mbCapObjColorPosSetV(void);
extern void _kerjmp_mbCapObjColorRotSetV(void);
extern void _kerjmp_mbCapObjColorScaleSetV(void);
extern void _kerjmp_mbCapObjColorPosGet(void);
extern void _kerjmp_mbCapObjColorRotGet(void);
extern void _kerjmp_mbCapObjColorScaleGet(void);
extern void _kerjmp_mbCapObjColorLayerSet(void);
extern void _kerjmp_mbCapObjColorLayerGet(void);
extern void _kerjmp_mbCapObjColorAttrSet(void);
extern void _kerjmp_mbCapObjColorAttrReset(void);
extern void _kerjmp_mbCapObjColorDispSet(void);
extern void _kerjmp_mbCapObjColorAlphaSet(void);
extern void _kerjmp_mbCapObjColorMtxSet(void);
extern void _kerjmp_mbCapObjColorMtxGet(void);
extern void _kerjmp_mbCoinInit(void);
extern void _kerjmp_mbCoinClose(void);
extern void _kerjmp_mbCoinEffObjCreate(void);
extern void _kerjmp_mbCoinEffCreate(void);
extern void _kerjmp_mbCoinDispKill(void);
extern void _kerjmp_mbCoinDispKillCheck(void);
extern void _kerjmp_mbCoinDispCapsuleCreate(void);
extern void _kerjmp_mbCoinDispMasuCreate(void);
extern void _kerjmp_mbCoinDispCreate(void);
extern void _kerjmp_mbCoinAddProcExec(void);
extern void _kerjmp_mbCoinAddDispExec(void);
extern void _kerjmp_mbCoinAddExec(void);
extern void _kerjmp_mbCoinAddAllProcExecV(void);
extern void _kerjmp_mbCoinAddAllProcExec(void);
extern void _kerjmp_mbCoinAddAllExec(void);
extern void _kerjmp_mbStatTeamMinValGet(void);
extern void _kerjmp_mbCoinCreate(void);
extern void _kerjmp_mbCoinCreate2(void);
extern void _kerjmp_mbCoinObjCreate(void);
extern void _kerjmp_mbCoinObjCreate2(void);
extern void _kerjmp_mbCoinObjNumDec(void);
extern void _kerjmp_mbCoinObjKill(void);
extern void _kerjmp_mbCoinObjGet(void);
extern void _kerjmp_mbCoinObjPosSet(void);
extern void _kerjmp_mbCoinObjPosSetV(void);
extern void _kerjmp_mbCoinObjRotSet(void);
extern void _kerjmp_mbCoinObjRotSetV(void);
extern void _kerjmp_mbCoinObjScaleSet(void);
extern void _kerjmp_mbCoinObjScaleSetV(void);
extern void _kerjmp_mbCoinObjAlphaSet(void);
extern void _kerjmp_mbCoinObjDispSet(void);
extern void _kerjmp_mbCoinObjLayerSet(void);
extern void _kerjmp_mbCoinObjMotSet(void);
extern void _kerjmp_mbCoinObjPosGet(void);
extern void _kerjmp_mbCoinObjRotGet(void);
extern void _kerjmp_mbCoinObjScaleGet(void);
extern void _kerjmp_mbCoinObjAlphaGet(void);
extern void _kerjmp_mbCoinObjDispGet(void);
extern void _kerjmp_mbComChoiceLeftSet(void);
extern void _kerjmp_mbComChoiceUpSet(void);
extern void _kerjmp_mbComChoiceRightSet(void);
extern void _kerjmp_mbComChoiceDownSet(void);
extern void _kerjmp_mbComChoiceListDownSet(void);
extern void _kerjmp_mbComChoiceListRightSet(void);
extern void _kerjmp_mbComChoiceListSet(void);
extern void _kerjmp_mbConfigExec(void);
extern void _kerjmp_mbPauseDispCopyKill(void);
extern void _kerjmp_mbPauseDispCopyCreate(void);
extern void _kerjmp_mbConfigPadDisableSet(void);
extern void _kerjmp_mbPauseGuideCreate(void);
extern void _kerjmp_mbPauseGuideKill(void);
extern void _kerjmp_mbPausePanelCreate(void);
extern void _kerjmp_mbPausePanelKill(void);
extern void _kerjmp_mbPausePanelPosSet(void);
extern void _kerjmp_mbPausePanelRotSet(void);
extern void _kerjmp_mbPausePanelScaleSet(void);
extern void _kerjmp_mbPausePanelBankSet(void);
extern void _kerjmp_mbPausePanelBatsuSet(void);
extern void _kerjmp_mbPausePanelPosGet(void);
extern void _kerjmp_mbPausePanelScaleGet(void);
extern void _kerjmp_mbPausePanelFreezeGet(void);
extern void _kerjmp_mbPausePanelUnlockSet(void);
extern void _kerjmp_mbPausePanelSlideSet(void);
extern void _kerjmp_mbPausePanelSizeSet(void);
extern void _kerjmp_mbPausePanelAnmNoSet(void);
extern void _kerjmp_mbPausePanelGrowSet(void);
extern void _kerjmp_mbPausePanelShrinkSet(void);
extern void _kerjmp_mbPauseGuideMoveSet(void);
extern void _kerjmp_mbPadDStkRepGetAll(void);
extern void _kerjmp_mbPadBtnDownGetAll(void);
extern void _kerjmp_mbev_Last5(void);
extern void _kerjmp_mbDiceInit(void);
extern void _kerjmp_mbDicePlayerExec(void);
extern void _kerjmp_mbDiceExec(void);
extern void _kerjmp_mbDiceProcExec(void);
extern void _kerjmp_mbDiceChanceTradeExec(void);
extern void _kerjmp_mbDiceChanceCharExec(void);
extern void _kerjmp_mbDiceFadeSet(void);
extern void _kerjmp_mbDiceKill(void);
extern void _kerjmp_mbDiceClose(void);
extern void _kerjmp_mbDiceKillCheck(void);
extern void _kerjmp_mbDiceKillCheckAll(void);
extern void _kerjmp_mbDiceResultGet(void);
extern void _kerjmp_mbDiceHitHookSet(void);
extern void _kerjmp_mbDiceTypeGet(void);
extern void _kerjmp_mbDiceTutorialNumSet(void);
extern void _kerjmp_mbDiceTutorialNumGet(void);
extern void _kerjmp_mbDiceValueNoGet(void);
extern void _kerjmp_mbDiceNumKill(void);
extern void _kerjmp_mbDiceNumShrinkSet(void);
extern void _kerjmp_mbDiceNumStopCheck(void);
extern void _kerjmp_mbDiceStub(void);
extern void _kerjmp_mbDiceSNpcNumCreate(void);
extern void _kerjmp_mbDiceNumObjCreate(void);
extern void _kerjmp_mbDiceMaxGet(void);
extern void _kerjmp_mbDiceValueMaxGet(void);
extern void _kerjmp_mbDicePadBtnHookSet(void);
extern void _kerjmp_mbDiceMotHookSet(void);
extern void _kerjmp_mbDiceObjHit(void);
extern void _kerjmp_mbDiceSNpcNumObjCreate(void);
extern void _kerjmp_mbDiceSNpcNumKill(void);
extern void _kerjmp_mbDiceSNpcNumDispSet(void);
extern void _kerjmp_mbDiceSNpcNumSet(void);
extern void _kerjmp_mbDiceSNpcNumPosSet(void);
extern void _kerjmp_mbDiceSNpcNumOfsSet(void);
extern void _kerjmp_mbDiceSNpcNumOfsGet(void);
extern void _kerjmp_mbEffInit(void);
extern void _kerjmp_mbEffClose(void);
extern void _kerjmp_mbEffConfettiCreate(void);
extern void _kerjmp_mbEffConfettiKill(void);
extern void _kerjmp_mbEffConfettiReset(void);
extern void _kerjmp_mbEffFadeOutSet(void);
extern void _kerjmp_mbEffFadeCreate(void);
extern void _kerjmp_mbEffFadeDoneCheck(void);
extern void _kerjmp_mbEffFadeCheck(void);
extern void _kerjmp_mbEffFadeCameraSet(void);
extern void _kerjmp_mbParticleCreate(void);
extern void _kerjmp_mbParticleKill(void);
extern void _kerjmp_mbParticleHookSet(void);
extern void _kerjmp_mbParticleAttrSet(void);
extern void _kerjmp_mbParticleAttrReset(void);
extern void _kerjmp_mbParticleUnkTotalGet(void);
extern void _kerjmp_mbParticleDataCreate(void);
extern void _kerjmp_mbParticleColorCreate(void);
extern void _kerjmp_mbParManCreate(void);
extern void _kerjmp_mbParManKill(void);
extern void _kerjmp_mbParManPosSet(void);
extern void _kerjmp_mbParManVecSet(void);
extern void _kerjmp_mbParManRotSet(void);
extern void _kerjmp_mbParManAttrSet(void);
extern void _kerjmp_mbParManAttrReset(void);
extern void _kerjmp_mbParticleBlendModeSet(void);
extern void _kerjmp_mbGateInit(void);
extern void _kerjmp_mbGateClose(void);
extern void _kerjmp_mbGateCreate(void);
extern void _kerjmp_mbev_GateMasu(void);
extern void _kerjmp_mbev_GateBattanMasuFind(void);
extern void _kerjmp_mbev_GateBattan(void);
extern void _kerjmp_mbGateBattanKill(void);
extern void _kerjmp_mbGuideInit(void);
extern void _kerjmp_mbGuideCreateIn(void);
extern void _kerjmp_mbGuideCreateFlag(void);
extern void _kerjmp_mbGuideCreate(void);
extern void _kerjmp_mbGuideKill(void);
extern void _kerjmp_mbGuideEnd(void);
extern void _kerjmp_mbGuideFadeIn(void);
extern void _kerjmp_mbGuideFadeOut(void);
extern void _kerjmp_mbGuideModelGet(void);
extern void _kerjmp_mbGuideIdleCheck(void);
extern void _kerjmp_mbGuideScreenSet(void);
extern void _kerjmp_mbGuideMotionNextSet(void);
extern void _kerjmp_mbGuideMotionSet(void);
extern void _kerjmp_mbGuideMotionShiftSet(void);
extern void _kerjmp_mbGuideMotionStop(void);
extern void _kerjmp_mbGuideMotionCheck(void);
extern void _kerjmp_mbGuideNoGet(void);
extern void _kerjmp_mbGuideSpeakerNoGet(void);
extern void _kerjmp_mbExitWatchCreate(void);
extern void _kerjmp_mbExitReq(void);
extern void _kerjmp_mbExitCheck(void);
extern void _kerjmp_mbExitWatchProcGet(void);
extern void _kerjmp_mbChangeTimeSet(void);
extern void _kerjmp_mbChangeTime(void);
extern void _kerjmp_mbev_NextTimeSet(void);
extern void _kerjmp_mbev_LoadTimeSet(void);
extern void _kerjmp_mbObjectSetup(void);
extern void _kerjmp_mbPauseEnableSet(void);
extern void _kerjmp_mbPauseEnableReset(void);
extern void _kerjmp_mbPauseEnableCheck(void);
extern void _kerjmp_mbLightSet(void);
extern void _kerjmp_mbLightReset(void);
extern void _kerjmp_mbLightFuncSet(void);
extern void _kerjmp_mbBGRead(void);
extern void _kerjmp_mbBGReadWait(void);
extern void _kerjmp_mbReturnMgCheck(void);
extern void _kerjmp_mbOvlCall(void);
extern void _kerjmp_mbDirClose(void);
extern void _kerjmp_mbev_TurnStartSet(void);
extern void _kerjmp_mbev_TurnEndSet(void);
extern void _kerjmp_fn_8014C3B4(void);
extern void _kerjmp_fn_8014C3BC(void);
extern void _kerjmp_fn_8014C3C4(void);
extern void _kerjmp_fn_8014C3CC(void);
extern void _kerjmp_mbScrollInit(void);
extern void _kerjmp_mbScrollClose(void);
extern void _kerjmp_mbev_Scroll(void);
extern void _kerjmp_mbev_ScrollCapsule(void);
extern void _kerjmp_mbScrollStarFindFuncSet(void);
extern void _kerjmp_mbScrollHookSet(void);
extern void _kerjmp_mbMapCameraSet(void);
extern void _kerjmp_mbMapHookSet(void);
extern void _kerjmp_mbMapSprAdd(void);
extern void _kerjmp_mbev_StarScroll(void);
extern void _kerjmp_mbMasuInit(void);
extern void _kerjmp_mbMasuClose(void);
extern void _kerjmp_mbev_MasuStop(void);
extern void _kerjmp_mbMasuDataRead(void);
extern void _kerjmp_mbev_MasuMasuEnd(void);
extern void _kerjmp_mbev_MasuMasuStart(void);
extern void _kerjmp_mbev_MasuCapStop(void);
extern void _kerjmp_mbev_MasuMove(void);
extern void _kerjmp_mbMasuNextSet(void);
extern void _kerjmp_mbMasuNextDispSet(void);
extern void _kerjmp_mbMasuNumGet(void);
extern void _kerjmp_mbMasuRawNumGet(void);
extern void _kerjmp_mbMasuGet(void);
extern void _kerjmp_mbMasuLayerSet(void);
extern void _kerjmp_mbMasuLayerGet(void);
extern void _kerjmp_mbMasuAttrGet(void);
extern void _kerjmp_mbMasuAttrSet(void);
extern void _kerjmp_mbMasuMAttrGet(void);
extern void _kerjmp_mbMasuMAttrSet(void);
extern void _kerjmp_mbev_MasuBitGet(void);
extern void _kerjmp_mbev_MasuAttrGet(void);
extern void _kerjmp_mbMasuTypeGet(void);
extern void _kerjmp_mbMasuTypeSet(void);
extern void _kerjmp_mbMasuTypeChange(void);
extern void _kerjmp_mbMasuDispCheck(void);
extern void _kerjmp_mbMasuCapsuleReset(void);
extern void _kerjmp_mbMasuCapsuleSet(void);
extern void _kerjmp_mbMasuCapsuleGet(void);
extern void _kerjmp_mbMasuPosGet(void);
extern void _kerjmp_mbMasuPosSet(void);
extern void _kerjmp_mbMasuPosSetV(void);
extern void _kerjmp_mbMasuCornerRotPosGet(void);
extern void _kerjmp_mbMasuCornerPosGet(void);
extern void _kerjmp_mbMasuRotGet(void);
extern void _kerjmp_mbMasuRotSet(void);
extern void _kerjmp_mbMasuRotSetV(void);
extern void _kerjmp_mbMasuMtxGet(void);
extern void _kerjmp_mbMasuMtxSet(void);
extern void _kerjmp_mbMasuDispGet(void);
extern void _kerjmp_mbMasuDispSet(void);
extern void _kerjmp_mbMasuDispAttrReset(void);
extern void _kerjmp_mbMasuDispAttrSet(void);
extern void _kerjmp_mbMasuDispAttrGet(void);
extern void _kerjmp_mbMasuDispMAttrReset(void);
extern void _kerjmp_mbMasuDispMAttrSet(void);
extern void _kerjmp_mbMasuDispMAttrGet(void);
extern void _kerjmp_mbMasuModelDispSet(void);
extern void _kerjmp_mbMasuLinkGet(void);
extern void _kerjmp_mbMasuLinkNumGet(void);
extern void _kerjmp_mbMasuLinkTblGet(void);
extern void _kerjmp_mbMasuLinkTblGet2(void);
extern void _kerjmp_mbMasuAttrFindLink(void);
extern void _kerjmp_mbMasuAttrMatchFindLink(void);
extern void _kerjmp_mbMasuMAttrFindLink(void);
extern void _kerjmp_mbMasuMAttrMatchFindLink(void);
extern void _kerjmp_mbMasuTypeFindLink(void);
extern void _kerjmp_mbMasuLinkParentGet(void);
extern void _kerjmp_mbMasuFind_TypeStepGet(void);
extern void _kerjmp_mbMasuFind_TypeStepGet2(void);
extern void _kerjmp_mbMasuFind_AttrStepGet(void);
extern void _kerjmp_mbMasuFind_AttrMatchStepGet(void);
extern void _kerjmp_mbMasuFind_AttrStepGet2(void);
extern void _kerjmp_mbMasuFind_MAttrStepGet(void);
extern void _kerjmp_mbMasuFind_MAttrMatchStepGet(void);
extern void _kerjmp_mbMasuFind_MAttrStepGet2(void);
extern void _kerjmp_mbMasuFind_IdStepGet(void);
extern void _kerjmp_mbMasuFind_IdStepGet2(void);
extern void _kerjmp_mbMasuFind_TypeSearch(void);
extern void _kerjmp_mbMasuFind_TypeIdGet(void);
extern void _kerjmp_mbMasuFind_AttrIdGet(void);
extern void _kerjmp_mbMasuFind_AttrMatchIdGet(void);
extern void _kerjmp_mbMasuFind_AttrMatchIdGet2(void);
extern void _kerjmp_mbMasuFind_MAttrIdGet(void);
extern void _kerjmp_mbMasuFind_MAttrMatchIdGet(void);
extern void _kerjmp_mbMasuFind_MAttrMatchIdGet2(void);
extern void _kerjmp_mbMasuTypeListGet(void);
extern void _kerjmp_mbMasuAttrListGet(void);
extern void _kerjmp_mbMasuAttrMatchListGet(void);
extern void _kerjmp_mbMasuMAttrListGet(void);
extern void _kerjmp_mbMasuMAttrMatchTblGet(void);
extern void _kerjmp_mbMasuFind_TypeListGet(void);
extern void _kerjmp_mbMasuFind_TypeListGet2(void);
extern void _kerjmp_mbMasuFind_AttrNumGet(void);
extern void _kerjmp_mbMasuFind_AttrMatchNumGet(void);
extern void _kerjmp_mbMasuFind_AttrMatchNumGet2(void);
extern void _kerjmp_mbMasuFind_MAttrNumGet(void);
extern void _kerjmp_mbMasuFind_MAttrMatchListGet(void);
extern void _kerjmp_mbMasuFind_MAttrMatchListGet2(void);
extern void _kerjmp_mbMasuFind_IdListGet(void);
extern void _kerjmp_mbMasuFind_IdListGet2(void);
extern void _kerjmp_mbev_MasuMoveEndSet(void);
extern void _kerjmp_mbev_MasuHatenaSet(void);
extern void _kerjmp_mbev_MasuMoveStartSet(void);
extern void _kerjmp_mbev_MasuLinkTblHookSet(void);
extern void _kerjmp_mbMasuPlayerDispSet(void);
extern void _kerjmp_mbMasuPlayerFadeSet(void);
extern void _kerjmp_mbMasuPlayerCapMoveCheck(void);
extern void _kerjmp_mbMasuPlayerCapStopCheck(void);
extern void _kerjmp_mbMasuStub(void);
extern void _kerjmp_mbMasuPlayerColorSet(void);
extern void _kerjmp_mbMasuPlayerPrizeReset(void);
extern void _kerjmp_mbMasuPKinokoValueGet(void);
extern void _kerjmp_mbMathInit(void);
extern void _kerjmp_mbMathClose(void);
extern void _kerjmp_mbRandMod(void);
extern void _kerjmp_mbVecMagCheck(void);
extern void _kerjmp_mbAngleAdd(void);
extern void _kerjmp_mbAngleMoveTo(void);
extern void _kerjmp_mbAngleWrap2(void);
extern void _kerjmp_mbVecMagXZ(void);
extern void _kerjmp_mbMtxLookAtCalc(void);
extern void _kerjmp_mbAngleWrapV(void);
extern void _kerjmp_mbAngleWrap(void);
extern void _kerjmp_mbVecMagXZCheck(void);
extern void _kerjmp_mbBezierCalc(void);
extern void _kerjmp_mbBezierCalcV(void);
extern void _kerjmp_mbBezierCalcVList(void);
extern void _kerjmp_mbBezierCalcSlope(void);
extern void _kerjmp_mbBezierCalcSlopeV(void);
extern void _kerjmp_mbHermiteCalc(void);
extern void _kerjmp_mbHermiteCalcV(void);
extern void _kerjmp_mbHermiteCalcSlope(void);
extern void _kerjmp_mbPos3Dto2D(void);
extern void _kerjmp_mbPos2Dto3D(void);
extern void _kerjmp_mbPos3DtoNorm(void);
extern void _kerjmp_mbNormPosto3D(void);
extern void _kerjmp_mbNormPosto2D(void);
extern void _kerjmp_mbAngleLerp(void);
extern void _kerjmp_mbAngleEaseOut(void);
extern void _kerjmp_mbAngleEaseIn(void);
extern void _kerjmp_mbMathDistScale(void);
extern void _kerjmp_mbCosDeg(void);
extern void _kerjmp_mbSinDeg(void);
extern void _kerjmp_mbCosRad(void);
extern void _kerjmp_mbSinRad(void);
extern void _kerjmp_mbMtxRot(void);
extern void _kerjmp_mbMtxRotAxisDeg(void);
extern void _kerjmp_mbMtxRotAxisRad(void);
extern void _kerjmp_mbMtxRotXDeg(void);
extern void _kerjmp_mbMtxRotXRad(void);
extern void _kerjmp_mbMtxRotYDeg(void);
extern void _kerjmp_mbMtxRotYRad(void);
extern void _kerjmp_mbMtxRotZDeg(void);
extern void _kerjmp_mbMtxRotZRad(void);
extern void _kerjmp_mbMtxScaleRotXDeg(void);
extern void _kerjmp_mbMtxScaleRotYDeg(void);
extern void _kerjmp_mbMtxScaleRotZDeg(void);
extern void _kerjmp_mbMtxRotTrigX(void);
extern void _kerjmp_mbMtxRotTrigY(void);
extern void _kerjmp_mbMtxRotTrigZ(void);
extern void _kerjmp_mbMtxRotTrigScaleX(void);
extern void _kerjmp_mbMtxRotTrigScaleY(void);
extern void _kerjmp_mbMtxRotTrigScaleZ(void);
extern void _kerjmp_mbMtxTransCat(void);
extern void _kerjmp_mbObjCullInit(void);
extern void _kerjmp_mbTelopPlayerCreate(void);
extern void _kerjmp_mbTelopPlayerSkipCreate(void);
extern void _kerjmp_mbTelopCreate(void);
extern void _kerjmp_mbTelopCheck(void);
extern void _kerjmp_mbTelopLastTurnCreate(void);
extern void _kerjmp_mbPadStkXGet(void);
extern void _kerjmp_mbPadStkYGet(void);
extern void _kerjmp_mbTauntInit(void);
extern void _kerjmp_mbTauntClose(void);
extern void _kerjmp_mbLanguageGet(void);
extern void _kerjmp_mbLanguageSet(void);
extern void _kerjmp_mbBoardDataNumGet(void);
extern void _kerjmp_mbBoardDataDirRead(void);
extern void _kerjmp_mbTelopTimeChangeCreate(void);
extern void _kerjmp_mbTelopTimeChangeCheck(void);
extern void _kerjmp_mbTelopTimeChangeKill(void);
extern void _kerjmp_mbTelopTimeCreate(void);
extern void _kerjmp_mbTelopTimeSprCreate(void);
extern void _kerjmp_mbTelopTimeSprKill(void);
extern void _kerjmp_mbTelopTimeDispSet(void);
extern void _kerjmp_mbTelopTimeSprRotSet(void);
extern void _kerjmp_mbTelopTimeStarSet(void);
extern void _kerjmp_mbTelopTimeTPLvlSet(void);
extern void _kerjmp_mbObjInit(void);
extern void _kerjmp_mbObjClose(void);
extern void _kerjmp_mbObjKill(void);
extern void _kerjmp_mbObjDirSet(void);
extern void _kerjmp_mbObjDataNumGet(void);
extern void _kerjmp_mbObjCreate(void);
extern void _kerjmp_mbObjCharCreate(void);
extern void _kerjmp_mbObjGet(void);
extern void _kerjmp_mbObjDispGet(void);
extern void _kerjmp_mbObjDispSet(void);
extern void _kerjmp_mbObjCameraSet(void);
extern void _kerjmp_mbObjLayerSet(void);
extern void _kerjmp_mbObjLayerGet(void);
extern void _kerjmp_mbObjLayerSetAll(void);
extern void _kerjmp_mbObjModelIDGet(void);
extern void _kerjmp_mbObjPosGet(void);
extern void _kerjmp_mbObjPosSetV(void);
extern void _kerjmp_mbObjPosSet(void);
extern void _kerjmp_mbObjOffsetGet(void);
extern void _kerjmp_mbObjOffsetSetV(void);
extern void _kerjmp_mbObjOffsetSet(void);
extern void _kerjmp_mbObjRotGet(void);
extern void _kerjmp_mbObjRotSetV(void);
extern void _kerjmp_mbObjRotSet(void);
extern void _kerjmp_mbObjRotYGet(void);
extern void _kerjmp_mbObjRotYSet(void);
extern void _kerjmp_mbObjScaleGet(void);
extern void _kerjmp_mbObjScaleSetV(void);
extern void _kerjmp_mbObjScaleSet(void);
extern void _kerjmp_mbObjMtxGet(void);
extern void _kerjmp_mbObjMtxSet(void);
extern void _kerjmp_mbObjAttrSet(void);
extern void _kerjmp_mbObjAttrReset(void);
extern void _kerjmp_mbObjAmbSet(void);
extern void _kerjmp_mbObjAmbSetAll(void);
extern void _kerjmp_mbObjAlphaGet(void);
extern void _kerjmp_mbObjAlphaSet(void);
extern void _kerjmp_mbObjFadeSet(void);
extern void _kerjmp_mbObjZWriteOffSet(void);
extern void _kerjmp_mbObjSePlay(void);
extern void _kerjmp_mbObjMotionVoiceOnSet(void);
extern void _kerjmp_mbObjCullRadiusSet(void);
extern void _kerjmp_mbObjHookSet(void);
extern void _kerjmp_mbObjHookReset(void);
extern void _kerjmp_mbObjHookObjReset(void);
extern void _kerjmp_mbObjMotionGet(void);
extern void _kerjmp_mbObjMotionSet(void);
extern void _kerjmp_mbObjMotionIDCurGet(void);
extern void _kerjmp_mbObjMotionIDGet(void);
extern void _kerjmp_mbObjMotionShiftSet(void);
extern void _kerjmp_mbObjMotionShiftIDGet(void);
extern void _kerjmp_mbObjMotionNoCreate(void);
extern void _kerjmp_mbObjMotionCreate(void);
extern void _kerjmp_mbObjMotionKill(void);
extern void _kerjmp_mbObjMotionTimeGet(void);
extern void _kerjmp_mbObjMotionTimeSet(void);
extern void _kerjmp_mbObjMotionMaxTimeGet(void);
extern void _kerjmp_mbObjMotionSpeedGet(void);
extern void _kerjmp_mbObjMotionSpeedSet(void);
extern void _kerjmp_mbObjMotionEndCheck(void);
extern void _kerjmp_mbObjMotionStartEndSet(void);
extern void _kerjmp_mbObjMotionLoopSet(void);
extern void _kerjmp_mbObjMotionShapeSet(void);
extern void _kerjmp_mbObjMotionShapeTimeGet(void);
extern void _kerjmp_mbObjMotionShapeTimeSet(void);
extern void _kerjmp_mbObjMotionShapeMaxTimeGet(void);
extern void _kerjmp_mbObjMotionShapeSpeedGet(void);
extern void _kerjmp_mbObjMotionShapeSpeedSet(void);
extern void _kerjmp_mbObjMotionShapeEndCheck(void);
extern void _kerjmp_mbObjMotionShapeStartEndSet(void);
extern void _kerjmp_mbObjShadowSet(void);
extern void _kerjmp_mbObjShadowReset(void);
extern void _kerjmp_mbObjShadowMapSet(void);
extern void _kerjmp_mbObjShadowMapReset(void);
extern void _kerjmp_mbObjStub0(void);
extern void _kerjmp_mbev_OpeningParty(void);
extern void _kerjmp_mbev_OpeningSingle(void);
extern void _kerjmp_mbev_Opening(void);
extern void _kerjmp_mbOpeningViewSet(void);
extern void _kerjmp_mbOpeningZoomGet(void);
extern void _kerjmp_mbOpeningRotGet(void);
extern void _kerjmp_mbOpeningPosGet(void);
extern void _kerjmp_mbOpeningInstHookSet(void);
extern void _kerjmp_mbOpeningStarInstHookSet(void);
extern void _kerjmp_mbOpeningGuideObjIdGet(void);
extern void _kerjmp_mbOpeningGuidePosRestore(void);
extern void _kerjmp_mbOpeningCameraPosRestore(void);
extern void _kerjmp_mbPauseInit(void);
extern void _kerjmp_mbPauseCreate(void);
extern void _kerjmp_mbPauseStartCheck(void);
extern void _kerjmp_mbPauseProcCheck(void);
extern void _kerjmp_mbPauseDisableSet(void);
extern void _kerjmp_mbPauseDisableGet(void);
extern void _kerjmp_mbPauseHookPush(void);
extern void _kerjmp_mbPauseHookPop(void);
extern void _kerjmp_mbPauseSet(void);
extern void _kerjmp_mbPauseDataDirGet(void);
extern void _kerjmp_mbPauseDataNumGet(void);
extern void _kerjmp_mbPlayerMetalSet(void);
extern void _kerjmp_mbPlayerEffectSet(void);
extern void _kerjmp_mbPlayerMetalColorSet(void);
extern void _kerjmp_mbPlayerBiriQSet(void);
extern void _kerjmp_mbPlayerInit(void);
extern void _kerjmp_mbPlayerClose(void);
extern void _kerjmp_mbTurnExec(void);
extern void _kerjmp_mbSingleTurnExec(void);
extern void _kerjmp_mbPlayerSwap(void);
extern void _kerjmp_mbPlayerNameMesGet(void);
extern void _kerjmp_mbPlayerNameGet(void);
extern void _kerjmp_mbPlayerTagNameMesGet(void);
extern void _kerjmp_mbPlayerColorGet(void);
extern void _kerjmp_mbPlayerAmbSet(void);
extern void _kerjmp_mbPlayerWorkGet(void);
extern void _kerjmp_mbPlayerAllComCheck(void);
extern void _kerjmp_mbPlayerObjIDGet(void);
extern void _kerjmp_mbPlayerModelIDGet(void);
extern void _kerjmp_mbPlayerDispSet(void);
extern void _kerjmp_mbPlayerDispGet(void);
extern void _kerjmp_mbPlayerLayerSet(void);
extern void _kerjmp_mbPlayerCameraSet(void);
extern void _kerjmp_mbPlayerCullRadiusSet(void);
extern void _kerjmp_mbPlayerStubValSet(void);
extern void _kerjmp_mbPlayerMtxSet(void);
extern void _kerjmp_mbPlayerMtxGet(void);
extern void _kerjmp_mbPlayerPosSet(void);
extern void _kerjmp_mbPlayerPosSetV(void);
extern void _kerjmp_mbPlayerPosGet(void);
extern void _kerjmp_mbPlayerRotSet(void);
extern void _kerjmp_mbPlayerRotSetV(void);
extern void _kerjmp_mbPlayerRotGet(void);
extern void _kerjmp_mbPlayerRotYSet(void);
extern void _kerjmp_mbPlayerRotYGet(void);
extern void _kerjmp_mbPlayerScaleSet(void);
extern void _kerjmp_mbPlayerScaleSetV(void);
extern void _kerjmp_mbPlayerScaleGet(void);
extern void _kerjmp_mbPlayerMotionGet(void);
extern void _kerjmp_mbPlayerMotionEndCheck(void);
extern void _kerjmp_mbPlayerMotionEndCheckAll(void);
extern void _kerjmp_mbPlayerMotionEndWait(void);
extern void _kerjmp_mbPlayerMotionMaxTimeGet(void);
extern void _kerjmp_mbPlayerMotionTimeGet(void);
extern void _kerjmp_mbPlayerMotionSet(void);
extern void _kerjmp_mbPlayerMotionVoiceOnSet(void);
extern void _kerjmp_mbPlayerMotionShiftSet(void);
extern void _kerjmp_mbPlayerMotionCreate(void);
extern void _kerjmp_mbPlayerMotionKill(void);
extern void _kerjmp_mbPlayerMotionSpeedSet(void);
extern void _kerjmp_mbPlayerMotionTimeSet(void);
extern void _kerjmp_mbPlayerMotionStartEndSet(void);
extern void _kerjmp_mbPlayerAttrSet(void);
extern void _kerjmp_mbPlayerAttrReset(void);
extern void _kerjmp_mbPlayerMotIdleSet(void);
extern void _kerjmp_mbPlayerRotateStart(void);
extern void _kerjmp_mbPlayerRotateCheck(void);
extern void _kerjmp_mbPlayerRotateCheckAll(void);
extern void _kerjmp_mbPlayerEyeMatDarkSet(void);
extern void _kerjmp_mbPlayerRankGet(void);
extern void _kerjmp_mbPlayerCapsuleUseGet(void);
extern void _kerjmp_mbPlayerCapsuleUseSet(void);
extern void _kerjmp_mbPlayerCapsuleRemove(void);
extern void _kerjmp_mbPlayerCapsuleFind(void);
extern void _kerjmp_mbPlayerCapsuleAdd(void);
extern void _kerjmp_mbPlayerCapsuleNumGet(void);
extern void _kerjmp_mbPlayerCapsuleGet(void);
extern void _kerjmp_mbPlayerCapsuleMaxGet(void);
extern void _kerjmp_mbPlayerCoinAdd(void);
extern void _kerjmp_mbPlayerCoinSet(void);
extern void _kerjmp_mbPlayerCoinGet(void);
extern void _kerjmp_mbPlayerMaxCoinGet(void);
extern void _kerjmp_mbPlayerStarAdd(void);
extern void _kerjmp_mbPlayerStarSet(void);
extern void _kerjmp_mbPlayerStarGet(void);
extern void _kerjmp_mbPlayerTeamFind(void);
extern void _kerjmp_mbPlayerTeamFindOther(void);
extern void _kerjmp_mbPlayerTeamCoinSet(void);
extern void _kerjmp_mbPlayerTeamCoinGet(void);
extern void _kerjmp_mbPlayerGrpStarSet(void);
extern void _kerjmp_mbPlayerGrpStarGet(void);
extern void _kerjmp_mbPlayerTeamRankGet(void);
extern void _kerjmp_mbPlayerTeamFindPlayer(void);
extern void _kerjmp_mbPlayerTeamCapsuleGet(void);
extern void _kerjmp_mbPlayerTeamCapsuleNumGet(void);
extern void _kerjmp_mbPlayerTeamFindOpp(void);
extern void _kerjmp_mbPlayerTeamGet(void);
extern void _kerjmp_mbPlayerTeamCheckSame(void);
extern void _kerjmp_mbPlayerWalkSpeedGet(void);
extern void _kerjmp_mbPlayerMoveExec(void);
extern void _kerjmp_mbPlayerMoveMain(void);
extern void _kerjmp_mbev_PlayerColReserve(void);
extern void _kerjmp_mbev_PlayerColSet(void);
extern void _kerjmp_mbMoveNumCreateColor(void);
extern void _kerjmp_mbMoveNumCreate(void);
extern void _kerjmp_mbMoveNumKill(void);
extern void _kerjmp_mbMoveNumDispSet(void);
extern void _kerjmp_mbPlayerMasuMoveTo(void);
extern void _kerjmp_mbPlayerMasuMove(void);
extern void _kerjmp_mbPlayerMasuMovePos(void);
extern void _kerjmp_mbPlayerMasuMoveSpeed(void);
extern void _kerjmp_mbPlayerColSnapSet(void);
extern void _kerjmp_mbev_PlayerColMasuSet(void);
extern void _kerjmp_mbev_PlayerColMasuAllSet(void);
extern void _kerjmp_mbev_PlayerColMasuAdd(void);
extern void _kerjmp_mbev_PlayerColMasu(void);
extern void _kerjmp_mbev_PlayerColCircleAdd(void);
extern void _kerjmp_mbev_PlayerColBall(void);
extern void _kerjmp_mbPlayerColSnapPlayerSet(void);
extern void _kerjmp_mbPlayerColSnapGet(void);
extern void _kerjmp_mbPlayerColRestSet(void);
extern void _kerjmp_mbPlayerColCheck(void);
extern void _kerjmp_mbPlayerBestPathGet(void);
extern void _kerjmp_mbPlayerPosReset(void);
extern void _kerjmp_mbPlayerPosResetAll(void);
extern void _kerjmp_mbPlayerMasuCornerSet(void);
extern void _kerjmp_mbPlayerMasuCornerGet(void);
extern void _kerjmp_mbPlayerDiceMotExec(void);
extern void _kerjmp_mbPlayerKettouCheck(void);
extern void _kerjmp_mbPlayerBlackoutSet(void);
extern void _kerjmp_mbPlayerBlackoutGet(void);
extern void _kerjmp_mbPlayerWinLoseVoicePlay(void);
extern void _kerjmp_mbPlayerVoicePlay(void);
extern void _kerjmp_mbPlayerVoicePanPlay(void);
extern void _kerjmp_mbPlayerStartTurnHookSet(void);
extern void _kerjmp_mbPlayerEndTurnHookSet(void);
extern void _kerjmp_mbPlayerMoveHookSet(void);
extern void _kerjmp_mbPlayerDiceTypeGet(void);
extern void _kerjmp_mbPlayerTurnInitHookSet(void);
extern void _kerjmp_mbPlayerTurnCloseHookSet(void);
extern void _kerjmp_mbPlayerColFirstSet(void);
extern void _kerjmp_mbPlayerColOrderReset(void);
extern void _kerjmp_mbPlayerPlusMasuExec(void);
extern void _kerjmp_mbPlayerMinusMasuExec(void);
extern void _kerjmp_mbPlayerCapCoinMasuExec(void);
extern void _kerjmp_mbRouletteCreate(void);
extern void _kerjmp_mbRouletteKaneCreate(void);
extern void _kerjmp_mbRouletteCheck(void);
extern void _kerjmp_mbRouletteWait(void);
extern void _kerjmp_mbRouletteResultGet(void);
extern void _kerjmp_mbRouletteMaxSpeedSet(void);
extern void _kerjmp_mbRouletteComValueSet(void);
extern void _kerjmp_mbSingleInit(void);
extern void _kerjmp_mbSingleClose(void);
extern void _kerjmp_mbSingleReturn(void);
extern void _kerjmp_mbSingleReturnWrite(void);
extern void _kerjmp_mbSingleGameEnd(void);
extern void _kerjmp_mbSingleSaveInit(void);
extern void _kerjmp_mbSingleMgUnlockWrite(void);
extern void _kerjmp_mbSingleMgUnlockInit(void);
extern void _kerjmp_mbSingleMgUnlockSet(void);
extern void _kerjmp_mbSingleMgUnlockReset(void);
extern void _kerjmp_mbSingleMgUnlockGet(void);
extern void _kerjmp_mbSingleMgUnlockCheckAny(void);
extern void _kerjmp_mbSingleMgUnlockNumGet(void);
extern void _kerjmp_mbev_SingleMg(void);
extern void _kerjmp_mbev_SingleMgEnd(void);
extern void _kerjmp_mbSingleStepGet(void);
extern void _kerjmp_mbSingleCall(void);
extern void _kerjmp_mbSingleOppCharGet(void);
extern void _kerjmp_mbSingleTeamCharSet(void);
extern void _kerjmp_mbSingleTeamCharGet(void);
extern void _kerjmp_mbSingleSaveFlush(void);
extern void _kerjmp_mbSingleMgUnlockCheck(void);
extern void _kerjmp_mbMusPlay(void);
extern void _kerjmp_mbMusFadeOut(void);
extern void _kerjmp_mbMusFadeOutSpeed(void);
extern void _kerjmp_mbMusStub(void);
extern void _kerjmp_mbMusParamSet(void);
extern void _kerjmp_mbMusPauseFadeOut(void);
extern void _kerjmp_mbMusStatGet(void);
extern void _kerjmp_mbAudClose(void);
extern void _kerjmp_mbAudInit(void);
extern void _kerjmp_mbAudStub1(void);
extern void _kerjmp_mbAudStub2(void);
extern void _kerjmp_mbMusBoardPlay(void);
extern void _kerjmp_mbMusCheck(void);
extern void _kerjmp_mbMusEndCheck(void);
extern void _kerjmp_mbMusStopCheck(void);
extern void _kerjmp_mbMusFadeCheck(void);
extern void _kerjmp_mbMusPauseSet(void);
extern void _kerjmp_mbMusJinglePlay(void);
extern void _kerjmp_mbMusJingleWait(void);
extern void _kerjmp_mbMusJingleStatGet(void);
extern void _kerjmp_mbAudFXObjSet(void);
extern void _kerjmp_mbAudFXObjCreate(void);
extern void _kerjmp_mbAudFXObjVolSet(void);
extern void _kerjmp_mbAudFXObjKill(void);
extern void _kerjmp_mbAudFXObjClose(void);
extern void _kerjmp_mbAudFXObjEmitterSet(void);
extern void _kerjmp_mbAudFXObjEmitterCreate(void);
extern void _kerjmp_mbAudFXObjEmitterUpdate(void);
extern void _kerjmp_mbAudFXObjEmiterPosSet(void);
extern void _kerjmp_mbAudFXObjRefSet(void);
extern void _kerjmp_mbAudFXPlay(void);
extern void _kerjmp_mbAudFXStop(void);
extern void _kerjmp_mbAudFXStopAll(void);
extern void _kerjmp_mbAudFXPosPlay(void);
extern void _kerjmp_mbAudFXPosPanGet(void);
extern void _kerjmp_mbAudFXEmitterPlay(void);
extern void _kerjmp_mbAudFXVolSet(void);
extern void _kerjmp_mbAudFXPanning(void);
extern void _kerjmp_mbAudFXPosPanning(void);
extern void _kerjmp_mbAudFXDelaySet(void);
extern void _kerjmp_mbAudGuidePlay(void);
extern void _kerjmp_mbev_StarMasu(void);
extern void _kerjmp_mbev_StarFreeMasu(void);
extern void _kerjmp_mbStarGetMain(void);
extern void _kerjmp_mbStarGetPosExec(void);
extern void _kerjmp_mbStarMapViewExec(void);
extern void _kerjmp_mbStarStub2(void);
extern void _kerjmp_mbStarInit(void);
extern void _kerjmp_mbStarClose(void);
extern void _kerjmp_mbStarStub(void);
extern void _kerjmp_mbStarNumSet(void);
extern void _kerjmp_mbStarNumGet(void);
extern void _kerjmp_mbStarFlagInit(void);
extern void _kerjmp_mbStarFlagSet(void);
extern void _kerjmp_mbStarFlagReset(void);
extern void _kerjmp_mbStarFlagGet(void);
extern void _kerjmp_mbStarNextNoSet(void);
extern void _kerjmp_mbStarNextNoGet(void);
extern void _kerjmp_mbStarNoSet(void);
extern void _kerjmp_mbStarNoRandGet(void);
extern void _kerjmp_mbStarReset(void);
extern void _kerjmp_mbStarNoDispSet(void);
extern void _kerjmp_mbStarMasuDispSet(void);
extern void _kerjmp_mbStarDispSetAll(void);
extern void _kerjmp_mbStarObjDispSetAll(void);
extern void _kerjmp_mbStarMasuNextSet(void);
extern void _kerjmp_mbStarGetExec(void);
extern void _kerjmp_mbStarMoveHookSet(void);
extern void _kerjmp_mbStarMasuFuncSet(void);
extern void _kerjmp_mbStarAddNumSet(void);
extern void _kerjmp_mbStarDispCreate(void);
extern void _kerjmp_mbStarDispPlayerCreate(void);
extern void _kerjmp_mbStarDispCheck(void);
extern void _kerjmp_mbStarObjCreate(void);
extern void _kerjmp_mbStarObjPosSet(void);
extern void _kerjmp_mbStarObjRotSet(void);
extern void _kerjmp_mbStarObjScaleSet(void);
extern void _kerjmp_mbStarObjPosSetV(void);
extern void _kerjmp_mbStarObjRotSetV(void);
extern void _kerjmp_mbStarObjScaleSetV(void);
extern void _kerjmp_mbStarObjPosGet(void);
extern void _kerjmp_mbStarObjRotGet(void);
extern void _kerjmp_mbStarObjScaleGet(void);
extern void _kerjmp_mbStarObjDispSet(void);
extern void _kerjmp_mbStarObjDispFlagSet(void);
extern void _kerjmp_mbStarObjKill(void);
extern void _kerjmp_mbStarChestCreate(void);
extern void _kerjmp_mbStarAddProcExec(void);
extern void _kerjmp_mbStarAddDispExec(void);
extern void _kerjmp_mbStarAddExec(void);
extern void _kerjmp_mbStarAddAllProcExecV(void);
extern void _kerjmp_mbStarAddAllProcExec(void);
extern void _kerjmp_mbStarAddAllExec(void);
extern void _kerjmp_mbZtarObjInit(void);
extern void _kerjmp_mbZtarObjClose(void);
extern void _kerjmp_mbZtarMasuNextSet(void);
extern void _kerjmp_mbZtarGetExec(void);
extern void _kerjmp_mbZtarGetMain(void);
extern void _kerjmp_mbZtarObjDispFlagSet(void);
extern void _kerjmp_mbStarMapViewProcExec(void);
extern void _kerjmp_mbStatusInit(void);
extern void _kerjmp_mbStatusClose(void);
extern void _kerjmp_mbStatusReset(void);
extern void _kerjmp_mbStatusDispGet(void);
extern void _kerjmp_mbStatusDispSet(void);
extern void _kerjmp_mbStatusDispFocusSet(void);
extern void _kerjmp_mbStatusDispSetAll(void);
extern void _kerjmp_mbStatusDispForceSet(void);
extern void _kerjmp_mbStatusDispForceSetAll(void);
extern void _kerjmp_mbStatusMoveCheck(void);
extern void _kerjmp_mbStatusOffCheckAll(void);
extern void _kerjmp_mbSprCreate(void);
extern void _kerjmp_mbStatusPosGet(void);
extern void _kerjmp_mbStatusNoPosGet(void);
extern void _kerjmp_mbStatusCapsuleDispSet(void);
extern void _kerjmp_mbStatusKaoNoSet(void);
extern void _kerjmp_mbSprNumSet(void);
extern void _kerjmp_mbStatusMoveTo(void);
extern void _kerjmp_mbStatusMoveSet(void);
extern void _kerjmp_mbStatusPosOnGet(void);
extern void _kerjmp_mbStatusPosOffGet(void);
extern void _kerjmp_mbStatusRainbowSet(void);
extern void _kerjmp_mbStatusColorSet(void);
extern void _kerjmp_mbStatusColorAllSet(void);
extern void _kerjmp_mbStatusColorGet(void);
extern void _kerjmp_mbStatusLayoutSet(void);
extern void _kerjmp_mbStatusCapsuleFocusSet(void);
extern void _kerjmp_mbStatusNoGet(void);
extern void _kerjmp_mbStatusNoMoveSet(void);
extern void _kerjmp_mbStatusDispBackup(void);
extern void _kerjmp_mbStatusDispRestoreForce(void);
extern void _kerjmp_mbStatusDispRestore(void);
extern void _kerjmp_mbStatusMasuDispSet(void);
extern void _kerjmp_mbSNpcInit(void);
extern void _kerjmp_mbSNpcCreate(void);
extern void _kerjmp_mbSNpcKill(void);
extern void _kerjmp_mbSNpcMotReset(void);
extern void _kerjmp_mbSNpcPlayerWalkSet(void);
extern void _kerjmp_mbSNpcStarExec(void);
extern void _kerjmp_mbSNpcMasuGet(void);
extern void _kerjmp_mbSNpcMotWinSet(void);
extern void _kerjmp_mbSNpcMotIdleSet(void);
extern void _kerjmp_mbSNpcDispSet(void);
extern void _kerjmp_mbMasuChanceCreate(void);
extern void _kerjmp_mbMasuChanceTypeSet(void);
extern void _kerjmp_mbMasuChanceFlagSet(void);
extern void _kerjmp_mbMasuChancePlayerSet(void);
extern void _kerjmp_mbMasuChanceSet(void);
extern void _kerjmp_mbMasuChanceKill(void);
extern void _kerjmp_mbSNpcMasuStarNextGet(void);
extern void _kerjmp_mbObjStarTevStageSet(void);
extern void _kerjmp_mbObjFadeCreate(void);
extern void _kerjmp_mbObjFadeKill(void);
extern void _kerjmp_mbObjFadeTexRotSet(void);
extern void _kerjmp_mbObjFadeTexColorSet(void);
extern void _kerjmp_mbObjMetalCreate(void);
extern void _kerjmp_mbObjMetalKill(void);
extern void _kerjmp_mbObjMetalTPLvlSet(void);
extern void _kerjmp_mbObjMetalColorSet(void);
extern void _kerjmp_mbObjBiriQCreate(void);
extern void _kerjmp_mbObjBiriQKill(void);
extern void _kerjmp_mbObjBiriQColorSet(void);
extern void _kerjmp_mbTutorialInit(void);
extern void _kerjmp_mbTutorialCreate(void);
extern void _kerjmp_mbTutorialExitReqGet(void);
extern void _kerjmp_mbTutorialExitSet(void);
extern void _kerjmp_mbTutorialExitOnSet(void);
extern void _kerjmp_mbTutorialCall(void);
extern void _kerjmp_mbTutorialMainFuncSet(void);
extern void _kerjmp_mbTutorialSprCreate(void);
extern void _kerjmp_mbTutorialSprDispOn(void);
extern void _kerjmp_mbTutorialSprDispOff(void);
extern void _kerjmp_mbTutorialSprKill(void);
extern void _kerjmp_mbTutorialSprGrpSet(void);
extern void _kerjmp_mbTutorialSprGrpKill(void);
extern void _kerjmp_mbTutorialModelCreate(void);
extern void _kerjmp_mbTutorialModelKill(void);
extern void _kerjmp_mbTutorialGuideCreate(void);
extern void _kerjmp_mbTutorialGuideGet(void);
extern void _kerjmp_mbTutorialGuideClose(void);
extern void _kerjmp_mbTutorialWinWait(void);
extern void _kerjmp_mbTutorialWinClose(void);
extern void _kerjmp_mbTutorialWinMesExec(void);
extern void _kerjmp_mbTutorialWinCreate(void);
extern void _kerjmp_mbTutorialWinMesMasuExec(void);
extern void _kerjmp_mbTutorialWinKeyWait(void);
extern void _kerjmp_mbTutorialViewSet(void);
extern void _kerjmp_mbTutorialViewMasuSet(void);
extern void _kerjmp_mbTutorialVSleep(void);
extern void _kerjmp_mbTutorialCallEnd(void);
extern void _kerjmp_mbTutorialCallWait(void);
extern void _kerjmp_mbTutorialMultiCall(void);
extern void _kerjmp_mbTutorialCallResult(void);
extern void _kerjmp_mbTutorialMesCall(void);
extern void _kerjmp_mbTutorialMesMasuCall(void);
extern void _kerjmp_mbTutorialModeSet(void);
extern void _kerjmp_mbTutorialModeGet(void);
extern void _kerjmp_mbTutorialResultSet(void);
extern void _kerjmp_mbTutorialSceneGet(void);
extern void _kerjmp_mbTutorialTurnCall(void);
extern void _kerjmp_mbTutorialGuideCall(void);
extern void _kerjmp_mbTutorialCapsuleUseCall(void);
extern void _kerjmp_mbTutorialMgCallExec(void);
extern void _kerjmp_mbTutorialMgCallClose(void);
extern void _kerjmp_mbWinInit(void);
extern void _kerjmp_mbWinClose(void);
extern void _kerjmp_mbWinCreate(void);
extern void _kerjmp_mbWinCreateChoice(void);
extern void _kerjmp_mbWinCreateHelp(void);
extern void _kerjmp_mbWinCreateFrame(void);
extern void _kerjmp_mbWinCreateTime(void);
extern void _kerjmp_mbWinCreateBlank(void);
extern void _kerjmp_mbWinCreateBlankFrame(void);
extern void _kerjmp_mbWinKillAll(void);
extern void _kerjmp_mbWinKill(void);
extern void _kerjmp_mbWinTopKill(void);
extern void _kerjmp_mbWinPosSet(void);
extern void _kerjmp_mbWinTopPosSet(void);
extern void _kerjmp_mbWinPosGet(void);
extern void _kerjmp_mbWinTopPosGet(void);
extern void _kerjmp_mbWinSizeSet(void);
extern void _kerjmp_mbWinTopSizeSet(void);
extern void _kerjmp_mbWinMesMaxSizeGet(void);
extern void _kerjmp_mbWinTopMesMaxSizeGet(void);
extern void _kerjmp_mbWinScaleSet(void);
extern void _kerjmp_mbWinTopScaleSet(void);
extern void _kerjmp_mbWinScaleGet(void);
extern void _kerjmp_mbWinTopScaleGet(void);
extern void _kerjmp_mbWinChoiceGet(void);
extern void _kerjmp_mbWinTopChoiceGet(void);
extern void _kerjmp_mbWinPause(void);
extern void _kerjmp_mbWinTopPause(void);
extern void _kerjmp_mbWinInsertMesSet(void);
extern void _kerjmp_mbWinTopInsertMesSet(void);
extern void _kerjmp_mbWinDoneCheck(void);
extern void _kerjmp_mbWinTopDoneCheck(void);
extern void _kerjmp_mbWinWait(void);
extern void _kerjmp_mbWinTopWait(void);
extern void _kerjmp_mbWinAttrSet(void);
extern void _kerjmp_mbWinTopAttrSet(void);
extern void _kerjmp_mbWinAttrReset(void);
extern void _kerjmp_mbWinTopAttrReset(void);
extern void _kerjmp_mbWinChoiceDisable(void);
extern void _kerjmp_mbWinTopChoiceDisable(void);
extern void _kerjmp_mbWinMesSpeedSet(void);
extern void _kerjmp_mbWinTopMesSpeedSet(void);
extern void _kerjmp_mbWinMesColSet(void);
extern void _kerjmp_mbWinTopMesColSet(void);
extern void _kerjmp_mbWinChoiceNowGet(void);
extern void _kerjmp_mbWinTopChoiceNowGet(void);
extern void _kerjmp_mbWinPriSet(void);
extern void _kerjmp_mbWinTopPriSet(void);
extern void _kerjmp_mbWinChoiceNowGet2(void);
extern void _kerjmp_mbWinTopChoiceNowGet2(void);
extern void _kerjmp_mbWinPlayerDisable(void);
extern void _kerjmp_mbWinTopPlayerDisable(void);
extern void _kerjmp_mbWinIDGet(void);
extern void _kerjmp_mbWinTopIDGet(void);
extern void _kerjmp_mbWinDispSet(void);
extern void _kerjmp_mbWinTopDispSet(void);
extern void _kerjmp_mbWinPauseHook(void);
extern void _kerjmp_mbWinGet(void);
extern void _kerjmp_mbWinCenterGet(void);
extern void _kerjmp_mbWinCenterInsertGet(void);
extern void _kerjmp_mbWinTopCenterInsertGet(void);
extern void _kerjmp_mbWipeSpecialInit(void);
extern void _kerjmp_mbWipeSpecialClose(void);
extern void _kerjmp_mbWipeSpecialKill(void);
extern void _kerjmp_mbWipeWait(void);
extern void _kerjmp_mbWipeCreate(void);
extern void _kerjmp_mbWipeFadeOut(void);
extern void _kerjmp_mbWipeFadeIn(void);
extern void _kerjmp_mbWipeWhiteFadeOut(void);
extern void _kerjmp_mbWipeWhiteFadeIn(void);
extern void _kerjmp_mbWipeDissolveFadeOut(void);
extern void _kerjmp_mbWipeDissolveFadeIn(void);
extern void _kerjmp_mbWipeFadeOutTime(void);
extern void _kerjmp_mbWipeFadeInTime(void);
extern void _kerjmp_mbWipeWhiteFadeOutTime(void);
extern void _kerjmp_mbWipeWhiteFadeInTime(void);
extern void _kerjmp_mbWipeDissolveFadeOutTime(void);
extern void _kerjmp_mbWipeDissolveFadeInTime(void);
extern void _kerjmp_mbWipeSpecialCreate(void);
extern void _kerjmp_mbWipeSpecialWait(void);
extern void _kerjmp_mbWipeSpecialFadeOutCreate(void);
extern void _kerjmp_mbWipeSpecialFadeInCreate(void);
extern void _kerjmp_mbWipeSpecialStatGet(void);
extern void _kerjmp_mbMgCallInit(void);
extern void _kerjmp_mbev_MgCall(void);
extern void _kerjmp_mbev_MgCallKettou(void);
extern void _kerjmp_mbev_MgCallDonkey(void);
extern void _kerjmp_mbev_MgCallKoopa(void);
extern void _kerjmp_mbev_MgCallSingle(void);
extern void _kerjmp_mbev_MgCallSingleKoopa(void);
extern void _kerjmp_mbMgCallDataClose(void);
extern void _kerjmp_mbev_MgCallTutorial(void);
extern void _kerjmp_mbMgRouletteFocusKill(void);
extern void _kerjmp_mbMgRouletteNumGet(void);
extern void _kerjmp_mbMgCallSingleOnCheck(void);
extern void _kerjmp_mbMgCallVsEffCreate(void);

asm void _kerent(void) {
    #ifdef __MWERKS__ // clang-format off
    nofralloc
    entry _kerjmp_OSDumpStopwatch
    b OSDumpStopwatch
    entry _kerjmp_PSVECDotProduct
    b PSVECDotProduct
    entry _kerjmp_PSVECDistance
    b PSVECDistance
    entry _kerjmp_PSVECSquareDistance
    b PSVECSquareDistance
    entry _kerjmp_VIGetDTVStatus
    b VIGetDTVStatus
    entry _kerjmp_OSGetProgressiveMode
    b OSGetProgressiveMode
    entry _kerjmp_OSSetProgressiveMode
    b OSSetProgressiveMode
    entry _kerjmp_PSVECSubtract
    b PSVECSubtract
    entry _kerjmp_PSMTXTranspose
    b PSMTXTranspose
    entry _kerjmp_C_VECReflect
    b C_VECReflect
    entry _kerjmp_PSMTXInvXpose
    b PSMTXInvXpose
    entry _kerjmp_PSMTXRotRad
    b PSMTXRotRad
    entry _kerjmp_PSMTXRotAxisRad
    b PSMTXRotAxisRad
    entry _kerjmp_PSVECMag
    b PSVECMag
    entry _kerjmp_PSVECScale
    b PSVECScale
    entry _kerjmp_PSMTXMultVecSR
    b PSMTXMultVecSR
    entry _kerjmp_C_QUATRotAxisRad
    b C_QUATRotAxisRad
    entry _kerjmp_C_QUATMtx
    b C_QUATMtx
    entry _kerjmp_C_QUATAdd
    b C_QUATAdd
    entry _kerjmp_C_QUATSlerp
    b C_QUATSlerp
    entry _kerjmp_PSQUATDotProduct
    b PSQUATDotProduct
    entry _kerjmp_PSQUATAdd
    b PSQUATAdd
    entry _kerjmp_PSMTXQuat
    b PSMTXQuat
    entry _kerjmp_PSQUATMultiply
    b PSQUATMultiply
    entry _kerjmp_PSQUATInverse
    b PSQUATInverse
    entry _kerjmp_PSQUATNormalize
    b PSQUATNormalize
    entry _kerjmp_PSQUATScale
    b PSQUATScale
    entry _kerjmp_PSQUATSubtract
    b PSQUATSubtract
    entry _kerjmp_OSTicksToCalendarTime
    b OSTicksToCalendarTime
    entry _kerjmp_PSMTXTransApply
    b PSMTXTransApply
    entry _kerjmp_PSMTXScaleApply
    b PSMTXScaleApply
    entry _kerjmp_OSGetLanguage
    b OSGetLanguage
    entry _kerjmp_PSMTXMultVecArraySR
    b PSMTXMultVecArraySR
    entry _kerjmp_PSMTX44MultVec
    b PSMTX44MultVec
    entry _kerjmp_SIGetTypeString
    b SIGetTypeString
    entry _kerjmp_OSCheckActiveThreads
    b OSCheckActiveThreads
    entry _kerjmp_OSGetThreadPriority
    b OSGetThreadPriority
    entry _kerjmp_OSInitSemaphore
    b OSInitSemaphore
    entry _kerjmp_OSIsThreadTerminated
    b OSIsThreadTerminated
    entry _kerjmp_OSSignalSemaphore
    b OSSignalSemaphore
    entry _kerjmp_OSWaitSemaphore
    b OSWaitSemaphore
    entry _kerjmp_log
    b log
    entry _kerjmp_exp
    b exp
    entry _kerjmp_GXWaitDrawDone
    b GXWaitDrawDone
    entry _kerjmp_GXProject
    b GXProject
    entry _kerjmp_GXGetProjectionv
    b GXGetProjectionv
    entry _kerjmp_GXGetViewportv
    b GXGetViewportv
    entry _kerjmp_GXPixModeSync
    b GXPixModeSync
    entry _kerjmp_GXSetIndTexOrder
    b GXSetIndTexOrder
    entry _kerjmp_GXSetIndTexMtx
    b GXSetIndTexMtx
    entry _kerjmp_GXSetTevIndWarp
    b GXSetTevIndWarp
    entry _kerjmp_GXSetTexCoordScaleManually
    b GXSetTexCoordScaleManually
    entry _kerjmp_GXSetTevIndTile
    b GXSetTevIndTile
    entry _kerjmp_GXNtsc480Prog
    b GXNtsc480Prog
    entry _kerjmp_GXNtsc480Int
    b GXNtsc480Int
    entry _kerjmp_GXNtsc240Int
    b GXNtsc240Int
    entry _kerjmp_GXSetVtxDescv
    b GXSetVtxDescv
    entry _kerjmp_GXDrawSphere
    b GXDrawSphere
    entry _kerjmp_GXDrawCylinder
    b GXDrawCylinder
    entry _kerjmp_omSetStatBit
    b omSetStatBit
    entry _kerjmp_GameMesKillCheck
    b GameMesKillCheck
    entry _kerjmp_WipeCheck
    b WipeCheck
    entry _kerjmp_WipeTypeGet
    b WipeTypeGet
    entry _kerjmp_omMakeGroupEx
    b omMakeGroupEx
    entry _kerjmp_omGetGroupMemberListEx
    b omGetGroupMemberListEx
    entry _kerjmp_GameMesDispSet
    b GameMesDispSet
    entry _kerjmp_GameMesKill
    b GameMesKill
    entry _kerjmp_ClusterAdjustObject
    b ClusterAdjustObject
    entry _kerjmp_printWin
    b printWin
    entry _kerjmp_print8
    b print8
    entry _kerjmp_HuSysVWaitSet
    b HuSysVWaitSet
    entry _kerjmp_GameMesGet
    b GameMesGet
    entry _kerjmp_omObjManPause
    b omObjManPause
    entry _kerjmp_GameMesInit
    b GameMesInit
    entry _kerjmp_GameMesCreate
    b GameMesCreate
    entry _kerjmp_omDelObjEx
    b omDelObjEx
    entry _kerjmp_GameMesPosSet
    b GameMesPosSet
    entry _kerjmp_atan2
    b atan2
    entry _kerjmp_atan2f
    b atan2f
    entry _kerjmp_cos
    b cos
    entry _kerjmp_GameMesStatGet
    b GameMesStatGet
    entry _kerjmp_Center
    b Center
    entry _kerjmp_CRotM
    b CRotM
    entry _kerjmp_CenterM
    b CenterM
    entry _kerjmp_CZoomM
    b CZoomM
    entry _kerjmp_omOutViewMulti
    b omOutViewMulti
    entry _kerjmp_omOutView
    b omOutView
    entry _kerjmp_omAddObjEx
    b omAddObjEx
    entry _kerjmp_omInitObjMan
    b omInitObjMan
    entry _kerjmp_rand8
    b rand8
    entry _kerjmp_frandmod
    b frandmod
    entry _kerjmp_frandom
    b frandom
    entry _kerjmp_SetDefLight
    b SetDefLight
    entry _kerjmp_HuSetVecF
    b HuSetVecF
    entry _kerjmp_HuSubVecF
    b HuSubVecF
    entry _kerjmp_frandf
    b frandf
    entry _kerjmp_GameMesSprKill
    b GameMesSprKill
    entry _kerjmp_mtxRotCat
    b mtxRotCat
    entry _kerjmp_MgScoreBoxInit
    b MgScoreBoxInit
    entry _kerjmp_MgScoreBoxCreate
    b MgScoreBoxCreate
    entry _kerjmp_MgScoreBoxCreateChar
    b MgScoreBoxCreateChar
    entry _kerjmp_MgScoreBoxCreateCharMulti
    b MgScoreBoxCreateCharMulti
    entry _kerjmp_MgScoreBoxColorSet
    b MgScoreBoxColorSet
    entry _kerjmp_MgScoreBoxTPLvlSet
    b MgScoreBoxTPLvlSet
    entry _kerjmp_MgScoreBoxPosSet
    b MgScoreBoxPosSet
    entry _kerjmp_MgScoreBoxDispSet
    b MgScoreBoxDispSet
    entry _kerjmp_GameMesStrWinCreate
    b GameMesStrWinCreate
    entry _kerjmp_Hu3DCameraLayerHookSet
    b Hu3DCameraLayerHookSet
    entry _kerjmp_Hu3DCameraLayerHookReset
    b Hu3DCameraLayerHookReset
    entry _kerjmp_Hu3DLayerHookSet
    b Hu3DLayerHookSet
    entry _kerjmp_Hu3DLayerHookReset
    b Hu3DLayerHookReset
    entry _kerjmp_Hu3DNoSyncSet
    b Hu3DNoSyncSet
    entry _kerjmp_Hu3DModelCreate
    b Hu3DModelCreate
    entry _kerjmp_Hu3DModelLink
    b Hu3DModelLink
    entry _kerjmp_Hu3DHookFuncCreate
    b Hu3DHookFuncCreate
    entry _kerjmp_Hu3DModelPosSet
    b Hu3DModelPosSet
    entry _kerjmp_Hu3DModelPosSetV
    b Hu3DModelPosSetV
    entry _kerjmp_Hu3DModelRotSet
    b Hu3DModelRotSet
    entry _kerjmp_Hu3DModelRotSetV
    b Hu3DModelRotSetV
    entry _kerjmp_Hu3DModelScaleSet
    b Hu3DModelScaleSet
    entry _kerjmp_Hu3DModelScaleSetV
    b Hu3DModelScaleSetV
    entry _kerjmp_Hu3DModelPosGet
    b Hu3DModelPosGet
    entry _kerjmp_Hu3DModelRotGet
    b Hu3DModelRotGet
    entry _kerjmp_Hu3DModelScaleGet
    b Hu3DModelScaleGet
    entry _kerjmp_Hu3DModelKill
    b Hu3DModelKill
    entry _kerjmp_Hu3DModelAllKill
    b Hu3DModelAllKill
    entry _kerjmp_Hu3DModelAttrSet
    b Hu3DModelAttrSet
    entry _kerjmp_Hu3DModelAttrReset
    b Hu3DModelAttrReset
    entry _kerjmp_Hu3DModelAttrGet
    b Hu3DModelAttrGet
    entry _kerjmp_Hu3DModelMotionAttrGet
    b Hu3DModelMotionAttrGet
    entry _kerjmp_Hu3DBGColorSet
    b Hu3DBGColorSet
    entry _kerjmp_Hu3DModelCameraSet
    b Hu3DModelCameraSet
    entry _kerjmp_Hu3DModelLayerSet
    b Hu3DModelLayerSet
    entry _kerjmp_Hu3DModelTPLvlSet
    b Hu3DModelTPLvlSet
    entry _kerjmp_Hu3DModelHookSet
    b Hu3DModelHookSet
    entry _kerjmp_Hu3DModelHookReset
    b Hu3DModelHookReset
    entry _kerjmp_Hu3DModelHookObjReset
    b Hu3DModelHookObjReset
    entry _kerjmp_Hu3DModelAmbSet
    b Hu3DModelAmbSet
    entry _kerjmp_Hu3DModelShadowDispOn
    b Hu3DModelShadowDispOn
    entry _kerjmp_Hu3DModelShadowDispOff
    b Hu3DModelShadowDispOff
    entry _kerjmp_Hu3DModelProjectionSet
    b Hu3DModelProjectionSet
    entry _kerjmp_Hu3DModelProjectionReset
    b Hu3DModelProjectionReset
    entry _kerjmp_Hu3DModelObjPosGet
    b Hu3DModelObjPosGet
    entry _kerjmp_Hu3DModelObjMtxGet
    b Hu3DModelObjMtxGet
    entry _kerjmp_Hu3DModelHiliteTypeSet
    b Hu3DModelHiliteTypeSet
    entry _kerjmp_Hu3DModelReflectTypeSet
    b Hu3DModelReflectTypeSet
    entry _kerjmp_Hu3DModelHiliteMapSet
    b Hu3DModelHiliteMapSet
    entry _kerjmp_Hu3DReflectModelSet
    b Hu3DReflectModelSet
    entry _kerjmp_Hu3DModelMatHookSet
    b Hu3DModelMatHookSet
    entry _kerjmp_Hu3DModelDebug
    b Hu3DModelDebug
    entry _kerjmp_Hu3DModelMtxSet
    b Hu3DModelMtxSet
    entry _kerjmp_Hu3DModelMtxGet
    b Hu3DModelMtxGet
    entry _kerjmp_Hu3DModelShadowMapTPLvlSet
    b Hu3DModelShadowMapTPLvlSet
    entry _kerjmp_Hu3DModelShadowMapObjTPLvlSet
    b Hu3DModelShadowMapObjTPLvlSet
    entry _kerjmp_Hu3DModelDieCreate
    b Hu3DModelDieCreate
    entry _kerjmp_Hu3DModelLinkDieCreate
    b Hu3DModelLinkDieCreate
    entry _kerjmp_HuPadInit
    b HuPadInit
    entry _kerjmp_HuPadRead
    b HuPadRead
    entry _kerjmp_HuPadRumbleSet
    b HuPadRumbleSet
    entry _kerjmp_HuPadRumbleStop
    b HuPadRumbleStop
    entry _kerjmp_HuPadRumbleAllStop
    b HuPadRumbleAllStop
    entry _kerjmp_HuPadStatGet
    b HuPadStatGet
    entry _kerjmp_HuPadRumbleGet
    b HuPadRumbleGet
    entry _kerjmp_HuPadStkDirGet
    b HuPadStkDirGet
    entry _kerjmp_HuPadSubStkDirGet
    b HuPadSubStkDirGet
    entry _kerjmp_Hu3DCameraCreate
    b Hu3DCameraCreate
    entry _kerjmp_Hu3DCameraPerspectiveSet
    b Hu3DCameraPerspectiveSet
    entry _kerjmp_Hu3DCameraViewportSet
    b Hu3DCameraViewportSet
    entry _kerjmp_Hu3DCameraScissorSet
    b Hu3DCameraScissorSet
    entry _kerjmp_Hu3DCameraPosSet
    b Hu3DCameraPosSet
    entry _kerjmp_Hu3DCameraKill
    b Hu3DCameraKill
    entry _kerjmp_Hu3DCameraPosGet
    b Hu3DCameraPosGet
    entry _kerjmp_Hu3DCameraPosSetV
    b Hu3DCameraPosSetV
    entry _kerjmp_Hu3DCameraAllKill
    b Hu3DCameraAllKill
    entry _kerjmp_Hu3DCameraSet
    b Hu3DCameraSet
    entry _kerjmp_Hu3DCameraMotionOn
    b Hu3DCameraMotionOn
    entry _kerjmp_Hu3DCameraMotionStart
    b Hu3DCameraMotionStart
    entry _kerjmp_Hu3DCameraMotionOff
    b Hu3DCameraMotionOff
    entry _kerjmp_Hu3DModelCameraCreate
    b Hu3DModelCameraCreate
    entry _kerjmp_Hu3DModelCameraInfoSet
    b Hu3DModelCameraInfoSet
    entry _kerjmp_Hu3DCameraPerspectiveGet
    b Hu3DCameraPerspectiveGet
    entry _kerjmp_Hu3DLighInit
    b Hu3DLighInit
    entry _kerjmp_Hu3DGLightCreate
    b Hu3DGLightCreate
    entry _kerjmp_Hu3DGLightCreateV
    b Hu3DGLightCreateV
    entry _kerjmp_Hu3DGLightKill
    b Hu3DGLightKill
    entry _kerjmp_Hu3DLightAllKill
    b Hu3DLightAllKill
    entry _kerjmp_Hu3DLightSet
    b Hu3DLightSet
    entry _kerjmp_Hu3DGLightSpotSet
    b Hu3DGLightSpotSet
    entry _kerjmp_Hu3DGLightInfinitytSet
    b Hu3DGLightInfinitytSet
    entry _kerjmp_Hu3DGLightPointSet
    b Hu3DGLightPointSet
    entry _kerjmp_Hu3DGLightColorSet
    b Hu3DGLightColorSet
    entry _kerjmp_Hu3DGLightPosSetV
    b Hu3DGLightPosSetV
    entry _kerjmp_Hu3DGLightPosSet
    b Hu3DGLightPosSet
    entry _kerjmp_Hu3DGLightPosAimSetV
    b Hu3DGLightPosAimSetV
    entry _kerjmp_Hu3DGLightPosAimSet
    b Hu3DGLightPosAimSet
    entry _kerjmp_Hu3DGLightStaticSet
    b Hu3DGLightStaticSet
    entry _kerjmp_Hu3DLLightParamGet
    b Hu3DLLightParamGet
    entry _kerjmp_Hu3DGLightParamGet
    b Hu3DGLightParamGet
    entry _kerjmp_Hu3DGLightPosAngleSet
    b Hu3DGLightPosAngleSet
    entry _kerjmp_Hu3DModelLightBitSet
    b Hu3DModelLightBitSet
    entry _kerjmp_Hu3DModelLightBitReset
    b Hu3DModelLightBitReset
    entry _kerjmp_Hu3DModelLightInfoSet
    b Hu3DModelLightInfoSet
    entry _kerjmp_Hu3DModelLightIdGet
    b Hu3DModelLightIdGet
    entry _kerjmp_Hu3DReflectMapSet
    b Hu3DReflectMapSet
    entry _kerjmp_Hu3DReflectNoSet
    b Hu3DReflectNoSet
    entry _kerjmp_Hu3DFogSet
    b Hu3DFogSet
    entry _kerjmp_Hu3DFogClear
    b Hu3DFogClear
    entry _kerjmp_Hu3DModelObjPtrGet
    b Hu3DModelObjPtrGet
    entry _kerjmp_Hu3DModelShadowSet
    b Hu3DModelShadowSet
    entry _kerjmp_Hu3DModelShadowReset
    b Hu3DModelShadowReset
    entry _kerjmp_Hu3DShadowCreate
    b Hu3DShadowCreate
    entry _kerjmp_Hu3DShadowPosSet
    b Hu3DShadowPosSet
    entry _kerjmp_Hu3DShadowTPLvlSet
    b Hu3DShadowTPLvlSet
    entry _kerjmp_Hu3DShadowColSet
    b Hu3DShadowColSet
    entry _kerjmp_Hu3DShadowMultiCreate
    b Hu3DShadowMultiCreate
    entry _kerjmp_Hu3DShadowMultiPosSet
    b Hu3DShadowMultiPosSet
    entry _kerjmp_Hu3DShadowMultiSizeSet
    b Hu3DShadowMultiSizeSet
    entry _kerjmp_Hu3DShadowMultiColSet
    b Hu3DShadowMultiColSet
    entry _kerjmp_Hu3DShadowMultiTPLvlSet
    b Hu3DShadowMultiTPLvlSet
    entry _kerjmp_Hu3DModelShadowMapSet
    b Hu3DModelShadowMapSet
    entry _kerjmp_Hu3DModelShadowMapObjSet
    b Hu3DModelShadowMapObjSet
    entry _kerjmp_Hu3DModelShadowMapObjReset
    b Hu3DModelShadowMapObjReset
    entry _kerjmp_Hu3DModelShadowMapReset
    b Hu3DModelShadowMapReset
    entry _kerjmp_Hu3DShadowSizeSet
    b Hu3DShadowSizeSet
    entry _kerjmp_Hu3DMipMapSet
    b Hu3DMipMapSet
    entry _kerjmp_Hu3DModelClusterAttrSet
    b Hu3DModelClusterAttrSet
    entry _kerjmp_Hu3DModelClusterAttrReset
    b Hu3DModelClusterAttrReset
    entry _kerjmp_Hu3DLLightCreate
    b Hu3DLLightCreate
    entry _kerjmp_Hu3DLLightCreateV
    b Hu3DLLightCreateV
    entry _kerjmp_Hu3DLLightSpotSet
    b Hu3DLLightSpotSet
    entry _kerjmp_Hu3DLLightInfinitytSet
    b Hu3DLLightInfinitytSet
    entry _kerjmp_Hu3DLLightPointSet
    b Hu3DLLightPointSet
    entry _kerjmp_Hu3DLLightKill
    b Hu3DLLightKill
    entry _kerjmp_Hu3DLLightColorSet
    b Hu3DLLightColorSet
    entry _kerjmp_Hu3DLLightPosSetV
    b Hu3DLLightPosSetV
    entry _kerjmp_Hu3DLLightPosSet
    b Hu3DLLightPosSet
    entry _kerjmp_Hu3DLLightPosAimSetV
    b Hu3DLLightPosAimSetV
    entry _kerjmp_Hu3DLLightPosAimSet
    b Hu3DLLightPosAimSet
    entry _kerjmp_Hu3DLLightStaticSet
    b Hu3DLLightStaticSet
    entry _kerjmp_Hu3DLLightPosAngleSet
    b Hu3DLLightPosAngleSet
    entry _kerjmp_Hu3DShineSet
    b Hu3DShineSet
    entry _kerjmp_Hu3DObjHookSet
    b Hu3DObjHookSet
    entry _kerjmp_Hu3DAnimInit
    b Hu3DAnimInit
    entry _kerjmp_Hu3DAnimCreate
    b Hu3DAnimCreate
    entry _kerjmp_Hu3DAnimAnimSet
    b Hu3DAnimAnimSet
    entry _kerjmp_Hu3DAnimLink
    b Hu3DAnimLink
    entry _kerjmp_Hu3DAnimKill
    b Hu3DAnimKill
    entry _kerjmp_Hu3DAnimAllKill
    b Hu3DAnimAllKill
    entry _kerjmp_Hu3DAnimSet
    b Hu3DAnimSet
    entry _kerjmp_Hu3DAnimExec
    b Hu3DAnimExec
    entry _kerjmp_Hu3DAnimAttrSet
    b Hu3DAnimAttrSet
    entry _kerjmp_Hu3DAnimAttrReset
    b Hu3DAnimAttrReset
    entry _kerjmp_Hu3DAnimSpeedSet
    b Hu3DAnimSpeedSet
    entry _kerjmp_Hu3DAnimBankSet
    b Hu3DAnimBankSet
    entry _kerjmp_Hu3DAnmNoSet
    b Hu3DAnmNoSet
    entry _kerjmp_Hu3DTexScrollCreate
    b Hu3DTexScrollCreate
    entry _kerjmp_Hu3DTexScrollPosSet
    b Hu3DTexScrollPosSet
    entry _kerjmp_Hu3DTexScrollPosMoveSet
    b Hu3DTexScrollPosMoveSet
    entry _kerjmp_Hu3DTexScrollRotSet
    b Hu3DTexScrollRotSet
    entry _kerjmp_Hu3DTexScrollRotMoveSet
    b Hu3DTexScrollRotMoveSet
    entry _kerjmp_Hu3DTexScrollKill
    b Hu3DTexScrollKill
    entry _kerjmp_Hu3DTexScrollAllKill
    b Hu3DTexScrollAllKill
    entry _kerjmp_Hu3DTexScrollPauseDisableSet
    b Hu3DTexScrollPauseDisableSet
    entry _kerjmp_Hu3DParticleCreate
    b Hu3DParticleCreate
    entry _kerjmp_Hu3DParticleScaleSet
    b Hu3DParticleScaleSet
    entry _kerjmp_Hu3DParticleZRotSet
    b Hu3DParticleZRotSet
    entry _kerjmp_Hu3DParticleColSet
    b Hu3DParticleColSet
    entry _kerjmp_Hu3DParticleTPLvlSet
    b Hu3DParticleTPLvlSet
    entry _kerjmp_Hu3DParticleBlendModeSet
    b Hu3DParticleBlendModeSet
    entry _kerjmp_Hu3DParticleHookSet
    b Hu3DParticleHookSet
    entry _kerjmp_Hu3DParticleCntSet
    b Hu3DParticleCntSet
    entry _kerjmp_Hu3DParticleAttrSet
    b Hu3DParticleAttrSet
    entry _kerjmp_Hu3DParticleAttrReset
    b Hu3DParticleAttrReset
    entry _kerjmp_Hu3DParticleAnimModeSet
    b Hu3DParticleAnimModeSet
    entry _kerjmp_Hu3DParManInit
    b Hu3DParManInit
    entry _kerjmp_Hu3DParManCreate
    b Hu3DParManCreate
    entry _kerjmp_Hu3DParManKill
    b Hu3DParManKill
    entry _kerjmp_Hu3DParManAllKill
    b Hu3DParManAllKill
    entry _kerjmp_Hu3DParManPtrGet
    b Hu3DParManPtrGet
    entry _kerjmp_Hu3DParManAttrSet
    b Hu3DParManAttrSet
    entry _kerjmp_Hu3DParManAttrReset
    b Hu3DParManAttrReset
    entry _kerjmp_Hu3DParManPosSet
    b Hu3DParManPosSet
    entry _kerjmp_Hu3DParManVecSet
    b Hu3DParManVecSet
    entry _kerjmp_Hu3DParManRotSet
    b Hu3DParManRotSet
    entry _kerjmp_Hu3DParManModelIDGet
    b Hu3DParManModelIDGet
    entry _kerjmp_Hu3DParManLink
    b Hu3DParManLink
    entry _kerjmp_Hu3DParManTimeLimitSet
    b Hu3DParManTimeLimitSet
    entry _kerjmp_Hu3DParManColorSet
    b Hu3DParManColorSet
    entry _kerjmp_Hu3DParManLayerSet
    b Hu3DParManLayerSet
    entry _kerjmp_Hu3DWaterCreate
    b Hu3DWaterCreate
    entry _kerjmp_Hu3DWaterTexPosSet
    b Hu3DWaterTexPosSet
    entry _kerjmp_Hu3DWaterTexScaleSet
    b Hu3DWaterTexScaleSet
    entry _kerjmp_Hu3DWaterPadYSet
    b Hu3DWaterPadYSet
    entry _kerjmp_Hu3DWaterGlowSet
    b Hu3DWaterGlowSet
    entry _kerjmp_Hu3DWaterIndTexMtxSet
    b Hu3DWaterIndTexMtxSet
    entry _kerjmp_Hu3DWaterHiliteSet
    b Hu3DWaterHiliteSet
    entry _kerjmp_Hu3DWaterHiliteTPLvlSet
    b Hu3DWaterHiliteTPLvlSet
    entry _kerjmp_Hu3DWaterCameraSet
    b Hu3DWaterCameraSet
    entry _kerjmp_Hu3DWaterWaveCreate
    b Hu3DWaterWaveCreate
    entry _kerjmp_Hu3DMotionCreate
    b Hu3DMotionCreate
    entry _kerjmp_Hu3DMotionInit
    b Hu3DMotionInit
    entry _kerjmp_Hu3DMotionModelCreate
    b Hu3DMotionModelCreate
    entry _kerjmp_Hu3DMotionKill
    b Hu3DMotionKill
    entry _kerjmp_Hu3DMotionAllKill
    b Hu3DMotionAllKill
    entry _kerjmp_Hu3DMotionSet
    b Hu3DMotionSet
    entry _kerjmp_Hu3DMotionExec
    b Hu3DMotionExec
    entry _kerjmp_Hu3DSubMotionExec
    b Hu3DSubMotionExec
    entry _kerjmp_Hu3DMotionNext
    b Hu3DMotionNext
    entry _kerjmp_Hu3DMotionTimeSet
    b Hu3DMotionTimeSet
    entry _kerjmp_Hu3DMotionTimeGet
    b Hu3DMotionTimeGet
    entry _kerjmp_Hu3DMotionMaxTimeGet
    b Hu3DMotionMaxTimeGet
    entry _kerjmp_Hu3DMotionStartEndSet
    b Hu3DMotionStartEndSet
    entry _kerjmp_Hu3DMotionMotionMaxTimeGet
    b Hu3DMotionMotionMaxTimeGet
    entry _kerjmp_Hu3DMotionEndCheck
    b Hu3DMotionEndCheck
    entry _kerjmp_Hu3DMotionShiftSet
    b Hu3DMotionShiftSet
    entry _kerjmp_Hu3DMotionIDGet
    b Hu3DMotionIDGet
    entry _kerjmp_Hu3DMotionShiftIDGet
    b Hu3DMotionShiftIDGet
    entry _kerjmp_Hu3DMotionShiftTimeGet
    b Hu3DMotionShiftTimeGet
    entry _kerjmp_Hu3DMotionShiftMaxTimeGet
    b Hu3DMotionShiftMaxTimeGet
    entry _kerjmp_Hu3DMotionShiftStartEndSet
    b Hu3DMotionShiftStartEndSet
    entry _kerjmp_Hu3DMotionShiftSpeedSet
    b Hu3DMotionShiftSpeedSet
    entry _kerjmp_Hu3DMotionSpeedSet
    b Hu3DMotionSpeedSet
    entry _kerjmp_Hu3DMotionShapeSpeedSet
    b Hu3DMotionShapeSpeedSet
    entry _kerjmp_Hu3DMotionShapeTimeSet
    b Hu3DMotionShapeTimeSet
    entry _kerjmp_Hu3DMotionShapeTimeGet
    b Hu3DMotionShapeTimeGet
    entry _kerjmp_Hu3DMotionNoMotSet
    b Hu3DMotionNoMotSet
    entry _kerjmp_Hu3DMotionNoMotReset
    b Hu3DMotionNoMotReset
    entry _kerjmp_Hu3DMotionForceSet
    b Hu3DMotionForceSet
    entry _kerjmp_Hu3DMotionShapeSet
    b Hu3DMotionShapeSet
    entry _kerjmp_Hu3DMotionShapeIDGet
    b Hu3DMotionShapeIDGet
    entry _kerjmp_Hu3DMotionShapeMaxTimeGet
    b Hu3DMotionShapeMaxTimeGet
    entry _kerjmp_Hu3DMotionClusterSet
    b Hu3DMotionClusterSet
    entry _kerjmp_Hu3DMotionClusterNoSet
    b Hu3DMotionClusterNoSet
    entry _kerjmp_Hu3DMotionShapeReset
    b Hu3DMotionShapeReset
    entry _kerjmp_Hu3DMotionClusterReset
    b Hu3DMotionClusterReset
    entry _kerjmp_Hu3DJointMotion
    b Hu3DJointMotion
    entry _kerjmp_Hu3DMotionOverlaySet
    b Hu3DMotionOverlaySet
    entry _kerjmp_Hu3DMotionOverlayReset
    b Hu3DMotionOverlayReset
    entry _kerjmp_Hu3DMotionOverlayTimeGet
    b Hu3DMotionOverlayTimeGet
    entry _kerjmp_Hu3DMotionOverlayTimeSet
    b Hu3DMotionOverlayTimeSet
    entry _kerjmp_Hu3DMotionOverlaySpeedSet
    b Hu3DMotionOverlaySpeedSet
    entry _kerjmp_Hu3DMotionOverlayMaxTimeGet
    b Hu3DMotionOverlayMaxTimeGet
    entry _kerjmp_Hu3DMotionOverlayEndCheck
    b Hu3DMotionOverlayEndCheck
    entry _kerjmp_Hu3DModelObjDrawInit
    b Hu3DModelObjDrawInit
    entry _kerjmp_Hu3DModelObjDraw
    b Hu3DModelObjDraw
    entry _kerjmp_Hu3DModelObjPtrDraw
    b Hu3DModelObjPtrDraw
    entry _kerjmp_Hu3DMotionCalc
    b Hu3DMotionCalc
    entry _kerjmp_Hu3DMotionShapeStartEndSet
    b Hu3DMotionShapeStartEndSet
    entry _kerjmp_Hu3DSubMotionSet
    b Hu3DSubMotionSet
    entry _kerjmp_Hu3DSubMotionReset
    b Hu3DSubMotionReset
    entry _kerjmp_Hu3DSubMotionTimeSet
    b Hu3DSubMotionTimeSet
    entry _kerjmp_Hu3DMotionShiftTimeSet
    b Hu3DMotionShiftTimeSet
    entry _kerjmp_Hu3DMotionClusterSpeedSet
    b Hu3DMotionClusterSpeedSet
    entry _kerjmp_Hu3DMotionClusterTimeSet
    b Hu3DMotionClusterTimeSet
    entry _kerjmp_Hu3DMotionClusterMaxTimeGet
    b Hu3DMotionClusterMaxTimeGet
    entry _kerjmp_Hu3DMotionAttrSet
    b Hu3DMotionAttrSet
    entry _kerjmp_Hu3DMotionAttrReset
    b Hu3DMotionAttrReset
    entry _kerjmp_Hu3DMotionTimingHookSet
    b Hu3DMotionTimingHookSet
    entry _kerjmp_Hu3DMotionTimingHookReset
    b Hu3DMotionTimingHookReset
    entry _kerjmp_Hu3DMatLightSet
    b Hu3DMatLightSet
    entry _kerjmp_Hu3DFbCopyLayerSet
    b Hu3DFbCopyLayerSet
    entry _kerjmp_Hu3DFbCopyExec
    b Hu3DFbCopyExec
    entry _kerjmp_Hu3DZClear
    b Hu3DZClear
    entry _kerjmp_Hu3DZClearLayerSet
    b Hu3DZClearLayerSet
    entry _kerjmp_CamMotionEx
    b CamMotionEx
    entry _kerjmp_CamMotionEx2
    b CamMotionEx2
    entry _kerjmp_CamMotionExPathGet
    b CamMotionExPathGet
    entry _kerjmp_CamMotionExPath
    b CamMotionExPath
    entry _kerjmp_Hu3D2Dto3D
    b Hu3D2Dto3D
    entry _kerjmp_Hu3D3Dto2D
    b Hu3D3Dto2D
    entry _kerjmp_Hu3DMtxRotGet
    b Hu3DMtxRotGet
    entry _kerjmp_Hu3DMtxTransGet
    b Hu3DMtxTransGet
    entry _kerjmp_Hu3DMtxScaleGet
    b Hu3DMtxScaleGet
    entry _kerjmp_Hu3DProjectionCreate
    b Hu3DProjectionCreate
    entry _kerjmp_Hu3DProjectionPosSet
    b Hu3DProjectionPosSet
    entry _kerjmp_Hu3DProjectionTPLvlSet
    b Hu3DProjectionTPLvlSet
    entry _kerjmp_Hu3DProjectionKill
    b Hu3DProjectionKill
    entry _kerjmp_HuAudInit
    b HuAudInit
    entry _kerjmp_HuAudStreamPlay
    b HuAudStreamPlay
    entry _kerjmp_HuAudAllStop
    b HuAudAllStop
    entry _kerjmp_HuAudFadeOut
    b HuAudFadeOut
    entry _kerjmp_HuAudStreamPauseOn
    b HuAudStreamPauseOn
    entry _kerjmp_HuAudStreamPauseOff
    b HuAudStreamPauseOff
    entry _kerjmp_HuAudStreamFadeOut
    b HuAudStreamFadeOut
    entry _kerjmp_HuAudStreamVolSet
    b HuAudStreamVolSet
    entry _kerjmp_HuAudFXPlay
    b HuAudFXPlay
    entry _kerjmp_HuAudFXPlayVol
    b HuAudFXPlayVol
    entry _kerjmp_HuAudFXPlayVolPan
    b HuAudFXPlayVolPan
    entry _kerjmp_HuAudFXPlayPan
    b HuAudFXPlayPan
    entry _kerjmp_HuAudFXPlayDelay
    b HuAudFXPlayDelay
    entry _kerjmp_HuAudFXPlayDelayVol
    b HuAudFXPlayDelayVol
    entry _kerjmp_HuAudFXPlayDelayPan
    b HuAudFXPlayDelayPan
    entry _kerjmp_HuAudFXPlayDelayVolPan
    b HuAudFXPlayDelayVolPan
    entry _kerjmp_HuAudFXStop
    b HuAudFXStop
    entry _kerjmp_HuAudFXFadeOut
    b HuAudFXFadeOut
    entry _kerjmp_HuAudFXPanning
    b HuAudFXPanning
    entry _kerjmp_HuAudSeqPlay
    b HuAudSeqPlay
    entry _kerjmp_HuAudSeqStop
    b HuAudSeqStop
    entry _kerjmp_HuAudSeqFadeOut
    b HuAudSeqFadeOut
    entry _kerjmp_HuAudSeqMidiCtrlGet
    b HuAudSeqMidiCtrlGet
    entry _kerjmp_HuAudFXListnerSet
    b HuAudFXListnerSet
    entry _kerjmp_HuAudFXListnerUpdate
    b HuAudFXListnerUpdate
    entry _kerjmp_HuAudFXEmiterPlay
    b HuAudFXEmiterPlay
    entry _kerjmp_HuAudFXEmiterUpDate
    b HuAudFXEmiterUpDate
    entry _kerjmp_HuAudFXListnerKill
    b HuAudFXListnerKill
    entry _kerjmp_HuAudFXStatusGet
    b HuAudFXStatusGet
    entry _kerjmp_HuAudFXPitchSet
    b HuAudFXPitchSet
    entry _kerjmp_HuAudFXVolSet
    b HuAudFXVolSet
    entry _kerjmp_HuAudDllSndGrpSet
    b HuAudDllSndGrpSet
    entry _kerjmp_HuAudSndGrpSetSet
    b HuAudSndGrpSetSet
    entry _kerjmp_HuAudSndGrpSet
    b HuAudSndGrpSet
    entry _kerjmp_HuAudSndCommonGrpSet
    b HuAudSndCommonGrpSet
    entry _kerjmp_HuAudSStreamPlay
    b HuAudSStreamPlay
    entry _kerjmp_HuAudSStreamStop
    b HuAudSStreamStop
    entry _kerjmp_HuAudSStreamFadeOut
    b HuAudSStreamFadeOut
    entry _kerjmp_HuAudSStreamAllFadeOut
    b HuAudSStreamAllFadeOut
    entry _kerjmp_HuAudSStreamAllStop
    b HuAudSStreamAllStop
    entry _kerjmp_HuAudSStreamStatGet
    b HuAudSStreamStatGet
    entry _kerjmp_HuAudSStreamPause
    b HuAudSStreamPause
    entry _kerjmp_HuAudSStreamPauseFadeOut
    b HuAudSStreamPauseFadeOut
    entry _kerjmp_HuAudSStreamParamSet
    b HuAudSStreamParamSet
    entry _kerjmp_PlayerFXPlay
    b PlayerFXPlay
    entry _kerjmp_CharFXPlay
    b CharFXPlay
    entry _kerjmp_CharFXPlayVol
    b CharFXPlayVol
    entry _kerjmp_CharFXPlayPan
    b CharFXPlayPan
    entry _kerjmp_CharFXPlayVolPan
    b CharFXPlayVolPan
    entry _kerjmp_CharFXPlayDelayVolPan
    b CharFXPlayDelayVolPan
    entry _kerjmp_CharFXPlayDelay
    b CharFXPlayDelay
    entry _kerjmp_CharFXPlayDelayVol
    b CharFXPlayDelayVol
    entry _kerjmp_CharFXPlayDelayPan
    b CharFXPlayDelayPan
    entry _kerjmp_PlayerFXPlayPos
    b PlayerFXPlayPos
    entry _kerjmp_CharFXPlayPos
    b CharFXPlayPos
    entry _kerjmp_PlayerFXStop
    b PlayerFXStop
    entry _kerjmp_CharFXStop
    b CharFXStop
    entry _kerjmp_HuAudAUXSet
    b HuAudAUXSet
    entry _kerjmp_HuAudSStreamChanPlay
    b HuAudSStreamChanPlay
    entry _kerjmp_HuAudBGMPlay
    b HuAudBGMPlay
    entry _kerjmp_HuAudJinglePlay
    b HuAudJinglePlay
    entry _kerjmp_msmMusPlay
    b msmMusPlay
    entry _kerjmp_espInit
    b espInit
    entry _kerjmp_espEntry
    b espEntry
    entry _kerjmp_espKill
    b espKill
    entry _kerjmp_espGrpIDGet
    b espGrpIDGet
    entry _kerjmp_espDispOn
    b espDispOn
    entry _kerjmp_espDispOff
    b espDispOff
    entry _kerjmp_espAttrSet
    b espAttrSet
    entry _kerjmp_espAttrReset
    b espAttrReset
    entry _kerjmp_espPosSet
    b espPosSet
    entry _kerjmp_espScaleSet
    b espScaleSet
    entry _kerjmp_espZRotSet
    b espZRotSet
    entry _kerjmp_espTPLvlSet
    b espTPLvlSet
    entry _kerjmp_espColorSet
    b espColorSet
    entry _kerjmp_espSpeedSet
    b espSpeedSet
    entry _kerjmp_espBankSet
    b espBankSet
    entry _kerjmp_espDrawNoSet
    b espDrawNoSet
    entry _kerjmp_espPriSet
    b espPriSet
    entry _kerjmp_HuPrcInit
    b HuPrcInit
    entry _kerjmp_HuPrcCall
    b HuPrcCall
    entry _kerjmp_HuPrcCreate
    b HuPrcCreate
    entry _kerjmp_HuPrcKill
    b HuPrcKill
    entry _kerjmp_HuPrcEnd
    b HuPrcEnd
    entry _kerjmp_HuPrcWakeup
    b HuPrcWakeup
    entry _kerjmp_HuPrcSleep
    b HuPrcSleep
    entry _kerjmp_HuPrcVSleep
    b HuPrcVSleep
    entry _kerjmp_HuPrcCurrentGet
    b HuPrcCurrentGet
    entry _kerjmp_HuPrcDestructorSet
    b HuPrcDestructorSet
    entry _kerjmp_HuPrcDestructorSet2
    b HuPrcDestructorSet2
    entry _kerjmp_HuPrcChildCreate
    b HuPrcChildCreate
    entry _kerjmp_HuPrcChildLink
    b HuPrcChildLink
    entry _kerjmp_HuPrcChildUnlink
    b HuPrcChildUnlink
    entry _kerjmp_HuPrcChildKill
    b HuPrcChildKill
    entry _kerjmp_HuPrcMemAlloc
    b HuPrcMemAlloc
    entry _kerjmp_HuPrcMemFree
    b HuPrcMemFree
    entry _kerjmp_Hu3DParManVacumeSet
    b Hu3DParManVacumeSet
    entry _kerjmp_HuDataInit
    b HuDataInit
    entry _kerjmp_HuDataDirRead
    b HuDataDirRead
    entry _kerjmp_HuDataRead
    b HuDataRead
    entry _kerjmp_HuDataReadNum
    b HuDataReadNum
    entry _kerjmp_HuDataReadMulti
    b HuDataReadMulti
    entry _kerjmp_HuDataDirReadAsync
    b HuDataDirReadAsync
    entry _kerjmp_HuDataDirReadNumAsync
    b HuDataDirReadNumAsync
    entry _kerjmp_HuDataSelHeapRead
    b HuDataSelHeapRead
    entry _kerjmp_HuDataSelHeapReadNum
    b HuDataSelHeapReadNum
    entry _kerjmp_HuDataGetSize
    b HuDataGetSize
    entry _kerjmp_HuDataDirClose
    b HuDataDirClose
    entry _kerjmp_HuDataDirCloseNum
    b HuDataDirCloseNum
    entry _kerjmp_HuDataDirCloseAll
    b HuDataDirCloseAll
    entry _kerjmp_HuDataClose
    b HuDataClose
    entry _kerjmp_HuDataCloseMulti
    b HuDataCloseMulti
    entry _kerjmp_HuDataReadNumHeapShortForce
    b HuDataReadNumHeapShortForce
    entry _kerjmp_HuDataDirCancel
    b HuDataDirCancel
    entry _kerjmp_ARRegisterDMACallback
    b ARRegisterDMACallback
    entry _kerjmp_ARGetDMAStatus
    b ARGetDMAStatus
    entry _kerjmp_ARStartDMA
    b ARStartDMA
    entry _kerjmp_ARInit
    b ARInit
    entry _kerjmp_ARAlloc
    b ARAlloc
    entry _kerjmp_ARFree
    b ARFree
    entry _kerjmp_ARGetSize
    b ARGetSize
    entry _kerjmp_ARSetSize
    b ARSetSize
    entry _kerjmp_HuSprBegin
    b HuSprBegin
    entry _kerjmp_HuSprCall
    b HuSprCall
    entry _kerjmp_HuSprFinish
    b HuSprFinish
    entry _kerjmp_HuSprAnimRead
    b HuSprAnimRead
    entry _kerjmp_HuSprCreate
    b HuSprCreate
    entry _kerjmp_HuSprGrpCreate
    b HuSprGrpCreate
    entry _kerjmp_HuSprGrpMemberSet
    b HuSprGrpMemberSet
    entry _kerjmp_HuSprGrpMemberKill
    b HuSprGrpMemberKill
    entry _kerjmp_HuSprGrpKill
    b HuSprGrpKill
    entry _kerjmp_HuSprKill
    b HuSprKill
    entry _kerjmp_HuSprAnimKill
    b HuSprAnimKill
    entry _kerjmp_HuSprAttrSet
    b HuSprAttrSet
    entry _kerjmp_HuSprAttrReset
    b HuSprAttrReset
    entry _kerjmp_HuSprAttrGet
    b HuSprAttrGet
    entry _kerjmp_HuSprPosSet
    b HuSprPosSet
    entry _kerjmp_HuSprScaleSet
    b HuSprScaleSet
    entry _kerjmp_HuSprZRotSet
    b HuSprZRotSet
    entry _kerjmp_HuSprGrpPosSet
    b HuSprGrpPosSet
    entry _kerjmp_HuSprGrpZRotSet
    b HuSprGrpZRotSet
    entry _kerjmp_HuSprGrpScaleSet
    b HuSprGrpScaleSet
    entry _kerjmp_HuSprTPLvlSet
    b HuSprTPLvlSet
    entry _kerjmp_HuSprColorSet
    b HuSprColorSet
    entry _kerjmp_HuSprSpeedSet
    b HuSprSpeedSet
    entry _kerjmp_HuSprBankSet
    b HuSprBankSet
    entry _kerjmp_HuSprGrpDrawNoSet
    b HuSprGrpDrawNoSet
    entry _kerjmp_HuSprDrawNoSet
    b HuSprDrawNoSet
    entry _kerjmp_HuSprPriSet
    b HuSprPriSet
    entry _kerjmp_HuSprFuncCreate
    b HuSprFuncCreate
    entry _kerjmp_HuSprAnimMake
    b HuSprAnimMake
    entry _kerjmp_HuSprGrpCenterSet
    b HuSprGrpCenterSet
    entry _kerjmp_HuSprAnimLock
    b HuSprAnimLock
    entry _kerjmp_AnimDebug
    b AnimDebug
    entry _kerjmp_HuSprExecLayerSet
    b HuSprExecLayerSet
    entry _kerjmp_HuSprSprBGSet
    b HuSprSprBGSet
    entry _kerjmp_HuSprBGSet
    b HuSprBGSet
    entry _kerjmp_HuSprGrpScissorSet
    b HuSprGrpScissorSet
    entry _kerjmp_HuSprScissorSet
    b HuSprScissorSet
    entry _kerjmp_HuSprGrpTPLvlSet
    b HuSprGrpTPLvlSet
    entry _kerjmp_HuSprExecLayerCameraSet
    b HuSprExecLayerCameraSet
    entry _kerjmp_HuSprAnimNoSet
    b HuSprAnimNoSet
    entry _kerjmp_HuSprAnimNoSetPause
    b HuSprAnimNoSetPause
    entry _kerjmp_HuSpr3DHookSet
    b HuSpr3DHookSet
    entry _kerjmp_HuSpr3DDataCreate
    b HuSpr3DDataCreate
    entry _kerjmp_HuSpr3DSet
    b HuSpr3DSet
    entry _kerjmp_HuSpr3DRotSet
    b HuSpr3DRotSet
    entry _kerjmp_HuSpr3DFovSet
    b HuSpr3DFovSet
    entry _kerjmp_HuSprVtxColorSet
    b HuSprVtxColorSet
    entry _kerjmp_HuSprVtxColorReset
    b HuSprVtxColorReset
    entry _kerjmp_HuMemMemoryNumSet
    b HuMemMemoryNumSet
    entry _kerjmp_HuARMalloc
    b HuARMalloc
    entry _kerjmp_HuARFree
    b HuARFree
    entry _kerjmp_HuAR_DVDtoARAM
    b HuAR_DVDtoARAM
    entry _kerjmp_HuAR_MRAMtoARAM
    b HuAR_MRAMtoARAM
    entry _kerjmp_HuAR_MRAMtoARAM2
    b HuAR_MRAMtoARAM2
    entry _kerjmp_HuAR_ARAMtoMRAM
    b HuAR_ARAMtoMRAM
    entry _kerjmp_HuAR_ARAMtoMRAMNum
    b HuAR_ARAMtoMRAMNum
    entry _kerjmp_HuARDMACheck
    b HuARDMACheck
    entry _kerjmp_HuARDirCheck
    b HuARDirCheck
    entry _kerjmp_HuARDirFree
    b HuARDirFree
    entry _kerjmp_HuAR_ARAMtoMRAMFileRead
    b HuAR_ARAMtoMRAMFileRead
    entry _kerjmp_HuAudFXListnerSetEX
    b HuAudFXListnerSetEX
    entry _kerjmp_HuWindowInit
    b HuWindowInit
    entry _kerjmp_HuWinInit
    b HuWinInit
    entry _kerjmp_HuWinCreate
    b HuWinCreate
    entry _kerjmp_HuWinKill
    b HuWinKill
    entry _kerjmp_HuWinAllKill
    b HuWinAllKill
    entry _kerjmp_HuWinMesSet
    b HuWinMesSet
    entry _kerjmp_HuWinInsertMesSet
    b HuWinInsertMesSet
    entry _kerjmp_HuWinChoiceGet
    b HuWinChoiceGet
    entry _kerjmp_HuWinChoiceSet
    b HuWinChoiceSet
    entry _kerjmp_HuWinChoiceNumGet
    b HuWinChoiceNumGet
    entry _kerjmp_HuWinMesWait
    b HuWinMesWait
    entry _kerjmp_HuWinSprKill
    b HuWinSprKill
    entry _kerjmp_HuWinMesRead
    b HuWinMesRead
    entry _kerjmp_HuWinAnimSet
    b HuWinAnimSet
    entry _kerjmp_HuWinSprSet
    b HuWinSprSet
    entry _kerjmp_HuWinSprPosSet
    b HuWinSprPosSet
    entry _kerjmp_HuWinSprIDGet
    b HuWinSprIDGet
    entry _kerjmp_HuWinMesMaxSizeGet
    b HuWinMesMaxSizeGet
    entry _kerjmp_HuWinInsertMesSizeGet
    b HuWinInsertMesSizeGet
    entry _kerjmp_HuWinMesMaxSizeBetGet
    b HuWinMesMaxSizeBetGet
    entry _kerjmp_HuWinExCreate
    b HuWinExCreate
    entry _kerjmp_HuWinDispOn
    b HuWinDispOn
    entry _kerjmp_HuWinDispOff
    b HuWinDispOff
    entry _kerjmp_HuWinExOpen
    b HuWinExOpen
    entry _kerjmp_HuWinExClose
    b HuWinExClose
    entry _kerjmp_HuWinExKill
    b HuWinExKill
    entry _kerjmp_HuWinExSpeakerSet
    b HuWinExSpeakerSet
    entry _kerjmp_HuWinComKeyWait
    b HuWinComKeyWait
    entry _kerjmp_HuWinComKeySet
    b HuWinComKeySet
    entry _kerjmp_HuWinComKeyReset
    b HuWinComKeyReset
    entry _kerjmp_HuWinDrawNoSet
    b HuWinDrawNoSet
    entry _kerjmp_HuWinAttrSet
    b HuWinAttrSet
    entry _kerjmp_HuWinAttrReset
    b HuWinAttrReset
    entry _kerjmp_HuWinBGTPLvlSet
    b HuWinBGTPLvlSet
    entry _kerjmp_HuWinMesSpeedSet
    b HuWinMesSpeedSet
    entry _kerjmp_HuWinBGColSet
    b HuWinBGColSet
    entry _kerjmp_HuWinChoiceDisable
    b HuWinChoiceDisable
    entry _kerjmp_HuWinKeyWaitEntry
    b HuWinKeyWaitEntry
    entry _kerjmp_HuWinCenterPosSet
    b HuWinCenterPosSet
    entry _kerjmp_HuWinPosSet
    b HuWinPosSet
    entry _kerjmp_HuWinZRotSet
    b HuWinZRotSet
    entry _kerjmp_HuWinScaleSet
    b HuWinScaleSet
    entry _kerjmp_HuWinChoiceNowGet
    b HuWinChoiceNowGet
    entry _kerjmp_HuWinStatGet
    b HuWinStatGet
    entry _kerjmp_HuWinMesColSet
    b HuWinMesColSet
    entry _kerjmp_HuWinScissorSet
    b HuWinScissorSet
    entry _kerjmp_HuWinExCreateFrame
    b HuWinExCreateFrame
    entry _kerjmp_HuWinPriSet
    b HuWinPriSet
    entry _kerjmp_HuWinSprPriSet
    b HuWinSprPriSet
    entry _kerjmp_HuWinKeyWaitNumGet
    b HuWinKeyWaitNumGet
    entry _kerjmp_HuWinMesSizeCancelCRSet
    b HuWinMesSizeCancelCRSet
    entry _kerjmp_HuWinMesPalSet
    b HuWinMesPalSet
    entry _kerjmp_HuWinHomeClear
    b HuWinHomeClear
    entry _kerjmp_HuWinPushKeySet
    b HuWinPushKeySet
    entry _kerjmp_HuWinDisablePlayerSet
    b HuWinDisablePlayerSet
    entry _kerjmp_HuWinDisablePlayerReset
    b HuWinDisablePlayerReset
    entry _kerjmp_HuWinMesPtrGet
    b HuWinMesPtrGet
    entry _kerjmp_HuWinMesMaxDirGet
    b HuWinMesMaxDirGet
    entry _kerjmp_HuWinMesMaxNumGet
    b HuWinMesMaxNumGet
    entry _kerjmp_HuWinMesLanguageSet
    b HuWinMesLanguageSet
    entry _kerjmp_HuWinWarningCreate
    b HuWinWarningCreate
    entry _kerjmp_HuWinWarningKill
    b HuWinWarningKill
    entry _kerjmp_HuWinWarningOpen
    b HuWinWarningOpen
    entry _kerjmp_HuWinWarningClose
    b HuWinWarningClose
    entry _kerjmp_HuWinFrameSet
    b HuWinFrameSet
    entry _kerjmp_HuWinCallbackStub
    b HuWinCallbackStub
    entry _kerjmp_HuWinCallbackSet
    b HuWinCallbackSet
    entry _kerjmp_HuWinPadMaskSet
    b HuWinPadMaskSet
    entry _kerjmp_CharInit
    b CharInit
    entry _kerjmp_CharMotionInit
    b CharMotionInit
    entry _kerjmp_CharMotionInitAsync
    b CharMotionInitAsync
    entry _kerjmp_CharDataClose
    b CharDataClose
    entry _kerjmp_CharModelKill
    b CharModelKill
    entry _kerjmp_CharClose
    b CharClose
    entry _kerjmp_CharModelCreate
    b CharModelCreate
    entry _kerjmp_CharMotionCreate
    b CharMotionCreate
    entry _kerjmp_CharMotionNoSet
    b CharMotionNoSet
    entry _kerjmp_CharMotionKill
    b CharMotionKill
    entry _kerjmp_CharMotionDataClose
    b CharMotionDataClose
    entry _kerjmp_CharModelDataClose
    b CharModelDataClose
    entry _kerjmp_CharMotionAMemPGet
    b CharMotionAMemPGet
    entry _kerjmp_CharMotionSet
    b CharMotionSet
    entry _kerjmp_CharMotionTimeSet
    b CharMotionTimeSet
    entry _kerjmp_CharMotionTimeGet
    b CharMotionTimeGet
    entry _kerjmp_CharMotionMaxTimeGet
    b CharMotionMaxTimeGet
    entry _kerjmp_CharMotionEndCheck
    b CharMotionEndCheck
    entry _kerjmp_CharMotionShiftIDGet
    b CharMotionShiftIDGet
    entry _kerjmp_CharMotionShiftSet
    b CharMotionShiftSet
    entry _kerjmp_CharMotionShiftTimeGet
    b CharMotionShiftTimeGet
    entry _kerjmp_CharMotionSpeedSet
    b CharMotionSpeedSet
    entry _kerjmp_CharModelHookDustCreate
    b CharModelHookDustCreate
    entry _kerjmp_CharEffectSmokeCreate
    b CharEffectSmokeCreate
    entry _kerjmp_CharEffectSmokeCreateScale
    b CharEffectSmokeCreateScale
    entry _kerjmp_CharEffectCoinGlowCreate
    b CharEffectCoinGlowCreate
    entry _kerjmp_CharModelHitCreate
    b CharModelHitCreate
    entry _kerjmp_CharEffectHitCreate
    b CharEffectHitCreate
    entry _kerjmp_CharEffectShoeHitCreate
    b CharEffectShoeHitCreate
    entry _kerjmp_CharModelShoeHitCreate
    b CharModelShoeHitCreate
    entry _kerjmp_GWCharColorGet
    b GWCharColorGet
    entry _kerjmp_CharEffectLayerSet
    b CharEffectLayerSet
    entry _kerjmp_CharEyeBmpNameTbl
    b CharEyeBmpNameTbl
    entry _kerjmp_CharModelEyeBmpGet
    b CharModelEyeBmpGet
    entry _kerjmp_CharMotionVoiceOnSet
    b CharMotionVoiceOnSet
    entry _kerjmp_CharNpcDustVoiceOffSet
    b CharNpcDustVoiceOffSet
    entry _kerjmp_CharModelStepSet
    b CharModelStepSet
    entry _kerjmp_CharModelVoicePanAutoSet
    b CharModelVoicePanAutoSet
    entry _kerjmp_CharModelVoiceFlagSet
    b CharModelVoiceFlagSet
    entry _kerjmp_CharModelLandDustCreate
    b CharModelLandDustCreate
    entry _kerjmp_CharModelAttrSet
    b CharModelAttrSet
    entry _kerjmp_CharModelAttrReset
    b CharModelAttrReset
    entry _kerjmp_CharMotionStartEndSet
    b CharMotionStartEndSet
    entry _kerjmp_CharModelHeightGet
    b CharModelHeightGet
    entry _kerjmp_CharModelMotListCreate
    b CharModelMotListCreate
    entry _kerjmp_CharEffectDustCreate
    b CharEffectDustCreate
    entry _kerjmp_CharEffectCryCreate
    b CharEffectCryCreate
    entry _kerjmp_CharModelCryCreate
    b CharModelCryCreate
    entry _kerjmp_CharModelTimingHookNoGet
    b CharModelTimingHookNoGet
    entry _kerjmp_CharMotionUpdateSet
    b CharMotionUpdateSet
    entry _kerjmp_CharModelVoicePanSet
    b CharModelVoicePanSet
    entry _kerjmp_CharModelVoiceVolSet
    b CharModelVoiceVolSet
    entry _kerjmp_CharWinLoseVoicePlay
    b CharWinLoseVoicePlay
    entry _kerjmp_CharLoseVoicePlay
    b CharLoseVoicePlay
    entry _kerjmp_CharMotionTotalTimeGet
    b CharMotionTotalTimeGet
    entry _kerjmp_CharMotionExtraTimeGet
    b CharMotionExtraTimeGet
    entry _kerjmp_CharLightCreate
    b CharLightCreate
    entry _kerjmp_CharLightCreateV
    b CharLightCreateV
    entry _kerjmp_CharLightSpotSet
    b CharLightSpotSet
    entry _kerjmp_CharLightInfinitytSet
    b CharLightInfinitytSet
    entry _kerjmp_CharLightPointSet
    b CharLightPointSet
    entry _kerjmp_CharLightColorSet
    b CharLightColorSet
    entry _kerjmp_CharLightPosAimSetV
    b CharLightPosAimSetV
    entry _kerjmp_CharLightPosAimSet
    b CharLightPosAimSet
    entry _kerjmp_CharLightStaticSet
    b CharLightStaticSet
    entry _kerjmp_CharEffectWarnCreate
    b CharEffectWarnCreate
    entry _kerjmp_CharMotionLoad
    b CharMotionLoad
    entry _kerjmp_CharNpcDustSet
    b CharNpcDustSet
    entry _kerjmp_HuPrcSetStat
    b HuPrcSetStat
    entry _kerjmp_HuPrcResetStat
    b HuPrcResetStat
    entry _kerjmp_omGameSysInit
    b omGameSysInit
    entry _kerjmp_omResetStatBit
    b omResetStatBit
    entry _kerjmp_omCurrentOvlGet
    b omCurrentOvlGet
    entry _kerjmp_omPauseChk
    b omPauseChk
    entry _kerjmp_omOvlHisChg
    b omOvlHisChg
    entry _kerjmp_omOvlHisGet
    b omOvlHisGet
    entry _kerjmp_omovlevtno
    b omovlevtno
    entry _kerjmp_omovlstat
    b omovlstat
    entry _kerjmp_AddX
    b AddX
    entry _kerjmp_AddZ
    b AddZ
    entry _kerjmp_MapObject
    b MapObject
    entry _kerjmp_CharObject
    b CharObject
    entry _kerjmp_nMap
    b nMap
    entry _kerjmp_nChar
    b nChar
    entry _kerjmp_MapWall
    b MapWall
    entry _kerjmp_MapPos
    b MapPos
    entry _kerjmp_HitFaceVec
    b HitFaceVec
    entry _kerjmp_HitFace
    b HitFace
    entry _kerjmp_HitFaceCount
    b HitFaceCount
    entry _kerjmp_GwPlayer
    b GwPlayer
    entry _kerjmp_GwSystem
    b GwSystem
    entry _kerjmp_GWSinglePrizeFlagSet
    b GWSinglePrizeFlagSet
    entry _kerjmp_GWSinglePrizeFlagGet
    b GWSinglePrizeFlagGet
    entry _kerjmp_GWSingleDataInit
    b GWSingleDataInit
    entry _kerjmp_GWSinglePrizeSaveFlagSet
    b GWSinglePrizeSaveFlagSet
    entry _kerjmp_GWSinglePrizeSaveFlagGet
    b GWSinglePrizeSaveFlagGet
    entry _kerjmp_GWBankStarAdd
    b GWBankStarAdd
    entry _kerjmp_GWBankStarSub
    b GWBankStarSub
    entry _kerjmp_GWBankStarGet
    b GWBankStarGet
    entry _kerjmp_GWBankStarAwardGet
    b GWBankStarAwardGet
    entry _kerjmp_GWBankStarAwardReset
    b GWBankStarAwardReset
    entry _kerjmp_GWBankFlagSet
    b GWBankFlagSet
    entry _kerjmp_GWBankFlagReset
    b GWBankFlagReset
    entry _kerjmp_GWBankFlagGet
    b GWBankFlagGet
    entry _kerjmp_GWSingleMgFlagGet
    b GWSingleMgFlagGet
    entry _kerjmp_GWSingleMgWinNumGet
    b GWSingleMgWinNumGet
    entry _kerjmp_GWSingleMgWinNumSet
    b GWSingleMgWinNumSet
    entry _kerjmp_GWSingleMgRecordNumGet
    b GWSingleMgRecordNumGet
    entry _kerjmp_GWSingleMgRecordNumSet
    b GWSingleMgRecordNumSet
    entry _kerjmp_HuTHPSprCreateVol
    b HuTHPSprCreateVol
    entry _kerjmp_HuTHPSprCreate
    b HuTHPSprCreate
    entry _kerjmp_HuTHP3DCreateVol
    b HuTHP3DCreateVol
    entry _kerjmp_HuTHP3DCreate
    b HuTHP3DCreate
    entry _kerjmp_HuTHPStop
    b HuTHPStop
    entry _kerjmp_HuTHPClose
    b HuTHPClose
    entry _kerjmp_HuTHPRestart
    b HuTHPRestart
    entry _kerjmp_HuTHPEndCheck
    b HuTHPEndCheck
    entry _kerjmp_HuTHPFrameGet
    b HuTHPFrameGet
    entry _kerjmp_HuTHPTotalFrameGet
    b HuTHPTotalFrameGet
    entry _kerjmp_HuTHPSetVolume
    b HuTHPSetVolume
    entry _kerjmp_THPSimpleGetFrameRate
    b THPSimpleGetFrameRate
    entry _kerjmp_HuTHPMatHook
    b HuTHPMatHook
    entry _kerjmp_HuTHPStartCheck
    b HuTHPStartCheck
    entry _kerjmp_HuCardCheck
    b HuCardCheck
    entry _kerjmp_HuCardMount
    b HuCardMount
    entry _kerjmp_HuCardFormat
    b HuCardFormat
    entry _kerjmp_HuCardOpen
    b HuCardOpen
    entry _kerjmp_HuCardRead
    b HuCardRead
    entry _kerjmp_HuCardCreate
    b HuCardCreate
    entry _kerjmp_HuCardClose
    b HuCardClose
    entry _kerjmp_HuCardWrite
    b HuCardWrite
    entry _kerjmp_HuCardDelete
    b HuCardDelete
    entry _kerjmp_HuCardSectorSizeGet
    b HuCardSectorSizeGet
    entry _kerjmp_HuCardFreeSpaceGet
    b HuCardFreeSpaceGet
    entry _kerjmp_HuCardSlotCheck
    b HuCardSlotCheck
    entry _kerjmp_HuCardWriteAsync
    b HuCardWriteAsync
    entry _kerjmp_HuCardWriteCheck
    b HuCardWriteCheck
    entry _kerjmp_HuCardWriteIdle
    b HuCardWriteIdle
    entry _kerjmp_SLFileOpen
    b SLFileOpen
    entry _kerjmp_SLFileCreate
    b SLFileCreate
    entry _kerjmp_SLFileWrite
    b SLFileWrite
    entry _kerjmp_SLFileClose
    b SLFileClose
    entry _kerjmp_SLFileRead
    b SLFileRead
    entry _kerjmp_SLSaveEmptySet
    b SLSaveEmptySet
    entry _kerjmp_SLCurSlotNoSet
    b SLCurSlotNoSet
    entry _kerjmp_SLCurSlotNoGet
    b SLCurSlotNoGet
    entry _kerjmp_SLSaveDataMake
    b SLSaveDataMake
    entry _kerjmp_SLSaveDataSlotMake
    b SLSaveDataSlotMake
    entry _kerjmp_SLCommonSet
    b SLCommonSet
    entry _kerjmp_SLBoardSave
    b SLBoardSave
    entry _kerjmp_SLSave
    b SLSave
    entry _kerjmp_SLCommonLoad
    b SLCommonLoad
    entry _kerjmp_SLBoardLoad
    b SLBoardLoad
    entry _kerjmp_SLLoad
    b SLLoad
    entry _kerjmp_SLCurBoxNoSet
    b SLCurBoxNoSet
    entry _kerjmp_SLCurBoxNoGet
    b SLCurBoxNoGet
    entry _kerjmp_SLSaveFlagSet
    b SLSaveFlagSet
    entry _kerjmp_SLSaveFlagGet
    b SLSaveFlagGet
    entry _kerjmp_SLCheckSumCheck
    b SLCheckSumCheck
    entry _kerjmp_SLCheckSumBoxSlotCheck
    b SLCheckSumBoxSlotCheck
    entry _kerjmp_SLCheckSumGet
    b SLCheckSumGet
    entry _kerjmp_SLCheckSumSlotGet
    b SLCheckSumSlotGet
    entry _kerjmp_SLSerialNoGet
    b SLSerialNoGet
    entry _kerjmp_SLSerialNoCheck
    b SLSerialNoCheck
    entry _kerjmp_saveExecF
    b saveExecF
    entry _kerjmp_SLWinIdSet
    b SLWinIdSet
    entry _kerjmp_SLBoxBackupLoad
    b SLBoxBackupLoad
    entry _kerjmp_SLBoxBackupSlotLoad
    b SLBoxBackupSlotLoad
    entry _kerjmp_SLCheckSumBoxSet
    b SLCheckSumBoxSet
    entry _kerjmp_SLBoxDataOffsetGet
    b SLBoxDataOffsetGet
    entry _kerjmp_SLSaveBoardTurnExec
    b SLSaveBoardTurnExec
    entry _kerjmp_SLSaveBoardEndExec
    b SLSaveBoardEndExec
    entry _kerjmp_SLSaveModeExec
    b SLSaveModeExec
    entry _kerjmp_curSlotNo
    b curSlotNo
    entry _kerjmp_UnMountCnt
    b UnMountCnt
    entry _kerjmp_SLCommonSaveCopy
    b SLCommonSaveCopy
    entry _kerjmp_SLCommonLoadCopy
    b SLCommonLoadCopy
    entry _kerjmp__CheckFlag
    b _CheckFlag
    entry _kerjmp__SetFlag
    b _SetFlag
    entry _kerjmp__ClearFlag
    b _ClearFlag
    entry _kerjmp_MgDataTbl
    b MgDataTbl
    entry _kerjmp_MgModeSubMode
    b MgModeSubMode
    entry _kerjmp_MgModeScore
    b MgModeScore
    entry _kerjmp_MgModePlayNum_0
    b MgModePlayNum
    entry _kerjmp_MgModeWork
    b MgModeWork
    entry _kerjmp_MgModePlayNum_1
    b MgModePlayNum
    entry _kerjmp_GwCommonBackup
    b GwCommonBackup
    entry _kerjmp_lbl_802C0438
    b lbl_802C0438
    entry _kerjmp_MgExitReq
    b MgExitReq
    entry _kerjmp_MgPauseExitF
    b MgPauseExitF
    entry _kerjmp_MgBoard2Force
    b MgBoard2Force
    entry _kerjmp_MgNoGet
    b MgNoGet
    entry _kerjmp_MiracleBookEvtNo
    b MiracleBookEvtNo
    entry _kerjmp_MgSubMode
    b MgSubMode
    entry _kerjmp_MgSubModeGet
    b MgSubModeGet
    entry _kerjmp_MgBattleOrder
    b MgBattleOrder
    entry _kerjmp_MgDecaScoreCalc
    b MgDecaScoreCalc
    entry _kerjmp_MgInstExitF
    b MgInstExitF
    entry _kerjmp_GWRecordSet
    b GWRecordSet
    entry _kerjmp_GWRecordGet
    b GWRecordGet
    entry _kerjmp_GWBoardPlayNumAdd
    b GWBoardPlayNumAdd
    entry _kerjmp_GWBoardPlayNumSet
    b GWBoardPlayNumSet
    entry _kerjmp_GWBoardPlayNumGet
    b GWBoardPlayNumGet
    entry _kerjmp_GWCharPlayNumInc
    b GWCharPlayNumInc
    entry _kerjmp_GWCharPlayNumGet
    b GWCharPlayNumGet
    entry _kerjmp_GWCharPlayNumSet
    b GWCharPlayNumSet
    entry _kerjmp_GWBoardMaxStarSet
    b GWBoardMaxStarSet
    entry _kerjmp_GWBoardMaxStarGet
    b GWBoardMaxStarGet
    entry _kerjmp_GWBoardMaxCoinSet
    b GWBoardMaxCoinSet
    entry _kerjmp_GWBoardMaxCoinGet
    b GWBoardMaxCoinGet
    entry _kerjmp_GWPlayerStarAdd
    b GWPlayerStarAdd
    entry _kerjmp_GWPlayerStarGet
    b GWPlayerStarGet
    entry _kerjmp_GWPlayerStarSet
    b GWPlayerStarSet
    entry _kerjmp_GWPlayerCoinAdd
    b GWPlayerCoinAdd
    entry _kerjmp_GWPlayerCoinSet
    b GWPlayerCoinSet
    entry _kerjmp_GWPlayerCoinGet
    b GWPlayerCoinGet
    entry _kerjmp_GWMgCustomReset
    b GWMgCustomReset
    entry _kerjmp_GWMgCustomSet
    b GWMgCustomSet
    entry _kerjmp_GWMgCustomGet
    b GWMgCustomGet
    entry _kerjmp_GWMgUnlockGet
    b GWMgUnlockGet
    entry _kerjmp_GWMgPlayNumSet
    b GWMgPlayNumSet
    entry _kerjmp_GWMgPlayNumGet
    b GWMgPlayNumGet
    entry _kerjmp_GWMgPlayNumAdd
    b GWMgPlayNumAdd
    entry _kerjmp_GWMiracleBookFlagSet
    b GWMiracleBookFlagSet
    entry _kerjmp_GWMiracleBookFlagGet
    b GWMiracleBookFlagGet
    entry _kerjmp_GWMikeActRecordSet
    b GWMikeActRecordSet
    entry _kerjmp_GWMikeActRecordGet
    b GWMikeActRecordGet
    entry _kerjmp_GWSaveDebugSet
    b GWSaveDebugSet
    entry _kerjmp_MgModeWorkInt
    b MgModeWorkInt
    entry _kerjmp_MgModeWorkFloat
    b MgModeWorkFloat
    entry _kerjmp_GwMgDecaScore
    b GwMgDecaScore
    entry _kerjmp_GwLanguage
    b GwLanguage
    entry _kerjmp_fmod
    b fmod
    entry _kerjmp__savegpr_15
    b _savegpr_15
    entry _kerjmp__restgpr_15
    b _restgpr_15
    entry _kerjmp__savegpr_16
    b _savegpr_16
    entry _kerjmp__restgpr_16
    b _restgpr_16
    entry _kerjmp__savegpr_14
    b _savegpr_14
    entry _kerjmp__restgpr_14
    b _restgpr_14
    entry _kerjmp_omDBGMenuButton
    b omDBGMenuButton
    entry _kerjmp_omDBGSystemKeyCheckSetup
    b omDBGSystemKeyCheckSetup
    entry _kerjmp_msmSeSetListener
    b msmSeSetListener
    entry _kerjmp_msmSeUpdataListener
    b msmSeUpdataListener
    entry _kerjmp_msmSeGetIndexPtr
    b msmSeGetIndexPtr
    entry _kerjmp_msmMusSetMasterVolume
    b msmMusSetMasterVolume
    entry _kerjmp_msmSysGetOutputMode
    b msmSysGetOutputMode
    entry _kerjmp_msmSysSetGroupLoadMode
    b msmSysSetGroupLoadMode
    entry _kerjmp_WipeCreate
    b WipeCreate
    entry _kerjmp_WipeColorSet
    b WipeColorSet
    entry _kerjmp_omOvlReturnEx
    b omOvlReturnEx
    entry _kerjmp_HuDataGetAsyncStat
    b HuDataGetAsyncStat
    entry _kerjmp_omVibrate
    b omVibrate
    entry _kerjmp_GameMesOvlPrev
    b GameMesOvlPrev
    entry _kerjmp_MgTimerCreate
    b MgTimerCreate
    entry _kerjmp_MgTimerKill
    b MgTimerKill
    entry _kerjmp_MgTimerModeGet
    b MgTimerModeGet
    entry _kerjmp_MgTimerModeOnSet
    b MgTimerModeOnSet
    entry _kerjmp_MgTimerModeOffSet
    b MgTimerModeOffSet
    entry _kerjmp_MgTimerParamSet
    b MgTimerParamSet
    entry _kerjmp_MgTimerValueGet
    b MgTimerValueGet
    entry _kerjmp_MgTimerDoneCheck
    b MgTimerDoneCheck
    entry _kerjmp_MgTimerPosSet
    b MgTimerPosSet
    entry _kerjmp_MgTimerPosGet
    b MgTimerPosGet
    entry _kerjmp_MgTimerBackColorSet
    b MgTimerBackColorSet
    entry _kerjmp_MgTimerColorSet
    b MgTimerColorSet
    entry _kerjmp_MgTimerRecordSet
    b MgTimerRecordSet
    entry _kerjmp_MgTimerRecordDispOn
    b MgTimerRecordDispOn
    entry _kerjmp_MgTimerRecordDispOff
    b MgTimerRecordDispOff
    entry _kerjmp_MgScoreInit
    b MgScoreInit
    entry _kerjmp_MgScoreKill
    b MgScoreKill
    entry _kerjmp_MgScoreModeGet
    b MgScoreModeGet
    entry _kerjmp_MgScoreValueSet
    b MgScoreValueSet
    entry _kerjmp_MgScoreValueGet
    b MgScoreValueGet
    entry _kerjmp_MgScoreMaxDigitSet
    b MgScoreMaxDigitSet
    entry _kerjmp_MgScoreMaxDigitGet
    b MgScoreMaxDigitGet
    entry _kerjmp_MgScorePosSet
    b MgScorePosSet
    entry _kerjmp_MgScorePosGet
    b MgScorePosGet
    entry _kerjmp_MgScoreScaleSet
    b MgScoreScaleSet
    entry _kerjmp_MgScoreScaleGet
    b MgScoreScaleGet
    entry _kerjmp_MgScoreZRotSet
    b MgScoreZRotSet
    entry _kerjmp_MgScoreZRotGet
    b MgScoreZRotGet
    entry _kerjmp_MgScoreDigitWidthSet
    b MgScoreDigitWidthSet
    entry _kerjmp_MgScoreDigitWidthGet
    b MgScoreDigitWidthGet
    entry _kerjmp_MgScoreDispOn
    b MgScoreDispOn
    entry _kerjmp_MgScoreDispOff
    b MgScoreDispOff
    entry _kerjmp_MgScoreTPLvlSet
    b MgScoreTPLvlSet
    entry _kerjmp_MgScoreTPLvlGet
    b MgScoreTPLvlGet
    entry _kerjmp_MgScoreModeDefaultSet
    b MgScoreModeDefaultSet
    entry _kerjmp_MgScoreCreate
    b MgScoreCreate
    entry _kerjmp_MgScoreColorSet
    b MgScoreColorSet
    entry _kerjmp_MgScoreDigitScaleSet
    b MgScoreDigitScaleSet
    entry _kerjmp_MgScorePriSet
    b MgScorePriSet
    entry _kerjmp_MgScoreUnitBankSet
    b MgScoreUnitBankSet
    entry _kerjmp_MgSeqCreate
    b MgSeqCreate
    entry _kerjmp_MgSeqCreatePrio
    b MgSeqCreatePrio
    entry _kerjmp_MgSeqKill
    b MgSeqKill
    entry _kerjmp_MgSeqModeGet
    b MgSeqModeGet
    entry _kerjmp_MgSeqModeNext
    b MgSeqModeNext
    entry _kerjmp_MgSeqModeSet
    b MgSeqModeSet
    entry _kerjmp_MgSeqRecordSet
    b MgSeqRecordSet
    entry _kerjmp_MgSeqModeChangeOff
    b MgSeqModeChangeOff
    entry _kerjmp_MgSeqModeChangeOn
    b MgSeqModeChangeOn
    entry _kerjmp_MgSeqStatGet
    b MgSeqStatGet
    entry _kerjmp_MgSeqWinnerSet
    b MgSeqWinnerSet
    entry _kerjmp_MgSeqDrawSet
    b MgSeqDrawSet
    entry _kerjmp_fn_80071CCC
    b fn_80071CCC
    entry _kerjmp_MgSeqModeDelaySet
    b MgSeqModeDelaySet
    entry _kerjmp_MgSeqModeHookAdd
    b MgSeqModeHookAdd
    entry _kerjmp_MgSeqModeHookReset
    b MgSeqModeHookReset
    entry _kerjmp_MgSeqTimerValueGet
    b MgSeqTimerValueGet
    entry _kerjmp_MgSeqFrameNoGet
    b MgSeqFrameNoGet
    entry _kerjmp_MgSeqFrameNoSet
    b MgSeqFrameNoSet
    entry _kerjmp_MgSeqGameMesIdGet
    b MgSeqGameMesIdGet
    entry _kerjmp_MgSeqTimerKill
    b MgSeqTimerKill
    entry _kerjmp_MgSeqStatBitSet
    b MgSeqStatBitSet
    entry _kerjmp_MgSeqStatBitReset
    b MgSeqStatBitReset
    entry _kerjmp_omCameraViewInit
    b omCameraViewInit
    entry _kerjmp_omCameraViewSetMulti
    b omCameraViewSetMulti
    entry _kerjmp_omCameraViewSet
    b omCameraViewSet
    entry _kerjmp_omCameraViewMoveSimpleMulti
    b omCameraViewMoveSimpleMulti
    entry _kerjmp_omCameraViewMoveSimple
    b omCameraViewMoveSimple
    entry _kerjmp_omCameraViewMoveMulti
    b omCameraViewMoveMulti
    entry _kerjmp_omCameraViewMove
    b omCameraViewMove
    entry _kerjmp_omCameraViewCheck
    b omCameraViewCheck
    entry _kerjmp_MgActorObjectSetup
    b MgActorObjectSetup
    entry _kerjmp_MgActorColMapInit
    b MgActorColMapInit
    entry _kerjmp_MgPlayerPadSet
    b MgPlayerPadSet
    entry _kerjmp_MgPlayerModeFuncSet
    b MgPlayerModeFuncSet
    entry _kerjmp_MgPlayerCreate
    b MgPlayerCreate
    entry _kerjmp_MgPlayerCreateJumpAlt
    b MgPlayerCreateJumpAlt
    entry _kerjmp_MgActorCreate
    b MgActorCreate
    entry _kerjmp_MgActorKill
    b MgActorKill
    entry _kerjmp_MgActorExec
    b MgActorExec
    entry _kerjmp_MgPlayerModeLandSet
    b MgPlayerModeLandSet
    entry _kerjmp_MgPlayerModeSet
    b MgPlayerModeSet
    entry _kerjmp_MgPlayerComStkOn
    b MgPlayerComStkOn
    entry _kerjmp_MgPlayerComStkOff
    b MgPlayerComStkOff
    entry _kerjmp_MgPlayerVecChase
    b MgPlayerVecChase
    entry _kerjmp_MgActorVecChase
    b MgActorVecChase
    entry _kerjmp_MgActorColAttrSet
    b MgActorColAttrSet
    entry _kerjmp_MgActorColAttrReset
    b MgActorColAttrReset
    entry _kerjmp_MgActorColAttrGet
    b MgActorColAttrGet
    entry _kerjmp_MgActorColMapPolyGet
    b MgActorColMapPolyGet
    entry _kerjmp_MgActorPosSet
    b MgActorPosSet
    entry _kerjmp_MgActorPosSetRaw
    b MgActorPosSetRaw
    entry _kerjmp_MgActorPosGet
    b MgActorPosGet
    entry _kerjmp_MgPlayerDespawn
    b MgPlayerDespawn
    entry _kerjmp_MgPlayerSpawn
    b MgPlayerSpawn
    entry _kerjmp_MgPlayerColDisable
    b MgPlayerColDisable
    entry _kerjmp_MgPlayerColEnable
    b MgPlayerColEnable
    entry _kerjmp_MgPlayerVibrateCreate
    b MgPlayerVibrateCreate
    entry _kerjmp_MgPlayerVibrateKill
    b MgPlayerVibrateKill
    entry _kerjmp_MgActorColAttrParamSet
    b MgActorColAttrParamSet
    entry _kerjmp_MgActorColAttrParamGet
    b MgActorColAttrParamGet
    entry _kerjmp_MgActorColMapMaskSet
    b MgActorColMapMaskSet
    entry _kerjmp_MgActorColMapMaskGet
    b MgActorColMapMaskGet
    entry _kerjmp_MgActorColMaskSet
    b MgActorColMaskSet
    entry _kerjmp_MgActorColMaskGet
    b MgActorColMaskGet
    entry _kerjmp_MgPlayerAttrCheck
    b MgPlayerAttrCheck
    entry _kerjmp_MgPlayerAttrSet
    b MgPlayerAttrSet
    entry _kerjmp_MgPlayerAttrReset
    b MgPlayerAttrReset
    entry _kerjmp_MgPlayerModeAttrCheck
    b MgPlayerModeAttrCheck
    entry _kerjmp_MgPlayerModeAttrReset
    b MgPlayerModeAttrReset
    entry _kerjmp_MgPlayerModeAttrSet
    b MgPlayerModeAttrSet
    entry _kerjmp_MgPlayerVibAttrCheck
    b MgPlayerVibAttrCheck
    entry _kerjmp_MgPlayerVibAttrSet
    b MgPlayerVibAttrSet
    entry _kerjmp_MgPlayerStunSet
    b MgPlayerStunSet
    entry _kerjmp_MgActorColCylReset
    b MgActorColCylReset
    entry _kerjmp_MgActorColCylSet
    b MgActorColCylSet
    entry _kerjmp_MgActorColCodeGet
    b MgActorColCodeGet
    entry _kerjmp_MgActorColNormalGet
    b MgActorColNormalGet
    entry _kerjmp_MgActorRotYSet
    b MgActorRotYSet
    entry _kerjmp_MgActorRotYGet
    b MgActorRotYGet
    entry _kerjmp_MgActorPushSet
    b MgActorPushSet
    entry _kerjmp_MgPlayerPosSet
    b MgPlayerPosSet
    entry _kerjmp_MgActorGravitySet
    b MgActorGravitySet
    entry _kerjmp_MgActorVelYSet
    b MgActorVelYSet
    entry _kerjmp_MgActorColMeshGet
    b MgActorColMeshGet
    entry _kerjmp_MgActorVelSet
    b MgActorVelSet
    entry _kerjmp_MgActorColBounceSet
    b MgActorColBounceSet
    entry _kerjmp_CharModelFileNumGet
    b CharModelFileNumGet
    entry _kerjmp_CharMotionFileNumGet
    b CharMotionFileNumGet
    entry _kerjmp_lbl_802BF860
    b lbl_802BF860
    entry _kerjmp_lbl_802BF864
    b lbl_802BF864
    entry _kerjmp_lbl_802BF868
    b lbl_802BF868
    entry _kerjmp__savefpr_22
    b _savefpr_22
    entry _kerjmp__restfpr_22
    b _restfpr_22
    entry _kerjmp__savefpr_24
    b _savefpr_24
    entry _kerjmp__restfpr_24
    b _restfpr_24
    entry _kerjmp__savefpr_27
    b _savefpr_27
    entry _kerjmp__restfpr_27
    b _restfpr_27
    entry _kerjmp__savefpr_28
    b _savefpr_28
    entry _kerjmp__restfpr_28
    b _restfpr_28
    entry _kerjmp___construct_array
    b __construct_array
    entry _kerjmp___destroy_arr
    b __destroy_arr
    entry _kerjmp___msl_assertion_failed
    b __msl_assertion_failed
    entry _kerjmp___nw__FUl
    b __nw__FUl
    entry _kerjmp___nwa__FUl
    b __nwa__FUl
    entry _kerjmp___ptmf_test
    b __ptmf_test
    entry _kerjmp___ptmf_scall
    b __ptmf_scall
    entry _kerjmp___dla__FPv
    b __dla__FPv
    entry _kerjmp_lbl_802166D8
    b lbl_802166D8
    entry _kerjmp___ptmf_cmpr
    b __ptmf_cmpr
    entry _kerjmp_HuDataDirCancelAsync
    b HuDataDirCancelAsync
    entry _kerjmp_HuDataDirCloseAsync
    b HuDataDirCloseAsync
    entry _kerjmp_HuTHPProcCheck
    b HuTHPProcCheck
    entry _kerjmp_mbSaveInit
    b mbSaveInit
    entry _kerjmp_mbSavePartyInit
    b mbSavePartyInit
    entry _kerjmp_mbSaveStoryInit
    b mbSaveStoryInit
    entry _kerjmp_fn_80146BA0
    b fn_80146BA0
    entry _kerjmp_fn_80146C88
    b fn_80146C88
    entry _kerjmp_HuLoadProcStart
    b HuLoadProcStart
    entry _kerjmp_GwMgNightF
    b GwMgNightF
    entry _kerjmp_HuMCSysInit
    b HuMCSysInit
    entry _kerjmp_HuMCInit
    b HuMCInit
    entry _kerjmp_HuMCClose
    b HuMCClose
    entry _kerjmp_HuMCContextSet
    b HuMCContextSet
    entry _kerjmp_HuMCResponseGet2
    b HuMCResponseGet2
    entry _kerjmp_HuMCStatGet
    b HuMCStatGet
    entry _kerjmp_HuMCContextCallbackSet
    b HuMCContextCallbackSet
    entry _kerjmp_HuMCResponseGet
    b HuMCResponseGet
    entry _kerjmp_HuMCContextCreate
    b HuMCContextCreate
    entry _kerjmp_HuMCContextKill
    b HuMCContextKill
    entry _kerjmp_HuMCThresholdSet
    b HuMCThresholdSet
    entry _kerjmp_HuMCCurResponseGet
    b HuMCCurResponseGet
    entry _kerjmp_HuMCButtonGet
    b HuMCButtonGet
    entry _kerjmp_HuMCButtonDownGet
    b HuMCButtonDownGet
    entry _kerjmp_HuMCProbe
    b HuMCProbe
    entry _kerjmp_HuMCMount
    b HuMCMount
    entry _kerjmp_HuMCMicSprCreate
    b HuMCMicSprCreate
    entry _kerjmp_HuMCMicSprKill
    b HuMCMicSprKill
    entry _kerjmp_HuMCMicGet
    b HuMCMicGet
    entry _kerjmp_HuMCMicSet
    b HuMCMicSet
    entry _kerjmp_HuMCSelWinCreate
    b HuMCSelWinCreate
    entry _kerjmp_HuMCSelWinKill
    b HuMCSelWinKill
    entry _kerjmp_HuMCSelWinItemSet
    b HuMCSelWinItemSet
    entry _kerjmp_HuMCSelWinItemRandSet
    b HuMCSelWinItemRandSet
    entry _kerjmp_HuMCSelWinCheck
    b HuMCSelWinCheck
    entry _kerjmp_HuMCSelModeGet
    b HuMCSelModeGet
    entry _kerjmp_HuMCSelWinContextSet
    b HuMCSelWinContextSet
    entry _kerjmp_HuMCSelWinChoiceGet
    b HuMCSelWinChoiceGet
    entry _kerjmp_HuMCSelWinContextKill
    b HuMCSelWinContextKill
    entry _kerjmp_HuMCSelWinMaxTimeSet
    b HuMCSelWinMaxTimeSet
    entry _kerjmp_HuMCSelWinMaxTimeGet
    b HuMCSelWinMaxTimeGet
    entry _kerjmp_HuMCListenerCreate
    b HuMCListenerCreate
    entry _kerjmp_HuMCListenerKill
    b HuMCListenerKill
    entry _kerjmp_HuMCSessionSet
    b HuMCSessionSet
    entry _kerjmp_HuMCSessionKill
    b HuMCSessionKill
    entry _kerjmp_HuMCSessionClose
    b HuMCSessionClose
    entry _kerjmp_HuMCUnkResponseCheck
    b HuMCUnkResponseCheck
    entry _kerjmp_HuMCNewResponseGet
    b HuMCNewResponseGet
    entry _kerjmp_HuMCVolSampleCreate
    b HuMCVolSampleCreate
    entry _kerjmp_HuMCVolGet
    b HuMCVolGet
    entry _kerjmp_HuMCShiftsSet
    b HuMCShiftsSet
    entry _kerjmp_fn_8014C818
    b fn_8014C818
    entry _kerjmp_fn_8014C81C
    b fn_8014C81C
    entry _kerjmp_mbBranchInit
    b mbBranchInit
    entry _kerjmp_mbev_Branch
    b mbev_Branch
    entry _kerjmp_mbev_BranchDebug
    b mbev_BranchDebug
    entry _kerjmp_mbBranchAttrSet
    b mbBranchAttrSet
    entry _kerjmp_mbBranchAttrReset
    b mbBranchAttrReset
    entry _kerjmp_mbBranchAttrGet
    b mbBranchAttrGet
    entry _kerjmp_mbBranchMAttrSet
    b mbBranchMAttrSet
    entry _kerjmp_mbBranchMAttrReset
    b mbBranchMAttrReset
    entry _kerjmp_mbBranchMAttrGet
    b mbBranchMAttrGet
    entry _kerjmp_mbBranchAttrCheck
    b mbBranchAttrCheck
    entry _kerjmp_mbBranchComStarHookSet
    b mbBranchComStarHookSet
    entry _kerjmp_mbCameraInit
    b mbCameraInit
    entry _kerjmp_mbCameraZoomSet
    b mbCameraZoomSet
    entry _kerjmp_mbCameraZoomGet
    b mbCameraZoomGet
    entry _kerjmp_mbCameraRotSetV
    b mbCameraRotSetV
    entry _kerjmp_mbCameraRotSet
    b mbCameraRotSet
    entry _kerjmp_mbCameraRotGet
    b mbCameraRotGet
    entry _kerjmp_mbCameraCenterSetV
    b mbCameraCenterSetV
    entry _kerjmp_mbCameraCenterSet
    b mbCameraCenterSet
    entry _kerjmp_mbCameraCenterGet
    b mbCameraCenterGet
    entry _kerjmp_mbCameraEyeSetV
    b mbCameraEyeSetV
    entry _kerjmp_mbCameraEyeSet
    b mbCameraEyeSet
    entry _kerjmp_mbCameraEyeGet
    b mbCameraEyeGet
    entry _kerjmp_mbCameraOffsetSetV
    b mbCameraOffsetSetV
    entry _kerjmp_mbCameraOffsetSet
    b mbCameraOffsetSet
    entry _kerjmp_mbCameraOffsetGet
    b mbCameraOffsetGet
    entry _kerjmp_mbCameraDirGet
    b mbCameraDirGet
    entry _kerjmp_mbCameraPosDirGet
    b mbCameraPosDirGet
    entry _kerjmp_mbCameraLookAtFuncSet
    b mbCameraLookAtFuncSet
    entry _kerjmp_mbCameraBitSet
    b mbCameraBitSet
    entry _kerjmp_mbCameraFovSet
    b mbCameraFovSet
    entry _kerjmp_mbCameraScissorSet
    b mbCameraScissorSet
    entry _kerjmp_mbCameraNearFarSet
    b mbCameraNearFarSet
    entry _kerjmp_mbCameraNearFarGet
    b mbCameraNearFarGet
    entry _kerjmp_mbCameraGet
    b mbCameraGet
    entry _kerjmp_mbCameraStackPush
    b mbCameraStackPush
    entry _kerjmp_mbCameraStackIdxSet
    b mbCameraStackIdxSet
    entry _kerjmp_mbCameraStackPop
    b mbCameraStackPop
    entry _kerjmp_mbCameraLookAtGet
    b mbCameraLookAtGet
    entry _kerjmp_mbCameraLookAtInvGet
    b mbCameraLookAtInvGet
    entry _kerjmp_mbCameraCurveTypeSet
    b mbCameraCurveTypeSet
    entry _kerjmp_mbCameraCurveTypeGet
    b mbCameraCurveTypeGet
    entry _kerjmp_mbCameraCullCheck
    b mbCameraCullCheck
    entry _kerjmp_mbCameraPlayerViewSet
    b mbCameraPlayerViewSet
    entry _kerjmp_mbCameraPlayerViewSetFast
    b mbCameraPlayerViewSetFast
    entry _kerjmp_mbCameraPlayerViewGet
    b mbCameraPlayerViewGet
    entry _kerjmp_mbCameraPlayerViewNoGet
    b mbCameraPlayerViewNoGet
    entry _kerjmp_mbCameraPlayerViewZoomGet
    b mbCameraPlayerViewZoomGet
    entry _kerjmp_mbCameraPlayerViewRotXGet
    b mbCameraPlayerViewRotXGet
    entry _kerjmp_mbCameraFocusReset
    b mbCameraFocusReset
    entry _kerjmp_mbCameraFocusPosSet
    b mbCameraFocusPosSet
    entry _kerjmp_mbCameraFocusPosAdd
    b mbCameraFocusPosAdd
    entry _kerjmp_mbCameraFocusPlayerSet
    b mbCameraFocusPlayerSet
    entry _kerjmp_mbCameraFocusPlayerAdd
    b mbCameraFocusPlayerAdd
    entry _kerjmp_mbCameraFocusObjSet
    b mbCameraFocusObjSet
    entry _kerjmp_mbCameraFocusObjAdd
    b mbCameraFocusObjAdd
    entry _kerjmp_mbCameraFocusMasuSet
    b mbCameraFocusMasuSet
    entry _kerjmp_mbCameraFocusMasuAdd
    b mbCameraFocusMasuAdd
    entry _kerjmp_mbCameraMoveOnSet
    b mbCameraMoveOnSet
    entry _kerjmp_mbCameraSpeedSet
    b mbCameraSpeedSet
    entry _kerjmp_mbCameraMovePos
    b mbCameraMovePos
    entry _kerjmp_mbCameraMovePlayer
    b mbCameraMovePlayer
    entry _kerjmp_mbCameraMoveObj
    b mbCameraMoveObj
    entry _kerjmp_mbCameraMoveMasu
    b mbCameraMoveMasu
    entry _kerjmp_mbCameraShakeSet
    b mbCameraShakeSet
    entry _kerjmp_mbCameraShakeReset
    b mbCameraShakeReset
    entry _kerjmp_mbCameraMoveStop
    b mbCameraMoveStop
    entry _kerjmp_mbCameraMoveCheck
    b mbCameraMoveCheck
    entry _kerjmp_mbCameraMoveWait
    b mbCameraMoveWait
    entry _kerjmp_mbCapSelect
    b mbCapSelect
    entry _kerjmp_mbCapDelete
    b mbCapDelete
    entry _kerjmp_fn_8019A618
    b fn_8019A618
    entry _kerjmp_fn_8019A61C
    b fn_8019A61C
    entry _kerjmp_fn_8019A620
    b fn_8019A620
    entry _kerjmp_fn_8019A624
    b fn_8019A624
    entry _kerjmp_fn_8019A628
    b fn_8019A628
    entry _kerjmp_fn_8019A62C
    b fn_8019A62C
    entry _kerjmp_mbCapSelectGrow
    b mbCapSelectGrow
    entry _kerjmp_mbCapSelectShrinkCheck
    b mbCapSelectShrinkCheck
    entry _kerjmp_mbCapSelectResultSet
    b mbCapSelectResultSet
    entry _kerjmp_mbCapSelectResultGet
    b mbCapSelectResultGet
    entry _kerjmp_mbCapSelectResultReset
    b mbCapSelectResultReset
    entry _kerjmp_mbCapMasuExec
    b mbCapMasuExec
    entry _kerjmp_mbCapMasuObjInit
    b mbCapMasuObjInit
    entry _kerjmp_mbCapMasuObjClose
    b mbCapMasuObjClose
    entry _kerjmp_mbCapMasuObjCreateAll
    b mbCapMasuObjCreateAll
    entry _kerjmp_mbCapMasuObjCreate
    b mbCapMasuObjCreate
    entry _kerjmp_mbCapMasuDispSet
    b mbCapMasuDispSet
    entry _kerjmp_mbCapCapsuleGet
    b mbCapCapsuleGet
    entry _kerjmp_mbev_CapKinoko
    b mbev_CapKinoko
    entry _kerjmp_mbev_CapKinokoKill
    b mbev_CapKinokoKill
    entry _kerjmp_mbev_CapSKinoko
    b mbev_CapSKinoko
    entry _kerjmp_mbev_CapSKinokoKill
    b mbev_CapSKinokoKill
    entry _kerjmp_mbev_CapPKinoko
    b mbev_CapPKinoko
    entry _kerjmp_mbev_CapPKinokoKill
    b mbev_CapPKinokoKill
    entry _kerjmp_mbev_CapMKinoko
    b mbev_CapMKinoko
    entry _kerjmp_mbev_CapMKinokoKill
    b mbev_CapMKinokoKill
    entry _kerjmp_mbev_CapKiller
    b mbev_CapKiller
    entry _kerjmp_mbev_CapKillerKill
    b mbev_CapKillerKill
    entry _kerjmp_mbev_CapDokan
    b mbev_CapDokan
    entry _kerjmp_mbev_CapDokanKill
    b mbev_CapDokanKill
    entry _kerjmp_mbev_CapHanachan
    b mbev_CapHanachan
    entry _kerjmp_mbev_CapHanachanKill
    b mbev_CapHanachanKill
    entry _kerjmp_mbev_CapNKinoko
    b mbev_CapNKinoko
    entry _kerjmp_mbev_CapNKinokoKill
    b mbev_CapNKinokoKill
    entry _kerjmp_mbev_CapKillerMove
    b mbev_CapKillerMove
    entry _kerjmp_mbev_CapKillerMoveKill
    b mbev_CapKillerMoveKill
    entry _kerjmp_mbev_CapTogezo
    b mbev_CapTogezo
    entry _kerjmp_mbev_CapTogezoKill
    b mbev_CapTogezoKill
    entry _kerjmp_mbev_CapKuribo
    b mbev_CapKuribo
    entry _kerjmp_mbev_CapKuriboKill
    b mbev_CapKuriboKill
    entry _kerjmp_mbev_CapPakkun
    b mbev_CapPakkun
    entry _kerjmp_mbev_CapPakkunKill
    b mbev_CapPakkunKill
    entry _kerjmp_mbev_CapJango
    b mbev_CapJango
    entry _kerjmp_mbev_CapJangoKill
    b mbev_CapJangoKill
    entry _kerjmp_mbev_CapPatapata
    b mbev_CapPatapata
    entry _kerjmp_mbev_CapPatapataKill
    b mbev_CapPatapataKill
    entry _kerjmp_mbev_CapKokamekku
    b mbev_CapKokamekku
    entry _kerjmp_mbev_CapKokamekkuKill
    b mbev_CapKokamekkuKill
    entry _kerjmp_mbev_CapKamekku
    b mbev_CapKamekku
    entry _kerjmp_mbev_CapKamekkuKill
    b mbev_CapKamekkuKill
    entry _kerjmp_mbev_CapThrowman
    b mbev_CapThrowman
    entry _kerjmp_mbev_CapThrowmanKill
    b mbev_CapThrowmanKill
    entry _kerjmp_mbev_CapBoble
    b mbev_CapBoble
    entry _kerjmp_mbev_CapBobleKill
    b mbev_CapBobleKill
    entry _kerjmp_mbev_CapBobleTrap
    b mbev_CapBobleTrap
    entry _kerjmp_mbev_CapBobleMove
    b mbev_CapBobleMove
    entry _kerjmp_mbev_CapBiriQ
    b mbev_CapBiriQ
    entry _kerjmp_mbev_CapBiriQKill
    b mbev_CapBiriQKill
    entry _kerjmp_mbev_CapBiriQTrap
    b mbev_CapBiriQTrap
    entry _kerjmp_mbev_CapBiriQMetalShock
    b mbev_CapBiriQMetalShock
    entry _kerjmp_mbev_CapTumujikun
    b mbev_CapTumujikun
    entry _kerjmp_mbev_CapTumujikunKill
    b mbev_CapTumujikunKill
    entry _kerjmp_mbev_CapTumujikunTrap
    b mbev_CapTumujikunTrap
    entry _kerjmp_mbev_CapDossun
    b mbev_CapDossun
    entry _kerjmp_mbev_CapDossunKill
    b mbev_CapDossunKill
    entry _kerjmp_mbev_CapDossunTrap
    b mbev_CapDossunTrap
    entry _kerjmp_mbev_CapBomhei
    b mbev_CapBomhei
    entry _kerjmp_mbev_CapBomheiKill
    b mbev_CapBomheiKill
    entry _kerjmp_mbev_CapBomheiTrap
    b mbev_CapBomheiTrap
    entry _kerjmp_mbev_CapBomheiMove
    b mbev_CapBomheiMove
    entry _kerjmp_mbev_CapTeresa
    b mbev_CapTeresa
    entry _kerjmp_mbev_CapTeresaKill
    b mbev_CapTeresaKill
    entry _kerjmp_mbev_CapTeresaFadeCreate
    b mbev_CapTeresaFadeCreate
    entry _kerjmp_mbev_CapTeresaFadeKill
    b mbev_CapTeresaFadeKill
    entry _kerjmp_mbev_CapTeresaFadeSet
    b mbev_CapTeresaFadeSet
    entry _kerjmp_mbev_CapMiracle
    b mbev_CapMiracle
    entry _kerjmp_mbev_CapMiracleKill
    b mbev_CapMiracleKill
    entry _kerjmp_mbev_CapKettou
    b mbev_CapKettou
    entry _kerjmp_mbev_CapKettouKill
    b mbev_CapKettouKill
    entry _kerjmp_mbev_CapDonkey
    b mbev_CapDonkey
    entry _kerjmp_mbev_CapDonkeyKill
    b mbev_CapDonkeyKill
    entry _kerjmp_mbev_CapKoopa
    b mbev_CapKoopa
    entry _kerjmp_mbev_CapKoopaKill
    b mbev_CapKoopaKill
    entry _kerjmp_mbev_CapCall
    b mbev_CapCall
    entry _kerjmp_mbev_CapCallTrap
    b mbev_CapCallTrap
    entry _kerjmp_mbev_CapCapGet
    b mbev_CapCapGet
    entry _kerjmp_mbev_CapWait
    b mbev_CapWait
    entry _kerjmp_mbev_CapCallKettou
    b mbev_CapCallKettou
    entry _kerjmp_mbev_CapCircuitCallKettou
    b mbev_CapCircuitCallKettou
    entry _kerjmp_mbev_CapCallDonkey
    b mbev_CapCallDonkey
    entry _kerjmp_mbev_CapCallKoopa
    b mbev_CapCallKoopa
    entry _kerjmp_MBCapsuleStub5
    b MBCapsuleStub5
    entry _kerjmp_MBCapsuleStub6
    b MBCapsuleStub6
    entry _kerjmp_MBCapsuleStub7
    b MBCapsuleStub7
    entry _kerjmp_mbev_CapCallTeresa
    b mbev_CapCallTeresa
    entry _kerjmp_mbev_CapCallMiracle
    b mbev_CapCallMiracle
    entry _kerjmp_mbev_CapBiriQShockCreate
    b mbev_CapBiriQShockCreate
    entry _kerjmp_mbev_CapBiriQShockDelayGet
    b mbev_CapBiriQShockDelayGet
    entry _kerjmp_mbev_CapBiriQMetalShockCreate
    b mbev_CapBiriQMetalShockCreate
    entry _kerjmp_mbev_CapRandomBonusCoin
    b mbev_CapRandomBonusCoin
    entry _kerjmp_mbev_CapBonusCoinCall
    b mbev_CapBonusCoinCall
    entry _kerjmp_mbev_CapBonusCoin
    b mbev_CapBonusCoin
    entry _kerjmp_mbev_CapBonusCoinCheck
    b mbev_CapBonusCoinCheck
    entry _kerjmp_mbev_CapMoveMasuSet
    b mbev_CapMoveMasuSet
    entry _kerjmp_mbev_CapStopMasuSet
    b mbev_CapStopMasuSet
    entry _kerjmp_mbev_CapKettouEndCall
    b mbev_CapKettouEndCall
    entry _kerjmp_mbev_CapDonkeyEndCall
    b mbev_CapDonkeyEndCall
    entry _kerjmp_mbev_CapKoopaEndCall
    b mbev_CapKoopaEndCall
    entry _kerjmp_mbev_CapVsEndCall
    b mbev_CapVsEndCall
    entry _kerjmp_mbev_CapKillerMoveCall
    b mbev_CapKillerMoveCall
    entry _kerjmp_MBCapsuleStub11
    b MBCapsuleStub11
    entry _kerjmp_MBCapsuleStub12
    b MBCapsuleStub12
    entry _kerjmp_mbev_CapKillerMoveCheck
    b mbev_CapKillerMoveCheck
    entry _kerjmp_mbev_CapKillerMoveCheckAll
    b mbev_CapKillerMoveCheckAll
    entry _kerjmp_mbev_CapBankCoinInit
    b mbev_CapBankCoinInit
    entry _kerjmp_mbev_CapInit
    b mbev_CapInit
    entry _kerjmp_mbev_CapKillerCall
    b mbev_CapKillerCall
    entry _kerjmp_mbev_CapKillerMultiCall
    b mbev_CapKillerMultiCall
    entry _kerjmp_mbev_CapOpeningAdd
    b mbev_CapOpeningAdd
    entry _kerjmp_mbev_CapKoopaAdd
    b mbev_CapKoopaAdd
    entry _kerjmp_mbev_CapBankCoinGet
    b mbev_CapBankCoinGet
    entry _kerjmp_mbev_CapBubbleHookSet
    b mbev_CapBubbleHookSet
    entry _kerjmp_mbev_CapBubbleHookCall
    b mbev_CapBubbleHookCall
    entry _kerjmp_mbev_CapBubbleHookCallStory
    b mbev_CapBubbleHookCallStory
    entry _kerjmp_mbev_CapKettouCoinLoseGet
    b mbev_CapKettouCoinLoseGet
    entry _kerjmp_mbev_CapKettouOppCoinLoseGet
    b mbev_CapKettouOppCoinLoseGet
    entry _kerjmp_mbev_CapKettouCoinLoseGet2
    b mbev_CapKettouCoinLoseGet2
    entry _kerjmp_mbev_CapKettouOppCoinLoseGet2
    b mbev_CapKettouOppCoinLoseGet2
    entry _kerjmp_mbev_CapPlayerMotionCreate
    b mbev_CapPlayerMotionCreate
    entry _kerjmp_mbev_CapObjCreate
    b mbev_CapObjCreate
    entry _kerjmp_mbev_CapObjPosSet
    b mbev_CapObjPosSet
    entry _kerjmp_mbev_CapPlayerPosSet
    b mbev_CapPlayerPosSet
    entry _kerjmp_mbev_CapSprCreate
    b mbev_CapSprCreate
    entry _kerjmp_mbev_CapMalloc
    b mbev_CapMalloc
    entry _kerjmp_mbev_CapObjClose
    b mbev_CapObjClose
    entry _kerjmp_mbev_CapSprClose
    b mbev_CapSprClose
    entry _kerjmp_mbev_CapMallocClose
    b mbev_CapMallocClose
    entry _kerjmp_mbev_CapEffExplodeCreate
    b mbev_CapEffExplodeCreate
    entry _kerjmp_mbev_CapEffExhaustCreate
    b mbev_CapEffExhaustCreate
    entry _kerjmp_mbev_CapEffExplodeOMExec
    b mbev_CapEffExplodeOMExec
    entry _kerjmp_mbev_CapEffExplodeKill
    b mbev_CapEffExplodeKill
    entry _kerjmp_mbev_CapEffExplodeAnimGet
    b mbev_CapEffExplodeAnimGet
    entry _kerjmp_mbev_CapEffExplodeAnimSet
    b mbev_CapEffExplodeAnimSet
    entry _kerjmp_mbev_CapEffExplodeAdd
    b mbev_CapEffExplodeAdd
    entry _kerjmp_mbev_CapEffExplodeKillerAdd
    b mbev_CapEffExplodeKillerAdd
    entry _kerjmp_mbev_CapEffExplodeCircleAdd
    b mbev_CapEffExplodeCircleAdd
    entry _kerjmp_mbev_CapEffDustCloudAdd
    b mbev_CapEffDustCloudAdd
    entry _kerjmp_mbev_CapEffDustExplodeAdd
    b mbev_CapEffDustExplodeAdd
    entry _kerjmp_mbev_CapEffDustHeavyAdd
    b mbev_CapEffDustHeavyAdd
    entry _kerjmp_mbev_CapEffDustMultiAdd
    b mbev_CapEffDustMultiAdd
    entry _kerjmp_mbev_CapEffBoostCreate
    b mbev_CapEffBoostCreate
    entry _kerjmp_mbev_CapEffBoostOMExec
    b mbev_CapEffBoostOMExec
    entry _kerjmp_mbev_CapEffBoostKill
    b mbev_CapEffBoostKill
    entry _kerjmp_mbev_CapEffBoostTimeGet
    b mbev_CapEffBoostTimeGet
    entry _kerjmp_mbev_CapEffBoostBlendModeSet
    b mbev_CapEffBoostBlendModeSet
    entry _kerjmp_mbev_CapEffBoostAdd
    b mbev_CapEffBoostAdd
    entry _kerjmp_mbev_CapEffSnowCreate
    b mbev_CapEffSnowCreate
    entry _kerjmp_mbev_CapEffSnowOMExec
    b mbev_CapEffSnowOMExec
    entry _kerjmp_mbev_CapEffSnowKill
    b mbev_CapEffSnowKill
    entry _kerjmp_mbev_CapEffSnowDispGet
    b mbev_CapEffSnowDispGet
    entry _kerjmp_mbev_CapEffSnowAdd
    b mbev_CapEffSnowAdd
    entry _kerjmp_mbev_CapEffGlowCreate
    b mbev_CapEffGlowCreate
    entry _kerjmp_mbev_CapEffGlowFireCreate
    b mbev_CapEffGlowFireCreate
    entry _kerjmp_mbev_CapEffGlowOMExec
    b mbev_CapEffGlowOMExec
    entry _kerjmp_mbev_CapEffGlowKill
    b mbev_CapEffGlowKill
    entry _kerjmp_mbev_CapEffGlowDispGet
    b mbev_CapEffGlowDispGet
    entry _kerjmp_mbev_CapEffGlowPatSet
    b mbev_CapEffGlowPatSet
    entry _kerjmp_mbev_CapEffGlowBlendModeSet
    b mbev_CapEffGlowBlendModeSet
    entry _kerjmp_mbev_CapEffGlowAnimSet
    b mbev_CapEffGlowAnimSet
    entry _kerjmp_mbev_CapEffGlowAdd
    b mbev_CapEffGlowAdd
    entry _kerjmp_mbev_CapEffGlowKinokoAdd
    b mbev_CapEffGlowKinokoAdd
    entry _kerjmp_mbev_CapEffGlowKinokoAddAlt
    b mbev_CapEffGlowKinokoAddAlt
    entry _kerjmp_mbev_CapEffGlowKinokoTimeSet
    b mbev_CapEffGlowKinokoTimeSet
    entry _kerjmp_mbev_CapEffGlowCoinAdd
    b mbev_CapEffGlowCoinAdd
    entry _kerjmp_mbev_CapEffRingCreate
    b mbev_CapEffRingCreate
    entry _kerjmp_mbev_CapEffRingHitCreate
    b mbev_CapEffRingHitCreate
    entry _kerjmp_mbev_CapEffRingOMExec
    b mbev_CapEffRingOMExec
    entry _kerjmp_mbev_CapEffRingKill
    b mbev_CapEffRingKill
    entry _kerjmp_mbev_CapEffRingDispGet
    b mbev_CapEffRingDispGet
    entry _kerjmp_mbev_CapEffRingAdd
    b mbev_CapEffRingAdd
    entry _kerjmp_mbev_CapEffRingHitAdd
    b mbev_CapEffRingHitAdd
    entry _kerjmp_mbev_CapEffRingAnimSet
    b mbev_CapEffRingAnimSet
    entry _kerjmp_mbev_CapEffElectricCreate
    b mbev_CapEffElectricCreate
    entry _kerjmp_mbev_CapEffElectricOMExec
    b mbev_CapEffElectricOMExec
    entry _kerjmp_mbev_CapEffElectricKill
    b mbev_CapEffElectricKill
    entry _kerjmp_mbev_CapEffElectricDispGet
    b mbev_CapEffElectricDispGet
    entry _kerjmp_mbev_CapEffElectricAdd
    b mbev_CapEffElectricAdd
    entry _kerjmp_mbev_CapEffElectricModelSet
    b mbev_CapEffElectricModelSet
    entry _kerjmp_mbev_CapEffRayCreate
    b mbev_CapEffRayCreate
    entry _kerjmp_mbev_CapEffRayOMExec
    b mbev_CapEffRayOMExec
    entry _kerjmp_mbev_CapEffRayKill
    b mbev_CapEffRayKill
    entry _kerjmp_mbev_CapEffRayDraw
    b mbev_CapEffRayDraw
    entry _kerjmp_mbev_CapEffRayAdd
    b mbev_CapEffRayAdd
    entry _kerjmp_mbev_CapEffRayAlphaSet
    b mbev_CapEffRayAlphaSet
    entry _kerjmp_mbev_CapEffRayTransformSet
    b mbev_CapEffRayTransformSet
    entry _kerjmp_mbev_CapEffMasuHitCreate
    b mbev_CapEffMasuHitCreate
    entry _kerjmp_mbev_CapEffMasuHitOMExec
    b mbev_CapEffMasuHitOMExec
    entry _kerjmp_mbev_CapEffMasuHitKill
    b mbev_CapEffMasuHitKill
    entry _kerjmp_mbev_CapEffMasuHitAdd
    b mbev_CapEffMasuHitAdd
    entry _kerjmp_mbev_CapEffMasuHitTransformSet
    b mbev_CapEffMasuHitTransformSet
    entry _kerjmp_mbev_CapEffCoinCreate
    b mbev_CapEffCoinCreate
    entry _kerjmp_mbev_CapEffCoinOMExec
    b mbev_CapEffCoinOMExec
    entry _kerjmp_mbev_CapEffCoinKill
    b mbev_CapEffCoinKill
    entry _kerjmp_mbev_CapEffCoinNumGet
    b mbev_CapEffCoinNumGet
    entry _kerjmp_mbev_CapEffCoinAdd
    b mbev_CapEffCoinAdd
    entry _kerjmp_mbev_CapEffCoinMaxYSet
    b mbev_CapEffCoinMaxYSet
    entry _kerjmp_mbev_CapEffCoinMultiAdd
    b mbev_CapEffCoinMultiAdd
    entry _kerjmp_mbev_CapEffCoinGlowSet
    b mbev_CapEffCoinGlowSet
    entry _kerjmp_mbev_CapCoinAdd
    b mbev_CapCoinAdd
    entry _kerjmp_mbev_CapEffCapLoseCreate
    b mbev_CapEffCapLoseCreate
    entry _kerjmp_mbev_CapEffCapLoseOMExec
    b mbev_CapEffCapLoseOMExec
    entry _kerjmp_mbev_CapEffCapLoseKill
    b mbev_CapEffCapLoseKill
    entry _kerjmp_mbev_CapEffCapLoseNumGet
    b mbev_CapEffCapLoseNumGet
    entry _kerjmp_mbev_CapEffCapLoseObjAdd
    b mbev_CapEffCapLoseObjAdd
    entry _kerjmp_mbev_CapEffCapLoseAdd
    b mbev_CapEffCapLoseAdd
    entry _kerjmp_mbev_CapCoinManCreate
    b mbev_CapCoinManCreate
    entry _kerjmp_mbev_CapCoinManOMExec
    b mbev_CapCoinManOMExec
    entry _kerjmp_mbev_CapCoinManKill
    b mbev_CapCoinManKill
    entry _kerjmp_mbev_CapCoinManNumGet
    b mbev_CapCoinManNumGet
    entry _kerjmp_mbev_CapCoinManObjAdd
    b mbev_CapCoinManObjAdd
    entry _kerjmp_mbev_CapCoinManAdd2
    b mbev_CapCoinManAdd2
    entry _kerjmp_mbev_CapCoinManAdd
    b mbev_CapCoinManAdd
    entry _kerjmp_mbev_CapStarManCreate
    b mbev_CapStarManCreate
    entry _kerjmp_mbev_CapStarManOMExec
    b mbev_CapStarManOMExec
    entry _kerjmp_mbev_CapStarManKill
    b mbev_CapStarManKill
    entry _kerjmp_mbev_CapStarManNumGet
    b mbev_CapStarManNumGet
    entry _kerjmp_mbev_CapStarManObjAdd
    b mbev_CapStarManObjAdd
    entry _kerjmp_mbev_CapStarManAdd2
    b mbev_CapStarManAdd2
    entry _kerjmp_mbev_CapStarManAdd
    b mbev_CapStarManAdd
    entry _kerjmp_mbev_CapPlayerMoveObjInit
    b mbev_CapPlayerMoveObjInit
    entry _kerjmp_mbev_CapPlayerMoveHitCreate
    b mbev_CapPlayerMoveHitCreate
    entry _kerjmp_mbev_CapPlayerMoveEjectCreate
    b mbev_CapPlayerMoveEjectCreate
    entry _kerjmp_mbev_CapPlayerMoveIdleCreate
    b mbev_CapPlayerMoveIdleCreate
    entry _kerjmp_mbev_CapPlayerMoveMinYSet
    b mbev_CapPlayerMoveMinYSet
    entry _kerjmp_mbev_CapPlayerMoveVelSet
    b mbev_CapPlayerMoveVelSet
    entry _kerjmp_mbev_CapPlayerMoveObjExec
    b mbev_CapPlayerMoveObjExec
    entry _kerjmp_mbev_CapPlayerMoveObjCheck
    b mbev_CapPlayerMoveObjCheck
    entry _kerjmp_mbev_CapPlayerMoveObjClose
    b mbev_CapPlayerMoveObjClose
    entry _kerjmp_mbev_CapPlayerMoveObjKill
    b mbev_CapPlayerMoveObjKill
    entry _kerjmp_mbev_CapPlayerSquishSet
    b mbev_CapPlayerSquishSet
    entry _kerjmp_mbev_CapPlayerSquishVoiceSet
    b mbev_CapPlayerSquishVoiceSet
    entry _kerjmp_mbev_CapPlayerStunSet
    b mbev_CapPlayerStunSet
    entry _kerjmp_mbev_CapPlayerShockSet
    b mbev_CapPlayerShockSet
    entry _kerjmp_mbev_CapPlayerCheck
    b mbev_CapPlayerCheck
    entry _kerjmp_mbev_CapCullPlayerCheck
    b mbev_CapCullPlayerCheck
    entry _kerjmp_mbev_CapCullCheck
    b mbev_CapCullCheck
    entry _kerjmp_mbev_CapPointCullCheck
    b mbev_CapPointCullCheck
    entry _kerjmp_mbev_CapPlayerMasuNumGet
    b mbev_CapPlayerMasuNumGet
    entry _kerjmp_mbev_CapPlayerNoSearch
    b mbev_CapPlayerNoSearch
    entry _kerjmp_mbev_CapPlayerOrderGet
    b mbev_CapPlayerOrderGet
    entry _kerjmp_mbev_CapMasuLinkNextGet
    b mbev_CapMasuLinkNextGet
    entry _kerjmp_mbev_CapMasuLinkNextRandomGet
    b mbev_CapMasuLinkNextRandomGet
    entry _kerjmp_mbev_CapMasuValidPrevGet
    b mbev_CapMasuValidPrevGet
    entry _kerjmp_mbev_CapMasuPrevGet
    b mbev_CapMasuPrevGet
    entry _kerjmp_mbev_CapPlayerComSelGet
    b mbev_CapPlayerComSelGet
    entry _kerjmp_mbev_CapPlayerComSelSameGet
    b mbev_CapPlayerComSelSameGet
    entry _kerjmp_mbev_CapPlayerComSelRandomGet
    b mbev_CapPlayerComSelRandomGet
    entry _kerjmp_mbev_CapPlayerComSelKettouGet
    b mbev_CapPlayerComSelKettouGet
    entry _kerjmp_mbev_CapEffColorSet
    b mbev_CapEffColorSet
    entry _kerjmp_mbev_CapPlayerMotShiftCheck
    b mbev_CapPlayerMotShiftCheck
    entry _kerjmp_mbev_CapPlayerMotShiftSet
    b mbev_CapPlayerMotShiftSet
    entry _kerjmp_mbev_CapPlayerMotShiftWait
    b mbev_CapPlayerMotShiftWait
    entry _kerjmp_mbev_CapObjMotionSet
    b mbev_CapObjMotionSet
    entry _kerjmp_mbev_CapObjMotionOMExec
    b mbev_CapObjMotionOMExec
    entry _kerjmp_mbev_CapPlayerMotionSet
    b mbev_CapPlayerMotionSet
    entry _kerjmp_mbev_CapPlayerMotionOMExec
    b mbev_CapPlayerMotionOMExec
    entry _kerjmp_mbev_CapPlayerRotate
    b mbev_CapPlayerRotate
    entry _kerjmp_mbev_CapPlayerIdleWait
    b mbev_CapPlayerIdleWait
    entry _kerjmp_mbev_CapCoinDisp
    b mbev_CapCoinDisp
    entry _kerjmp_mbev_CapMasuMoveCheck
    b mbev_CapMasuMoveCheck
    entry _kerjmp_mbev_CapVibrate
    b mbev_CapVibrate
    entry _kerjmp_mbev_CapStatusDispSet
    b mbev_CapStatusDispSet
    entry _kerjmp_mbev_CapDuelStatusOffSet
    b mbev_CapDuelStatusOffSet
    entry _kerjmp_mbev_CapDuelStatusOnSet
    b mbev_CapDuelStatusOnSet
    entry _kerjmp_mbev_CapDuelStatusDispSet
    b mbev_CapDuelStatusDispSet
    entry _kerjmp_mbev_CapStatusDispSetAll
    b mbev_CapStatusDispSetAll
    entry _kerjmp_mbev_CapStatusDispCheck
    b mbev_CapStatusDispCheck
    entry _kerjmp_mbev_CapChoiceSet
    b mbev_CapChoiceSet
    entry _kerjmp_mbev_CapAngleWrap
    b mbev_CapAngleWrap
    entry _kerjmp_mbev_CapAngleLerp
    b mbev_CapAngleLerp
    entry _kerjmp_mbev_CapAngleSumLerp
    b mbev_CapAngleSumLerp
    entry _kerjmp_mbev_CapVecChase
    b mbev_CapVecChase
    entry _kerjmp_mbev_CapVecRotGet
    b mbev_CapVecRotGet
    entry _kerjmp_mbev_CapColorLerp
    b mbev_CapColorLerp
    entry _kerjmp_mbev_CapHermiteConstGet
    b mbev_CapHermiteConstGet
    entry _kerjmp_mbev_CapHermiteConstGet2
    b mbev_CapHermiteConstGet2
    entry _kerjmp_mbev_CapHermiteGetV
    b mbev_CapHermiteGetV
    entry _kerjmp_mbev_CapBezierGet
    b mbev_CapBezierGet
    entry _kerjmp_mbev_CapBezierGetV
    b mbev_CapBezierGetV
    entry _kerjmp_mbev_CapBezierSlopeGet
    b mbev_CapBezierSlopeGet
    entry _kerjmp_mbev_CapBezierNormGetV
    b mbev_CapBezierNormGetV
    entry _kerjmp_mbev_ShopEnableSet
    b mbev_ShopEnableSet
    entry _kerjmp_mbev_ShopInit
    b mbev_ShopInit
    entry _kerjmp_mbev_ShopCreate
    b mbev_ShopCreate
    entry _kerjmp_mbev_ShopExInit
    b mbev_ShopExInit
    entry _kerjmp_mbev_Shop
    b mbev_Shop
    entry _kerjmp_mbev_ShopExObjHookSet
    b mbev_ShopExObjHookSet
    entry _kerjmp_mbev_ShopBackCreate
    b mbev_ShopBackCreate
    entry _kerjmp_mbev_ShopBackMotCreate
    b mbev_ShopBackMotCreate
    entry _kerjmp_mbCapUse
    b mbCapUse
    entry _kerjmp_MBCapsuleStub1
    b MBCapsuleStub1
    entry _kerjmp_MBCapsuleStub2
    b MBCapsuleStub2
    entry _kerjmp_mbCapEffUseCreate
    b mbCapEffUseCreate
    entry _kerjmp_mbCapEffUseModeGet
    b mbCapEffUseModeGet
    entry _kerjmp_mbCapEffUsePosGet
    b mbCapEffUsePosGet
    entry _kerjmp_mbCapEffUseWanWanCreate
    b mbCapEffUseWanWanCreate
    entry _kerjmp_mbCapPlayerThrow
    b mbCapPlayerThrow
    entry _kerjmp_mbCapPlayerThrowCheck
    b mbCapPlayerThrowCheck
    entry _kerjmp_mbCapThrowColCreate
    b mbCapThrowColCreate
    entry _kerjmp_mbCapThrowColCheck
    b mbCapThrowColCheck
    entry _kerjmp_mbCapEffThrowCreate
    b mbCapEffThrowCreate
    entry _kerjmp_mbCapEffThrowCheck
    b mbCapEffThrowCheck
    entry _kerjmp_mbCapAutoThrow
    b mbCapAutoThrow
    entry _kerjmp_mbCapThrowHookSet
    b mbCapThrowHookSet
    entry _kerjmp_mbCapSelectMasu
    b mbCapSelectMasu
    entry _kerjmp_mbCapSelectMasuInit
    b mbCapSelectMasuInit
    entry _kerjmp_mbCapListInit
    b mbCapListInit
    entry _kerjmp_mbCapListRead
    b mbCapListRead
    entry _kerjmp_mbCapListCopy
    b mbCapListCopy
    entry _kerjmp_mbCapListDebug
    b mbCapListDebug
    entry _kerjmp_mbCapNumInc
    b mbCapNumInc
    entry _kerjmp_mbCapNumDebug
    b mbCapNumDebug
    entry _kerjmp_mbCapValueTypeGet
    b mbCapValueTypeGet
    entry _kerjmp_mbCapMasuTypeGet
    b mbCapMasuTypeGet
    entry _kerjmp_mbCapValuePlayerGet
    b mbCapValuePlayerGet
    entry _kerjmp_mbCapMasuPlayerGet2
    b mbCapMasuPlayerGet2
    entry _kerjmp_mbCapMasuPlayerSet
    b mbCapMasuPlayerSet
    entry _kerjmp_mbCapMasuPlayerTypeSet
    b mbCapMasuPlayerTypeSet
    entry _kerjmp_mbCapMasuPlayerGet
    b mbCapMasuPlayerGet
    entry _kerjmp_mbCapUseModeGet
    b mbCapUseModeGet
    entry _kerjmp_mbCapMasuDispTypeGet
    b mbCapMasuDispTypeGet
    entry _kerjmp_mbCapUseTrapCheck
    b mbCapUseTrapCheck
    entry _kerjmp_mbCapBuyCostGet
    b mbCapBuyCostGet
    entry _kerjmp_mbCapSellCostGet
    b mbCapSellCostGet
    entry _kerjmp_mbCapCostGet
    b mbCapCostGet
    entry _kerjmp_mbCapThrowMasuCheck
    b mbCapThrowMasuCheck
    entry _kerjmp_mbCapComChanceGet
    b mbCapComChanceGet
    entry _kerjmp_mbCapSelectComGet
    b mbCapSelectComGet
    entry _kerjmp_mbCapSelectDeleteComGet
    b mbCapSelectDeleteComGet
    entry _kerjmp_mbCapRandomThrowAdd
    b mbCapRandomThrowAdd
    entry _kerjmp_mbCapRandomTrapAdd
    b mbCapRandomTrapAdd
    entry _kerjmp_mbCapMasuCapsuleSet
    b mbCapMasuCapsuleSet
    entry _kerjmp_mbCapFileGet
    b mbCapFileGet
    entry _kerjmp_mbCapDescMesGet
    b mbCapDescMesGet
    entry _kerjmp_mbCapUseMesGet
    b mbCapUseMesGet
    entry _kerjmp_mbCapDebugNameGet
    b mbCapDebugNameGet
    entry _kerjmp_mbCapMasuPatGet
    b mbCapMasuPatGet
    entry _kerjmp_mbCapColorGet
    b mbCapColorGet
    entry _kerjmp_mbCapUseCostGet
    b mbCapUseCostGet
    entry _kerjmp_mbCapUseCheck
    b mbCapUseCheck
    entry _kerjmp_mbCapValidCheck
    b mbCapValidCheck
    entry _kerjmp_mbCapSelectMasuNum
    b mbCapSelectMasuNum
    entry _kerjmp_mbCapSelectMasuFrontNum
    b mbCapSelectMasuFrontNum
    entry _kerjmp_mbCapSelectMasuBackNum
    b mbCapSelectMasuBackNum
    entry _kerjmp_mbCapValidListGet
    b mbCapValidListGet
    entry _kerjmp_mbCapNextGet
    b mbCapNextGet
    entry _kerjmp_mbCapMasuNextGet
    b mbCapMasuNextGet
    entry _kerjmp_mbCapShopListGet
    b mbCapShopListGet
    entry _kerjmp_mbCapRandomListGet
    b mbCapRandomListGet
    entry _kerjmp_mbCapBonusCoinNumGet
    b mbCapBonusCoinNumGet
    entry _kerjmp_mbCapDescWinCreate
    b mbCapDescWinCreate
    entry _kerjmp_mbCapInit
    b mbCapInit
    entry _kerjmp_mbCapObjCreate
    b mbCapObjCreate
    entry _kerjmp_mbCapObjBorderCreate
    b mbCapObjBorderCreate
    entry _kerjmp_mbCapObjKill
    b mbCapObjKill
    entry _kerjmp_mbCapObjBorderKill
    b mbCapObjBorderKill
    entry _kerjmp_mbCapObjColorCreate
    b mbCapObjColorCreate
    entry _kerjmp_mbCapObjColorKill
    b mbCapObjColorKill
    entry _kerjmp_mbCapObjColorPosSet
    b mbCapObjColorPosSet
    entry _kerjmp_mbCapObjColorRotSet
    b mbCapObjColorRotSet
    entry _kerjmp_mbCapObjColorScaleSet
    b mbCapObjColorScaleSet
    entry _kerjmp_mbCapObjColorPosSetV
    b mbCapObjColorPosSetV
    entry _kerjmp_mbCapObjColorRotSetV
    b mbCapObjColorRotSetV
    entry _kerjmp_mbCapObjColorScaleSetV
    b mbCapObjColorScaleSetV
    entry _kerjmp_mbCapObjColorPosGet
    b mbCapObjColorPosGet
    entry _kerjmp_mbCapObjColorRotGet
    b mbCapObjColorRotGet
    entry _kerjmp_mbCapObjColorScaleGet
    b mbCapObjColorScaleGet
    entry _kerjmp_mbCapObjColorLayerSet
    b mbCapObjColorLayerSet
    entry _kerjmp_mbCapObjColorLayerGet
    b mbCapObjColorLayerGet
    entry _kerjmp_mbCapObjColorAttrSet
    b mbCapObjColorAttrSet
    entry _kerjmp_mbCapObjColorAttrReset
    b mbCapObjColorAttrReset
    entry _kerjmp_mbCapObjColorDispSet
    b mbCapObjColorDispSet
    entry _kerjmp_mbCapObjColorAlphaSet
    b mbCapObjColorAlphaSet
    entry _kerjmp_mbCapObjColorMtxSet
    b mbCapObjColorMtxSet
    entry _kerjmp_mbCapObjColorMtxGet
    b mbCapObjColorMtxGet
    entry _kerjmp_mbCoinInit
    b mbCoinInit
    entry _kerjmp_mbCoinClose
    b mbCoinClose
    entry _kerjmp_mbCoinEffObjCreate
    b mbCoinEffObjCreate
    entry _kerjmp_mbCoinEffCreate
    b mbCoinEffCreate
    entry _kerjmp_mbCoinDispKill
    b mbCoinDispKill
    entry _kerjmp_mbCoinDispKillCheck
    b mbCoinDispKillCheck
    entry _kerjmp_mbCoinDispCapsuleCreate
    b mbCoinDispCapsuleCreate
    entry _kerjmp_mbCoinDispMasuCreate
    b mbCoinDispMasuCreate
    entry _kerjmp_mbCoinDispCreate
    b mbCoinDispCreate
    entry _kerjmp_mbCoinAddProcExec
    b mbCoinAddProcExec
    entry _kerjmp_mbCoinAddDispExec
    b mbCoinAddDispExec
    entry _kerjmp_mbCoinAddExec
    b mbCoinAddExec
    entry _kerjmp_mbCoinAddAllProcExecV
    b mbCoinAddAllProcExecV
    entry _kerjmp_mbCoinAddAllProcExec
    b mbCoinAddAllProcExec
    entry _kerjmp_mbCoinAddAllExec
    b mbCoinAddAllExec
    entry _kerjmp_mbStatTeamMinValGet
    b mbStatTeamMinValGet
    entry _kerjmp_mbCoinCreate
    b mbCoinCreate
    entry _kerjmp_mbCoinCreate2
    b mbCoinCreate2
    entry _kerjmp_mbCoinObjCreate
    b mbCoinObjCreate
    entry _kerjmp_mbCoinObjCreate2
    b mbCoinObjCreate2
    entry _kerjmp_mbCoinObjNumDec
    b mbCoinObjNumDec
    entry _kerjmp_mbCoinObjKill
    b mbCoinObjKill
    entry _kerjmp_mbCoinObjGet
    b mbCoinObjGet
    entry _kerjmp_mbCoinObjPosSet
    b mbCoinObjPosSet
    entry _kerjmp_mbCoinObjPosSetV
    b mbCoinObjPosSetV
    entry _kerjmp_mbCoinObjRotSet
    b mbCoinObjRotSet
    entry _kerjmp_mbCoinObjRotSetV
    b mbCoinObjRotSetV
    entry _kerjmp_mbCoinObjScaleSet
    b mbCoinObjScaleSet
    entry _kerjmp_mbCoinObjScaleSetV
    b mbCoinObjScaleSetV
    entry _kerjmp_mbCoinObjAlphaSet
    b mbCoinObjAlphaSet
    entry _kerjmp_mbCoinObjDispSet
    b mbCoinObjDispSet
    entry _kerjmp_mbCoinObjLayerSet
    b mbCoinObjLayerSet
    entry _kerjmp_mbCoinObjMotSet
    b mbCoinObjMotSet
    entry _kerjmp_mbCoinObjPosGet
    b mbCoinObjPosGet
    entry _kerjmp_mbCoinObjRotGet
    b mbCoinObjRotGet
    entry _kerjmp_mbCoinObjScaleGet
    b mbCoinObjScaleGet
    entry _kerjmp_mbCoinObjAlphaGet
    b mbCoinObjAlphaGet
    entry _kerjmp_mbCoinObjDispGet
    b mbCoinObjDispGet
    entry _kerjmp_mbComChoiceLeftSet
    b mbComChoiceLeftSet
    entry _kerjmp_mbComChoiceUpSet
    b mbComChoiceUpSet
    entry _kerjmp_mbComChoiceRightSet
    b mbComChoiceRightSet
    entry _kerjmp_mbComChoiceDownSet
    b mbComChoiceDownSet
    entry _kerjmp_mbComChoiceListDownSet
    b mbComChoiceListDownSet
    entry _kerjmp_mbComChoiceListRightSet
    b mbComChoiceListRightSet
    entry _kerjmp_mbComChoiceListSet
    b mbComChoiceListSet
    entry _kerjmp_mbConfigExec
    b mbConfigExec
    entry _kerjmp_mbPauseDispCopyKill
    b mbPauseDispCopyKill
    entry _kerjmp_mbPauseDispCopyCreate
    b mbPauseDispCopyCreate
    entry _kerjmp_mbConfigPadDisableSet
    b mbConfigPadDisableSet
    entry _kerjmp_mbPauseGuideCreate
    b mbPauseGuideCreate
    entry _kerjmp_mbPauseGuideKill
    b mbPauseGuideKill
    entry _kerjmp_mbPausePanelCreate
    b mbPausePanelCreate
    entry _kerjmp_mbPausePanelKill
    b mbPausePanelKill
    entry _kerjmp_mbPausePanelPosSet
    b mbPausePanelPosSet
    entry _kerjmp_mbPausePanelRotSet
    b mbPausePanelRotSet
    entry _kerjmp_mbPausePanelScaleSet
    b mbPausePanelScaleSet
    entry _kerjmp_mbPausePanelBankSet
    b mbPausePanelBankSet
    entry _kerjmp_mbPausePanelBatsuSet
    b mbPausePanelBatsuSet
    entry _kerjmp_mbPausePanelPosGet
    b mbPausePanelPosGet
    entry _kerjmp_mbPausePanelScaleGet
    b mbPausePanelScaleGet
    entry _kerjmp_mbPausePanelFreezeGet
    b mbPausePanelFreezeGet
    entry _kerjmp_mbPausePanelUnlockSet
    b mbPausePanelUnlockSet
    entry _kerjmp_mbPausePanelSlideSet
    b mbPausePanelSlideSet
    entry _kerjmp_mbPausePanelSizeSet
    b mbPausePanelSizeSet
    entry _kerjmp_mbPausePanelAnmNoSet
    b mbPausePanelAnmNoSet
    entry _kerjmp_mbPausePanelGrowSet
    b mbPausePanelGrowSet
    entry _kerjmp_mbPausePanelShrinkSet
    b mbPausePanelShrinkSet
    entry _kerjmp_mbPauseGuideMoveSet
    b mbPauseGuideMoveSet
    entry _kerjmp_mbPadDStkRepGetAll
    b mbPadDStkRepGetAll
    entry _kerjmp_mbPadBtnDownGetAll
    b mbPadBtnDownGetAll
    entry _kerjmp_mbev_Last5
    b mbev_Last5
    entry _kerjmp_mbDiceInit
    b mbDiceInit
    entry _kerjmp_mbDicePlayerExec
    b mbDicePlayerExec
    entry _kerjmp_mbDiceExec
    b mbDiceExec
    entry _kerjmp_mbDiceProcExec
    b mbDiceProcExec
    entry _kerjmp_mbDiceChanceTradeExec
    b mbDiceChanceTradeExec
    entry _kerjmp_mbDiceChanceCharExec
    b mbDiceChanceCharExec
    entry _kerjmp_mbDiceFadeSet
    b mbDiceFadeSet
    entry _kerjmp_mbDiceKill
    b mbDiceKill
    entry _kerjmp_mbDiceClose
    b mbDiceClose
    entry _kerjmp_mbDiceKillCheck
    b mbDiceKillCheck
    entry _kerjmp_mbDiceKillCheckAll
    b mbDiceKillCheckAll
    entry _kerjmp_mbDiceResultGet
    b mbDiceResultGet
    entry _kerjmp_mbDiceHitHookSet
    b mbDiceHitHookSet
    entry _kerjmp_mbDiceTypeGet
    b mbDiceTypeGet
    entry _kerjmp_mbDiceTutorialNumSet
    b mbDiceTutorialNumSet
    entry _kerjmp_mbDiceTutorialNumGet
    b mbDiceTutorialNumGet
    entry _kerjmp_mbDiceValueNoGet
    b mbDiceValueNoGet
    entry _kerjmp_mbDiceNumKill
    b mbDiceNumKill
    entry _kerjmp_mbDiceNumShrinkSet
    b mbDiceNumShrinkSet
    entry _kerjmp_mbDiceNumStopCheck
    b mbDiceNumStopCheck
    entry _kerjmp_mbDiceStub
    b mbDiceStub
    entry _kerjmp_mbDiceSNpcNumCreate
    b mbDiceSNpcNumCreate
    entry _kerjmp_mbDiceNumObjCreate
    b mbDiceNumObjCreate
    entry _kerjmp_mbDiceMaxGet
    b mbDiceMaxGet
    entry _kerjmp_mbDiceValueMaxGet
    b mbDiceValueMaxGet
    entry _kerjmp_mbDicePadBtnHookSet
    b mbDicePadBtnHookSet
    entry _kerjmp_mbDiceMotHookSet
    b mbDiceMotHookSet
    entry _kerjmp_mbDiceObjHit
    b mbDiceObjHit
    entry _kerjmp_mbDiceSNpcNumObjCreate
    b mbDiceSNpcNumObjCreate
    entry _kerjmp_mbDiceSNpcNumKill
    b mbDiceSNpcNumKill
    entry _kerjmp_mbDiceSNpcNumDispSet
    b mbDiceSNpcNumDispSet
    entry _kerjmp_mbDiceSNpcNumSet
    b mbDiceSNpcNumSet
    entry _kerjmp_mbDiceSNpcNumPosSet
    b mbDiceSNpcNumPosSet
    entry _kerjmp_mbDiceSNpcNumOfsSet
    b mbDiceSNpcNumOfsSet
    entry _kerjmp_mbDiceSNpcNumOfsGet
    b mbDiceSNpcNumOfsGet
    entry _kerjmp_mbEffInit
    b mbEffInit
    entry _kerjmp_mbEffClose
    b mbEffClose
    entry _kerjmp_mbEffConfettiCreate
    b mbEffConfettiCreate
    entry _kerjmp_mbEffConfettiKill
    b mbEffConfettiKill
    entry _kerjmp_mbEffConfettiReset
    b mbEffConfettiReset
    entry _kerjmp_mbEffFadeOutSet
    b mbEffFadeOutSet
    entry _kerjmp_mbEffFadeCreate
    b mbEffFadeCreate
    entry _kerjmp_mbEffFadeDoneCheck
    b mbEffFadeDoneCheck
    entry _kerjmp_mbEffFadeCheck
    b mbEffFadeCheck
    entry _kerjmp_mbEffFadeCameraSet
    b mbEffFadeCameraSet
    entry _kerjmp_mbParticleCreate
    b mbParticleCreate
    entry _kerjmp_mbParticleKill
    b mbParticleKill
    entry _kerjmp_mbParticleHookSet
    b mbParticleHookSet
    entry _kerjmp_mbParticleAttrSet
    b mbParticleAttrSet
    entry _kerjmp_mbParticleAttrReset
    b mbParticleAttrReset
    entry _kerjmp_mbParticleUnkTotalGet
    b mbParticleUnkTotalGet
    entry _kerjmp_mbParticleDataCreate
    b mbParticleDataCreate
    entry _kerjmp_mbParticleColorCreate
    b mbParticleColorCreate
    entry _kerjmp_mbParManCreate
    b mbParManCreate
    entry _kerjmp_mbParManKill
    b mbParManKill
    entry _kerjmp_mbParManPosSet
    b mbParManPosSet
    entry _kerjmp_mbParManVecSet
    b mbParManVecSet
    entry _kerjmp_mbParManRotSet
    b mbParManRotSet
    entry _kerjmp_mbParManAttrSet
    b mbParManAttrSet
    entry _kerjmp_mbParManAttrReset
    b mbParManAttrReset
    entry _kerjmp_mbParticleBlendModeSet
    b mbParticleBlendModeSet
    entry _kerjmp_mbGateInit
    b mbGateInit
    entry _kerjmp_mbGateClose
    b mbGateClose
    entry _kerjmp_mbGateCreate
    b mbGateCreate
    entry _kerjmp_mbev_GateMasu
    b mbev_GateMasu
    entry _kerjmp_mbev_GateBattanMasuFind
    b mbev_GateBattanMasuFind
    entry _kerjmp_mbev_GateBattan
    b mbev_GateBattan
    entry _kerjmp_mbGateBattanKill
    b mbGateBattanKill
    entry _kerjmp_mbGuideInit
    b mbGuideInit
    entry _kerjmp_mbGuideCreateIn
    b mbGuideCreateIn
    entry _kerjmp_mbGuideCreateFlag
    b mbGuideCreateFlag
    entry _kerjmp_mbGuideCreate
    b mbGuideCreate
    entry _kerjmp_mbGuideKill
    b mbGuideKill
    entry _kerjmp_mbGuideEnd
    b mbGuideEnd
    entry _kerjmp_mbGuideFadeIn
    b mbGuideFadeIn
    entry _kerjmp_mbGuideFadeOut
    b mbGuideFadeOut
    entry _kerjmp_mbGuideModelGet
    b mbGuideModelGet
    entry _kerjmp_mbGuideIdleCheck
    b mbGuideIdleCheck
    entry _kerjmp_mbGuideScreenSet
    b mbGuideScreenSet
    entry _kerjmp_mbGuideMotionNextSet
    b mbGuideMotionNextSet
    entry _kerjmp_mbGuideMotionSet
    b mbGuideMotionSet
    entry _kerjmp_mbGuideMotionShiftSet
    b mbGuideMotionShiftSet
    entry _kerjmp_mbGuideMotionStop
    b mbGuideMotionStop
    entry _kerjmp_mbGuideMotionCheck
    b mbGuideMotionCheck
    entry _kerjmp_mbGuideNoGet
    b mbGuideNoGet
    entry _kerjmp_mbGuideSpeakerNoGet
    b mbGuideSpeakerNoGet
    entry _kerjmp_mbExitWatchCreate
    b mbExitWatchCreate
    entry _kerjmp_mbExitReq
    b mbExitReq
    entry _kerjmp_mbExitCheck
    b mbExitCheck
    entry _kerjmp_mbExitWatchProcGet
    b mbExitWatchProcGet
    entry _kerjmp_mbChangeTimeSet
    b mbChangeTimeSet
    entry _kerjmp_mbChangeTime
    b mbChangeTime
    entry _kerjmp_mbev_NextTimeSet
    b mbev_NextTimeSet
    entry _kerjmp_mbev_LoadTimeSet
    b mbev_LoadTimeSet
    entry _kerjmp_mbObjectSetup
    b mbObjectSetup
    entry _kerjmp_mbPauseEnableSet
    b mbPauseEnableSet
    entry _kerjmp_mbPauseEnableReset
    b mbPauseEnableReset
    entry _kerjmp_mbPauseEnableCheck
    b mbPauseEnableCheck
    entry _kerjmp_mbLightSet
    b mbLightSet
    entry _kerjmp_mbLightReset
    b mbLightReset
    entry _kerjmp_mbLightFuncSet
    b mbLightFuncSet
    entry _kerjmp_mbBGRead
    b mbBGRead
    entry _kerjmp_mbBGReadWait
    b mbBGReadWait
    entry _kerjmp_mbReturnMgCheck
    b mbReturnMgCheck
    entry _kerjmp_mbOvlCall
    b mbOvlCall
    entry _kerjmp_mbDirClose
    b mbDirClose
    entry _kerjmp_mbev_TurnStartSet
    b mbev_TurnStartSet
    entry _kerjmp_mbev_TurnEndSet
    b mbev_TurnEndSet
    entry _kerjmp_fn_8014C3B4
    b fn_8014C3B4
    entry _kerjmp_fn_8014C3BC
    b fn_8014C3BC
    entry _kerjmp_fn_8014C3C4
    b fn_8014C3C4
    entry _kerjmp_fn_8014C3CC
    b fn_8014C3CC
    entry _kerjmp_mbScrollInit
    b mbScrollInit
    entry _kerjmp_mbScrollClose
    b mbScrollClose
    entry _kerjmp_mbev_Scroll
    b mbev_Scroll
    entry _kerjmp_mbev_ScrollCapsule
    b mbev_ScrollCapsule
    entry _kerjmp_mbScrollStarFindFuncSet
    b mbScrollStarFindFuncSet
    entry _kerjmp_mbScrollHookSet
    b mbScrollHookSet
    entry _kerjmp_mbMapCameraSet
    b mbMapCameraSet
    entry _kerjmp_mbMapHookSet
    b mbMapHookSet
    entry _kerjmp_mbMapSprAdd
    b mbMapSprAdd
    entry _kerjmp_mbev_StarScroll
    b mbev_StarScroll
    entry _kerjmp_mbMasuInit
    b mbMasuInit
    entry _kerjmp_mbMasuClose
    b mbMasuClose
    entry _kerjmp_mbev_MasuStop
    b mbev_MasuStop
    entry _kerjmp_mbMasuDataRead
    b mbMasuDataRead
    entry _kerjmp_mbev_MasuMasuEnd
    b mbev_MasuMasuEnd
    entry _kerjmp_mbev_MasuMasuStart
    b mbev_MasuMasuStart
    entry _kerjmp_mbev_MasuCapStop
    b mbev_MasuCapStop
    entry _kerjmp_mbev_MasuMove
    b mbev_MasuMove
    entry _kerjmp_mbMasuNextSet
    b mbMasuNextSet
    entry _kerjmp_mbMasuNextDispSet
    b mbMasuNextDispSet
    entry _kerjmp_mbMasuNumGet
    b mbMasuNumGet
    entry _kerjmp_mbMasuRawNumGet
    b mbMasuRawNumGet
    entry _kerjmp_mbMasuGet
    b mbMasuGet
    entry _kerjmp_mbMasuLayerSet
    b mbMasuLayerSet
    entry _kerjmp_mbMasuLayerGet
    b mbMasuLayerGet
    entry _kerjmp_mbMasuAttrGet
    b mbMasuAttrGet
    entry _kerjmp_mbMasuAttrSet
    b mbMasuAttrSet
    entry _kerjmp_mbMasuMAttrGet
    b mbMasuMAttrGet
    entry _kerjmp_mbMasuMAttrSet
    b mbMasuMAttrSet
    entry _kerjmp_mbev_MasuBitGet
    b mbev_MasuBitGet
    entry _kerjmp_mbev_MasuAttrGet
    b mbev_MasuAttrGet
    entry _kerjmp_mbMasuTypeGet
    b mbMasuTypeGet
    entry _kerjmp_mbMasuTypeSet
    b mbMasuTypeSet
    entry _kerjmp_mbMasuTypeChange
    b mbMasuTypeChange
    entry _kerjmp_mbMasuDispCheck
    b mbMasuDispCheck
    entry _kerjmp_mbMasuCapsuleReset
    b mbMasuCapsuleReset
    entry _kerjmp_mbMasuCapsuleSet
    b mbMasuCapsuleSet
    entry _kerjmp_mbMasuCapsuleGet
    b mbMasuCapsuleGet
    entry _kerjmp_mbMasuPosGet
    b mbMasuPosGet
    entry _kerjmp_mbMasuPosSet
    b mbMasuPosSet
    entry _kerjmp_mbMasuPosSetV
    b mbMasuPosSetV
    entry _kerjmp_mbMasuCornerRotPosGet
    b mbMasuCornerRotPosGet
    entry _kerjmp_mbMasuCornerPosGet
    b mbMasuCornerPosGet
    entry _kerjmp_mbMasuRotGet
    b mbMasuRotGet
    entry _kerjmp_mbMasuRotSet
    b mbMasuRotSet
    entry _kerjmp_mbMasuRotSetV
    b mbMasuRotSetV
    entry _kerjmp_mbMasuMtxGet
    b mbMasuMtxGet
    entry _kerjmp_mbMasuMtxSet
    b mbMasuMtxSet
    entry _kerjmp_mbMasuDispGet
    b mbMasuDispGet
    entry _kerjmp_mbMasuDispSet
    b mbMasuDispSet
    entry _kerjmp_mbMasuDispAttrReset
    b mbMasuDispAttrReset
    entry _kerjmp_mbMasuDispAttrSet
    b mbMasuDispAttrSet
    entry _kerjmp_mbMasuDispAttrGet
    b mbMasuDispAttrGet
    entry _kerjmp_mbMasuDispMAttrReset
    b mbMasuDispMAttrReset
    entry _kerjmp_mbMasuDispMAttrSet
    b mbMasuDispMAttrSet
    entry _kerjmp_mbMasuDispMAttrGet
    b mbMasuDispMAttrGet
    entry _kerjmp_mbMasuModelDispSet
    b mbMasuModelDispSet
    entry _kerjmp_mbMasuLinkGet
    b mbMasuLinkGet
    entry _kerjmp_mbMasuLinkNumGet
    b mbMasuLinkNumGet
    entry _kerjmp_mbMasuLinkTblGet
    b mbMasuLinkTblGet
    entry _kerjmp_mbMasuLinkTblGet2
    b mbMasuLinkTblGet2
    entry _kerjmp_mbMasuAttrFindLink
    b mbMasuAttrFindLink
    entry _kerjmp_mbMasuAttrMatchFindLink
    b mbMasuAttrMatchFindLink
    entry _kerjmp_mbMasuMAttrFindLink
    b mbMasuMAttrFindLink
    entry _kerjmp_mbMasuMAttrMatchFindLink
    b mbMasuMAttrMatchFindLink
    entry _kerjmp_mbMasuTypeFindLink
    b mbMasuTypeFindLink
    entry _kerjmp_mbMasuLinkParentGet
    b mbMasuLinkParentGet
    entry _kerjmp_mbMasuFind_TypeStepGet
    b mbMasuFind_TypeStepGet
    entry _kerjmp_mbMasuFind_TypeStepGet2
    b mbMasuFind_TypeStepGet2
    entry _kerjmp_mbMasuFind_AttrStepGet
    b mbMasuFind_AttrStepGet
    entry _kerjmp_mbMasuFind_AttrMatchStepGet
    b mbMasuFind_AttrMatchStepGet
    entry _kerjmp_mbMasuFind_AttrStepGet2
    b mbMasuFind_AttrStepGet2
    entry _kerjmp_mbMasuFind_MAttrStepGet
    b mbMasuFind_MAttrStepGet
    entry _kerjmp_mbMasuFind_MAttrMatchStepGet
    b mbMasuFind_MAttrMatchStepGet
    entry _kerjmp_mbMasuFind_MAttrStepGet2
    b mbMasuFind_MAttrStepGet2
    entry _kerjmp_mbMasuFind_IdStepGet
    b mbMasuFind_IdStepGet
    entry _kerjmp_mbMasuFind_IdStepGet2
    b mbMasuFind_IdStepGet2
    entry _kerjmp_mbMasuFind_TypeSearch
    b mbMasuFind_TypeSearch
    entry _kerjmp_mbMasuFind_TypeIdGet
    b mbMasuFind_TypeIdGet
    entry _kerjmp_mbMasuFind_AttrIdGet
    b mbMasuFind_AttrIdGet
    entry _kerjmp_mbMasuFind_AttrMatchIdGet
    b mbMasuFind_AttrMatchIdGet
    entry _kerjmp_mbMasuFind_AttrMatchIdGet2
    b mbMasuFind_AttrMatchIdGet2
    entry _kerjmp_mbMasuFind_MAttrIdGet
    b mbMasuFind_MAttrIdGet
    entry _kerjmp_mbMasuFind_MAttrMatchIdGet
    b mbMasuFind_MAttrMatchIdGet
    entry _kerjmp_mbMasuFind_MAttrMatchIdGet2
    b mbMasuFind_MAttrMatchIdGet2
    entry _kerjmp_mbMasuTypeListGet
    b mbMasuTypeListGet
    entry _kerjmp_mbMasuAttrListGet
    b mbMasuAttrListGet
    entry _kerjmp_mbMasuAttrMatchListGet
    b mbMasuAttrMatchListGet
    entry _kerjmp_mbMasuMAttrListGet
    b mbMasuMAttrListGet
    entry _kerjmp_mbMasuMAttrMatchTblGet
    b mbMasuMAttrMatchTblGet
    entry _kerjmp_mbMasuFind_TypeListGet
    b mbMasuFind_TypeListGet
    entry _kerjmp_mbMasuFind_TypeListGet2
    b mbMasuFind_TypeListGet2
    entry _kerjmp_mbMasuFind_AttrNumGet
    b mbMasuFind_AttrNumGet
    entry _kerjmp_mbMasuFind_AttrMatchNumGet
    b mbMasuFind_AttrMatchNumGet
    entry _kerjmp_mbMasuFind_AttrMatchNumGet2
    b mbMasuFind_AttrMatchNumGet2
    entry _kerjmp_mbMasuFind_MAttrNumGet
    b mbMasuFind_MAttrNumGet
    entry _kerjmp_mbMasuFind_MAttrMatchListGet
    b mbMasuFind_MAttrMatchListGet
    entry _kerjmp_mbMasuFind_MAttrMatchListGet2
    b mbMasuFind_MAttrMatchListGet2
    entry _kerjmp_mbMasuFind_IdListGet
    b mbMasuFind_IdListGet
    entry _kerjmp_mbMasuFind_IdListGet2
    b mbMasuFind_IdListGet2
    entry _kerjmp_mbev_MasuMoveEndSet
    b mbev_MasuMoveEndSet
    entry _kerjmp_mbev_MasuHatenaSet
    b mbev_MasuHatenaSet
    entry _kerjmp_mbev_MasuMoveStartSet
    b mbev_MasuMoveStartSet
    entry _kerjmp_mbev_MasuLinkTblHookSet
    b mbev_MasuLinkTblHookSet
    entry _kerjmp_mbMasuPlayerDispSet
    b mbMasuPlayerDispSet
    entry _kerjmp_mbMasuPlayerFadeSet
    b mbMasuPlayerFadeSet
    entry _kerjmp_mbMasuPlayerCapMoveCheck
    b mbMasuPlayerCapMoveCheck
    entry _kerjmp_mbMasuPlayerCapStopCheck
    b mbMasuPlayerCapStopCheck
    entry _kerjmp_mbMasuStub
    b mbMasuStub
    entry _kerjmp_mbMasuPlayerColorSet
    b mbMasuPlayerColorSet
    entry _kerjmp_mbMasuPlayerPrizeReset
    b mbMasuPlayerPrizeReset
    entry _kerjmp_mbMasuPKinokoValueGet
    b mbMasuPKinokoValueGet
    entry _kerjmp_mbMathInit
    b mbMathInit
    entry _kerjmp_mbMathClose
    b mbMathClose
    entry _kerjmp_mbRandMod
    b mbRandMod
    entry _kerjmp_mbVecMagCheck
    b mbVecMagCheck
    entry _kerjmp_mbAngleAdd
    b mbAngleAdd
    entry _kerjmp_mbAngleMoveTo
    b mbAngleMoveTo
    entry _kerjmp_mbAngleWrap2
    b mbAngleWrap2
    entry _kerjmp_mbVecMagXZ
    b mbVecMagXZ
    entry _kerjmp_mbMtxLookAtCalc
    b mbMtxLookAtCalc
    entry _kerjmp_mbAngleWrapV
    b mbAngleWrapV
    entry _kerjmp_mbAngleWrap
    b mbAngleWrap
    entry _kerjmp_mbVecMagXZCheck
    b mbVecMagXZCheck
    entry _kerjmp_mbBezierCalc
    b mbBezierCalc
    entry _kerjmp_mbBezierCalcV
    b mbBezierCalcV
    entry _kerjmp_mbBezierCalcVList
    b mbBezierCalcVList
    entry _kerjmp_mbBezierCalcSlope
    b mbBezierCalcSlope
    entry _kerjmp_mbBezierCalcSlopeV
    b mbBezierCalcSlopeV
    entry _kerjmp_mbHermiteCalc
    b mbHermiteCalc
    entry _kerjmp_mbHermiteCalcV
    b mbHermiteCalcV
    entry _kerjmp_mbHermiteCalcSlope
    b mbHermiteCalcSlope
    entry _kerjmp_mbPos3Dto2D
    b mbPos3Dto2D
    entry _kerjmp_mbPos2Dto3D
    b mbPos2Dto3D
    entry _kerjmp_mbPos3DtoNorm
    b mbPos3DtoNorm
    entry _kerjmp_mbNormPosto3D
    b mbNormPosto3D
    entry _kerjmp_mbNormPosto2D
    b mbNormPosto2D
    entry _kerjmp_mbAngleLerp
    b mbAngleLerp
    entry _kerjmp_mbAngleEaseOut
    b mbAngleEaseOut
    entry _kerjmp_mbAngleEaseIn
    b mbAngleEaseIn
    entry _kerjmp_mbMathDistScale
    b mbMathDistScale
    entry _kerjmp_mbCosDeg
    b mbCosDeg
    entry _kerjmp_mbSinDeg
    b mbSinDeg
    entry _kerjmp_mbCosRad
    b mbCosRad
    entry _kerjmp_mbSinRad
    b mbSinRad
    entry _kerjmp_mbMtxRot
    b mbMtxRot
    entry _kerjmp_mbMtxRotAxisDeg
    b mbMtxRotAxisDeg
    entry _kerjmp_mbMtxRotAxisRad
    b mbMtxRotAxisRad
    entry _kerjmp_mbMtxRotXDeg
    b mbMtxRotXDeg
    entry _kerjmp_mbMtxRotXRad
    b mbMtxRotXRad
    entry _kerjmp_mbMtxRotYDeg
    b mbMtxRotYDeg
    entry _kerjmp_mbMtxRotYRad
    b mbMtxRotYRad
    entry _kerjmp_mbMtxRotZDeg
    b mbMtxRotZDeg
    entry _kerjmp_mbMtxRotZRad
    b mbMtxRotZRad
    entry _kerjmp_mbMtxScaleRotXDeg
    b mbMtxScaleRotXDeg
    entry _kerjmp_mbMtxScaleRotYDeg
    b mbMtxScaleRotYDeg
    entry _kerjmp_mbMtxScaleRotZDeg
    b mbMtxScaleRotZDeg
    entry _kerjmp_mbMtxRotTrigX
    b mbMtxRotTrigX
    entry _kerjmp_mbMtxRotTrigY
    b mbMtxRotTrigY
    entry _kerjmp_mbMtxRotTrigZ
    b mbMtxRotTrigZ
    entry _kerjmp_mbMtxRotTrigScaleX
    b mbMtxRotTrigScaleX
    entry _kerjmp_mbMtxRotTrigScaleY
    b mbMtxRotTrigScaleY
    entry _kerjmp_mbMtxRotTrigScaleZ
    b mbMtxRotTrigScaleZ
    entry _kerjmp_mbMtxTransCat
    b mbMtxTransCat
    entry _kerjmp_mbObjCullInit
    b mbObjCullInit
    entry _kerjmp_mbTelopPlayerCreate
    b mbTelopPlayerCreate
    entry _kerjmp_mbTelopPlayerSkipCreate
    b mbTelopPlayerSkipCreate
    entry _kerjmp_mbTelopCreate
    b mbTelopCreate
    entry _kerjmp_mbTelopCheck
    b mbTelopCheck
    entry _kerjmp_mbTelopLastTurnCreate
    b mbTelopLastTurnCreate
    entry _kerjmp_mbPadStkXGet
    b mbPadStkXGet
    entry _kerjmp_mbPadStkYGet
    b mbPadStkYGet
    entry _kerjmp_mbTauntInit
    b mbTauntInit
    entry _kerjmp_mbTauntClose
    b mbTauntClose
    entry _kerjmp_mbLanguageGet
    b mbLanguageGet
    entry _kerjmp_mbLanguageSet
    b mbLanguageSet
    entry _kerjmp_mbBoardDataNumGet
    b mbBoardDataNumGet
    entry _kerjmp_mbBoardDataDirRead
    b mbBoardDataDirRead
    entry _kerjmp_mbTelopTimeChangeCreate
    b mbTelopTimeChangeCreate
    entry _kerjmp_mbTelopTimeChangeCheck
    b mbTelopTimeChangeCheck
    entry _kerjmp_mbTelopTimeChangeKill
    b mbTelopTimeChangeKill
    entry _kerjmp_mbTelopTimeCreate
    b mbTelopTimeCreate
    entry _kerjmp_mbTelopTimeSprCreate
    b mbTelopTimeSprCreate
    entry _kerjmp_mbTelopTimeSprKill
    b mbTelopTimeSprKill
    entry _kerjmp_mbTelopTimeDispSet
    b mbTelopTimeDispSet
    entry _kerjmp_mbTelopTimeSprRotSet
    b mbTelopTimeSprRotSet
    entry _kerjmp_mbTelopTimeStarSet
    b mbTelopTimeStarSet
    entry _kerjmp_mbTelopTimeTPLvlSet
    b mbTelopTimeTPLvlSet
    entry _kerjmp_mbObjInit
    b mbObjInit
    entry _kerjmp_mbObjClose
    b mbObjClose
    entry _kerjmp_mbObjKill
    b mbObjKill
    entry _kerjmp_mbObjDirSet
    b mbObjDirSet
    entry _kerjmp_mbObjDataNumGet
    b mbObjDataNumGet
    entry _kerjmp_mbObjCreate
    b mbObjCreate
    entry _kerjmp_mbObjCharCreate
    b mbObjCharCreate
    entry _kerjmp_mbObjGet
    b mbObjGet
    entry _kerjmp_mbObjDispGet
    b mbObjDispGet
    entry _kerjmp_mbObjDispSet
    b mbObjDispSet
    entry _kerjmp_mbObjCameraSet
    b mbObjCameraSet
    entry _kerjmp_mbObjLayerSet
    b mbObjLayerSet
    entry _kerjmp_mbObjLayerGet
    b mbObjLayerGet
    entry _kerjmp_mbObjLayerSetAll
    b mbObjLayerSetAll
    entry _kerjmp_mbObjModelIDGet
    b mbObjModelIDGet
    entry _kerjmp_mbObjPosGet
    b mbObjPosGet
    entry _kerjmp_mbObjPosSetV
    b mbObjPosSetV
    entry _kerjmp_mbObjPosSet
    b mbObjPosSet
    entry _kerjmp_mbObjOffsetGet
    b mbObjOffsetGet
    entry _kerjmp_mbObjOffsetSetV
    b mbObjOffsetSetV
    entry _kerjmp_mbObjOffsetSet
    b mbObjOffsetSet
    entry _kerjmp_mbObjRotGet
    b mbObjRotGet
    entry _kerjmp_mbObjRotSetV
    b mbObjRotSetV
    entry _kerjmp_mbObjRotSet
    b mbObjRotSet
    entry _kerjmp_mbObjRotYGet
    b mbObjRotYGet
    entry _kerjmp_mbObjRotYSet
    b mbObjRotYSet
    entry _kerjmp_mbObjScaleGet
    b mbObjScaleGet
    entry _kerjmp_mbObjScaleSetV
    b mbObjScaleSetV
    entry _kerjmp_mbObjScaleSet
    b mbObjScaleSet
    entry _kerjmp_mbObjMtxGet
    b mbObjMtxGet
    entry _kerjmp_mbObjMtxSet
    b mbObjMtxSet
    entry _kerjmp_mbObjAttrSet
    b mbObjAttrSet
    entry _kerjmp_mbObjAttrReset
    b mbObjAttrReset
    entry _kerjmp_mbObjAmbSet
    b mbObjAmbSet
    entry _kerjmp_mbObjAmbSetAll
    b mbObjAmbSetAll
    entry _kerjmp_mbObjAlphaGet
    b mbObjAlphaGet
    entry _kerjmp_mbObjAlphaSet
    b mbObjAlphaSet
    entry _kerjmp_mbObjFadeSet
    b mbObjFadeSet
    entry _kerjmp_mbObjZWriteOffSet
    b mbObjZWriteOffSet
    entry _kerjmp_mbObjSePlay
    b mbObjSePlay
    entry _kerjmp_mbObjMotionVoiceOnSet
    b mbObjMotionVoiceOnSet
    entry _kerjmp_mbObjCullRadiusSet
    b mbObjCullRadiusSet
    entry _kerjmp_mbObjHookSet
    b mbObjHookSet
    entry _kerjmp_mbObjHookReset
    b mbObjHookReset
    entry _kerjmp_mbObjHookObjReset
    b mbObjHookObjReset
    entry _kerjmp_mbObjMotionGet
    b mbObjMotionGet
    entry _kerjmp_mbObjMotionSet
    b mbObjMotionSet
    entry _kerjmp_mbObjMotionIDCurGet
    b mbObjMotionIDCurGet
    entry _kerjmp_mbObjMotionIDGet
    b mbObjMotionIDGet
    entry _kerjmp_mbObjMotionShiftSet
    b mbObjMotionShiftSet
    entry _kerjmp_mbObjMotionShiftIDGet
    b mbObjMotionShiftIDGet
    entry _kerjmp_mbObjMotionNoCreate
    b mbObjMotionNoCreate
    entry _kerjmp_mbObjMotionCreate
    b mbObjMotionCreate
    entry _kerjmp_mbObjMotionKill
    b mbObjMotionKill
    entry _kerjmp_mbObjMotionTimeGet
    b mbObjMotionTimeGet
    entry _kerjmp_mbObjMotionTimeSet
    b mbObjMotionTimeSet
    entry _kerjmp_mbObjMotionMaxTimeGet
    b mbObjMotionMaxTimeGet
    entry _kerjmp_mbObjMotionSpeedGet
    b mbObjMotionSpeedGet
    entry _kerjmp_mbObjMotionSpeedSet
    b mbObjMotionSpeedSet
    entry _kerjmp_mbObjMotionEndCheck
    b mbObjMotionEndCheck
    entry _kerjmp_mbObjMotionStartEndSet
    b mbObjMotionStartEndSet
    entry _kerjmp_mbObjMotionLoopSet
    b mbObjMotionLoopSet
    entry _kerjmp_mbObjMotionShapeSet
    b mbObjMotionShapeSet
    entry _kerjmp_mbObjMotionShapeTimeGet
    b mbObjMotionShapeTimeGet
    entry _kerjmp_mbObjMotionShapeTimeSet
    b mbObjMotionShapeTimeSet
    entry _kerjmp_mbObjMotionShapeMaxTimeGet
    b mbObjMotionShapeMaxTimeGet
    entry _kerjmp_mbObjMotionShapeSpeedGet
    b mbObjMotionShapeSpeedGet
    entry _kerjmp_mbObjMotionShapeSpeedSet
    b mbObjMotionShapeSpeedSet
    entry _kerjmp_mbObjMotionShapeEndCheck
    b mbObjMotionShapeEndCheck
    entry _kerjmp_mbObjMotionShapeStartEndSet
    b mbObjMotionShapeStartEndSet
    entry _kerjmp_mbObjShadowSet
    b mbObjShadowSet
    entry _kerjmp_mbObjShadowReset
    b mbObjShadowReset
    entry _kerjmp_mbObjShadowMapSet
    b mbObjShadowMapSet
    entry _kerjmp_mbObjShadowMapReset
    b mbObjShadowMapReset
    entry _kerjmp_mbObjStub0
    b mbObjStub0
    entry _kerjmp_mbev_OpeningParty
    b mbev_OpeningParty
    entry _kerjmp_mbev_OpeningSingle
    b mbev_OpeningSingle
    entry _kerjmp_mbev_Opening
    b mbev_Opening
    entry _kerjmp_mbOpeningViewSet
    b mbOpeningViewSet
    entry _kerjmp_mbOpeningZoomGet
    b mbOpeningZoomGet
    entry _kerjmp_mbOpeningRotGet
    b mbOpeningRotGet
    entry _kerjmp_mbOpeningPosGet
    b mbOpeningPosGet
    entry _kerjmp_mbOpeningInstHookSet
    b mbOpeningInstHookSet
    entry _kerjmp_mbOpeningStarInstHookSet
    b mbOpeningStarInstHookSet
    entry _kerjmp_mbOpeningGuideObjIdGet
    b mbOpeningGuideObjIdGet
    entry _kerjmp_mbOpeningGuidePosRestore
    b mbOpeningGuidePosRestore
    entry _kerjmp_mbOpeningCameraPosRestore
    b mbOpeningCameraPosRestore
    entry _kerjmp_mbPauseInit
    b mbPauseInit
    entry _kerjmp_mbPauseCreate
    b mbPauseCreate
    entry _kerjmp_mbPauseStartCheck
    b mbPauseStartCheck
    entry _kerjmp_mbPauseProcCheck
    b mbPauseProcCheck
    entry _kerjmp_mbPauseDisableSet
    b mbPauseDisableSet
    entry _kerjmp_mbPauseDisableGet
    b mbPauseDisableGet
    entry _kerjmp_mbPauseHookPush
    b mbPauseHookPush
    entry _kerjmp_mbPauseHookPop
    b mbPauseHookPop
    entry _kerjmp_mbPauseSet
    b mbPauseSet
    entry _kerjmp_mbPauseDataDirGet
    b mbPauseDataDirGet
    entry _kerjmp_mbPauseDataNumGet
    b mbPauseDataNumGet
    entry _kerjmp_mbPlayerMetalSet
    b mbPlayerMetalSet
    entry _kerjmp_mbPlayerEffectSet
    b mbPlayerEffectSet
    entry _kerjmp_mbPlayerMetalColorSet
    b mbPlayerMetalColorSet
    entry _kerjmp_mbPlayerBiriQSet
    b mbPlayerBiriQSet
    entry _kerjmp_mbPlayerInit
    b mbPlayerInit
    entry _kerjmp_mbPlayerClose
    b mbPlayerClose
    entry _kerjmp_mbTurnExec
    b mbTurnExec
    entry _kerjmp_mbSingleTurnExec
    b mbSingleTurnExec
    entry _kerjmp_mbPlayerSwap
    b mbPlayerSwap
    entry _kerjmp_mbPlayerNameMesGet
    b mbPlayerNameMesGet
    entry _kerjmp_mbPlayerNameGet
    b mbPlayerNameGet
    entry _kerjmp_mbPlayerTagNameMesGet
    b mbPlayerTagNameMesGet
    entry _kerjmp_mbPlayerColorGet
    b mbPlayerColorGet
    entry _kerjmp_mbPlayerAmbSet
    b mbPlayerAmbSet
    entry _kerjmp_mbPlayerWorkGet
    b mbPlayerWorkGet
    entry _kerjmp_mbPlayerAllComCheck
    b mbPlayerAllComCheck
    entry _kerjmp_mbPlayerObjIDGet
    b mbPlayerObjIDGet
    entry _kerjmp_mbPlayerModelIDGet
    b mbPlayerModelIDGet
    entry _kerjmp_mbPlayerDispSet
    b mbPlayerDispSet
    entry _kerjmp_mbPlayerDispGet
    b mbPlayerDispGet
    entry _kerjmp_mbPlayerLayerSet
    b mbPlayerLayerSet
    entry _kerjmp_mbPlayerCameraSet
    b mbPlayerCameraSet
    entry _kerjmp_mbPlayerCullRadiusSet
    b mbPlayerCullRadiusSet
    entry _kerjmp_mbPlayerStubValSet
    b mbPlayerStubValSet
    entry _kerjmp_mbPlayerMtxSet
    b mbPlayerMtxSet
    entry _kerjmp_mbPlayerMtxGet
    b mbPlayerMtxGet
    entry _kerjmp_mbPlayerPosSet
    b mbPlayerPosSet
    entry _kerjmp_mbPlayerPosSetV
    b mbPlayerPosSetV
    entry _kerjmp_mbPlayerPosGet
    b mbPlayerPosGet
    entry _kerjmp_mbPlayerRotSet
    b mbPlayerRotSet
    entry _kerjmp_mbPlayerRotSetV
    b mbPlayerRotSetV
    entry _kerjmp_mbPlayerRotGet
    b mbPlayerRotGet
    entry _kerjmp_mbPlayerRotYSet
    b mbPlayerRotYSet
    entry _kerjmp_mbPlayerRotYGet
    b mbPlayerRotYGet
    entry _kerjmp_mbPlayerScaleSet
    b mbPlayerScaleSet
    entry _kerjmp_mbPlayerScaleSetV
    b mbPlayerScaleSetV
    entry _kerjmp_mbPlayerScaleGet
    b mbPlayerScaleGet
    entry _kerjmp_mbPlayerMotionGet
    b mbPlayerMotionGet
    entry _kerjmp_mbPlayerMotionEndCheck
    b mbPlayerMotionEndCheck
    entry _kerjmp_mbPlayerMotionEndCheckAll
    b mbPlayerMotionEndCheckAll
    entry _kerjmp_mbPlayerMotionEndWait
    b mbPlayerMotionEndWait
    entry _kerjmp_mbPlayerMotionMaxTimeGet
    b mbPlayerMotionMaxTimeGet
    entry _kerjmp_mbPlayerMotionTimeGet
    b mbPlayerMotionTimeGet
    entry _kerjmp_mbPlayerMotionSet
    b mbPlayerMotionSet
    entry _kerjmp_mbPlayerMotionVoiceOnSet
    b mbPlayerMotionVoiceOnSet
    entry _kerjmp_mbPlayerMotionShiftSet
    b mbPlayerMotionShiftSet
    entry _kerjmp_mbPlayerMotionCreate
    b mbPlayerMotionCreate
    entry _kerjmp_mbPlayerMotionKill
    b mbPlayerMotionKill
    entry _kerjmp_mbPlayerMotionSpeedSet
    b mbPlayerMotionSpeedSet
    entry _kerjmp_mbPlayerMotionTimeSet
    b mbPlayerMotionTimeSet
    entry _kerjmp_mbPlayerMotionStartEndSet
    b mbPlayerMotionStartEndSet
    entry _kerjmp_mbPlayerAttrSet
    b mbPlayerAttrSet
    entry _kerjmp_mbPlayerAttrReset
    b mbPlayerAttrReset
    entry _kerjmp_mbPlayerMotIdleSet
    b mbPlayerMotIdleSet
    entry _kerjmp_mbPlayerRotateStart
    b mbPlayerRotateStart
    entry _kerjmp_mbPlayerRotateCheck
    b mbPlayerRotateCheck
    entry _kerjmp_mbPlayerRotateCheckAll
    b mbPlayerRotateCheckAll
    entry _kerjmp_mbPlayerEyeMatDarkSet
    b mbPlayerEyeMatDarkSet
    entry _kerjmp_mbPlayerRankGet
    b mbPlayerRankGet
    entry _kerjmp_mbPlayerCapsuleUseGet
    b mbPlayerCapsuleUseGet
    entry _kerjmp_mbPlayerCapsuleUseSet
    b mbPlayerCapsuleUseSet
    entry _kerjmp_mbPlayerCapsuleRemove
    b mbPlayerCapsuleRemove
    entry _kerjmp_mbPlayerCapsuleFind
    b mbPlayerCapsuleFind
    entry _kerjmp_mbPlayerCapsuleAdd
    b mbPlayerCapsuleAdd
    entry _kerjmp_mbPlayerCapsuleNumGet
    b mbPlayerCapsuleNumGet
    entry _kerjmp_mbPlayerCapsuleGet
    b mbPlayerCapsuleGet
    entry _kerjmp_mbPlayerCapsuleMaxGet
    b mbPlayerCapsuleMaxGet
    entry _kerjmp_mbPlayerCoinAdd
    b mbPlayerCoinAdd
    entry _kerjmp_mbPlayerCoinSet
    b mbPlayerCoinSet
    entry _kerjmp_mbPlayerCoinGet
    b mbPlayerCoinGet
    entry _kerjmp_mbPlayerMaxCoinGet
    b mbPlayerMaxCoinGet
    entry _kerjmp_mbPlayerStarAdd
    b mbPlayerStarAdd
    entry _kerjmp_mbPlayerStarSet
    b mbPlayerStarSet
    entry _kerjmp_mbPlayerStarGet
    b mbPlayerStarGet
    entry _kerjmp_mbPlayerTeamFind
    b mbPlayerTeamFind
    entry _kerjmp_mbPlayerTeamFindOther
    b mbPlayerTeamFindOther
    entry _kerjmp_mbPlayerTeamCoinSet
    b mbPlayerTeamCoinSet
    entry _kerjmp_mbPlayerTeamCoinGet
    b mbPlayerTeamCoinGet
    entry _kerjmp_mbPlayerGrpStarSet
    b mbPlayerGrpStarSet
    entry _kerjmp_mbPlayerGrpStarGet
    b mbPlayerGrpStarGet
    entry _kerjmp_mbPlayerTeamRankGet
    b mbPlayerTeamRankGet
    entry _kerjmp_mbPlayerTeamFindPlayer
    b mbPlayerTeamFindPlayer
    entry _kerjmp_mbPlayerTeamCapsuleGet
    b mbPlayerTeamCapsuleGet
    entry _kerjmp_mbPlayerTeamCapsuleNumGet
    b mbPlayerTeamCapsuleNumGet
    entry _kerjmp_mbPlayerTeamFindOpp
    b mbPlayerTeamFindOpp
    entry _kerjmp_mbPlayerTeamGet
    b mbPlayerTeamGet
    entry _kerjmp_mbPlayerTeamCheckSame
    b mbPlayerTeamCheckSame
    entry _kerjmp_mbPlayerWalkSpeedGet
    b mbPlayerWalkSpeedGet
    entry _kerjmp_mbPlayerMoveExec
    b mbPlayerMoveExec
    entry _kerjmp_mbPlayerMoveMain
    b mbPlayerMoveMain
    entry _kerjmp_mbev_PlayerColReserve
    b mbev_PlayerColReserve
    entry _kerjmp_mbev_PlayerColSet
    b mbev_PlayerColSet
    entry _kerjmp_mbMoveNumCreateColor
    b mbMoveNumCreateColor
    entry _kerjmp_mbMoveNumCreate
    b mbMoveNumCreate
    entry _kerjmp_mbMoveNumKill
    b mbMoveNumKill
    entry _kerjmp_mbMoveNumDispSet
    b mbMoveNumDispSet
    entry _kerjmp_mbPlayerMasuMoveTo
    b mbPlayerMasuMoveTo
    entry _kerjmp_mbPlayerMasuMove
    b mbPlayerMasuMove
    entry _kerjmp_mbPlayerMasuMovePos
    b mbPlayerMasuMovePos
    entry _kerjmp_mbPlayerMasuMoveSpeed
    b mbPlayerMasuMoveSpeed
    entry _kerjmp_mbPlayerColSnapSet
    b mbPlayerColSnapSet
    entry _kerjmp_mbev_PlayerColMasuSet
    b mbev_PlayerColMasuSet
    entry _kerjmp_mbev_PlayerColMasuAllSet
    b mbev_PlayerColMasuAllSet
    entry _kerjmp_mbev_PlayerColMasuAdd
    b mbev_PlayerColMasuAdd
    entry _kerjmp_mbev_PlayerColMasu
    b mbev_PlayerColMasu
    entry _kerjmp_mbev_PlayerColCircleAdd
    b mbev_PlayerColCircleAdd
    entry _kerjmp_mbev_PlayerColBall
    b mbev_PlayerColBall
    entry _kerjmp_mbPlayerColSnapPlayerSet
    b mbPlayerColSnapPlayerSet
    entry _kerjmp_mbPlayerColSnapGet
    b mbPlayerColSnapGet
    entry _kerjmp_mbPlayerColRestSet
    b mbPlayerColRestSet
    entry _kerjmp_mbPlayerColCheck
    b mbPlayerColCheck
    entry _kerjmp_mbPlayerBestPathGet
    b mbPlayerBestPathGet
    entry _kerjmp_mbPlayerPosReset
    b mbPlayerPosReset
    entry _kerjmp_mbPlayerPosResetAll
    b mbPlayerPosResetAll
    entry _kerjmp_mbPlayerMasuCornerSet
    b mbPlayerMasuCornerSet
    entry _kerjmp_mbPlayerMasuCornerGet
    b mbPlayerMasuCornerGet
    entry _kerjmp_mbPlayerDiceMotExec
    b mbPlayerDiceMotExec
    entry _kerjmp_mbPlayerKettouCheck
    b mbPlayerKettouCheck
    entry _kerjmp_mbPlayerBlackoutSet
    b mbPlayerBlackoutSet
    entry _kerjmp_mbPlayerBlackoutGet
    b mbPlayerBlackoutGet
    entry _kerjmp_mbPlayerWinLoseVoicePlay
    b mbPlayerWinLoseVoicePlay
    entry _kerjmp_mbPlayerVoicePlay
    b mbPlayerVoicePlay
    entry _kerjmp_mbPlayerVoicePanPlay
    b mbPlayerVoicePanPlay
    entry _kerjmp_mbPlayerStartTurnHookSet
    b mbPlayerStartTurnHookSet
    entry _kerjmp_mbPlayerEndTurnHookSet
    b mbPlayerEndTurnHookSet
    entry _kerjmp_mbPlayerMoveHookSet
    b mbPlayerMoveHookSet
    entry _kerjmp_mbPlayerDiceTypeGet
    b mbPlayerDiceTypeGet
    entry _kerjmp_mbPlayerTurnInitHookSet
    b mbPlayerTurnInitHookSet
    entry _kerjmp_mbPlayerTurnCloseHookSet
    b mbPlayerTurnCloseHookSet
    entry _kerjmp_mbPlayerColFirstSet
    b mbPlayerColFirstSet
    entry _kerjmp_mbPlayerColOrderReset
    b mbPlayerColOrderReset
    entry _kerjmp_mbPlayerPlusMasuExec
    b mbPlayerPlusMasuExec
    entry _kerjmp_mbPlayerMinusMasuExec
    b mbPlayerMinusMasuExec
    entry _kerjmp_mbPlayerCapCoinMasuExec
    b mbPlayerCapCoinMasuExec
    entry _kerjmp_mbRouletteCreate
    b mbRouletteCreate
    entry _kerjmp_mbRouletteKaneCreate
    b mbRouletteKaneCreate
    entry _kerjmp_mbRouletteCheck
    b mbRouletteCheck
    entry _kerjmp_mbRouletteWait
    b mbRouletteWait
    entry _kerjmp_mbRouletteResultGet
    b mbRouletteResultGet
    entry _kerjmp_mbRouletteMaxSpeedSet
    b mbRouletteMaxSpeedSet
    entry _kerjmp_mbRouletteComValueSet
    b mbRouletteComValueSet
    entry _kerjmp_mbSingleInit
    b mbSingleInit
    entry _kerjmp_mbSingleClose
    b mbSingleClose
    entry _kerjmp_mbSingleReturn
    b mbSingleReturn
    entry _kerjmp_mbSingleReturnWrite
    b mbSingleReturnWrite
    entry _kerjmp_mbSingleGameEnd
    b mbSingleGameEnd
    entry _kerjmp_mbSingleSaveInit
    b mbSingleSaveInit
    entry _kerjmp_mbSingleMgUnlockWrite
    b mbSingleMgUnlockWrite
    entry _kerjmp_mbSingleMgUnlockInit
    b mbSingleMgUnlockInit
    entry _kerjmp_mbSingleMgUnlockSet
    b mbSingleMgUnlockSet
    entry _kerjmp_mbSingleMgUnlockReset
    b mbSingleMgUnlockReset
    entry _kerjmp_mbSingleMgUnlockGet
    b mbSingleMgUnlockGet
    entry _kerjmp_mbSingleMgUnlockCheckAny
    b mbSingleMgUnlockCheckAny
    entry _kerjmp_mbSingleMgUnlockNumGet
    b mbSingleMgUnlockNumGet
    entry _kerjmp_mbev_SingleMg
    b mbev_SingleMg
    entry _kerjmp_mbev_SingleMgEnd
    b mbev_SingleMgEnd
    entry _kerjmp_mbSingleStepGet
    b mbSingleStepGet
    entry _kerjmp_mbSingleCall
    b mbSingleCall
    entry _kerjmp_mbSingleOppCharGet
    b mbSingleOppCharGet
    entry _kerjmp_mbSingleTeamCharSet
    b mbSingleTeamCharSet
    entry _kerjmp_mbSingleTeamCharGet
    b mbSingleTeamCharGet
    entry _kerjmp_mbSingleSaveFlush
    b mbSingleSaveFlush
    entry _kerjmp_mbSingleMgUnlockCheck
    b mbSingleMgUnlockCheck
    entry _kerjmp_mbMusPlay
    b mbMusPlay
    entry _kerjmp_mbMusFadeOut
    b mbMusFadeOut
    entry _kerjmp_mbMusFadeOutSpeed
    b mbMusFadeOutSpeed
    entry _kerjmp_mbMusStub
    b mbMusStub
    entry _kerjmp_mbMusParamSet
    b mbMusParamSet
    entry _kerjmp_mbMusPauseFadeOut
    b mbMusPauseFadeOut
    entry _kerjmp_mbMusStatGet
    b mbMusStatGet
    entry _kerjmp_mbAudClose
    b mbAudClose
    entry _kerjmp_mbAudInit
    b mbAudInit
    entry _kerjmp_mbAudStub1
    b mbAudStub1
    entry _kerjmp_mbAudStub2
    b mbAudStub2
    entry _kerjmp_mbMusBoardPlay
    b mbMusBoardPlay
    entry _kerjmp_mbMusCheck
    b mbMusCheck
    entry _kerjmp_mbMusEndCheck
    b mbMusEndCheck
    entry _kerjmp_mbMusStopCheck
    b mbMusStopCheck
    entry _kerjmp_mbMusFadeCheck
    b mbMusFadeCheck
    entry _kerjmp_mbMusPauseSet
    b mbMusPauseSet
    entry _kerjmp_mbMusJinglePlay
    b mbMusJinglePlay
    entry _kerjmp_mbMusJingleWait
    b mbMusJingleWait
    entry _kerjmp_mbMusJingleStatGet
    b mbMusJingleStatGet
    entry _kerjmp_mbAudFXObjSet
    b mbAudFXObjSet
    entry _kerjmp_mbAudFXObjCreate
    b mbAudFXObjCreate
    entry _kerjmp_mbAudFXObjVolSet
    b mbAudFXObjVolSet
    entry _kerjmp_mbAudFXObjKill
    b mbAudFXObjKill
    entry _kerjmp_mbAudFXObjClose
    b mbAudFXObjClose
    entry _kerjmp_mbAudFXObjEmitterSet
    b mbAudFXObjEmitterSet
    entry _kerjmp_mbAudFXObjEmitterCreate
    b mbAudFXObjEmitterCreate
    entry _kerjmp_mbAudFXObjEmitterUpdate
    b mbAudFXObjEmitterUpdate
    entry _kerjmp_mbAudFXObjEmiterPosSet
    b mbAudFXObjEmiterPosSet
    entry _kerjmp_mbAudFXObjRefSet
    b mbAudFXObjRefSet
    entry _kerjmp_mbAudFXPlay
    b mbAudFXPlay
    entry _kerjmp_mbAudFXStop
    b mbAudFXStop
    entry _kerjmp_mbAudFXStopAll
    b mbAudFXStopAll
    entry _kerjmp_mbAudFXPosPlay
    b mbAudFXPosPlay
    entry _kerjmp_mbAudFXPosPanGet
    b mbAudFXPosPanGet
    entry _kerjmp_mbAudFXEmitterPlay
    b mbAudFXEmitterPlay
    entry _kerjmp_mbAudFXVolSet
    b mbAudFXVolSet
    entry _kerjmp_mbAudFXPanning
    b mbAudFXPanning
    entry _kerjmp_mbAudFXPosPanning
    b mbAudFXPosPanning
    entry _kerjmp_mbAudFXDelaySet
    b mbAudFXDelaySet
    entry _kerjmp_mbAudGuidePlay
    b mbAudGuidePlay
    entry _kerjmp_mbev_StarMasu
    b mbev_StarMasu
    entry _kerjmp_mbev_StarFreeMasu
    b mbev_StarFreeMasu
    entry _kerjmp_mbStarGetMain
    b mbStarGetMain
    entry _kerjmp_mbStarGetPosExec
    b mbStarGetPosExec
    entry _kerjmp_mbStarMapViewExec
    b mbStarMapViewExec
    entry _kerjmp_mbStarStub2
    b mbStarStub2
    entry _kerjmp_mbStarInit
    b mbStarInit
    entry _kerjmp_mbStarClose
    b mbStarClose
    entry _kerjmp_mbStarStub
    b mbStarStub
    entry _kerjmp_mbStarNumSet
    b mbStarNumSet
    entry _kerjmp_mbStarNumGet
    b mbStarNumGet
    entry _kerjmp_mbStarFlagInit
    b mbStarFlagInit
    entry _kerjmp_mbStarFlagSet
    b mbStarFlagSet
    entry _kerjmp_mbStarFlagReset
    b mbStarFlagReset
    entry _kerjmp_mbStarFlagGet
    b mbStarFlagGet
    entry _kerjmp_mbStarNextNoSet
    b mbStarNextNoSet
    entry _kerjmp_mbStarNextNoGet
    b mbStarNextNoGet
    entry _kerjmp_mbStarNoSet
    b mbStarNoSet
    entry _kerjmp_mbStarNoRandGet
    b mbStarNoRandGet
    entry _kerjmp_mbStarReset
    b mbStarReset
    entry _kerjmp_mbStarNoDispSet
    b mbStarNoDispSet
    entry _kerjmp_mbStarMasuDispSet
    b mbStarMasuDispSet
    entry _kerjmp_mbStarDispSetAll
    b mbStarDispSetAll
    entry _kerjmp_mbStarObjDispSetAll
    b mbStarObjDispSetAll
    entry _kerjmp_mbStarMasuNextSet
    b mbStarMasuNextSet
    entry _kerjmp_mbStarGetExec
    b mbStarGetExec
    entry _kerjmp_mbStarMoveHookSet
    b mbStarMoveHookSet
    entry _kerjmp_mbStarMasuFuncSet
    b mbStarMasuFuncSet
    entry _kerjmp_mbStarAddNumSet
    b mbStarAddNumSet
    entry _kerjmp_mbStarDispCreate
    b mbStarDispCreate
    entry _kerjmp_mbStarDispPlayerCreate
    b mbStarDispPlayerCreate
    entry _kerjmp_mbStarDispCheck
    b mbStarDispCheck
    entry _kerjmp_mbStarObjCreate
    b mbStarObjCreate
    entry _kerjmp_mbStarObjPosSet
    b mbStarObjPosSet
    entry _kerjmp_mbStarObjRotSet
    b mbStarObjRotSet
    entry _kerjmp_mbStarObjScaleSet
    b mbStarObjScaleSet
    entry _kerjmp_mbStarObjPosSetV
    b mbStarObjPosSetV
    entry _kerjmp_mbStarObjRotSetV
    b mbStarObjRotSetV
    entry _kerjmp_mbStarObjScaleSetV
    b mbStarObjScaleSetV
    entry _kerjmp_mbStarObjPosGet
    b mbStarObjPosGet
    entry _kerjmp_mbStarObjRotGet
    b mbStarObjRotGet
    entry _kerjmp_mbStarObjScaleGet
    b mbStarObjScaleGet
    entry _kerjmp_mbStarObjDispSet
    b mbStarObjDispSet
    entry _kerjmp_mbStarObjDispFlagSet
    b mbStarObjDispFlagSet
    entry _kerjmp_mbStarObjKill
    b mbStarObjKill
    entry _kerjmp_mbStarChestCreate
    b mbStarChestCreate
    entry _kerjmp_mbStarAddProcExec
    b mbStarAddProcExec
    entry _kerjmp_mbStarAddDispExec
    b mbStarAddDispExec
    entry _kerjmp_mbStarAddExec
    b mbStarAddExec
    entry _kerjmp_mbStarAddAllProcExecV
    b mbStarAddAllProcExecV
    entry _kerjmp_mbStarAddAllProcExec
    b mbStarAddAllProcExec
    entry _kerjmp_mbStarAddAllExec
    b mbStarAddAllExec
    entry _kerjmp_mbZtarObjInit
    b mbZtarObjInit
    entry _kerjmp_mbZtarObjClose
    b mbZtarObjClose
    entry _kerjmp_mbZtarMasuNextSet
    b mbZtarMasuNextSet
    entry _kerjmp_mbZtarGetExec
    b mbZtarGetExec
    entry _kerjmp_mbZtarGetMain
    b mbZtarGetMain
    entry _kerjmp_mbZtarObjDispFlagSet
    b mbZtarObjDispFlagSet
    entry _kerjmp_mbStarMapViewProcExec
    b mbStarMapViewProcExec
    entry _kerjmp_mbStatusInit
    b mbStatusInit
    entry _kerjmp_mbStatusClose
    b mbStatusClose
    entry _kerjmp_mbStatusReset
    b mbStatusReset
    entry _kerjmp_mbStatusDispGet
    b mbStatusDispGet
    entry _kerjmp_mbStatusDispSet
    b mbStatusDispSet
    entry _kerjmp_mbStatusDispFocusSet
    b mbStatusDispFocusSet
    entry _kerjmp_mbStatusDispSetAll
    b mbStatusDispSetAll
    entry _kerjmp_mbStatusDispForceSet
    b mbStatusDispForceSet
    entry _kerjmp_mbStatusDispForceSetAll
    b mbStatusDispForceSetAll
    entry _kerjmp_mbStatusMoveCheck
    b mbStatusMoveCheck
    entry _kerjmp_mbStatusOffCheckAll
    b mbStatusOffCheckAll
    entry _kerjmp_mbSprCreate
    b mbSprCreate
    entry _kerjmp_mbStatusPosGet
    b mbStatusPosGet
    entry _kerjmp_mbStatusNoPosGet
    b mbStatusNoPosGet
    entry _kerjmp_mbStatusCapsuleDispSet
    b mbStatusCapsuleDispSet
    entry _kerjmp_mbStatusKaoNoSet
    b mbStatusKaoNoSet
    entry _kerjmp_mbSprNumSet
    b mbSprNumSet
    entry _kerjmp_mbStatusMoveTo
    b mbStatusMoveTo
    entry _kerjmp_mbStatusMoveSet
    b mbStatusMoveSet
    entry _kerjmp_mbStatusPosOnGet
    b mbStatusPosOnGet
    entry _kerjmp_mbStatusPosOffGet
    b mbStatusPosOffGet
    entry _kerjmp_mbStatusRainbowSet
    b mbStatusRainbowSet
    entry _kerjmp_mbStatusColorSet
    b mbStatusColorSet
    entry _kerjmp_mbStatusColorAllSet
    b mbStatusColorAllSet
    entry _kerjmp_mbStatusColorGet
    b mbStatusColorGet
    entry _kerjmp_mbStatusLayoutSet
    b mbStatusLayoutSet
    entry _kerjmp_mbStatusCapsuleFocusSet
    b mbStatusCapsuleFocusSet
    entry _kerjmp_mbStatusNoGet
    b mbStatusNoGet
    entry _kerjmp_mbStatusNoMoveSet
    b mbStatusNoMoveSet
    entry _kerjmp_mbStatusDispBackup
    b mbStatusDispBackup
    entry _kerjmp_mbStatusDispRestoreForce
    b mbStatusDispRestoreForce
    entry _kerjmp_mbStatusDispRestore
    b mbStatusDispRestore
    entry _kerjmp_mbStatusMasuDispSet
    b mbStatusMasuDispSet
    entry _kerjmp_mbSNpcInit
    b mbSNpcInit
    entry _kerjmp_mbSNpcCreate
    b mbSNpcCreate
    entry _kerjmp_mbSNpcKill
    b mbSNpcKill
    entry _kerjmp_mbSNpcMotReset
    b mbSNpcMotReset
    entry _kerjmp_mbSNpcPlayerWalkSet
    b mbSNpcPlayerWalkSet
    entry _kerjmp_mbSNpcStarExec
    b mbSNpcStarExec
    entry _kerjmp_mbSNpcMasuGet
    b mbSNpcMasuGet
    entry _kerjmp_mbSNpcMotWinSet
    b mbSNpcMotWinSet
    entry _kerjmp_mbSNpcMotIdleSet
    b mbSNpcMotIdleSet
    entry _kerjmp_mbSNpcDispSet
    b mbSNpcDispSet
    entry _kerjmp_mbMasuChanceCreate
    b mbMasuChanceCreate
    entry _kerjmp_mbMasuChanceTypeSet
    b mbMasuChanceTypeSet
    entry _kerjmp_mbMasuChanceFlagSet
    b mbMasuChanceFlagSet
    entry _kerjmp_mbMasuChancePlayerSet
    b mbMasuChancePlayerSet
    entry _kerjmp_mbMasuChanceSet
    b mbMasuChanceSet
    entry _kerjmp_mbMasuChanceKill
    b mbMasuChanceKill
    entry _kerjmp_mbSNpcMasuStarNextGet
    b mbSNpcMasuStarNextGet
    entry _kerjmp_mbObjStarTevStageSet
    b mbObjStarTevStageSet
    entry _kerjmp_mbObjFadeCreate
    b mbObjFadeCreate
    entry _kerjmp_mbObjFadeKill
    b mbObjFadeKill
    entry _kerjmp_mbObjFadeTexRotSet
    b mbObjFadeTexRotSet
    entry _kerjmp_mbObjFadeTexColorSet
    b mbObjFadeTexColorSet
    entry _kerjmp_mbObjMetalCreate
    b mbObjMetalCreate
    entry _kerjmp_mbObjMetalKill
    b mbObjMetalKill
    entry _kerjmp_mbObjMetalTPLvlSet
    b mbObjMetalTPLvlSet
    entry _kerjmp_mbObjMetalColorSet
    b mbObjMetalColorSet
    entry _kerjmp_mbObjBiriQCreate
    b mbObjBiriQCreate
    entry _kerjmp_mbObjBiriQKill
    b mbObjBiriQKill
    entry _kerjmp_mbObjBiriQColorSet
    b mbObjBiriQColorSet
    entry _kerjmp_mbTutorialInit
    b mbTutorialInit
    entry _kerjmp_mbTutorialCreate
    b mbTutorialCreate
    entry _kerjmp_mbTutorialExitReqGet
    b mbTutorialExitReqGet
    entry _kerjmp_mbTutorialExitSet
    b mbTutorialExitSet
    entry _kerjmp_mbTutorialExitOnSet
    b mbTutorialExitOnSet
    entry _kerjmp_mbTutorialCall
    b mbTutorialCall
    entry _kerjmp_mbTutorialMainFuncSet
    b mbTutorialMainFuncSet
    entry _kerjmp_mbTutorialSprCreate
    b mbTutorialSprCreate
    entry _kerjmp_mbTutorialSprDispOn
    b mbTutorialSprDispOn
    entry _kerjmp_mbTutorialSprDispOff
    b mbTutorialSprDispOff
    entry _kerjmp_mbTutorialSprKill
    b mbTutorialSprKill
    entry _kerjmp_mbTutorialSprGrpSet
    b mbTutorialSprGrpSet
    entry _kerjmp_mbTutorialSprGrpKill
    b mbTutorialSprGrpKill
    entry _kerjmp_mbTutorialModelCreate
    b mbTutorialModelCreate
    entry _kerjmp_mbTutorialModelKill
    b mbTutorialModelKill
    entry _kerjmp_mbTutorialGuideCreate
    b mbTutorialGuideCreate
    entry _kerjmp_mbTutorialGuideGet
    b mbTutorialGuideGet
    entry _kerjmp_mbTutorialGuideClose
    b mbTutorialGuideClose
    entry _kerjmp_mbTutorialWinWait
    b mbTutorialWinWait
    entry _kerjmp_mbTutorialWinClose
    b mbTutorialWinClose
    entry _kerjmp_mbTutorialWinMesExec
    b mbTutorialWinMesExec
    entry _kerjmp_mbTutorialWinCreate
    b mbTutorialWinCreate
    entry _kerjmp_mbTutorialWinMesMasuExec
    b mbTutorialWinMesMasuExec
    entry _kerjmp_mbTutorialWinKeyWait
    b mbTutorialWinKeyWait
    entry _kerjmp_mbTutorialViewSet
    b mbTutorialViewSet
    entry _kerjmp_mbTutorialViewMasuSet
    b mbTutorialViewMasuSet
    entry _kerjmp_mbTutorialVSleep
    b mbTutorialVSleep
    entry _kerjmp_mbTutorialCallEnd
    b mbTutorialCallEnd
    entry _kerjmp_mbTutorialCallWait
    b mbTutorialCallWait
    entry _kerjmp_mbTutorialMultiCall
    b mbTutorialMultiCall
    entry _kerjmp_mbTutorialCallResult
    b mbTutorialCallResult
    entry _kerjmp_mbTutorialMesCall
    b mbTutorialMesCall
    entry _kerjmp_mbTutorialMesMasuCall
    b mbTutorialMesMasuCall
    entry _kerjmp_mbTutorialModeSet
    b mbTutorialModeSet
    entry _kerjmp_mbTutorialModeGet
    b mbTutorialModeGet
    entry _kerjmp_mbTutorialResultSet
    b mbTutorialResultSet
    entry _kerjmp_mbTutorialSceneGet
    b mbTutorialSceneGet
    entry _kerjmp_mbTutorialTurnCall
    b mbTutorialTurnCall
    entry _kerjmp_mbTutorialGuideCall
    b mbTutorialGuideCall
    entry _kerjmp_mbTutorialCapsuleUseCall
    b mbTutorialCapsuleUseCall
    entry _kerjmp_mbTutorialMgCallExec
    b mbTutorialMgCallExec
    entry _kerjmp_mbTutorialMgCallClose
    b mbTutorialMgCallClose
    entry _kerjmp_mbWinInit
    b mbWinInit
    entry _kerjmp_mbWinClose
    b mbWinClose
    entry _kerjmp_mbWinCreate
    b mbWinCreate
    entry _kerjmp_mbWinCreateChoice
    b mbWinCreateChoice
    entry _kerjmp_mbWinCreateHelp
    b mbWinCreateHelp
    entry _kerjmp_mbWinCreateFrame
    b mbWinCreateFrame
    entry _kerjmp_mbWinCreateTime
    b mbWinCreateTime
    entry _kerjmp_mbWinCreateBlank
    b mbWinCreateBlank
    entry _kerjmp_mbWinCreateBlankFrame
    b mbWinCreateBlankFrame
    entry _kerjmp_mbWinKillAll
    b mbWinKillAll
    entry _kerjmp_mbWinKill
    b mbWinKill
    entry _kerjmp_mbWinTopKill
    b mbWinTopKill
    entry _kerjmp_mbWinPosSet
    b mbWinPosSet
    entry _kerjmp_mbWinTopPosSet
    b mbWinTopPosSet
    entry _kerjmp_mbWinPosGet
    b mbWinPosGet
    entry _kerjmp_mbWinTopPosGet
    b mbWinTopPosGet
    entry _kerjmp_mbWinSizeSet
    b mbWinSizeSet
    entry _kerjmp_mbWinTopSizeSet
    b mbWinTopSizeSet
    entry _kerjmp_mbWinMesMaxSizeGet
    b mbWinMesMaxSizeGet
    entry _kerjmp_mbWinTopMesMaxSizeGet
    b mbWinTopMesMaxSizeGet
    entry _kerjmp_mbWinScaleSet
    b mbWinScaleSet
    entry _kerjmp_mbWinTopScaleSet
    b mbWinTopScaleSet
    entry _kerjmp_mbWinScaleGet
    b mbWinScaleGet
    entry _kerjmp_mbWinTopScaleGet
    b mbWinTopScaleGet
    entry _kerjmp_mbWinChoiceGet
    b mbWinChoiceGet
    entry _kerjmp_mbWinTopChoiceGet
    b mbWinTopChoiceGet
    entry _kerjmp_mbWinPause
    b mbWinPause
    entry _kerjmp_mbWinTopPause
    b mbWinTopPause
    entry _kerjmp_mbWinInsertMesSet
    b mbWinInsertMesSet
    entry _kerjmp_mbWinTopInsertMesSet
    b mbWinTopInsertMesSet
    entry _kerjmp_mbWinDoneCheck
    b mbWinDoneCheck
    entry _kerjmp_mbWinTopDoneCheck
    b mbWinTopDoneCheck
    entry _kerjmp_mbWinWait
    b mbWinWait
    entry _kerjmp_mbWinTopWait
    b mbWinTopWait
    entry _kerjmp_mbWinAttrSet
    b mbWinAttrSet
    entry _kerjmp_mbWinTopAttrSet
    b mbWinTopAttrSet
    entry _kerjmp_mbWinAttrReset
    b mbWinAttrReset
    entry _kerjmp_mbWinTopAttrReset
    b mbWinTopAttrReset
    entry _kerjmp_mbWinChoiceDisable
    b mbWinChoiceDisable
    entry _kerjmp_mbWinTopChoiceDisable
    b mbWinTopChoiceDisable
    entry _kerjmp_mbWinMesSpeedSet
    b mbWinMesSpeedSet
    entry _kerjmp_mbWinTopMesSpeedSet
    b mbWinTopMesSpeedSet
    entry _kerjmp_mbWinMesColSet
    b mbWinMesColSet
    entry _kerjmp_mbWinTopMesColSet
    b mbWinTopMesColSet
    entry _kerjmp_mbWinChoiceNowGet
    b mbWinChoiceNowGet
    entry _kerjmp_mbWinTopChoiceNowGet
    b mbWinTopChoiceNowGet
    entry _kerjmp_mbWinPriSet
    b mbWinPriSet
    entry _kerjmp_mbWinTopPriSet
    b mbWinTopPriSet
    entry _kerjmp_mbWinChoiceNowGet2
    b mbWinChoiceNowGet2
    entry _kerjmp_mbWinTopChoiceNowGet2
    b mbWinTopChoiceNowGet2
    entry _kerjmp_mbWinPlayerDisable
    b mbWinPlayerDisable
    entry _kerjmp_mbWinTopPlayerDisable
    b mbWinTopPlayerDisable
    entry _kerjmp_mbWinIDGet
    b mbWinIDGet
    entry _kerjmp_mbWinTopIDGet
    b mbWinTopIDGet
    entry _kerjmp_mbWinDispSet
    b mbWinDispSet
    entry _kerjmp_mbWinTopDispSet
    b mbWinTopDispSet
    entry _kerjmp_mbWinPauseHook
    b mbWinPauseHook
    entry _kerjmp_mbWinGet
    b mbWinGet
    entry _kerjmp_mbWinCenterGet
    b mbWinCenterGet
    entry _kerjmp_mbWinCenterInsertGet
    b mbWinCenterInsertGet
    entry _kerjmp_mbWinTopCenterInsertGet
    b mbWinTopCenterInsertGet
    entry _kerjmp_mbWipeSpecialInit
    b mbWipeSpecialInit
    entry _kerjmp_mbWipeSpecialClose
    b mbWipeSpecialClose
    entry _kerjmp_mbWipeSpecialKill
    b mbWipeSpecialKill
    entry _kerjmp_mbWipeWait
    b mbWipeWait
    entry _kerjmp_mbWipeCreate
    b mbWipeCreate
    entry _kerjmp_mbWipeFadeOut
    b mbWipeFadeOut
    entry _kerjmp_mbWipeFadeIn
    b mbWipeFadeIn
    entry _kerjmp_mbWipeWhiteFadeOut
    b mbWipeWhiteFadeOut
    entry _kerjmp_mbWipeWhiteFadeIn
    b mbWipeWhiteFadeIn
    entry _kerjmp_mbWipeDissolveFadeOut
    b mbWipeDissolveFadeOut
    entry _kerjmp_mbWipeDissolveFadeIn
    b mbWipeDissolveFadeIn
    entry _kerjmp_mbWipeFadeOutTime
    b mbWipeFadeOutTime
    entry _kerjmp_mbWipeFadeInTime
    b mbWipeFadeInTime
    entry _kerjmp_mbWipeWhiteFadeOutTime
    b mbWipeWhiteFadeOutTime
    entry _kerjmp_mbWipeWhiteFadeInTime
    b mbWipeWhiteFadeInTime
    entry _kerjmp_mbWipeDissolveFadeOutTime
    b mbWipeDissolveFadeOutTime
    entry _kerjmp_mbWipeDissolveFadeInTime
    b mbWipeDissolveFadeInTime
    entry _kerjmp_mbWipeSpecialCreate
    b mbWipeSpecialCreate
    entry _kerjmp_mbWipeSpecialWait
    b mbWipeSpecialWait
    entry _kerjmp_mbWipeSpecialFadeOutCreate
    b mbWipeSpecialFadeOutCreate
    entry _kerjmp_mbWipeSpecialFadeInCreate
    b mbWipeSpecialFadeInCreate
    entry _kerjmp_mbWipeSpecialStatGet
    b mbWipeSpecialStatGet
    entry _kerjmp_mbMgCallInit
    b mbMgCallInit
    entry _kerjmp_mbev_MgCall
    b mbev_MgCall
    entry _kerjmp_mbev_MgCallKettou
    b mbev_MgCallKettou
    entry _kerjmp_mbev_MgCallDonkey
    b mbev_MgCallDonkey
    entry _kerjmp_mbev_MgCallKoopa
    b mbev_MgCallKoopa
    entry _kerjmp_mbev_MgCallSingle
    b mbev_MgCallSingle
    entry _kerjmp_mbev_MgCallSingleKoopa
    b mbev_MgCallSingleKoopa
    entry _kerjmp_mbMgCallDataClose
    b mbMgCallDataClose
    entry _kerjmp_mbev_MgCallTutorial
    b mbev_MgCallTutorial
    entry _kerjmp_mbMgRouletteFocusKill
    b mbMgRouletteFocusKill
    entry _kerjmp_mbMgRouletteNumGet
    b mbMgRouletteNumGet
    entry _kerjmp_mbMgCallSingleOnCheck
    b mbMgCallSingleOnCheck
    entry _kerjmp_mbMgCallVsEffCreate
    b mbMgCallVsEffCreate
    #endif
}
