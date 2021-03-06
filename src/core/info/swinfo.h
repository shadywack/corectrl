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

#include "core/info/iswinfo.h"
#include <string>
#include <unordered_map>

class SWInfo final : public ISWInfo
{
 public:
  std::string info(std::string_view key) const override;
  std::vector<std::string> keys() const override;
  void initialize(std::vector<std::unique_ptr<ISWInfo::IProvider>> const
                      &infoProviders) override;

 private:
  std::unordered_map<std::string, std::string> info_;
};
