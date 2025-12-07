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

#include "../../../Include/Daemon.h"
#include "../../Include/FlowLayout.h"
#include "DaemonBox.h"

namespace PWT::UI {
    class DaemonBoxContainer final: public QWidget {
        Q_OBJECT

    private:
        FlowLayout *lyt = nullptr;
        mutable DaemonBox *activeDBox = nullptr;
        QList<DaemonBox *> widgetList;

        [[nodiscard]] bool isValidDaemon(const Daemon &data) const;

    public:
        DaemonBoxContainer();

        [[nodiscard]] bool addDaemon(const Daemon &data);
        void addDaemons(const QList<Daemon> &daemonList);
        void setActiveDaemon(const QString &adr, quint16 port) const;
        void setDaemonAsDisconnected() const;
        void setDaemonAsConnected() const;

    private slots:
        void onDaemonConnectionRequested(const QString &adr, quint16 port);
        void onDaemonDisconnectionRequested();
        void onDaemonDeleted();

    signals:
        void daemonConnectionRequest(const QString &adr, quint16 port);
        void daemonDisconnectionRequest();
        void daemonDeleted(const PWT::Daemon &data);
    };
}
