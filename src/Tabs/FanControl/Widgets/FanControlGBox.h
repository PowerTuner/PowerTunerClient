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

#include "FanCurveWidget.h"
#include "../../Include/FlowLayout.h"
#include "pwtShared/Include/Feature.h"
#include "pwtShared/Include/Packets/DaemonPacket.h"

namespace PWT::UI::FANS {
    class FanControlGBox final: public QGroupBox {
        Q_OBJECT

    private:
        FlowLayout *pointsLyt = nullptr;
        QComboBox *mode = nullptr;
        QPushButton *addPointBtn = nullptr;
        QList<FanCurveWidget *> curveWidgetList;
        QString fanID;

        void addTripPoint();

    public:
        FanControlGBox(const QSet<PWTS::Feature> &features, const QString &fanLabel, const QString &id);

        [[nodiscard]] QString getFanID() const { return fanID; }

        void setData(const PWTS::DaemonPacket &packet);
        [[nodiscard]] PWTS::FanData getData() const;

    private slots:
        void onModeChanged(int idx) const;
        void onAddPointBtnClicked();
        void onRemovePoint(const QWidget *wdg);
    };
}
