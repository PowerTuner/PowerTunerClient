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

#include <QScrollBar>
#include <QVBoxLayout>

#include "../TabUtils.h"

namespace PWT::UI {
    class ScrollableTab: public QWidget {
    private:
        QScrollArea *scrollArea = nullptr;
        int insertIdx = 0;

    protected:
        QVBoxLayout *lyt = nullptr;
        QVBoxLayout *scrollWidgLyt = nullptr;

        int nextInsertIdx() { return insertIdx++; }
        void scrollToTop() const { scrollArea->verticalScrollBar()->setValue(0); }

    public:
        ScrollableTab() {
            lyt = new QVBoxLayout();
            scrollArea = createScrollArea();
            scrollWidgLyt = new QVBoxLayout();

            scrollWidgLyt->addStretch();
            scrollArea->widget()->setLayout(scrollWidgLyt);
            lyt->addWidget(scrollArea);
            setLayout(lyt);
        }
    };
}
