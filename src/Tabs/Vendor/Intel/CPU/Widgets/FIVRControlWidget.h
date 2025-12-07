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

#include "../../../../Include/GridWidget.h"
#include "../../../../Widgets/SliderUnitWidget.h"
#include "pwtShared/Include/Feature.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::INTEL {
    class FIVRControlWidget final: public GridWidget {
    private:
        QPointer<SliderUnitWidget> cpu;
        QPointer<SliderUnitWidget> gpu;
        QPointer<SliderUnitWidget> cache;
        QPointer<SliderUnitWidget> sysAgent;
        QPointer<SliderUnitWidget> unslice;

    public:
        explicit FIVRControlWidget(const QSet<PWTS::Feature> &cpuFeatures);

        void setRanges(const PWTS::MinMax &range) const;
        void setData(const PWTS::DaemonPacket &packet) const;
        void setDataForPacket(const PWTS::ClientPacket &packet) const;
    };
}
