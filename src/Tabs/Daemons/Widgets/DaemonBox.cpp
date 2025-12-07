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
#include <QLabel>
#include <QVBoxLayout>

#include "DaemonBox.h"
#include "../../TabUtils.h"

namespace PWT::UI {
    DaemonBox::DaemonBox(const QString &name, const QString &adr, const quint16 port): QGroupBox(truncateStr(name.toUpper(), 12)) {
        QVBoxLayout *lyt = new QVBoxLayout(this);
        QLabel *adrLbl = new QLabel(truncateStr(adr, 20));
        QLabel *portLbl = new QLabel(QString::number(port));
        QFont lblFont = adrLbl->font();

        connectBtn = new QPushButton("Connect");
        deleteBtn = new QPushButton("Delete");
        dname = name;
        dadr = adr;
        dport = port;

        lblFont.setWeight(QFont::Bold);
        setFixedSize(170, 210);
        setAlignment(Qt::AlignCenter);
        connectBtn->setToolTip(name);
        deleteBtn->setToolTip(name);

        adrLbl->setFont(lblFont);
        adrLbl->setAlignment(Qt::AlignRight);
        adrLbl->setToolTip(adr);

        portLbl->setFont(lblFont);
        portLbl->setAlignment(Qt::AlignRight);

        lyt->addWidget(new QLabel("Address"));
        lyt->addWidget(adrLbl);
        lyt->addWidget(new QLabel("Port"));
        lyt->addWidget(portLbl);
        lyt->addItem(new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed));
        lyt->addWidget(connectBtn);
        lyt->addWidget(deleteBtn);

        setLayout(lyt);

        QObject::connect(connectBtn, &QPushButton::clicked, this, &DaemonBox::connectBtnClicked);
        QObject::connect(deleteBtn, &QPushButton::clicked, this, &DaemonBox::deleteBtnClicked);
    }

    void DaemonBox::setConnectionResult(const bool connected) {
        connectBtn->setText(connected ? "Disconnect" : "Connect");
        connectBtn->setEnabled(true);
        daemonConnected = connected;
    }

    void DaemonBox::connectBtnClicked() {
        connectBtn->setEnabled(false);

        if (!daemonConnected) {
            connectBtn->setText("Connecting..");
            emit connectDaemon(dadr, dport);

        } else {
            connectBtn->setText("Disconnecting..");
            emit disconnectDaemon();
        }
    }

    void DaemonBox::deleteBtnClicked() {
        connectBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
        emit deleteDaemon();
    }
}
