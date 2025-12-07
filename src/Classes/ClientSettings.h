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

#include <QObject>

#include "../Include/Daemon.h"
#include "pwtClientCommon/Include/PWTWindowState.h"

namespace PWT {
    class ClientSettings final: public QObject {
        Q_OBJECT

    private:
        struct CData final {
            int version = 1;
            QList<Daemon> daemonList {};
            UI::PWTWindowState windowState {};
            bool closeToTray = false;
            bool alwaysOnTop = false;
            bool rememberWindowState = true;
            QString autoConnectDaemonName;
        };

        inline static const QString signature {"PWTF"};
        QString path;
        CData _data;

        friend QDataStream &operator<<(QDataStream &ds, const CData &cdata);
        friend QDataStream &operator>>(QDataStream &ds, CData &cdata);

        Daemon defaultDaemon() const;

    public:
        explicit ClientSettings(const QString &appDataPath);

        [[nodiscard]] QList<Daemon> getDaemonList() const { return _data.daemonList; }
        [[nodiscard]] UI::PWTWindowState getWindowState() const { return _data.windowState; }
        [[nodiscard]] bool getCloseToTray() const { return _data.closeToTray; }
        [[nodiscard]] bool getAlwaysOnTop() const { return _data.alwaysOnTop; }
        [[nodiscard]] bool getRememberWindowState() const { return _data.rememberWindowState; }
        [[nodiscard]] QString getAutoConnectDaemonName() const { return _data.autoConnectDaemonName; }
        void setWindowState(const UI::PWTWindowState &state) { _data.windowState = state; }
        void setCloseToTray(const bool enable) { _data.closeToTray = enable; }
        void setAlwaysOnTop(const bool enable) { _data.alwaysOnTop = enable; }
        void setRememberWindowState(const bool enable) { _data.rememberWindowState = enable; }
        void setAutoConnectDaemonName(const QString &adr) { _data.autoConnectDaemonName = adr; }
        void addDaemon(const Daemon &dbox) { _data.daemonList.append(dbox); }
        void removeDaemon(const Daemon &dbox) { _data.daemonList.removeOne(dbox); }

        void load();
        void save(bool quiet = false);
        void reset();

    signals:
        void logMessageSent(const QString &msg);
    };
}
