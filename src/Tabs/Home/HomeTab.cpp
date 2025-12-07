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
#include "HomeTab.h"
#include "../TabUtils.h"

namespace PWT::UI {
    HomeTab::HomeTab(const QSharedPointer<ClientSettings> &clientSettings) {
        QVBoxLayout *rightSideLyt = new QVBoxLayout();
        QHBoxLayout *tabLyt = new QHBoxLayout();
        QScrollArea *scrollArea = createScrollArea();

        leftSideLyt = new QVBoxLayout();
        cpuInfo = new CPUInfoGBox();
        sysInfo = new SystemInfoGBox();
        powerTuner = new PowerTunerGBox();
        settings = new ClientSettingsWidget(clientSettings);

        leftSideLyt->addWidget(powerTuner);
        leftSideLyt->addWidget(sysInfo);
        leftSideLyt->addWidget(cpuInfo);
        rightSideLyt->addWidget(settings);
        scrollArea->widget()->setLayout(leftSideLyt);
        tabLyt->addWidget(scrollArea);
        tabLyt->addLayout(rightSideLyt);

        setLayout(tabLyt);

        QObject::connect(settings, &ClientSettingsWidget::settingsSaved, this, &HomeTab::onClientSettingsSaved);
    }

    void HomeTab::init(const PWTS::DeviceInfoPacket &packet) {
        sysInfo->init(packet.cpuInfo, packet.sysInfo);
        cpuInfo->init(packet.cpuInfo);
        powerTuner->setDaemonVersion(packet.daemonMajorVersion, packet.daemonMinorVersion);
        powerTuner->setDaemonPWTSVersion(packet.daemonPwtsMajorVersion, packet.daemonPwtsMinorVersion);

        if (!packet.gpusInfo.isEmpty()) {
            gpuInfo = new GPUInfoGBox();

            leftSideLyt->addWidget(gpuInfo);
            gpuInfo->setData(packet.gpusInfo);
        }
    }

    void HomeTab::refreshTab(const PWTS::DaemonPacket &packet) const {
        cpuInfo->setData(packet);
        sysInfo->setData(packet);
    }

    void HomeTab::clearWidget() const {
        if (!gpuInfo.isNull()) {
            leftSideLyt->removeWidget(gpuInfo);
            delete gpuInfo;
        }

        cpuInfo->clearWidget();
        sysInfo->clearWidget();
        powerTuner->clearWidget();
    }

    void HomeTab::loadClientSettings() const {
        settings->initAutoConnectDaemonList();
        settings->loadSettings();
    }

    void HomeTab::onClientSettingsSaved() {
        emit clientSettingsSaved();
    }
}
