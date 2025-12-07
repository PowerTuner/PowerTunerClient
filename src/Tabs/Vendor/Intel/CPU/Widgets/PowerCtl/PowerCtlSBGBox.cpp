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
#include "PowerCtlSBGBox.h"
#include "pwtClientCommon/UILogger.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::INTEL {
    PowerCtlSBGBox::PowerCtlSBGBox() {
        FlowLayout *lyt = new FlowLayout();

        prochot = new QCheckBox("Bi-Directional PROCHOT");
        c1e = new QCheckBox("C1 Enhanced (C1E)");
        disableEfficiencyOptimization = new QCheckBox("Disable energy efficiency optimization");
        disableRaceToHalt = new QCheckBox("Disable race to halt optimization");

        lyt->addWidget(prochot);
        lyt->addWidget(c1e);
        lyt->addWidget(disableEfficiencyOptimization);
        lyt->addWidget(disableRaceToHalt);

        setLayout(lyt);
    }

    void PowerCtlSBGBox::setData(const PWTS::DaemonPacket &packet) {
        setEnabled(packet.intelData->powerCtl.isValid());

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        const PWTS::Intel::PowerCtl data = packet.intelData->powerCtl.getValue();

        prochot->setChecked(data.bdProcHot);
        c1e->setChecked(data.c1eEnable);
        disableEfficiencyOptimization->setChecked(data.disableEnergyEfficiencyOpt);
        disableRaceToHalt->setChecked(data.disableRaceToHaltOpt);
    }

    void PowerCtlSBGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->powerCtl.setValue({
            .bdProcHot = prochot->isChecked(),
            .c1eEnable = c1e->isChecked(),
            .disableEnergyEfficiencyOpt = disableEfficiencyOptimization->isChecked(),
            .disableRaceToHaltOpt = disableRaceToHalt->isChecked()
        }, true);
    }
}
