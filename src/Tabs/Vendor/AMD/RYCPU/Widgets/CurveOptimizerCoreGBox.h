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
#include <QComboBox>
#include <QCheckBox>

#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "../../../../Widgets/SliderUnitWidget.h"

namespace PWT::UI::AMD {
    class CurveOptimizerCoreGBox final: public QGroupBox {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            int co;
        };

        QComboBox *cpuSelect = nullptr;
        QCheckBox *enableChk = nullptr;
        QCheckBox *applyToAll = nullptr;
        SliderUnitWidget *co = nullptr;
        QList<WData> data;
        bool enableChecked = false;
        int prevCPU = 0;

        void updateCOSlider(int idx) const;

    public:
        explicit CurveOptimizerCoreGBox(int coreCount);

        void setRanges(const PWTS::MinMax &range) const;
        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet);

    private slots:
        void onCpuSelectChanged(int idx);
        void onApplyToAllStateChanged(Qt::CheckState state) const;
        void onEnableStateChanged(Qt::CheckState state);
    };
}
