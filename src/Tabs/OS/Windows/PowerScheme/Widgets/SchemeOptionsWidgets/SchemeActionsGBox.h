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

#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>

namespace PWT::UI::WIN {
    class SchemeActionsGBox final: public QGroupBox {
        Q_OBJECT

    private:
        QCheckBox *setActive = nullptr;
        QCheckBox *deleteFlag = nullptr;
        QCheckBox *resetFlag = nullptr;
        QPushButton *deleteBtn = nullptr;

    public:
        SchemeActionsGBox();

		void switchScheme(const QString &scheme, bool isActive, bool deleteFlagged, bool resetFlagged) const;

    private slots:
        void onSetActiveChanged(Qt::CheckState state);
        void onDeleteFlagChanged(Qt::CheckState state);
        void onResetFlagChanged(Qt::CheckState state);
        void onDeleteBtnClicked();

    signals:
        void setSchemeActive();
        void setDeleteFlag(bool flagged);
        void setResetFlag(bool flagged);
        void deleteScheme();
    };
}
