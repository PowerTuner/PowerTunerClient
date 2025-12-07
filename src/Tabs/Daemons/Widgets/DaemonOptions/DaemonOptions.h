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

#include "AddDaemonTab.h"
#include "DaemonSettingsTab.h"
#include "ActionsTab.h"

namespace PWT::UI {
    class DaemonOptions final: public QTabWidget {
        Q_OBJECT

    private:
        enum struct TabIndex: int {
            ADD = 0,
            SETTINGS,
            OPTIONS
        };

        AddDaemonTab *addDaemonTab = nullptr;
        DaemonSettingsTab *settingsTab = nullptr;
        ActionsTab *actionsTab = nullptr;

    public:
        DaemonOptions();

        void focusSettingsTab() { setCurrentIndex(static_cast<int>(TabIndex::SETTINGS)); }
        void focusOptionsTab() { setCurrentIndex(static_cast<int>(TabIndex::OPTIONS)); }
        void clearAddTab() const { addDaemonTab->clearTab(); }

        void init(const PWTS::DeviceInfoPacket &packet) const;
        void enableTabs(bool enable);
        void clearWidget();
        void setData(const PWTS::DaemonPacket &packet);
        void updateProfilesList(const QList<QString> &list) const;
        void updateSettings(const QByteArray &data);

    private slots:
        void onAddDaemon(const PWT::Daemon &daemon);
        void onReloadSettings();
        void onApplySettings(const QByteArray &data);
        void onExportProfile(const QString &name);
        void onImportProfiles();

    signals:
        void daemonAdded(const PWT::Daemon &data);
        void reloadDaemonSettings();
        void applyDaemonSettins(const QByteArray &data);
        void exportProfile(const QString &name);
        void importProfiles();
    };
}
