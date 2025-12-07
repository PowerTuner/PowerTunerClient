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
#include "SchemeSettingsWidget.h"
#include "../Include/MinMaxSliderGBox.h"
#include "../Include/OptionsGBox.h"

namespace PWT::UI::WIN {
    SchemeSettingsWidget::SchemeSettingsWidget() {
        lyt = new QVBoxLayout();

        lyt->setContentsMargins(0, 0, 0, 0);

        setLayout(lyt);
    }

    void SchemeSettingsWidget::disableSettingWidgets() {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            setting->enabled = false;
            setting->gbox->setVisible(false);
            setting->gbox->clearData();
        }

        startPageIdx = 0;
        endPageIdx = 0;
        pageItmList.clear();
    }

    void SchemeSettingsWidget::addSetting(const QString &guid, const PWTS::WIN::PowerSchemeSettingData &schemeSetting) {
        if (settingGBoxMap.contains(guid)) {
            if (settingGBoxMap[guid]->enabled)
                return;

            if (schemeSetting.isRangeDefined)
                qobject_cast<MinMaxSliderGBox *>(settingGBoxMap[guid]->gbox)->setLimits(schemeSetting);
            else
                qobject_cast<OptionsGBox *>(settingGBoxMap[guid]->gbox)->setOptions(schemeSetting.options);

            settingGBoxMap[guid]->enabled = true;
            return;
        }

        const QSharedPointer<SettingItm> settingGBox = QSharedPointer<SettingItm>::create();

        if (schemeSetting.isRangeDefined)
            settingGBox->gbox = new MinMaxSliderGBox(schemeSetting, guid);
        else
            settingGBox->gbox = new OptionsGBox(schemeSetting, guid);

        settingGBox->gbox->setVisible(false);
        settingGBoxMap.insert(guid, settingGBox);
        settingItmList.append(settingGBox);
        lyt->addWidget(settingGBox->gbox);
    }

    void SchemeSettingsWidget::addData(const QString &scheme, const QString &settingGuid, const PWTS::WIN::PowerSchemeSetting &setting) const {
        settingGBoxMap[settingGuid]->gbox->setData(scheme, setting);
    }

    QMap<QString, PWTS::WIN::PowerSchemeSetting> SchemeSettingsWidget::getWidgetsData(const QString &scheme) const {
        QMap<QString, PWTS::WIN::PowerSchemeSetting> data;

        for (const auto &[settGuid, settGBox]: settingGBoxMap.asKeyValueRange()) {
            if (settGBox->enabled) [[likely]]
                data.insert(settGuid, {.groupGuid = settGBox->gbox->getGroupGUID(), .value = settGBox->gbox->getData(scheme)});
        }

        return data;
    }

    bool SchemeSettingsWidget::isSettingVisible(const QString &filter, const PowerSettingGBox *gbox) const {
        return filter.isEmpty() || gbox->getTitle().contains(filter, Qt::CaseInsensitive) || gbox->getGroupTitle().contains(filter, Qt::CaseInsensitive);
    }

    void SchemeSettingsWidget::preparePreviousPage(const QString &optionsFilter) const {
        for (int i=(startPageIdx-1),j=0; i>=0 && j<pageSize; --i,--startPageIdx) {
            const QSharedPointer<SettingItm> itm = settingItmList[i];

            if (!itm->enabled || !isSettingVisible(optionsFilter, itm->gbox))
                continue;

            pageItmList.append(itm);
            ++j;
        }
    }

    void SchemeSettingsWidget::prepareNextPage(const QString &optionsFilter) const {
        for (int i=startPageIdx,j=0,l=settingItmList.size(); i<l && j<pageSize; ++i,++endPageIdx) {
            const QSharedPointer<SettingItm> itm = settingItmList[i];

            if (!itm->enabled || !isSettingVisible(optionsFilter, itm->gbox))
                continue;

            pageItmList.append(itm);
            ++j;
        }
    }

    void SchemeSettingsWidget::switchScheme(const QString &schemeGuid) const {
        for (const QSharedPointer<SettingItm> &itm: settingItmList) {
            if (itm->enabled) [[likely]]
                itm->gbox->switchScheme(schemeGuid);
        }
    }

    void SchemeSettingsWidget::hidePage() const {
        for (const QSharedPointer<SettingItm> &itm: pageItmList)
            itm->gbox->setVisible(false);

        pageItmList.clear();
    }

    void SchemeSettingsWidget::showPage() const {
        for (const QSharedPointer<SettingItm> &itm: pageItmList)
            itm->gbox->setVisible(true);
    }

    void SchemeSettingsWidget::previousPage(const QString &optionsFilter) const {
        if (startPageIdx <= 0)
            return;

        endPageIdx = startPageIdx;

        hidePage();
        preparePreviousPage(optionsFilter);
        showPage();
    }

    void SchemeSettingsWidget::nextPage(const QString &optionsFilter) const {
        if (endPageIdx >= settingItmList.size())
            return;

        startPageIdx = endPageIdx;

        hidePage();
        prepareNextPage(optionsFilter);
        showPage();
    }

    void SchemeSettingsWidget::duplicateSettings(const QString &current, const QString &duplicate) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->gbox->duplicateScheme(current, duplicate);
        }
    }

    void SchemeSettingsWidget::deleteScheme(const QString &scheme) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->gbox->deleteScheme(scheme);
        }
    }

    void SchemeSettingsWidget::filterVisibleSettings(const QString &filter) const {
        startPageIdx = endPageIdx = 0;

        hidePage();
        prepareNextPage(filter);
        showPage();
    }

    void SchemeSettingsWidget::setModeAll(const int idx) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->gbox->setMode(idx);
        }
    }

    void SchemeSettingsWidget::copySchemeValues(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) const {
        for (const QSharedPointer<SettingItm> &setting: settingItmList) {
            if (setting->enabled) [[likely]]
                setting->gbox->copyValue(fromScheme, fromMode, toScheme, toMode);
        }
    }
}
