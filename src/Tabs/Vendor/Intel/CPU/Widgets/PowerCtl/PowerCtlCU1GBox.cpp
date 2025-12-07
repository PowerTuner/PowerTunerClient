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
#include "PowerCtlCU1GBox.h"
#include "pwtClientCommon/UILogger.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PowerCtlCU1GBox::PowerCtlCU1GBox() {
        FlowLayout *lyt = new FlowLayout();

        prochot = new QCheckBox("Bi-Directional PROCHOT");
        c1e = new QCheckBox("C1 Enhanced (C1E)");
        sapmImcC2Policy = new QCheckBox("Allow self-refresh in package C2 state");
        fastBrkSnpEn = new QCheckBox("Use fast VID swing rate");
        powerPerformancePlatformOverride = new QCheckBox("Power performance platform override");
        disableEfficiencyOptimization = new QCheckBox("Disable P-State energy efficiency optimization");
        disableRaceToHalt = new QCheckBox("Disable race to halt optimization");
        prochotOutputDisable = new QCheckBox("Disable PROCHOT output");
        prochotConfigurableResponseEnable = new QCheckBox("Enable PROCHOT configurable response");
        vrThermAlertDisableLock = new QCheckBox("Lock PROCHOT bits until device reboot");
        vrThermAlertDisable = new QCheckBox("Disable VR_THERMAL_ALERT signaling");
        ringEEDisable = new QCheckBox("Disable ring EE");
        saOptimizationDisable = new QCheckBox("Disable SA optimization");
        ookDisable = new QCheckBox("Disable OOK");
        hwpAutonomousDisable = new QCheckBox("Disable HWP autonomous mode");
        cstatePrewakeDisable = new QCheckBox("Disable C-state pre-wake");

        lyt->addWidget(prochot);
        lyt->addWidget(c1e);
        lyt->addWidget(sapmImcC2Policy);
        lyt->addWidget(fastBrkSnpEn);
        lyt->addWidget(powerPerformancePlatformOverride);
        lyt->addWidget(disableEfficiencyOptimization);
        lyt->addWidget(disableRaceToHalt);
        lyt->addWidget(prochotOutputDisable);
        lyt->addWidget(prochotConfigurableResponseEnable);
        lyt->addWidget(vrThermAlertDisableLock);
        lyt->addWidget(vrThermAlertDisable);
        lyt->addWidget(ringEEDisable);
        lyt->addWidget(saOptimizationDisable);
        lyt->addWidget(ookDisable);
        lyt->addWidget(hwpAutonomousDisable);
        lyt->addWidget(cstatePrewakeDisable);

        setLayout(lyt);
    }

    void PowerCtlCU1GBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->powerCtl.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::PowerCtl data = packet.intelData->powerCtl.getValue();

        prochot->setChecked(data.bdProcHot);
        prochot->setEnabled(data.vrThermAlertDisableLock);
        c1e->setChecked(data.c1eEnable);
        sapmImcC2Policy->setChecked(data.sapmImcC2Policy);
        fastBrkSnpEn->setChecked(data.fastBrkSnpEn);
        powerPerformancePlatformOverride->setChecked(data.powerPerformancePlatformOverride);
        disableEfficiencyOptimization->setChecked(data.disableEnergyEfficiencyOpt);
        disableRaceToHalt->setChecked(data.disableRaceToHaltOpt);
        prochotOutputDisable->setChecked(data.prochotOutputDisable);
        prochotOutputDisable->setEnabled(data.vrThermAlertDisableLock);
        prochotConfigurableResponseEnable->setChecked(prochotConfigurableResponseEnable);
        prochotConfigurableResponseEnable->setEnabled(data.vrThermAlertDisableLock);
        vrThermAlertDisableLock->setChecked(vrThermAlertDisableLock);
        vrThermAlertDisable->setChecked(data.vrThermAlertDisable);
        ringEEDisable->setChecked(data.ringEEDisable);
        saOptimizationDisable->setChecked(data.saOptimizationDisable);
        ookDisable->setChecked(data.ookDisable);
        hwpAutonomousDisable->setChecked(data.hwpAutonomousDisable);
        cstatePrewakeDisable->setChecked(data.cstatePrewakeDisable);
    }

    void PowerCtlCU1GBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->powerCtl.setValue({
            .bdProcHot = prochot->isChecked(),
            .c1eEnable = c1e->isChecked(),
            .sapmImcC2Policy = sapmImcC2Policy->isChecked(),
            .fastBrkSnpEn = fastBrkSnpEn->isChecked(),
            .powerPerformancePlatformOverride = powerPerformancePlatformOverride->isChecked(),
            .disableEnergyEfficiencyOpt = disableEfficiencyOptimization->isChecked(),
            .disableRaceToHaltOpt = disableRaceToHalt->isChecked(),
            .prochotOutputDisable = prochotOutputDisable->isChecked(),
            .prochotConfigurableResponseEnable = prochotConfigurableResponseEnable->isChecked(),
            .vrThermAlertDisableLock = vrThermAlertDisableLock->isChecked(),
            .vrThermAlertDisable = vrThermAlertDisable->isChecked(),
            .ringEEDisable = ringEEDisable->isChecked(),
            .saOptimizationDisable = saOptimizationDisable->isChecked(),
            .ookDisable = ookDisable->isChecked(),
            .hwpAutonomousDisable = hwpAutonomousDisable->isChecked(),
            .cstatePrewakeDisable = cstatePrewakeDisable->isChecked()
        }, true);
    }
}
