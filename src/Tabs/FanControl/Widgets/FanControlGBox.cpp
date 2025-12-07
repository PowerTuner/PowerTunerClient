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
#include <QLabel>

#include "FanControlGBox.h"
#include "pwtClientCommon/UILogger.h"

namespace PWT::UI::FANS {
    FanControlGBox::FanControlGBox(const QSet<PWTS::Feature> &features, const QString &fanLabel, const QString &id): QGroupBox(fanLabel) {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *modeLyt = new QHBoxLayout();
        FanCurveWidget *curveWdg = new FanCurveWidget();

        fanID = id;
        pointsLyt = new FlowLayout();
        mode = new QComboBox();

        mode->addItems({"Auto", "Manual"});
        curveWdg->disableRemoveBtn();
        curveWidgetList.append(curveWdg); // 1 point is always available

        modeLyt->addStretch();
        modeLyt->addWidget(mode);

        pointsLyt->addWidget(curveWdg);
        lyt->addLayout(modeLyt);
        lyt->addSpacing(8);
        lyt->addLayout(pointsLyt);

        if (features.contains(PWTS::Feature::FAN_CURVE)) {
            QHBoxLayout *addPointLyt = new QHBoxLayout();

            addPointBtn = new QPushButton("Add trip point");

            addPointLyt->addStretch();
            addPointLyt->addWidget(addPointBtn);
            lyt->addSpacing(6);
            lyt->addLayout(addPointLyt);

            QObject::connect(addPointBtn, &QPushButton::clicked, this, &FanControlGBox::onAddPointBtnClicked);
        }

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);
        onModeChanged(mode->currentIndex());
        setEnabled(false);

        QObject::connect(mode, &QComboBox::currentIndexChanged, this, &FanControlGBox::onModeChanged);
    }

    void FanControlGBox::addTripPoint() {
        FanCurveWidget *wdg = new FanCurveWidget();

        pointsLyt->addWidget(wdg);
        curveWidgetList.append(wdg);
        wdg->setEnabled(mode->currentIndex() == 1);

        QObject::connect(wdg, &FanCurveWidget::removePoint, this, &FanControlGBox::onRemovePoint);
    }

    void FanControlGBox::setData(const PWTS::DaemonPacket &packet) {
        const bool isValid = packet.fanData.contains(fanID) && packet.fanData[fanID].mode.isValid();

        setEnabled(isValid);

        if (!isValid) {
            UILogger::getInstance()->writeLog(QStringLiteral("Invalid data from packet"));
            return;
        }

        mode->setCurrentIndex(packet.fanData[fanID].mode.getValue());

        for (int i=0,l=packet.fanData[fanID].curve.size(); i<l; ++i) {
            const auto [temp, speed] = packet.fanData[fanID].curve[i];

            if (i >= curveWidgetList.size())
                addTripPoint();

            curveWidgetList[i]->setData(temp, speed);
        }

        // remove unused trip points, if any
        for (int i=0,l=(curveWidgetList.size() - packet.fanData[fanID].curve.size()); i<l; ++i) {
            if (curveWidgetList.size() == 1) { // do not delete the last one
                curveWidgetList.last()->setData(0, 0);
                break;
            }

            FanCurveWidget *wdg = curveWidgetList.takeLast();

            pointsLyt->removeWidget(wdg);
            delete wdg;
        }
    }

    PWTS::FanData FanControlGBox::getData() const {
        PWTS::FanData data;

        data.mode.setValue(mode->currentIndex(), isEnabled());

        for (const FanCurveWidget *curve: curveWidgetList)
            data.curve.append(std::make_pair(curve->getTemperature(), curve->getSpeed()));

        return data;
    }

    void FanControlGBox::onModeChanged(const int idx) const {
        const bool isManual = idx != 0;

        addPointBtn->setEnabled(isManual);

        for (FanCurveWidget *wdg: curveWidgetList)
            wdg->setEnabled(isManual);
    }

    void FanControlGBox::onAddPointBtnClicked() {
        addTripPoint();
    }

    void FanControlGBox::onRemovePoint(const QWidget *wdg) {
        const int idx = curveWidgetList.indexOf(wdg);

        if (idx == -1) {
            UILogger::getInstance()->writeLog(QStringLiteral("failed to delete trip point"));
            return;
        }

        FanCurveWidget *curveWdg = curveWidgetList.takeAt(idx);

        pointsLyt->removeWidget(curveWdg);
        delete wdg;
    }
}
