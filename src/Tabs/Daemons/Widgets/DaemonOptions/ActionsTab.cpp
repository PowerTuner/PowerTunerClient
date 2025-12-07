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

#include "ActionsTab.h"

namespace PWT::UI {
    ActionsTab::ActionsTab() {
        importProfilesBtn = new QPushButton("Import profiles");
        exportProfileList = new QComboBox();
        exportProfileBtn = new QPushButton("Export profile");

        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Select profiles to export from daemon"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), exportProfileList);
        scrollWidgLyt->insertWidget(nextInsertIdx(), exportProfileBtn);
        scrollWidgLyt->insertWidget(nextInsertIdx(), new QLabel("Select profiles to import to daemon"));
        scrollWidgLyt->insertWidget(nextInsertIdx(), importProfilesBtn);

        QObject::connect(exportProfileBtn, &QPushButton::clicked, this, &ActionsTab::exportProfileBtnClicked);
        QObject::connect(importProfilesBtn, &QPushButton::clicked, this, &ActionsTab::importProfilesBtnClicked);
    }

    void ActionsTab::setData(const PWTS::DaemonPacket &packet) const {
        updateProfilesList(packet.profilesList);
    }

    void ActionsTab::updateProfilesList(const QList<QString> &list) const {
        exportProfileList->clear();

        if (list.isEmpty())
            return;

        exportProfileList->addItem("All");
        exportProfileList->addItems(list);
    }

    void ActionsTab::clearTab() const {
        exportProfileList->clear();
    }

    void ActionsTab::exportProfileBtnClicked() {
        emit exportProfile(exportProfileList->currentText());
    }

    void ActionsTab::importProfilesBtnClicked() {
        emit importProfiles();
    }
}
