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
#include "PowerSchemeTab.h"

namespace PWT::UI::WIN {
	PowerSchemeTab::PowerSchemeTab(): AddonTab() {
		constexpr int pageBtnWidth = 30;
		QVBoxLayout *lyt = qobject_cast<QVBoxLayout *>(layout());
		QHBoxLayout *headLyt = new QHBoxLayout();
		QHBoxLayout *searchLyt = new QHBoxLayout();
		QHBoxLayout *pageBtnLyt = new QHBoxLayout();
		QLabel *activeLbl = new QLabel("Active:");

		switchViewBtn = new QPushButton("Show options");
        schemeSelectInput = new SchemeSelectInput();
		schemeSettingsWidget = new SchemeSettingsWidget();
        schemeOptions = new SchemesOptionsWidget();
        searchBox = new QLineEdit();
		modeAll = new QComboBox();
		activeSchemeLbl = new QLabel("-");
		prevPageBtn = new QPushButton("< Previous");
		nextPageBtn = new QPushButton("Next >");

		prevPageBtn->setMinimumWidth(pageBtnWidth);
		nextPageBtn->setMinimumWidth(pageBtnWidth);
        searchBox->setPlaceholderText("Search setting name or group");
		modeAll->addItems({"", "Plugged In", "Battery"});
		activeLbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
		schemeOptions->setVisible(false);
		disablePageButtons();

		headLyt->addWidget(activeLbl);
		headLyt->addWidget(activeSchemeLbl);
		headLyt->addStretch();
		headLyt->addWidget(schemeSelectInput);
        searchLyt->addWidget(searchBox);
		searchLyt->addSpacing(4);
		searchLyt->addWidget(new QLabel("Mode:"));
		searchLyt->addWidget(modeAll);
		pageBtnLyt->addStretch();
		pageBtnLyt->addWidget(prevPageBtn);
		pageBtnLyt->addWidget(nextPageBtn);
		scrollWidgLyt->insertWidget(nextInsertIdx(), schemeSettingsWidget);
		scrollWidgLyt->insertWidget(nextInsertIdx(), schemeOptions);
		lyt->insertLayout(0, headLyt);
		lyt->insertLayout(1, searchLyt);
		lyt->insertWidget(2, switchViewBtn);
		lyt->addLayout(pageBtnLyt);

		QObject::connect(schemeSelectInput, &SchemeSelectInput::schemeSelected, this, &PowerSchemeTab::onSchemeSelected);
		QObject::connect(switchViewBtn, &QPushButton::clicked, this, &PowerSchemeTab::onSwitchViewBtnClicked);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setSchemeActive, this, &PowerSchemeTab::onSetSchemeActive);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::createDuplicate, this, &PowerSchemeTab::onDuplicateScheme);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setDeleteFlag, this, &PowerSchemeTab::onSetDeleteFlag);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::setResetFlag, this, &PowerSchemeTab::onSetResetFlag);
		QObject::connect(schemeOptions, &SchemesOptionsWidget::deleteScheme, this, &PowerSchemeTab::onDeleteScheme);
        QObject::connect(schemeOptions, &SchemesOptionsWidget::copySettings, this, &PowerSchemeTab::onCopySettings);
		QObject::connect(modeAll, &QComboBox::currentIndexChanged, this, &PowerSchemeTab::onModeAllChanged);
		QObject::connect(searchBox, &QLineEdit::returnPressed, this, &PowerSchemeTab::onSearchSetting);
		QObject::connect(prevPageBtn, &QPushButton::clicked, this, &PowerSchemeTab::onPrevBtnClicked);
		QObject::connect(nextPageBtn, &QPushButton::clicked, this, &PowerSchemeTab::onNextBtnClicked);
	}

	void PowerSchemeTab::refreshTab(const PWTS::DaemonPacket &packet) {
		const QSharedPointer<PWTS::WIN::WindowsData> wdata = packet.windowsData;

		schemeSettingsWidget->disableSettingWidgets();
		schemeSelectInput->clearInput();
		schemeOptions->clearWidget();
		schemesCache.clear();

		for (const auto &[schemeGuid, schemeData]: wdata->schemes.asKeyValueRange()) {
			const QString comboLbl = makeComboLabel(schemeGuid, schemeData.friendlyName);

			for (const auto &[settingGuid, setting]: schemeData.settings.asKeyValueRange()) {
				schemeSettingsWidget->addSetting(settingGuid, wdata->schemeOptionsData[settingGuid]); // create/enable setting
				schemeSettingsWidget->addData(schemeGuid, settingGuid, setting);
			}

			if (wdata->activeScheme == schemeGuid) [[unlikely]]
				activeSchemeLbl->setText(comboLbl);

			schemeSelectInput->addScheme(comboLbl);
			schemeOptions->addScheme(comboLbl);
			schemesCache.insert(comboLbl, {
				.friendlyName = schemeData.friendlyName,
				.guid = schemeGuid,
				.deleteFlag = schemeData.deleteFlag,
				.resetFlag = schemeData.resetFlag
			});
		}

		schemeOptions->refreshWidget(packet);
		schemeSelectInput->restoreSelectedScheme();
		schemeSettingsWidget->nextPage(searchBox->text());
		schemeSettingsWidget->showPage();
		enablePageButtons();
	}

	void PowerSchemeTab::setDataForPacket(PWTS::ClientPacket &packet) const {
		QMap<QString, PWTS::WIN::PowerScheme> schemes;

		for (const auto &[comboLbl, schemeData]: schemesCache.asKeyValueRange()) {
			const PWTS::WIN::PowerScheme pscheme {
				.settings = schemeSettingsWidget->getWidgetsData(schemeData.guid),
				.friendlyName = schemeData.friendlyName,
				.deleteFlag = schemeData.deleteFlag,
				.resetFlag = schemeData.resetFlag
			};

			schemes.insert(schemeData.guid, pscheme);
		}

		packet.windowsData->schemes = schemes;
		packet.windowsData->activeScheme = schemesCache[activeSchemeLbl->text()].guid;

		schemeOptions->setDataForPacket(packet);
	}

	QString PowerSchemeTab::makeComboLabel(const QString &schemeGuid, const QString &friendlyName) const {
		return QString("%1 %2").arg(schemeGuid, friendlyName);
	}

    void PowerSchemeTab::hideSettings() const {
        schemeSettingsWidget->hidePage();
		setOptionsVisible(false);
		schemeOptions->setCurrentSettingsVisible(false);
    }

	void PowerSchemeTab::setOptionsVisible(const bool visible) const {
		schemeOptions->setVisible(visible);
		schemeSettingsWidget->setVisible(!visible);
		prevPageBtn->setVisible(!visible);
		nextPageBtn->setVisible(!visible);
		switchViewBtn->setText(visible ? QStringLiteral("Show scheme settings") : QStringLiteral("Show options"));
	}

	void PowerSchemeTab::setOptionsEnabled(const bool enable) const {
		switchViewBtn->setEnabled(enable);
		schemeOptions->setEnabled(enable);
	}

	void PowerSchemeTab::disablePageButtons() const {
		prevPageBtn->setEnabled(false);
		nextPageBtn->setEnabled(false);
	}

	void PowerSchemeTab::enablePageButtons() const {
		const bool hasPrevPage = schemeSettingsWidget->hasPreviousPage();
		const bool hasNextPage = schemeSettingsWidget->hasNextPage();

		prevPageBtn->setEnabled(hasPrevPage);
		nextPageBtn->setEnabled(hasNextPage);
	}

	void PowerSchemeTab::onSchemeSelected(const QString &scheme) const {
		if (scheme.isEmpty()) {
			hideSettings();
			return;
		}

		const SchemeCacheData &selected = schemesCache[scheme];

		schemeSettingsWidget->switchScheme(selected.guid);
		schemeOptions->switchScheme(scheme, scheme == activeSchemeLbl->text(), selected.deleteFlag, selected.resetFlag);
		schemeOptions->setCurrentSettingsVisible(true);
		scrollToTop();
	}

	void PowerSchemeTab::onSwitchViewBtnClicked() const {
		setOptionsVisible(!schemeOptions->isVisible());
	}

	void PowerSchemeTab::onPrevBtnClicked() const {
		disablePageButtons();
		schemeSettingsWidget->previousPage(searchBox->text());
		scrollToTop();
		enablePageButtons();
	}

	void PowerSchemeTab::onNextBtnClicked() const {
		disablePageButtons();
		schemeSettingsWidget->nextPage(searchBox->text());
		scrollToTop();
		enablePageButtons();
	}

    void PowerSchemeTab::onModeAllChanged(const int idx) const {
        if (idx == 0)
            return;

        const QSignalBlocker sblock {modeAll};

        schemeSettingsWidget->setModeAll(idx - 1);
        modeAll->setCurrentIndex(0);
    }

	void PowerSchemeTab::onSetSchemeActive() const {
		activeSchemeLbl->setText(schemeSelectInput->getCurrentScheme());
	}

	void PowerSchemeTab::onDuplicateScheme(const QString &baseScheme, const QString &duplicateName) {
		setOptionsEnabled(false);

		const QString baseGuid = schemesCache[baseScheme].guid;
        const QString tmpGuid = QString("{guid-tbd-%1}").arg(++guidTbdID);
		const QString comboLbl = makeComboLabel(tmpGuid, duplicateName);

		schemeSettingsWidget->duplicateSettings(baseGuid, tmpGuid);
		schemeSelectInput->addScheme(comboLbl);
        schemeOptions->addDuplicatedScheme(comboLbl);

		schemesCache.insert(comboLbl, {
			.friendlyName = duplicateName,
			.guid = tmpGuid
		});

		setOptionsEnabled(true);
	}

	void PowerSchemeTab::onSetDeleteFlag(const bool flagged) const {
		const QString scheme = schemeSelectInput->getCurrentScheme();

		if (scheme.isEmpty())
			return;

		schemesCache[scheme].deleteFlag = flagged;
	}

	void PowerSchemeTab::onSetResetFlag(const bool flagged) const {
		const QString scheme = schemeSelectInput->getCurrentScheme();

		if (scheme.isEmpty())
			return;

		schemesCache[scheme].resetFlag = flagged;
	}

	void PowerSchemeTab::onDeleteScheme() const {
		const QString current = schemeSelectInput->getCurrentScheme();

		schemeOptions->removeScheme(current);
		schemeSettingsWidget->deleteScheme(schemesCache[current].guid);
		schemeSelectInput->deleteCurrentScheme();
		schemesCache.remove(current);
	}

    void PowerSchemeTab::onSearchSetting() const {
        schemeSettingsWidget->filterVisibleSettings(searchBox->text());
		enablePageButtons();
        scrollToTop();
    }

    void PowerSchemeTab::onCopySettings(const QString &fromScheme, const int fromMode, const QString &toScheme, const int toMode) const {
        setOptionsEnabled(false);

		const QString fromGuid = schemesCache[fromScheme].guid;
		const QString toGuid = schemesCache[toScheme].guid;

        schemeSettingsWidget->copySchemeValues(fromGuid, fromMode, toGuid, toMode);

        setOptionsEnabled(true);
    }
}
