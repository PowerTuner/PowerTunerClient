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
#include <QVBoxLayout>

#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "pwtShared/Include/CPU/CpuInfo.h"
#include "LabelInfoWidget.h"

namespace PWT::UI {
    class CPUInfoGBox final: public QGroupBox {
    private:
        static constexpr char defaultStr[] = "Unknown";
        QVBoxLayout *lyt = nullptr;
        LabelInfoWidget *cpu = nullptr;
        LabelInfoWidget *codename = nullptr;
        LabelInfoWidget *techNode = nullptr;
        LabelInfoWidget *vendor = nullptr;
        LabelInfoWidget *family = nullptr;
        LabelInfoWidget *extFamily = nullptr;
        LabelInfoWidget *model = nullptr;
        LabelInfoWidget *extModel = nullptr;
        LabelInfoWidget *coreCount = nullptr;
        LabelInfoWidget *threadCount = nullptr;
        LabelInfoWidget *stepping = nullptr;
        LabelInfoWidget *l1Data = nullptr;
        LabelInfoWidget *l1ICache = nullptr;
        LabelInfoWidget *l2 = nullptr;
        LabelInfoWidget *l3 = nullptr;
        QHash<QString, LabelInfoWidget *> addonInfoList;

    public:
        CPUInfoGBox();

        void init(const PWTS::CpuInfo &cpuInfo);
        void setData(const PWTS::DaemonPacket &packet) const;
        void clearWidget();
    };
}
