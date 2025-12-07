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

#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

namespace PWT::UI {
    class ProfileActions final: public QObject {
        Q_OBJECT

    private:
        const QRegularExpression nameFilter = QRegularExpression(R"([^a-zA-Z_\d\s])");
        QHBoxLayout *lyt = nullptr;
        QComboBox *profiles = nullptr;
        QPushButton *loadProfileBtn = nullptr;
        QPushButton *saveProfileBtn = nullptr;
        QPushButton *deleteProfileBtn = nullptr;
        QPushButton *applyProfileBtn = nullptr;
        QLineEdit *profileNameInput = nullptr;
        QPushButton *createProfileBtn = nullptr;

    public:
        ProfileActions();

        [[nodiscard]] QHBoxLayout *getLayout() const { return lyt; }

        [[nodiscard]] QList<QString> getProfileList() const;
        void clear() const;
        void enableButtons(bool enable) const;
        void updateProfileList(const QList<QString> &list) const;

    private slots:
        void onProfileComboIndexChanged(int idx) const;
        void onProfileCreateBtnClicked();
        void onProfileDeleteBtnClicked();
        void onProfileSaveBtnClicked();
        void onProfileLoadBtnClicked();
        void onProfileApplyBtnClicked();

    signals:
        void logMessageSent(const QString &msg);
        void profileCreateRequest(const QString &name, bool askOverwrite);
        void profileDeleteRequest(const QString &name);
        void profileSaveRequested(const QString &name);
        void profileLoadRequested(const QString &name);
        void profileApplyRequested(const QString &name);
    };
}
