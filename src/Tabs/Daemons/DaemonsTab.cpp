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
#include "DaemonsTab.h"
#include "../TabUtils.h"

namespace PWT::UI {
    DaemonsTab::DaemonsTab() {
        QScrollArea *scrollArea = createScrollArea();
        QVBoxLayout *scrollWidgLyt = new QVBoxLayout();
        QHBoxLayout *tabLyt = new QHBoxLayout();

        daemonOptions = new DaemonOptions();
        daemonContainer = new DaemonBoxContainer();

        scrollWidgLyt->addWidget(daemonContainer);
        scrollArea->widget()->setLayout(scrollWidgLyt);
        tabLyt->addWidget(scrollArea);
        tabLyt->addWidget(daemonOptions);

        setLayout(tabLyt);

        QObject::connect(daemonContainer, &DaemonBoxContainer::daemonDeleted, this, &DaemonsTab::onDaemonDeleted);
        QObject::connect(daemonContainer, &DaemonBoxContainer::daemonConnectionRequest, this, &DaemonsTab::onDaemonConnectionRequest);
        QObject::connect(daemonContainer, &DaemonBoxContainer::daemonDisconnectionRequest, this, &DaemonsTab::onDaemonDisconnectionRequest);
        QObject::connect(daemonOptions, &DaemonOptions::daemonAdded, this, &DaemonsTab::onDaemonAdded);
        QObject::connect(daemonOptions, &DaemonOptions::applyDaemonSettins, this, &DaemonsTab::onApplyDaemonSettings);
        QObject::connect(daemonOptions, &DaemonOptions::reloadDaemonSettings, this, &DaemonsTab::onReloadDaemonSettins);
        QObject::connect(daemonOptions, &DaemonOptions::exportProfile, this, &DaemonsTab::onExportProfileRequest);
        QObject::connect(daemonOptions, &DaemonOptions::importProfiles, this, &DaemonsTab::onImportProfilesRequest);
    }

    void DaemonsTab::init(const PWTS::DeviceInfoPacket &packet) const {
        daemonOptions->init(packet);
    }

    void DaemonsTab::refreshTab(const PWTS::DaemonPacket &packet) const {
        daemonOptions->setData(packet);
    }

    void DaemonsTab::clearWidget() const {
        daemonOptions->clearWidget();
    }

    void DaemonsTab::loadDaemons(const QList<Daemon> &daemonList) const {
        daemonContainer->addDaemons(daemonList);
    }

    void DaemonsTab::onDaemonAdded(const Daemon &data) {
        if (!daemonContainer->addDaemon(data))
            return;

        daemonOptions->clearAddTab();
        emit daemonAdded(data);
    }

    void DaemonsTab::onDaemonDeleted(const Daemon &data) {
        emit daemonDeleted(data);
    }

    void DaemonsTab::onDaemonConnectionRequest(const QString &adr, const quint16 port) {
        emit daemonConnectionRequest(adr, port);
    }

    void DaemonsTab::onDaemonDisconnectionRequest() {
        emit daemonDisconnectionRequest();
    }

    void DaemonsTab::onApplyDaemonSettings(const QByteArray &data) {
        emit daemonSettingsApplyRequested(data);
    }

    void DaemonsTab::onReloadDaemonSettins() {
        emit daemonSettingsReloadRequested();
    }

    void DaemonsTab::onExportProfileRequest(const QString &name) {
        emit exportProfileRequested(name);
    }

    void DaemonsTab::onImportProfilesRequest() {
        emit importProfilesRequested();
    }
}
