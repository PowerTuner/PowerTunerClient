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
#include "DaemonOptions.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI {
    DaemonOptions::DaemonOptions() {
        addDaemonTab = new AddDaemonTab();
        settingsTab = new DaemonSettingsTab();
        actionsTab = new ActionsTab();

        addTab(addDaemonTab, "Add");
        addTab(settingsTab, "Settings");
        addTab(actionsTab, "Options");
        setFixedWidth(300);
        setCurrentIndex(0);
        enableTabs(false);

        QObject::connect(addDaemonTab, &AddDaemonTab::addBtnClicked, this, &DaemonOptions::onAddDaemon);
        QObject::connect(settingsTab, &DaemonSettingsTab::applySettings, this, &DaemonOptions::onApplySettings);
        QObject::connect(settingsTab, &DaemonSettingsTab::reloadSettings, this, &DaemonOptions::onReloadSettings);
        QObject::connect(actionsTab, &ActionsTab::exportProfile, this, &DaemonOptions::onExportProfile);
        QObject::connect(actionsTab, &ActionsTab::importProfiles, this, &DaemonOptions::onImportProfiles);
    }

    void DaemonOptions::enableTabs(const bool enable) {
        setTabEnabled(static_cast<int>(TabIndex::SETTINGS), enable);
        setTabEnabled(static_cast<int>(TabIndex::OPTIONS), enable);
    }

    void DaemonOptions::clearWidget() {
        settingsTab->clearTab();
        actionsTab->clearTab();
        enableTabs(false);
    }

    void DaemonOptions::init(const PWTS::DeviceInfoPacket &packet) const {
        settingsTab->init(packet);
    }

    void DaemonOptions::setData(const PWTS::DaemonPacket &packet) {
        settingsTab->setData(packet);
        actionsTab->setData(packet);
        enableTabs(true);
    }

    void DaemonOptions::updateProfilesList(const QList<QString> &list) const {
        settingsTab->updateProfilesList(list);
        actionsTab->updateProfilesList(list);
    }

    void DaemonOptions::updateSettings(const QByteArray &data) {
        if (!settingsTab->updateSettings(data))
            UILogger::getInstance()->writeLog(QStringLiteral("Failed to load daemon settings"));
        else
            setCurrentIndex(static_cast<int>(TabIndex::SETTINGS));
    }

    void DaemonOptions::onAddDaemon(const PWT::Daemon &daemon) {
        emit daemonAdded(daemon);
    }

    void DaemonOptions::onApplySettings(const QByteArray &data) {
        emit applyDaemonSettins(data);
    }

    void DaemonOptions::onReloadSettings() {
        emit reloadDaemonSettings();
    }

    void DaemonOptions::onExportProfile(const QString &name) {
        emit exportProfile(name);
    }

    void DaemonOptions::onImportProfiles() {
        emit importProfiles();
    }
}
