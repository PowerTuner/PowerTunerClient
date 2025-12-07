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
#include <QApplication>

#include "TrayIcon.h"

namespace PWT {
    TrayIcon::TrayIcon() {
        QAction *showApp = new QAction("Open PowerTuner");

        tray = new QSystemTrayIcon(QIcon(":/ico/pwt"));
        contextMenu = new QMenu("PowerTuner");
        activeProfile = new QAction("Active profile: -");
        unreadLogs = new QAction("Logs: 0");
        connectionStatus = new QAction("Daemon: disconnected");
        commandStatus = new QAction("Command: -");
        closeApp = new QAction("Exit");

        activeProfile->setEnabled(false);
        unreadLogs->setEnabled(false);
        connectionStatus->setEnabled(false);
        commandStatus->setEnabled(false);

        contextMenu->addAction(showApp);
        contextMenu->addAction(closeApp);
        contextMenu->addSeparator();
        contextMenu->addActions({connectionStatus, commandStatus, unreadLogs, activeProfile});
        tray->setContextMenu(contextMenu);

        QObject::connect(tray, &QSystemTrayIcon::activated, this, &TrayIcon::onTrayActivated);
        QObject::connect(showApp, &QAction::triggered, this, &TrayIcon::onTrayShowApp);
        QObject::connect(closeApp, &QAction::triggered, this, &QApplication::quit);
    }

    TrayIcon::~TrayIcon() {
        contextMenu->clear();
        delete contextMenu;
        delete tray;
    }

    void TrayIcon::reset() const {
        setStatus(false, "-");
        unsetActiveProfile();
    }

    void TrayIcon::setActiveProfile(const QString &name) const {
        activeProfile->setText(QString("Active profile: %1").arg(name.isEmpty() ? "-" : name));
    }

    void TrayIcon::unsetActiveProfile() const {
        activeProfile->setText(QStringLiteral("Active profile: -"));
    }

    void TrayIcon::setUnreadLogs(const int unread) const {
        unreadLogs->setText(QString("Logs: %1").arg(unread));
    }

    void TrayIcon::setStatus(const bool isConnected, const QString &status) const {
        connectionStatus->setText(QString("Daemon: %1").arg(isConnected ? "Connected" : "Disconnected"));
        commandStatus->setText(QString("Command: %1").arg(status));
    }

    void TrayIcon::onTrayActivated(const QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
            case QSystemTrayIcon::Trigger:
                onTrayShowApp();
                break;
            default:
                break;
        }
    }

    void TrayIcon::onTrayShowApp() {
        close();
        emit showMainWindow();
    }
}
