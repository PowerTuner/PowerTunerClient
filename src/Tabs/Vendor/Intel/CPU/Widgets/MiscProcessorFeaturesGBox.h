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

#include <QGroupBox>
#include <QCheckBox>
#include <QPointer>

#include "pwtShared/Include/Feature.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::INTEL {
    class MiscProcessorFeaturesGBox final: public QGroupBox {
    private:
        QPointer<QCheckBox> enhancedSpeedStep;
        QPointer<QCheckBox> disableTurboBoost;

    public:
        explicit MiscProcessorFeaturesGBox(const QSet<PWTS::Feature> &cpuFeatures);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet) const;
    };
}
