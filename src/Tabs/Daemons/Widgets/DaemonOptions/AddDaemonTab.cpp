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

#include "AddDaemonTab.h"
#include "pwtShared/DaemonSettings.h"

namespace PWT::UI {
    AddDaemonTab::AddDaemonTab() {
        name = new QLineEdit();
        adr = new QLineEdit();
        tcpPort = new QSpinBox();
        addBtn = new QPushButton("Add");

        tcpPort->setRange(1000, UINT16_MAX);
        tcpPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);

        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Name"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), name);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Address"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), adr);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("TCP port"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), tcpPort);
        lyt->addWidget(addBtn);

        QObject::connect(addBtn, &QPushButton::clicked, this, &AddDaemonTab::onAddBtnClicked);
    }

    void AddDaemonTab::clearTab() const {
        name->clear();
        adr->clear();
        tcpPort->setValue(PWTS::DaemonSettings::DefaultTCPPort);
    }

    void AddDaemonTab::onAddBtnClicked() {
        emit addBtnClicked(Daemon(name->text(), adr->text(), tcpPort->value()));
    }
}
