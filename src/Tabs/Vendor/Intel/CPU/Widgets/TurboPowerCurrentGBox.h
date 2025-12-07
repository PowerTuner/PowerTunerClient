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

#include "../../../../Widgets/SliderUnitWidget.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::INTEL {
    class TurboPowerCurrentGBox final: public QGroupBox {
        Q_OBJECT

    private:
        SliderUnitWidget *tdp = nullptr;
        QCheckBox *tdpEnable = nullptr;
        SliderUnitWidget *tdc = nullptr;
        QCheckBox *tdcEnable = nullptr;

    public:
        TurboPowerCurrentGBox();

        void setRanges(const PWTS::MinMax &tdpRange, const PWTS::MinMax &tdcRange) const;
        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet) const;

    private slots:
        void onTDPEnableChanged(Qt::CheckState state) const;
        void onTDCEnableChanged(Qt::CheckState state) const;
    };
}
