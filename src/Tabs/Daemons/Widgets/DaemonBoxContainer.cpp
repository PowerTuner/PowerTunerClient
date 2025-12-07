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
#include "DaemonBoxContainer.h"
#include "pwtShared/Utils.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI {
    DaemonBoxContainer::DaemonBoxContainer() {
        lyt = new FlowLayout();

        setLayout(lyt);
    }

    bool DaemonBoxContainer::isValidDaemon(const Daemon &data) const {
        if (data.name.isEmpty()) {
            UILogger::getInstance()->writeLog(QString("Add daemon: name cannot be empty"));
            return false;

        } else if (data.adr.isEmpty()) {
            UILogger::getInstance()->writeLog(QString("Add daemon: %1: address cannot be empty").arg(data.name));
            return false;

        } else if (!PWTS::isValidPort(data.port)) {
            UILogger::getInstance()->writeLog(QString("Add daemon: %1: %2 is not a valid port").arg(data.name, QString::number(data.port)));
            return false;
        }

        return true;
    }

    bool DaemonBoxContainer::addDaemon(const Daemon &data) {
        if (!isValidDaemon(data))
            return false;

        DaemonBox *dbox = new DaemonBox(data.name, data.adr, data.port);

        lyt->addWidget(dbox);
        widgetList.append(dbox);

        QObject::connect(dbox, &DaemonBox::connectDaemon, this, &DaemonBoxContainer::onDaemonConnectionRequested);
        QObject::connect(dbox, &DaemonBox::disconnectDaemon, this, &DaemonBoxContainer::onDaemonDisconnectionRequested);
        QObject::connect(dbox, &DaemonBox::deleteDaemon, this, &DaemonBoxContainer::onDaemonDeleted);
        return true;
    }

    void DaemonBoxContainer::addDaemons(const QList<Daemon> &daemonList) {
       for (const Daemon &dbox: daemonList) {
            if (!addDaemon(dbox))
                UILogger::getInstance()->writeLog(QString("Add daemons: failed to add %1, skip..").arg(dbox.name));
        }
    }

    void DaemonBoxContainer::setActiveDaemon(const QString &adr, const quint16 port) const {
        for (DaemonBox *dbox: widgetList) {
            if (dbox->getAddress() != adr && dbox->getPort() != port)
                continue;

            activeDBox = dbox;
            break;
        }
    }

    void DaemonBoxContainer::setDaemonAsDisconnected() const {
        if (activeDBox == nullptr)
            return;

        activeDBox->setConnectionResult(false);
        activeDBox = nullptr;
    }

    void DaemonBoxContainer::setDaemonAsConnected() const {
        if (activeDBox == nullptr) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        activeDBox->setConnectionResult(true);
    }

    void DaemonBoxContainer::onDaemonConnectionRequested(const QString &adr, const quint16 port) {
        DaemonBox *dbox = qobject_cast<DaemonBox *>(QObject::sender());

        if (dbox == nullptr) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        if (activeDBox != nullptr)
            activeDBox->setConnectionResult(false);

        activeDBox = dbox;
        emit daemonConnectionRequest(adr, port);
    }

    void DaemonBoxContainer::onDaemonDisconnectionRequested() {
        emit daemonDisconnectionRequest();
    }

    void DaemonBoxContainer::onDaemonDeleted() {
        DaemonBox *dbox = qobject_cast<DaemonBox *>(QObject::sender());

        if (dbox == nullptr) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid object"));
            return;
        }

        if (dbox == activeDBox) {
            activeDBox = nullptr;
            emit daemonDisconnectionRequest();
        }

        lyt->removeWidget(dbox);
        widgetList.remove(widgetList.indexOf(dbox));
        dbox->deleteLater();
        emit daemonDeleted({dbox->getName(), dbox->getAddress(), dbox->getPort()});
    }
}
