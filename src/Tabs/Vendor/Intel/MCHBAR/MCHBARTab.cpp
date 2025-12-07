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
#include "MCHBARTab.h"
#include "pwtClientCommon/InputRanges/InputRanges.h"
#include "Widgets/PkgRaplLimit/PkgRaplLimitIVBGBox.h"
#include "Widgets/PkgRaplLimit/PkgRaplLimitTGLGBox.h"

namespace PWT::UI::INTEL {
    MCHBARTab::MCHBARTab(const PWTS::DeviceInfoPacket &packet) {
        const QSet<PWTS::Feature> &features = packet.features.cpu;

        if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT)) {
            if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT_IVB))
                pkgRaplLimitGBox = new PkgRaplLimitIVBGBox();
            else if (features.contains(PWTS::Feature::INTEL_MCHBAR_PKG_RAPL_LIMIT_TGL))
                pkgRaplLimitGBox = new PkgRaplLimitTGLGBox();

            scrollWidgLyt->insertWidget(nextInsertIdx(), pkgRaplLimitGBox);
        }
    }

    void MCHBARTab::refreshTab(const PWTS::DaemonPacket &packet) {
        const QSharedPointer<InputRanges> inputRanges = InputRanges::getInstance();

        if (!pkgRaplLimitGBox.isNull()) {
            pkgRaplLimitGBox->setRanges(inputRanges->getIntelPl());
            pkgRaplLimitGBox->setData(packet);
        }
    }

    void MCHBARTab::setDataForPacket(PWTS::ClientPacket &packet) const {
        if (!pkgRaplLimitGBox.isNull())
            pkgRaplLimitGBox->setDataForPacket(packet);
    }
}
