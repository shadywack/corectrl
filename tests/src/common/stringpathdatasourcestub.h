//
// Copyright 2019 Juan Palacios <jpalaciosdev@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// Distributed under the GPL version 3 or any later version.
//
#pragma once

#include "core/idatasource.h"
#include <filesystem>

class StringPathDataSourceStub
: public IDataSource<std::string, std::filesystem::path const>
{
 public:
  StringPathDataSourceStub(std::string_view path = "",
                           std::string_view data = "",
                           bool success = true) noexcept
  : source_(path)
  , data_(data)
  , success_(success)
  {
  }

  std::string source() const override
  {
    return source_;
  }

  bool read(std::string &data, std::filesystem::path const &) override
  {
    data = data_;
    return success_;
  }

  std::string const source_;
  std::string const data_;
  bool success_;
};
