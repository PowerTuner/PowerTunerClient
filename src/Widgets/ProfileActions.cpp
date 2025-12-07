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
#include "ProfileActions.h"

namespace PWT::UI {
    ProfileActions::ProfileActions() {
        lyt = new QHBoxLayout();
        profiles = new QComboBox();
        loadProfileBtn = new QPushButton("Load");
        saveProfileBtn = new QPushButton("Save");
        deleteProfileBtn = new QPushButton("Delete");
        applyProfileBtn = new QPushButton("Apply");
        profileNameInput = new QLineEdit();
        createProfileBtn = new QPushButton("Create");

        profiles->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        profileNameInput->setMinimumWidth(230);
        profileNameInput->setPlaceholderText("Profile name");
        enableButtons(false);

        lyt->addWidget(profiles);
        lyt->addWidget(deleteProfileBtn);
        lyt->addWidget(saveProfileBtn);
        lyt->addWidget(loadProfileBtn);
        lyt->addWidget(applyProfileBtn);
        lyt->addStretch();
        lyt->addWidget(profileNameInput);
        lyt->addWidget(createProfileBtn);

        QObject::connect(createProfileBtn, &QPushButton::clicked, this, &ProfileActions::onProfileCreateBtnClicked);
        QObject::connect(deleteProfileBtn, &QPushButton::clicked, this, &ProfileActions::onProfileDeleteBtnClicked);
        QObject::connect(saveProfileBtn, &QPushButton::clicked, this, &ProfileActions::onProfileSaveBtnClicked);
        QObject::connect(loadProfileBtn, &QPushButton::clicked, this, &ProfileActions::onProfileLoadBtnClicked);
        QObject::connect(applyProfileBtn, &QPushButton::clicked, this, &ProfileActions::onProfileApplyBtnClicked);
        QObject::connect(profileNameInput, &QLineEdit::returnPressed, this, &ProfileActions::onProfileCreateBtnClicked);
        QObject::connect(profiles, &QComboBox::currentIndexChanged, this, &ProfileActions::onProfileComboIndexChanged);
    }

    QList<QString> ProfileActions::getProfileList() const {
        QList<QString> list;

        for (int i=0,l=profiles->count(); i<l; ++i)
            list.append(profiles->itemText(i));

        return list;
    }

    void ProfileActions::clear() const {
        profiles->clear();
        profileNameInput->clear();
    }

    void ProfileActions::enableButtons(const bool enable) const {
        profiles->setEnabled(enable);
        loadProfileBtn->setEnabled(enable && profiles->currentIndex() > 0);
        saveProfileBtn->setEnabled(enable && profiles->currentIndex() > 0);
        deleteProfileBtn->setEnabled(enable && profiles->currentIndex() > 0);
        applyProfileBtn->setEnabled(enable && profiles->currentIndex() > 0);
        profileNameInput->setEnabled(enable);
        createProfileBtn->setEnabled(enable);
    }

    void ProfileActions::updateProfileList(const QList<QString> &list) const {
        const QString active = profiles->currentText();

        profiles->clear();
        profiles->addItem("");
        profiles->addItems(list);
        profiles->setCurrentIndex(profiles->findText(active));
    }

    void ProfileActions::onProfileComboIndexChanged(const int idx) const {
        loadProfileBtn->setEnabled(idx > 0);
        saveProfileBtn->setEnabled(idx > 0);
        deleteProfileBtn->setEnabled(idx > 0);
        applyProfileBtn->setEnabled(idx > 0);
    }

    void ProfileActions::onProfileCreateBtnClicked() {
        if (profileNameInput->text().isEmpty())
            return;

        const QString name = QString(profileNameInput->text()).remove(nameFilter);

        if (name.isEmpty()) {
            emit logMessageSent(QStringLiteral("Create profile: invalid name (allowed [A-Za-z0-9], underscore and space)"));

        } else {
            emit profileCreateRequest(name, profiles->findText(name) != -1);
            profileNameInput->clear();
        }
    }

    void ProfileActions::onProfileDeleteBtnClicked() {
        if (profiles->currentIndex() == 0)
            return;

        emit profileDeleteRequest(profiles->currentText());
    }

    void ProfileActions::onProfileSaveBtnClicked() {
        if (profiles->currentIndex() == 0)
            return;

        emit profileSaveRequested(profiles->currentText());
    }

    void ProfileActions::onProfileLoadBtnClicked() {
        if (profiles->currentIndex() == 0)
            return;

        emit profileLoadRequested(profiles->currentText());
    }

    void ProfileActions::onProfileApplyBtnClicked() {
        if (profiles->currentIndex() == 0)
            return;

        emit profileApplyRequested(profiles->currentText());
    }
}
