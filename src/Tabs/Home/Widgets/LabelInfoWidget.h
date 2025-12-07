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

#include <QLabel>

namespace PWT::UI {
    class LabelInfoWidget final: public QWidget {
    private:
        QLabel *textLbl = nullptr;

    public:
        LabelInfoWidget(const QString &label, const QString &text);

        void setText(const QString &text) const { textLbl->setText(text); }
        void setNum(const int value) const { textLbl->setNum(value); }
        [[nodiscard]] QString getText() const { return textLbl->text(); }
    };
}
