// Copyright © 2022-2025 ChefKiss. Licensed under the Thou Shalt Not Profit License version 1.5.
// See LICENSE for details.

#pragma once
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_util.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/DeviceType.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/Result.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/SWIP/DMCU.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/SWIP/GC.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/SWIP/IPVersion.hpp>
#include <PrivateHeaders/GPUDriversAMD/CAIL/SWIP/SDMA.hpp>
#include <PrivateHeaders/ObjectField.hpp>

namespace iVega {
    class X5000HWLibs {
        using t_createFirmware = void *(*)(const void *data, UInt32 size, UInt32 ipVersion, const char *filename);
        using t_putFirmware = bool (*)(void *that, AMDDeviceType deviceType, void *fw);

        bool initialised {false};
        ObjectField<void *> fwDirField {};
        ObjectField<UInt32> pspSOSField {};
        ObjectField<UInt8> pspSecurityCapsField {};
        ObjectField<UInt32> pspTOSVerField {};
        ObjectField<UInt8 *> pspCommandDataField {};
        ObjectField<bool> smuSwInitialisedFieldBase {};
        ObjectField<void *> smuInternalSWInitField {};
        ObjectField<void *> smuFullscreenEventField {};
        ObjectField<void *> smuGetUCodeConstsField {};
        ObjectField<void *> smuInternalHWInitField {};
        ObjectField<void *> smuNotifyEventField {};
        ObjectField<void *> smuInternalSWExitField {};
        ObjectField<void *> smuInternalHWExitField {};
        ObjectField<void *> smuFullAsicResetField {};
        ObjectField<GCFirmwareInfo> gcSwFirmwareField {};
        ObjectField<UInt32> dmcuEnablePSPFWLoadField {};
        ObjectField<UInt32> dmcuABMLevelField {};
        ObjectField<bool (*)(void *instance, const SDMAFWConstant **out)> sdmaGetFwConstantsField {};
        ObjectField<bool (*)(void *instance)> sdmaStartEngineField {};
        mach_vm_address_t orgGetIpFw {0};
        t_createFirmware orgCreateFirmware {nullptr};
        t_putFirmware orgPutFirmware {nullptr};
        mach_vm_address_t orgPspCmdKmSubmit {0};
        mach_vm_address_t orgSmuInitFunctionPointerList {0};
        mach_vm_address_t orgGcSetFwEntryInfo {0};
        mach_vm_address_t orgSdmaInitFunctionPointerList {0};

        public:
        static X5000HWLibs &singleton();

        void init();

        private:
        void processKext(KernelPatcher &patcher, size_t id, mach_vm_address_t slide, size_t size);

        static void wrapPopulateFirmwareDirectory(void *that);
        static bool wrapGetIpFw(void *that, UInt32 ipVersion, const char *name, void *out);
        static CAILResult cailGeneralFailure();
        static CAILResult cailUnsupported();
        static CAILResult cailNoop();
        static CAILResult pspBootloaderLoadSos10(void *instance);
        static CAILResult pspSecurityFeatureCapsSet10(void *instance);
        static CAILResult pspSecurityFeatureCapsSet12(void *instance);
        static CAILResult wrapPspCmdKmSubmit(void *instance, void *cmd, void *outData, void *outResponse);
        static CAILResult smuReset();
        static CAILResult smuPowerUp();
        static CAILResult smuInternalSwInit(void *instance);
        static CAILResult smu10InternalHwInit(void *instance);
        static CAILResult smu12InternalHwInit(void *instance);
        static CAILResult smuInternalHwExit(void *instance);
        static CAILResult smuFullAsicReset(void *instance, void *data);
        static CAILResult smu10NotifyEvent(void *instance, void *data);
        static CAILResult smu12NotifyEvent(void *instance, void *data);
        static CAILResult smuFullScreenEvent(void *instance, UInt32 event);
        static CAILResult wrapSmuInitFunctionPointerList(void *instance, SWIPIPVersion ipVersion);
        static void gc91GetFwConstants(void *instance, GCFirmwareInfo *fwData);
        static void gc92GetFwConstants(void *instance, GCFirmwareInfo *fwData);
        static void gc93GetFwConstants(void *instance, GCFirmwareInfo *fwData);
        static void processGCFWEntries(void *instance, void *initData);
        static CAILResult wrapGcSetFwEntryInfo(void *instance, SWIPIPVersion ipVersion, void *initData);
        static bool getDcn1FwConstants(void *instance, DMCUFirmwareInfo *fwData);
        static bool getDcn21FwConstants(void *instance, DMCUFirmwareInfo *fwData);
        static CAILResult wrapSdmaInitFunctionPointerList(void *instance, UInt32 verMajor, UInt32 verMinor,
            UInt32 verPatch);
    };
};    // namespace iVega
