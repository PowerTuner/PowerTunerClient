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

#include "DuplicateSchemeGBox.h"

namespace PWT::UI::WIN {
    DuplicateSchemeGBox::DuplicateSchemeGBox(): QGroupBox("Duplicate Scheme") {
        QVBoxLayout *lyt = new QVBoxLayout();
        QHBoxLayout *baseLyt = new QHBoxLayout();
        QHBoxLayout *duplNameLyt = new QHBoxLayout();
        QHBoxLayout *duplicateBtnLyt = new QHBoxLayout();

        schemesCombo = new QComboBox();
        duplicateName = new QLineEdit();
        duplicateBtn = new QPushButton("Duplicate scheme");

        schemesCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        baseLyt->addWidget(new QLabel("Base scheme:"));
        baseLyt->addStretch();
        baseLyt->addWidget(schemesCombo);
        duplNameLyt->addWidget(new QLabel("Duplicate name:"));
        duplNameLyt->addStretch();
        duplNameLyt->addWidget(duplicateName);
        duplicateBtnLyt->addStretch();
        duplicateBtnLyt->addWidget(duplicateBtn);
        lyt->addLayout(baseLyt);
        lyt->addLayout(duplNameLyt);
        lyt->addSpacing(10);
        lyt->addLayout(duplicateBtnLyt);

        setAlignment(Qt::AlignCenter);
        setLayout(lyt);

        QObject::connect(duplicateBtn, &QPushButton::clicked, this, &DuplicateSchemeGBox::onDuplicateClicked);
    }

    void DuplicateSchemeGBox::clearWidget() const {
        duplicateName->clear();
        schemesCombo->clear();
        schemesCombo->addItem("");
    }

    void DuplicateSchemeGBox::addScheme(const QString &label) const {
        schemesCombo->addItem(label);
    }

    void DuplicateSchemeGBox::removeScheme(const QString &label) const {
        schemesCombo->removeItem(schemesCombo->findText(label));
    }

    void DuplicateSchemeGBox::onDuplicateClicked() {
        const QString name = duplicateName->text();

        if (name.isEmpty() || schemesCombo->currentIndex() == 0)
            return;

        emit createDuplicate(schemesCombo->currentText(), name);
        duplicateName->clear();
        schemesCombo->setCurrentIndex(0);
    }
}
