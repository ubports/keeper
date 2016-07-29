/*
 * Copyright 2016 Canonical Ltd.
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

#include "tests/utils/xdg-user-dirs-sandbox.h"

#include "service/backup-choices.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QString>

#include <cstdio>
#include <memory>
#include <set>

inline void PrintTo(const QString& s, std::ostream* os)
{
    *os << "\"" << s.toStdString() << "\"";
}

inline void PrintTo(const std::set<QString>& s, std::ostream* os)
{
    *os << "{ ";
    for (const auto& str : s)
        *os << '"' << str.toStdString() << "\", ";
    *os << " }";
}

class UserDirsProviderTest : public ::testing::Test
{
protected:

    virtual void SetUp() override
    {
        temporary_dir_.reset(new XdgUserDirsSandbox());
    }

    virtual void TearDown() override
    {
        temporary_dir_.reset();
    }

private:

    std::shared_ptr<XdgUserDirsSandbox> temporary_dir_;
};


TEST_F(UserDirsProviderTest, UserDirs)
{
    BackupChoices tmp;
    const auto choices = tmp.get_backups();

    // confirm that choices has the advertised public properties
    const auto type_str = QStringLiteral("type");
    for(const auto& choice : choices)
    {
        ASSERT_FALSE(choice.key().isEmpty());
        ASSERT_FALSE(choice.display_name().isEmpty());
        ASSERT_TRUE(choice.has_property(type_str));
    }

    // confirm that we have a system-data choice
    int i, n;
    for(i=0, n=choices.size(); i<n; ++i)
        if (choices[i].get_property(type_str) == QStringLiteral("system-data"))
            break;
    ASSERT_TRUE(i != n);
    auto system_data = choices[i];
    EXPECT_TRUE(system_data.has_property(type_str));

    // confirm that we have user-dir choices
    std::set<QString> expected_user_dir_display_names = {
        QStringLiteral("Documents"),
        QStringLiteral("Movies"),
        QStringLiteral("Music"),
        QStringLiteral("Pictures")
    };
    std::set<QString> user_dir_display_names;
    for (const auto& choice : choices)
        if (choice.get_property(type_str) == QStringLiteral("folder"))
            user_dir_display_names.insert(choice.display_name());
    EXPECT_EQ(expected_user_dir_display_names, user_dir_display_names);
}
