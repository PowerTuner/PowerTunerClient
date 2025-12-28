/*
 * This file is part of PowerTunerClient.
 * Copyright (C) 2025 kylon
 *
 * PowerTunerClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PowerTunerClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RYCPUTab.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"

namespace PWT::UI::AMD {
    RYCPUTab::RYCPUTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::AMD_RY_FAST_LIMIT_W)) {
            fastLimitGBox = new FastLimitGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), fastLimitGBox);
            QObject::connect(fastLimitGBox, &FastLimitGBox::sliderValueChanged, this, &RYCPUTab::onFastLimitChanged);
        }

        if (features.contains(PWTS::Feature::AMD_RY_SLOW_LIMIT_W)) {
            slowLimitGBox = new SlowLimitGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), slowLimitGBox);
            QObject::connect(slowLimitGBox, &SlowLimitGBox::sliderValueChanged, this, &RYCPUTab::onSlowLimitChanged);
        }

        if (features.contains(PWTS::Feature::AMD_RY_STAPM_LIMIT_W)) {
            stapmLimitGBox = new StapmLimitGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), stapmLimitGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_APU_SLOW_W)) {
            apuSlowLimitGBox = new APUSlowLimitGBox(features.contains(PWTS::Feature::AMD_RY_APU_SLOW_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), apuSlowLimitGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_TCTL_TEMP_W)) {
            tctlTempGBox = new TctlTempGBox(features.contains(PWTS::Feature::AMD_RY_TCTL_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), tctlTempGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_APU_SKIN_TEMP_W)) {
            apuSkinTempGBox = new APUSkinTempGBox(features.contains(PWTS::Feature::AMD_RY_APU_SKIN_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), apuSkinTempGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_DGPU_SKIN_TEMP_W)) {
            dgpuSkinTempGBox = new DGPUSkinTempGBox(features.contains(PWTS::Feature::AMD_RY_DGPU_SKIN_TEMP_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), dgpuSkinTempGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_CURRENT_W)) {
            vrmCurrentGBox = new VRMCurrentGBox(features.contains(PWTS::Feature::AMD_RY_VRM_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmCurrentGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_SOC_CURRENT_W)) {
            vrmSocCurrentGBox = new VRMSocCurrentGBox(features.contains(PWTS::Feature::AMD_RY_VRM_SOC_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmSocCurrentGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_MAX_CURRENT_W)) {
            vrmMaxCurrentGBox = new VRMMaxCurrentGBox(features.contains(PWTS::Feature::AMD_RY_VRM_MAX_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmMaxCurrentGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_VRM_SOC_MAX_CURRENT_W)) {
            vrmSocMaxCurrentGBox = new VRMSocMaxCurrentGBox(features.contains(PWTS::Feature::AMD_RY_VRM_SOC_MAX_CURRENT_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), vrmSocMaxCurrentGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_CO_ALL_W)) {
            curveOptimizerAllGBox = new CurveOptimizerAllGBox(false);
            scrollWidgLyt->insertWidget(nextInsertIdx(), curveOptimizerAllGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_CO_PER_W)) {
            curveOptimizerCoreGBox = new CurveOptimizerCoreGBox(packet.cpuInfo.numCores);
            scrollWidgLyt->insertWidget(nextInsertIdx(), curveOptimizerCoreGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_STATIC_GFX_CLK_W)) {
            staticGfxClkGBox = new StaticGfxClkGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), staticGfxClkGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_MIN_GFX_CLOCK_W)) {
            minGfxClockGBox = new MinGfxClockGBox(features.contains(PWTS::Feature::AMD_RY_MIN_GFX_CLOCK_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), minGfxClockGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_MAX_GFX_CLOCK_W)) {
            maxGfxClockGBox = new MaxGfxClockGBox(features.contains(PWTS::Feature::AMD_RY_MAX_GFX_CLOCK_R));
            scrollWidgLyt->insertWidget(nextInsertIdx(), maxGfxClockGBox);
        }

        if (features.contains(PWTS::Feature::AMD_RY_POWER_PROFILE_W)) {
            powerProfileGBox = new PowerProfileGBox();
            scrollWidgLyt->insertWidget(nextInsertIdx(), powerProfileGBox);
        }
    }

    void RYCPUTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<InputRanges> inputRanges = InputRanges::getInstance();

        if (!stapmLimitGBox.isNull()) {
            stapmLimitGBox->setRange(inputRanges->getRADJPl());
            stapmLimitGBox->setData(packet);
        }

        if (!slowLimitGBox.isNull()) {
            const QSignalBlocker sblock {slowLimitGBox};

            slowLimitGBox->setRange(inputRanges->getRADJPl());
            slowLimitGBox->setData(packet);

            if (packet.amdData->slowLimit.isValid())
                onSlowLimitChanged(packet.amdData->slowLimit.getValue());
        }

        if (!fastLimitGBox.isNull()) {
            const QSignalBlocker sblock {fastLimitGBox};

            fastLimitGBox->setRange(inputRanges->getRADJPl());
            fastLimitGBox->setData(packet);

            if (packet.amdData->fastLimit.isValid())
                onFastLimitChanged(packet.amdData->fastLimit.getValue());
        }

        if (!tctlTempGBox.isNull()) {
            tctlTempGBox->setRange(inputRanges->getRADJTctl());
            tctlTempGBox->setData(packet);
        }

        if (!apuSlowLimitGBox.isNull()) {
            apuSlowLimitGBox->setRange(inputRanges->getRADJAPUSlow());
            apuSlowLimitGBox->setData(packet);
        }

        if (!apuSkinTempGBox.isNull()) {
            apuSkinTempGBox->setRange(inputRanges->getRADJAPUSkinTemp());
            apuSkinTempGBox->setData(packet);
        }

        if (!dgpuSkinTempGBox.isNull()) {
            dgpuSkinTempGBox->setRange(inputRanges->getRADJDGPUSkinTemp());
            dgpuSkinTempGBox->setData(packet);
        }

        if (!vrmCurrentGBox.isNull()) {
            vrmCurrentGBox->setRange(inputRanges->getRADJVrmCurrent());
            vrmCurrentGBox->setData(packet);
        }

        if (!vrmSocCurrentGBox.isNull()) {
            vrmSocCurrentGBox->setRange(inputRanges->getRADJVrmSocCurrent());
            vrmSocCurrentGBox->setData(packet);
        }

        if (!vrmMaxCurrentGBox.isNull()) {
            vrmMaxCurrentGBox->setRange(inputRanges->getRADJVrmCurrent());
            vrmMaxCurrentGBox->setData(packet);
        }

        if (!vrmSocMaxCurrentGBox.isNull()) {
            vrmSocMaxCurrentGBox->setRange(inputRanges->getRADJVrmSocCurrent());
            vrmSocMaxCurrentGBox->setData(packet);
        }

        if (!curveOptimizerAllGBox.isNull()) {
            curveOptimizerAllGBox->setRange(inputRanges->getRADJCO());
            curveOptimizerAllGBox->setData(packet);
        }

        if (!curveOptimizerCoreGBox.isNull()) {
            curveOptimizerCoreGBox->setRanges(inputRanges->getRADJCO());
            curveOptimizerCoreGBox->setData(packet);
        }

        if (!staticGfxClkGBox.isNull()) {
            staticGfxClkGBox->setRange(inputRanges->getRADJGfxClock());
            staticGfxClkGBox->setData(packet);
        }

        if (!minGfxClockGBox.isNull()) {
            minGfxClockGBox->setRange(inputRanges->getRADJGfxClock());
            minGfxClockGBox->setData(packet);
        }

        if (!maxGfxClockGBox.isNull()) {
            maxGfxClockGBox->setRange(inputRanges->getRADJGfxClock());
            maxGfxClockGBox->setData(packet);
        }

        if (!powerProfileGBox.isNull())
            powerProfileGBox->setData(packet);
    }

    void RYCPUTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!stapmLimitGBox.isNull())
            stapmLimitGBox->setDataForPacket(packet);

        if (!fastLimitGBox.isNull())
            fastLimitGBox->setDataForPacket(packet);

        if (!slowLimitGBox.isNull())
            slowLimitGBox->setDataForPacket(packet);

        if (!tctlTempGBox.isNull())
            tctlTempGBox->setDataForPacket(packet);

        if (!apuSlowLimitGBox.isNull())
            apuSlowLimitGBox->setDataForPacket(packet);

        if (!apuSkinTempGBox.isNull())
            apuSkinTempGBox->setDataForPacket(packet);

        if (!dgpuSkinTempGBox.isNull())
            dgpuSkinTempGBox->setDataForPacket(packet);

        if (!vrmCurrentGBox.isNull())
            vrmCurrentGBox->setDataForPacket(packet);

        if (!vrmSocCurrentGBox.isNull())
            vrmSocCurrentGBox->setDataForPacket(packet);

        if (!vrmMaxCurrentGBox.isNull())
            vrmMaxCurrentGBox->setDataForPacket(packet);

        if (!vrmSocMaxCurrentGBox.isNull())
            vrmSocMaxCurrentGBox->setDataForPacket(packet);

        if (!curveOptimizerAllGBox.isNull())
            curveOptimizerAllGBox->setDataForPacket(packet);

        if (!curveOptimizerCoreGBox.isNull())
            curveOptimizerCoreGBox->setDataForPacket(packet);

        if (!staticGfxClkGBox.isNull())
             staticGfxClkGBox->setDataForPacket(packet);

        if (!minGfxClockGBox.isNull())
            minGfxClockGBox->setDataForPacket(packet);

        if (!maxGfxClockGBox.isNull())
            maxGfxClockGBox->setDataForPacket(packet);

        if (!powerProfileGBox.isNull())
            powerProfileGBox->setDataForPacket(packet);
    }

    void RYCPUTab::onFastLimitChanged(const int v) const {
        slowLimitGBox->setMaximum(v);
    }

    void RYCPUTab::onSlowLimitChanged(const int v) const {
        stapmLimitGBox->setMaximum(v);
    }
}
