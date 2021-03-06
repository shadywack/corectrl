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
#include "catch.hpp"
#include "trompeloeil.hpp"

#include "common/commandqueuestub.h"
#include "common/controlmock.h"
#include "common/sensormock.h"
#include "core/components/cpu.h"
#include "core/info/icpuinfo.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace Tests {
namespace CPU {

class CPUInfoStub : public ICPUInfo
{
 public:
  int socketId() const override
  {
    return 0;
  }

  std::vector<ICPUInfo::ExecutionUnit> const &executionUnits() const override
  {
    return executionUnits_;
  }

  std::vector<std::string> keys() const override
  {
    return std::vector<std::string>{"info_k1"};
  }

  std::string info(std::string_view) const override
  {
    return "info_1";
  }

  bool hasCapability(std::string_view) const override
  {
    return true;
  }

  void initialize(std::vector<std::unique_ptr<ICPUInfo::IProvider>> const &) override
  {
  }

 private:
  std::vector<ICPUInfo::ExecutionUnit> const executionUnits_{{0, 0, "/cpu0"}};
};

class CPUImporterStub : public ICPU::Importer
{
 public:
  std::optional<std::reference_wrapper<Importable::Importer>>
  provideImporter(Item const &) override
  {
    return *this;
  }

  bool provideActive() const override
  {
    return false;
  }
};

class CPUExporterMock : public ICPU::Exporter
{
 public:
  MAKE_MOCK1(takeActive, void(bool), override);
  MAKE_MOCK1(takeInfo, void(ICPUInfo const &), override);
  MAKE_MOCK1(takeSensor, void(ISensor const &), override);
  MAKE_MOCK1(
      provideExporter,
      std::optional<std::reference_wrapper<Exportable::Exporter>>(Item const &),
      override);
};

TEST_CASE("CPU tests", "[CPU]")
{
  CommandQueueStub ctlCmds;

  auto info = std::make_unique<CPUInfoStub>();
  auto &infoStub = *info;

  std::vector<std::unique_ptr<IControl>> controls;
  controls.emplace_back(std::make_unique<ControlMock>());
  auto &controlMock = static_cast<ControlMock &>(*controls[0]);

  std::vector<std::unique_ptr<ISensor>> sensors;
  sensors.emplace_back(std::make_unique<SensorMock>());
  auto &sensorMock = static_cast<SensorMock &>(*sensors[0]);

  ::CPU ts(std::move(info), std::move(controls), std::move(sensors));

  SECTION("Has ICPU ID")
  {
    REQUIRE(ts.ID() == ICPU::ItemID);
  }

  SECTION("Is active by default")
  {
    REQUIRE(ts.active());

    SECTION("Can be deactivated")
    {
      ts.activate(false);

      REQUIRE_FALSE(ts.active());

      SECTION("Can be activated")
      {
        ts.activate(true);

        REQUIRE(ts.active());
      }
    }
  }

  SECTION("Has a unique key in a system component scope")
  {
    // 'CPU' + socket combination seems to be good enough to differentiate
    // between system components.
    REQUIRE_THAT(ts.key(), Catch::Contains("CPU") && Catch::Contains("0"));
  }

  SECTION("Its CPU information can be retrieved")
  {
    REQUIRE(&ts.info() == &infoStub);
  }

  SECTION("CPU description and information can be retrieved")
  {
    auto [gpuDesc, infos] = ts.componentInfo();
    REQUIRE_THAT(gpuDesc, Catch::Contains("CPU") && Catch::Contains("0"));
    REQUIRE_FALSE(infos.empty());
    auto &[infoKey, info] = infos[0];
    REQUIRE(infoKey == "info_k1");
    REQUIRE(info == "info_1");
  }

  SECTION("Update its sensors")
  {
    std::string const sensorid("sensorid");
    ALLOW_CALL(sensorMock, ID()).LR_RETURN(sensorid);
    REQUIRE_CALL(sensorMock, update());

    std::unordered_map<std::string, std::unordered_set<std::string>> ignored;
    ts.updateSensors(ignored);
  }

  SECTION("Does not update ignored sensors")
  {
    std::string const sensorid("sensorid");
    std::unordered_map<std::string, std::unordered_set<std::string>> ignored;
    ignored[ts.key()] = {};
    ignored[ts.key()].emplace(sensorid);

    ALLOW_CALL(sensorMock, ID()).LR_RETURN(sensorid);
    FORBID_CALL(sensorMock, update());
    ts.updateSensors(ignored);
  }

  SECTION("Pre-init its controls")
  {
    REQUIRE_CALL(controlMock, preInit(trompeloeil::_));
    ts.preInit(ctlCmds);
  }

  SECTION("Post-init its controls")
  {
    REQUIRE_CALL(controlMock, postInit(trompeloeil::_));
    ts.postInit(ctlCmds);
  }

  SECTION("Init controls")
  {
    REQUIRE_CALL(controlMock, init());
    ts.init();
  }

  SECTION("Only clear and sync controls when is active")
  {
    ALLOW_CALL(sensorMock, update());

    REQUIRE_CALL(controlMock, clean(trompeloeil::_));
    REQUIRE_CALL(controlMock, sync(trompeloeil::_));
    ts.activate(true);
    ts.sync(ctlCmds);

    FORBID_CALL(controlMock, clean(trompeloeil::_));
    FORBID_CALL(controlMock, sync(trompeloeil::_));
    ts.activate(false);
    ts.sync(ctlCmds);
  }

  SECTION("Imports its state and controls")
  {
    REQUIRE_CALL(controlMock, importWith(trompeloeil::_));

    ts.activate(true);
    CPUImporterStub i;
    ts.importWith(i);

    REQUIRE_FALSE(ts.active());
  }

  SECTION("Exports its state, sensors and controls")
  {
    CPUExporterMock e;
    ALLOW_CALL(e, provideExporter(trompeloeil::_)).LR_RETURN(e);
    REQUIRE_CALL(e, takeActive(true));
    REQUIRE_CALL(e, takeInfo(trompeloeil::_)).LR_WITH(&_1 == &infoStub);
    REQUIRE_CALL(e, takeSensor(trompeloeil::_)).LR_WITH(&_1 == &sensorMock);

    REQUIRE_CALL(controlMock, exportWith(trompeloeil::_));

    ts.activate(true);
    ts.exportWith(e);
  }
}
} // namespace CPU
} // namespace Tests
