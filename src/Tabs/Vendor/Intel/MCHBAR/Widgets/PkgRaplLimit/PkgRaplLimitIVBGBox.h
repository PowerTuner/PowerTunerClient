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

#include <QCheckBox>

#include "PkgRaplLimitGBox.h"
#include "../../../../../Widgets/SliderUnitWidget.h"

namespace PWT::UI::INTEL {
    class PkgRaplLimitIVBGBox final: public PkgRaplLimitGBox {
        Q_OBJECT

    private:
        QCheckBox *lock = nullptr;
        SliderUnitWidget *pl1 = nullptr;
        SliderUnitWidget *pl1Time = nullptr;
        SliderUnitWidget *pl2 = nullptr;
        QCheckBox *pl1Enable = nullptr;
        QCheckBox *pl2Enable = nullptr;

    public:
        PkgRaplLimitIVBGBox();

        void setRanges(const PWTS::MinMax &range) const override;
        void setData(const PWTS::DaemonPacket &packet) override;
        void setDataForPacket(const PWTS::ClientPacket &packet) const override;

    private slots:
        void onPl1EnableChanged(Qt::CheckState state) const;
        void onPl2EnableChanged(Qt::CheckState state) const;
    };
}
