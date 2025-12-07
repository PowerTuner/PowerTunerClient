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

#include "CPPCWidget.h"
#include "pwtShared/Include/Feature.h"
#include "pwtShared/Include/Packets/ClientPacket.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::AMD {
    class CPPCGBox final: public QGroupBox {
        Q_OBJECT

    private:
        struct WData final {
            bool valid;
            PWTS::AMD::CPPCCapability1 caps1;
            PWTS::AMD::CPPCRequest request;
        };

        QCheckBox *enable = nullptr;
        QComboBox *cpuSelect = nullptr;
        QCheckBox *applyToAll = nullptr;
        CPPCWidget *cppcWidget = nullptr;
        QPointer<QCheckBox> preferOSOpt;
        int prevCpuSelect = 0;
        QList<WData> data;

        void updateCPPCWidget(int idx) const;

    public:
        explicit CPPCGBox(const QSet<PWTS::Feature> &cpuFeatures, int threadCount);

        void setData(const PWTS::DaemonPacket &packet);
        void setDataForPacket(const PWTS::ClientPacket &packet);

    private slots:
        void onApplyToAllChanged(Qt::CheckState state) const;
        void onCpuSelectChanged(int idx);
    };
}
