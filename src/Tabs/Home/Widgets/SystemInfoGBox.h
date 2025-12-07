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

#include "pwtShared/Include/SystemInfo.h"
#include "pwtShared/Include/CPU/CpuInfo.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"
#include "LabelInfoWidget.h"

namespace PWT::UI {
    class SystemInfoGBox final: public QGroupBox {
    private:
        static constexpr char defaultStr[] = "Unknown";
        PWTS::OSType osType = PWTS::OSType::Unknown;
        int logicalCPUCount = 0;
        QVBoxLayout *lyt = nullptr;
        LabelInfoWidget *product = nullptr;
        LabelInfoWidget *distribution = nullptr;
        LabelInfoWidget *kernel = nullptr;
        LabelInfoWidget *ram = nullptr;
        LabelInfoWidget *arch = nullptr;
        LabelInfoWidget *profile = nullptr;
        QHash<QString, LabelInfoWidget *> addonInfoList;

        void setDefaultLabels() const;
        void clearGBox() const;
        void setLabels() const;

    public:
        SystemInfoGBox();

        [[nodiscard]] QString getActiveProfile() const { return profile->getText(); }
        void setAppliedProfile(const QString &name) const { profile->setText(name.isEmpty() ? "-" : name); }
        void unsetAppliedProfile() const { profile->setText("-"); }

        void init(const PWTS::CpuInfo &cpuInfo, const PWTS::SystemInfo &sysInfo);
        void setData(const PWTS::DaemonPacket &packet);
        void clearWidget();
    };
}
