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

#include "core/components/controls/icontrol.h"
#include "trompeloeil.hpp"

class ControlMock : public IControl
{
 public:
  MAKE_MOCK0(init, void(), override);
  MAKE_MOCK1(preInit, void(ICommandQueue &), override);
  MAKE_MOCK1(postInit, void(ICommandQueue &), override);
  MAKE_CONST_MOCK0(active, bool(), override);
  MAKE_MOCK1(activate, void(bool), override);
  MAKE_MOCK1(clean, void(ICommandQueue &), override);
  MAKE_MOCK1(sync, void(ICommandQueue &), override);
  MAKE_CONST_MOCK0(ID, std::string const &(), override);
  MAKE_MOCK1(importWith, void(Importable::Importer &), override);
  MAKE_CONST_MOCK1(exportWith, void(Exportable::Exporter &), override);
};
