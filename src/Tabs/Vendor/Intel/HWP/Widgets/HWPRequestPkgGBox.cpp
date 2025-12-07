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
#include "HWPRequestPkgGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::INTEL {
    HWPRequestPkgGBox::HWPRequestPkgGBox(const QSet<PWTS::Feature> &cpuFeatures): QGroupBox("Power Management Control Hints (Package)") {
        QVBoxLayout *lyt = new QVBoxLayout();

        hwpRequestWidget = new HWPRequestWidget(
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_EPP),
            cpuFeatures.contains(PWTS::Feature::INTEL_HWP_ACT_WIND),
            false,
            false
        );

        lyt->addWidget(hwpRequestWidget);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        setEnabled(false);
    }

    void HWPRequestPkgGBox::setData(const PWTS::DaemonPacket &packet) {
        const PWTS::Intel::HWPRequestPkg reqPkg = packet.intelData->hwpRequestPkg.getValue();
        PWTS::Intel::HWPCapabilities pkgCaps {INT_MAX, 0};

        for (const PWTS::Intel::IntelThreadData &thd: packet.intelData->threadData) {
            if (!thd.hwpCapapabilities.isValid())
                continue;

            const PWTS::Intel::HWPCapabilities caps = thd.hwpCapapabilities.getValue();

            if (caps.lowestPerf < pkgCaps.lowestPerf)
                pkgCaps.lowestPerf = caps.lowestPerf;

            if (caps.highestPerf > pkgCaps.highestPerf)
                pkgCaps.highestPerf = caps.highestPerf;
        }

        setEnabled(packet.intelData->hwpRequestPkg.isValid() && pkgCaps.lowestPerf < INT_MAX && pkgCaps.highestPerf > 0);

        if (!isEnabled()) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        hwpRequestWidget->setRanges(pkgCaps);
        hwpRequestWidget->setRequestPkgData(reqPkg);
    }

    void HWPRequestPkgGBox::setDataForPacket(const PWTS::ClientPacket &packet) const {
        if (!isEnabled())
            return;

        packet.intelData->hwpRequestPkg.setValue(hwpRequestWidget->getRequestPkgData(), isEnabled());
    }
}
