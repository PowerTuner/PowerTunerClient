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
#include <QFile>
#include <QCryptographicHash>

#include "ClientSettings.h"
#include "pwtShared/Utils.h"
#include "pwtShared/DaemonSettings.h"

namespace PWT {
    ClientSettings::ClientSettings(const QString &appDataPath) {
        if (!appDataPath.isEmpty())
            path = QString("%1/powertuner.pwt").arg(appDataPath);
    }

    Daemon ClientSettings::defaultDaemon() const {
        return {"Local", "127.0.0.1", PWTS::DaemonSettings::DefaultTCPPort};
    }

    void ClientSettings::load() {
        const QByteArray fdata = PWTS::loadFile(path);

        if (fdata.isEmpty()) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Failed to load client settings, using defaults");
            return;
        }

        QDataStream ds(fdata);
        QString fsignature;
        QByteArray checksum;
        QByteArray settingsData;
        CData cdata;

        ds >> fsignature;
        if (fsignature != signature) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Invalid client settings signature, using defaults");
            return;
        }

        ds >> checksum >> settingsData;
        if (checksum != QCryptographicHash::hash(settingsData, QCryptographicHash::Sha256)) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Invalid client settings checksum, using defaults");
            return;
        }

        if (!PWTS::unpackData<ClientSettings::CData>(settingsData, cdata)) {
            _data.daemonList.append(defaultDaemon());
            emit logMessageSent("Failed to unpack client settings, using defaults");
            return;
        }

        if (cdata.daemonList.isEmpty())
            cdata.daemonList.append(defaultDaemon());

        _data = cdata;
    }

    void ClientSettings::save(const bool quiet) {
        if (path.isEmpty()) {
            emit logMessageSent(QStringLiteral("ClientSettings: no disk path, cannot save to disk"));
            return;
        }

        QByteArray data, cdata;
        QDataStream ds(&data, QIODevice::WriteOnly);

        if (!PWTS::packData<ClientSettings::CData>(_data, cdata)) {
            emit logMessageSent("ClientSettings: failed to pack data, save failed");
            return;
        }

        ds << signature <<
            QCryptographicHash::hash(cdata, QCryptographicHash::Sha256) <<
            cdata;

        if (!PWTS::writeFile(path, data))
            emit logMessageSent("Failed to save client settings");
        else if (!quiet)
            emit logMessageSent(QStringLiteral("Client settings saved successfully"));
    }

    void ClientSettings::reset() {
        CData data {};

        data.daemonList = _data.daemonList;
        _data = data;
    }

    QDataStream &operator<<(QDataStream &ds, const ClientSettings::CData &cdata) {
        ds << cdata.version <<
            cdata.daemonList <<
            cdata.closeToTray <<
            cdata.alwaysOnTop <<
            cdata.rememberWindowState <<
            cdata.windowState <<
            cdata.autoConnectDaemonName;

        return ds;
    }

    QDataStream &operator>>(QDataStream &ds, ClientSettings::CData &cdata) {
        ds >> cdata.version >>
            cdata.daemonList >>
            cdata.closeToTray >>
            cdata.alwaysOnTop >>
            cdata.rememberWindowState >>
            cdata.windowState >>
            cdata.autoConnectDaemonName;

        return ds;
    }
}
