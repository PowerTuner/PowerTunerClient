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
#include "DPMForcePerformanceLevelGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::LNX::AMD {
    DPMForcePerformanceLevelGBox::DPMForcePerformanceLevelGBox(const int idx): QGroupBox("Power DPM Force Performance Level") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *levelComboLyt = new QHBoxLayout();

        index = idx;
        dpmPerfLevel = new QComboBox();
        resetPowerLevels = new QCheckBox("reset to default");
        engineClockSclkGBox = new EngineClockSCLKGBox(idx);

        dpmPerfLevel->addItems({"auto", "low", "high", "manual", "profile_standard", "profile_min_sclk", "profile_min_mclk", "profile_peak"});

        levelComboLyt->addWidget(new QLabel("DPM performance level: "));
        levelComboLyt->addWidget(dpmPerfLevel);
        levelComboLyt->addStretch();
        levelComboLyt->addWidget(resetPowerLevels);
        lyt->addLayout(levelComboLyt);
        lyt->addWidget(engineClockSclkGBox);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);

        QObject::connect(dpmPerfLevel, &QComboBox::currentTextChanged, this, &DPMForcePerformanceLevelGBox::onDpmPerfLevelChanged);
    }

    void DPMForcePerformanceLevelGBox::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.linuxData->amdGpuData.contains(index) &&
                             packet.linuxData->amdGpuData[index].odRanges.isValid() &&
                             packet.linuxData->amdGpuData[index].dpmForcePerfLevel.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::LNX::AMD::GPUDPMForcePerfLevel dpmPerfLvl = packet.linuxData->amdGpuData[index].dpmForcePerfLevel.getValue();
        const QSignalBlocker sblock {dpmPerfLevel};

        resetPowerLevels->setChecked(false);
        dpmPerfLevel->setCurrentIndex(dpmPerfLevel->findText(dpmPerfLvl.level));
        engineClockSclkGBox->setData(packet);
        onDpmPerfLevelChanged(dpmPerfLevel->currentText());
    }

    void DPMForcePerformanceLevelGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.linuxData->amdGpuData[index].dpmForcePerfLevel.setValue({
            .level = dpmPerfLevel->currentText(),
            .sclk = engineClockSclkGBox->getData(),
            .reset = resetPowerLevels->isChecked()
        }, isEnabled());
    }

    void DPMForcePerformanceLevelGBox::onDpmPerfLevelChanged(const QString &text) const {
        const bool isManual = text == "manual";

        engineClockSclkGBox->setEnabled(isManual);
        resetPowerLevels->setEnabled(isManual);
    }
}
