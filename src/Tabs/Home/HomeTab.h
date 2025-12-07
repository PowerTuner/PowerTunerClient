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

#include <QPointer>

#include "pwtShared/Include/Packets/DeviceInfoPacket.h"
#include "Widgets/CPUInfoGBox.h"
#include "Widgets/GPUInfoGBox.h"
#include "Widgets/SystemInfoGBox.h"
#include "Widgets/ClientSettingsWidget.h"
#include "Widgets/PowerTunerGBox.h"

namespace PWT::UI {
    class HomeTab final: public QWidget {
        Q_OBJECT

    private:
        QVBoxLayout *leftSideLyt = nullptr;
        CPUInfoGBox *cpuInfo = nullptr;
        SystemInfoGBox *sysInfo = nullptr;
        ClientSettingsWidget *settings = nullptr;
        PowerTunerGBox *powerTuner = nullptr;
        QPointer<GPUInfoGBox> gpuInfo;

    public:
        explicit HomeTab(const QSharedPointer<ClientSettings> &clientSettings);

        [[nodiscard]] QString getActiveProfile() const { return sysInfo->getActiveProfile(); }
        void initAutoConnectDaemonList() const { settings->initAutoConnectDaemonList(); }
        void setAppliedProfile(const QString &name) const { sysInfo->setAppliedProfile(name); }
        void unsetAppliedProfile() const { sysInfo->unsetAppliedProfile(); }

        void init(const PWTS::DeviceInfoPacket &packet);
        void refreshTab(const PWTS::DaemonPacket &packet) const;
        void clearWidget() const;
        void loadClientSettings() const;

    private slots:
        void onClientSettingsSaved();

    signals:
        void clientSettingsSaved();
    };
}
