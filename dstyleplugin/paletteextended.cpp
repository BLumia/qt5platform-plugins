/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "paletteextended.h"

#include <QSettings>
#include <QString>
#include <QColor>
#include <QMetaEnum>
#include <QDebug>

namespace dstyle {

static PaletteExtended *StaticInstance = nullptr;

PaletteExtended::PaletteExtended() :
    QObject(),
    m_colorScheme(nullptr)
{

}

QColor PaletteExtended::color(PaletteExtended::ColorName name) const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<PaletteExtended::ColorName>();
    QString colorName = metaEnum.valueToKey(name);

    const QString key = colorName.replace("_", "/");
    const QStringList value = m_colorScheme->value(key).toStringList();

    qDebug() << key << value << parseColor(value);

    return parseColor(value);
}

void PaletteExtended::setType(StyleType type)
{
    if (m_colorScheme) m_colorScheme->deleteLater();

    if (type == StyleType::StyleDark) {
        m_colorScheme = new QSettings(":/colorschemes/ddark.ini", QSettings::IniFormat);
    } else if (type == StyleType::StyleLight) {
        m_colorScheme = new QSettings(":/colorschemes/dlight.ini", QSettings::IniFormat);
    }
}

PaletteExtended *PaletteExtended::instance()
{
    if (!StaticInstance) {
        StaticInstance = new PaletteExtended;
    }

    return StaticInstance;
}

QColor PaletteExtended::parseColor(const QStringList &value) const
{
    if (value.length() >= 3) {
        const int r = value.at(0).toInt();
        const int g = value.at(1).toInt();
        const int b = value.at(2).toInt();

        QColor ret( QColor::fromRgb(r, g, b));

        if (value.length() > 3) {
            const float a = value.at(3).toFloat();
            ret.setAlphaF(a);
        }

        return ret;
    }

    return Qt::black;
}

}
