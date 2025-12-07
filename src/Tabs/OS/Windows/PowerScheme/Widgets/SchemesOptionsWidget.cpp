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

#include "SchemesOptionsWidget.h"

namespace PWT::UI::WIN {
    SchemesOptionsWidget::SchemesOptionsWidget() {
        QVBoxLayout *lyt = new QVBoxLayout();
        QLabel *globalLbl = new QLabel("Global options");
        QFont titleFont = globalLbl->font();

        currentLbl = new QLabel("Current scheme options");
        miscGlobalOptsGBox = new MiscGlobalOptionsGBox();
        copySchemeSettingsGBox = new CopySchemeSettingsGBox();
        duplicateSchemeGBox = new DuplicateSchemeGBox();
        schemeActionsGBox = new SchemeActionsGBox();

        titleFont.setBold(true);
        globalLbl->setAlignment(Qt::AlignCenter);
        globalLbl->setFont(titleFont);
        currentLbl->setAlignment(Qt::AlignCenter);
        currentLbl->setFont(titleFont);
        setCurrentSettingsVisible(false);

        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(globalLbl);
        lyt->addSpacing(12);
        lyt->addWidget(miscGlobalOptsGBox);
        lyt->addWidget(copySchemeSettingsGBox);
        lyt->addWidget(duplicateSchemeGBox);
        lyt->addSpacing(12);
        lyt->addWidget(currentLbl);
        lyt->addSpacing(12);
        lyt->addWidget(schemeActionsGBox);

        setLayout(lyt);

        QObject::connect(copySchemeSettingsGBox, &CopySchemeSettingsGBox::copySettings, this, &SchemesOptionsWidget::onCopySettings);
        QObject::connect(duplicateSchemeGBox, &DuplicateSchemeGBox::createDuplicate, this, &SchemesOptionsWidget::onCreateDuplicate);
        QObject::connect(schemeActionsGBox, &SchemeActionsGBox::setSchemeActive, this, &SchemesOptionsWidget::onSetSchemeActive);
        QObject::connect(schemeActionsGBox, &SchemeActionsGBox::setDeleteFlag, this, &SchemesOptionsWidget::onSetDeleteFlag);
        QObject::connect(schemeActionsGBox, &SchemeActionsGBox::setResetFlag, this, &SchemesOptionsWidget::onSetResetFlag);
        QObject::connect(schemeActionsGBox, &SchemeActionsGBox::deleteScheme, this, &SchemesOptionsWidget::onDeleteScheme);
    }

    void SchemesOptionsWidget::refreshWidget(const PWTS::DaemonPacket &packet) const {
        miscGlobalOptsGBox->refreshWidget(packet);
    }

    void SchemesOptionsWidget::setDataForPacket(const PWTS::ClientPacket &packet) const {
        packet.windowsData->resetSchemesDefault = miscGlobalOptsGBox->getResetSchemes();
        packet.windowsData->replaceDefaultSchemes = miscGlobalOptsGBox->getReplaceDefaultSchemes();
    }

    void SchemesOptionsWidget::setCurrentSettingsVisible(const bool visible) const {
        currentLbl->setVisible(visible);
        schemeActionsGBox->setVisible(visible);
    }

    void SchemesOptionsWidget::clearWidget() const {
        copySchemeSettingsGBox->clearWidget();
        duplicateSchemeGBox->clearWidget();
    }

    void SchemesOptionsWidget::addScheme(const QString &label) const {
        copySchemeSettingsGBox->addScheme(label);
        duplicateSchemeGBox->addScheme(label);
    }

    void SchemesOptionsWidget::addDuplicatedScheme(const QString &label) const {
        copySchemeSettingsGBox->addScheme(label);
    }

    void SchemesOptionsWidget::switchScheme(const QString &scheme, const bool isActive, const bool deleteFlag, const bool resetFlag) const {
        schemeActionsGBox->switchScheme(scheme, isActive, deleteFlag, resetFlag);
    }

    void SchemesOptionsWidget::removeScheme(const QString &label) const {
        copySchemeSettingsGBox->removeScheme(label);
        duplicateSchemeGBox->removeScheme(label);
    }

    void SchemesOptionsWidget::onSetSchemeActive() {
        emit setSchemeActive();
    }

    void SchemesOptionsWidget::onCreateDuplicate(const QString &baseScheme, const QString &duplicateName) {
        emit createDuplicate(baseScheme, duplicateName);
    }

    void SchemesOptionsWidget::onDeleteScheme() {
        emit deleteScheme();
    }

    void SchemesOptionsWidget::onSetDeleteFlag(const bool flagged) {
        emit setDeleteFlag(flagged);
    }

    void SchemesOptionsWidget::onSetResetFlag(const bool flagged) {
        emit setResetFlag(flagged);
    }

    void SchemesOptionsWidget::onCopySettings(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) {
        emit copySettings(fromScheme, fromMode, toScheme, toMode);
    }
}
