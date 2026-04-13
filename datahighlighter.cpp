/*
 * Copyright (c) 2016 Meinhard Ritscher <cyc1ingsir@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "datahighlighter.h"

DataHighlighter::DataHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    m_format_time.setForeground(Qt::darkGreen);
    m_pattern_time = new QRegularExpression("\\d{2,2}:\\d{2,2}:\\d{2,2}:\\d{3,3} ");
    m_format_bytes.setForeground(QColor(120, 180, 200));
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setPointSize(10);
    m_format_bytes.setFont(font);
    m_pattern_bytes = new QRegularExpression("^\\d{8} ");
    m_format_ctrl.setForeground(Qt::darkRed);
    m_format_ctrl.setFontWeight(QFont::Bold);
    m_pattern_ctrl = new QRegularExpression("[\u240A\u240D\u21E5]");
    font = QFont("Monospace");
    font.setStyleHint(QFont::Courier);
    font.setPointSize(10);
    m_format_hex.setFont(font);
    m_format_hex.setForeground(Qt::darkMagenta);
    m_pattern_hex = new QRegularExpression("<0x[\\da-f]{2}>");
    m_format_search.setBackground(QColor(230, 230, 180));
    m_format_search.setForeground(QColor(50, 50, 180));
}

void DataHighlighter::setSearchString(const QString &search)
{
    m_searchString = search;
    rehighlight();
}

void DataHighlighter::setCharFormat(QTextCharFormat *format, DataHighlighter::Formats type)
{
    Q_UNUSED(format)
    switch (type) {
    case Formats::HEX:
        // m_format_hex = format;
        break;
    default:
        break;
    }
}

void DataHighlighter::highlightBlock(const QString &text)
{
    if (text.isEmpty())
        return;
    QRegularExpressionMatch match = m_pattern_time->match(text);
    if (match.hasMatch())
        setFormat(match.capturedStart(), match.capturedLength(0), m_format_time);

    match = m_pattern_bytes->match(text);
    if (match.hasMatch())
        setFormat(match.capturedStart(), match.capturedLength(0), m_format_bytes);

    QRegularExpressionMatchIterator global_match = m_pattern_ctrl->globalMatch(text);
    while (global_match.hasNext()) {
        QRegularExpressionMatch match = global_match.next();
        setFormat(match.capturedStart(), 1, m_format_ctrl);
    }

    global_match = m_pattern_hex->globalMatch(text);
    while (global_match.hasNext()) {
        QRegularExpressionMatch match = global_match.next();

        setFormat(match.capturedStart(), match.capturedLength(0), m_format_hex);
    }

    if (m_searchString.isEmpty())
        return;

    const int length = m_searchString.length();
    int index = text.indexOf(m_searchString, 0, Qt::CaseInsensitive);
    while (index >= 0) {
        setFormat(index, length, m_format_search);
        index = text.indexOf(m_searchString, index + length, Qt::CaseInsensitive);
    }
}
