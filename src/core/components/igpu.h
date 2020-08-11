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

#include "core/isyscomponent.h"
#include <string_view>

class IGPUInfo;
class ISensor;

class IGPU : public ISysComponent
{
 public:
  static constexpr std::string_view ItemID{"GPU"};

  class Exporter : public ISysComponent::Exporter
  {
   public:
    virtual void takeInfo(IGPUInfo const &info) = 0;
    virtual void takeSensor(ISensor const &sensor) = 0;
  };

  virtual IGPUInfo const &info() const = 0;

  virtual ~IGPU() = default;
};
