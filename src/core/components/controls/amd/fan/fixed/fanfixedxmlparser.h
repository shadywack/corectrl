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

#include "core/profilepartxmlparser.h"
#include "fanfixedprofilepart.h"
#include <string>

namespace AMD {

class FanFixedXMLParser final
: public ProfilePartXMLParser
, public AMD::FanFixedProfilePart::Exporter
, public AMD::FanFixedProfilePart::Importer
{
 public:
  FanFixedXMLParser() noexcept;

  std::unique_ptr<Exportable::Exporter> factory(
      IProfilePartXMLParserProvider const &profilePartParserProvider) override;
  std::unique_ptr<Exportable::Exporter> initializer() override;

  std::optional<std::reference_wrapper<Exportable::Exporter>>
  provideExporter(Item const &i) override;
  std::optional<std::reference_wrapper<Importable::Importer>>
  provideImporter(Item const &i) override;

  void takeActive(bool active) override;
  bool provideActive() const override;

  void takeFanFixedValue(units::concentration::percent_t value) override;
  units::concentration::percent_t provideFanFixedValue() const override;

  void takeFanFixedFanStop(bool enabled) override;
  bool provideFanFixedFanStop() const override;

  void takeFanFixedFanStartValue(units::concentration::percent_t value) override;
  units::concentration::percent_t provideFanFixedFanStartValue() const override;

  void appendTo(pugi::xml_node &parentNode) override;

 protected:
  void resetAttributes() override;
  void loadPartFrom(pugi::xml_node const &parentNode) override;

 private:
  class Initializer;

  std::string const id_;

  bool active_;
  bool activeDefault_;

  unsigned int value_;
  unsigned int valueDefault_;

  bool fanStop_;
  bool fanStopDefault_;

  unsigned int fanStartValue_;
  unsigned int fanStartValueDefault_;

  static bool const registered_;
};

} // namespace AMD
