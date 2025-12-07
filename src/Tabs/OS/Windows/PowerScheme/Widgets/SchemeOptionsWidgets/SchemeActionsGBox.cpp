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
#include <QVBoxLayout>

#include "SchemeActionsGBox.h"
#include "../../../../../Include/FlowLayout.h"

namespace PWT::UI::WIN {
    SchemeActionsGBox::SchemeActionsGBox(): QGroupBox("Scheme Actions") {
        QVBoxLayout *lyt = new QVBoxLayout();
        FlowLayout *actionsLyt = new FlowLayout();
        QHBoxLayout *deleteBtnLyt = new QHBoxLayout();

        setActive = new QCheckBox("Set active");
        deleteFlag = new QCheckBox("Flag for deletion");
        resetFlag = new QCheckBox("Reset values to defaults");
        deleteBtn = new QPushButton("Delete scheme");

        resetFlag->setToolTip("Reset scheme to defaults from default user scheme stored in HKEY_USERS\\.Default.");

        actionsLyt->addWidget(setActive);
        actionsLyt->addWidget(deleteFlag);
        actionsLyt->addWidget(resetFlag);
        deleteBtnLyt->addStretch();
        deleteBtnLyt->addWidget(deleteBtn);

        lyt->addLayout(actionsLyt);
        lyt->addLayout(deleteBtnLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(deleteFlag, &QCheckBox::checkStateChanged, this, &SchemeActionsGBox::onDeleteFlagChanged);
        QObject::connect(resetFlag, &QCheckBox::checkStateChanged, this, &SchemeActionsGBox::onResetFlagChanged);
        QObject::connect(setActive, &QCheckBox::checkStateChanged, this, &SchemeActionsGBox::onSetActiveChanged);
        QObject::connect(deleteBtn, &QCheckBox::clicked, this, &SchemeActionsGBox::onDeleteBtnClicked);
    }

    void SchemeActionsGBox::switchScheme(const QString &scheme, const bool isActive, const bool deleteFlagged, const bool resetFlagged) const {
        const bool isTBD = scheme.startsWith("{guid-tbd");
        const QSignalBlocker sblockAct {setActive};
        const QSignalBlocker sblockDelFlag {deleteFlag};

        setActive->setChecked(isActive);
        setActive->setEnabled(!isActive && !deleteFlagged);
        deleteFlag->setChecked(deleteFlagged);
        deleteFlag->setVisible(!isActive && !isTBD);
        resetFlag->setChecked(resetFlagged);
        resetFlag->setVisible(!isTBD);
        deleteBtn->setVisible(!isActive && isTBD);
    }

    void SchemeActionsGBox::onDeleteFlagChanged(const Qt::CheckState state) {
        const bool flagged = state == Qt::Checked;

        setActive->setEnabled(!flagged);
        emit setDeleteFlag(flagged);
    }

    void SchemeActionsGBox::onResetFlagChanged(const Qt::CheckState state) {
        emit setResetFlag(state == Qt::Checked);
    }

    void SchemeActionsGBox::onSetActiveChanged(const Qt::CheckState state) {
        if (state != Qt::Checked)
            return;

        setActive->setEnabled(false);
        deleteFlag->setVisible(false);
        deleteBtn->setVisible(false);
        emit setSchemeActive();
    }

    void SchemeActionsGBox::onDeleteBtnClicked() {
        emit deleteScheme();
    }
}
