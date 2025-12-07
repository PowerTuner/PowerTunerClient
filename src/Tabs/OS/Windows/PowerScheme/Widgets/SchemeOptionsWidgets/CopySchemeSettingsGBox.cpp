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
#include <QHBoxLayout>
#include <QLabel>

#include "CopySchemeSettingsGBox.h"

namespace PWT::UI::WIN {
    CopySchemeSettingsGBox::CopySchemeSettingsGBox(): QGroupBox("Copy Scheme Settings") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *copyFromSchemeLyt = new QHBoxLayout();
        QHBoxLayout *copyFromModeLyt = new QHBoxLayout();
        QHBoxLayout *copyToSchemeLyt = new QHBoxLayout();
        QHBoxLayout *copyToModeLyt = new QHBoxLayout();
        QHBoxLayout *copyBtnLyt = new QHBoxLayout();

        copyFromScheme = new QComboBox();
        copyFromMode = new QComboBox();
        copyToScheme = new QComboBox();
        copyToMode = new QComboBox();
        copyBtn = new QPushButton("Copy settings");

        copyFromMode->addItems({"", "Plugged In", "Battery"});
        copyToMode->addItems({"", "Plugged In", "Battery", "All"});
        copyFromScheme->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        copyToScheme->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        copyFromSchemeLyt->addWidget(new QLabel("From scheme:"));
        copyFromSchemeLyt->addStretch();
        copyFromSchemeLyt->addWidget(copyFromScheme);
        copyFromModeLyt->addWidget(new QLabel("From mode:"));
        copyFromModeLyt->addStretch();
        copyFromModeLyt->addWidget(copyFromMode);
        copyToSchemeLyt->addWidget(new QLabel("To scheme:"));
        copyToSchemeLyt->addStretch();
        copyToSchemeLyt->addWidget(copyToScheme);
        copyToModeLyt->addWidget(new QLabel("To mode:"));
        copyToModeLyt->addStretch();
        copyToModeLyt->addWidget(copyToMode);
        copyBtnLyt->addStretch();
        copyBtnLyt->addWidget(copyBtn);
        lyt->addLayout(copyFromSchemeLyt);
        lyt->addLayout(copyFromModeLyt);
        lyt->addLayout(copyToSchemeLyt);
        lyt->addLayout(copyToModeLyt);
        lyt->addSpacing(10);
        lyt->addLayout(copyBtnLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(copyBtn, &QPushButton::clicked, this, &CopySchemeSettingsGBox::onCopySettingsClicked);
    }

    void CopySchemeSettingsGBox::clearWidget() const {
        copyFromScheme->clear();
        copyToScheme->clear();
        copyFromScheme->addItem("");
        copyToScheme->addItem("");
    }

    void CopySchemeSettingsGBox::addScheme(const QString &label) const {
        copyFromScheme->addItem(label);
        copyToScheme->addItem(label);
    }

    void CopySchemeSettingsGBox::removeScheme(const QString &label) const {
        const int idx = copyFromScheme->findText(label);

        if (idx != -1) {
            copyFromScheme->removeItem(idx);
            copyToScheme->removeItem(idx);
        }
    }

    void CopySchemeSettingsGBox::onCopySettingsClicked() {
        if (copyFromScheme->currentIndex() == 0 || copyFromMode->currentIndex() == 0 || copyToScheme->currentIndex() == 0 || copyToMode->currentIndex() == 0)
            return;

        emit copySettings(copyFromScheme->currentText(), copyFromMode->currentIndex() - 1, copyToScheme->currentText(), copyToMode->currentIndex() - 1);
        copyFromScheme->setCurrentIndex(0);
        copyFromMode->setCurrentIndex(0);
        copyToScheme->setCurrentIndex(0);
        copyToMode->setCurrentIndex(0);
    }
}
