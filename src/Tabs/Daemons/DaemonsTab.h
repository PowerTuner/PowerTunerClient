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

#include "Widgets/DaemonOptions/DaemonOptions.h"
#include "Widgets/DaemonBoxContainer.h"

namespace PWT::UI {
    class DaemonsTab final: public QWidget {
        Q_OBJECT

    private:
        DaemonOptions *daemonOptions = nullptr;
        DaemonBoxContainer *daemonContainer = nullptr;

    public:
        DaemonsTab();

        void enableDaemonOptionsTabs(const bool enable) const { daemonOptions->enableTabs(enable); }
        void updateProfileList(const QList<QString> &list) const { daemonOptions->updateProfilesList(list); }
        void updateDaemonSettings(const QByteArray &data) const { daemonOptions->updateSettings(data); }
        void optionsFocusSettingsTab() const { daemonOptions->focusSettingsTab(); }
        void optionsFocusOptionsTab() const { daemonOptions->focusOptionsTab(); }
        void setActiveDaemon(const QString &adr, const quint16 port) const { daemonContainer->setActiveDaemon(adr, port); }
        void setDaemonAsDisconnected() const { daemonContainer->setDaemonAsDisconnected(); }
        void setDaemonAsConnected() const { daemonContainer->setDaemonAsConnected(); }

        void init(const PWTS::DeviceInfoPacket &packet) const;
        void refreshTab(const PWTS::DaemonPacket &packet) const;
        void clearWidget() const;
        void loadDaemons(const QList<Daemon> &daemonList) const;

    private slots:
        void onDaemonAdded(const PWT::Daemon &data);
        void onDaemonDeleted(const PWT::Daemon &data);
        void onDaemonConnectionRequest(const QString &adr, quint16 port);
        void onDaemonDisconnectionRequest();
        void onReloadDaemonSettins();
        void onApplyDaemonSettings(const QByteArray &data);
        void onExportProfileRequest(const QString &name);
        void onImportProfilesRequest();

    signals:
        void daemonAdded(const PWT::Daemon &data);
        void daemonDeleted(const PWT::Daemon &data);
        void daemonConnectionRequest(const QString &adr, quint16 port);
        void daemonDisconnectionRequest();
        void daemonSettingsReloadRequested();
        void daemonSettingsApplyRequested(const QByteArray &data);
        void exportProfileRequested(const QString &name);
        void importProfilesRequested();
    };
}
