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

#include "icommandqueue.h"
#include <string>
#include <utility>
#include <vector>

class CommandQueue : public ICommandQueue
{
 public:
  CommandQueue() noexcept;

  void add(std::pair<std::string, std::string> &&cmd) override;
  void pack(QByteArray &data) override;

 protected:
  std::vector<std::pair<std::string, std::string>> &commands();

 private:
  std::vector<std::pair<std::string, std::string>> commands_;
};
