/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *   Charles Kerr <charles.kerr@canonical.com>
 */

#pragma once

#include <QLocalSocket>
#include <QObject>

#include <memory>

class Uploader: public QObject
{
    Q_OBJECT

public:

    Q_DISABLE_COPY(Uploader)

    Uploader(QObject *parent=nullptr): QObject(parent) {}
    virtual ~Uploader() =default;

    virtual std::shared_ptr<QLocalSocket> socket() =0;
    virtual void commit() =0;
    virtual QString file_name() const =0;

Q_SIGNALS:

    void commit_finished(bool success);
};
