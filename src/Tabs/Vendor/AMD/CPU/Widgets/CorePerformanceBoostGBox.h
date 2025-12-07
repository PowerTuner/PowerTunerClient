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

#include "CorePerformanceBoostWidget.h"

namespace PWT::UI::AMD {
    class CorePerformanceBoostGBox final: public QGroupBox {
        Q_OBJECT

    private:
        CorePerformanceBoostWidget *corePerformanceBoostWidget = nullptr;
        QComboBox *corePerfBoostAll = nullptr;

    public:
        explicit CorePerformanceBoostGBox(int threadCount);

        void setDataForPacket(const PWTS::ClientPacket &packet) const { corePerformanceBoostWidget->setDataForPacket(packet); }

        void setData(const PWTS::DaemonPacket &packet);

    private slots:
        void onCorePerfBoostAllChanged(int idx) const;
    };
}
