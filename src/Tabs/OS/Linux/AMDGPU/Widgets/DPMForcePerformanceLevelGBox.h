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
#pragma once

#include <QComboBox>
#include <QCheckBox>

#include "EngineClockSCLKGBox.h"
#include "pwtShared/Include/Packets/ClientPacket.h"

namespace PWT::UI::LNX::AMD {
    class DPMForcePerformanceLevelGBox final: public QGroupBox {
        Q_OBJECT

    private:
        QComboBox *dpmPerfLevel = nullptr;
        QCheckBox *resetPowerLevels = nullptr;
        EngineClockSCLKGBox *engineClockSclkGBox = nullptr;
        int index;

    public:
        explicit DPMForcePerformanceLevelGBox(int idx);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet) const;

    private slots:
        void onDpmPerfLevelChanged(const QString &text) const;
    };
}
