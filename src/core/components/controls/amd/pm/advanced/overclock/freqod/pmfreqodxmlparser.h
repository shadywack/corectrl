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
#include "pmfreqodprofilepart.h"
#include <string>

namespace AMD {

class PMFreqOdXMLParser final
: public ProfilePartXMLParser
, public AMD::PMFreqOdProfilePart::Exporter
, public AMD::PMFreqOdProfilePart::Importer
{
 public:
  PMFreqOdXMLParser() noexcept;

  std::unique_ptr<Exportable::Exporter> factory(
      IProfilePartXMLParserProvider const &profilePartParserProvider) override;
  std::unique_ptr<Exportable::Exporter> initializer() override;

  std::optional<std::reference_wrapper<Exportable::Exporter>>
  provideExporter(Item const &i) override;
  std::optional<std::reference_wrapper<Importable::Importer>>
  provideImporter(Item const &i) override;

  void takeActive(bool active) override;
  bool provideActive() const override;

  void takePMFreqOdSclkOd(unsigned int value) override;
  unsigned int providePMFreqOdSclkOd() const override;

  void takePMFreqOdMclkOd(unsigned int value) override;
  unsigned int providePMFreqOdMclkOd() const override;

  void appendTo(pugi::xml_node &parentNode) override;

 protected:
  void resetAttributes() override;
  void loadPartFrom(pugi::xml_node const &parentNode) override;

 private:
  class Initializer;

  std::string const id_;

  bool active_;
  bool activeDefault_;

  unsigned int sclkOd_;
  unsigned int sclkOdDefault_;

  unsigned int mclkOd_;
  unsigned int mclkOdDefault_;

  static bool const registered_;
};

} // namespace AMD
