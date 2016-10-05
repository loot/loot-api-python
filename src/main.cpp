/*  LOOT

    A load order optimisation tool for Oblivion, Skyrim, Fallout 3 and
    Fallout: New Vegas.

    Copyright (C) 2012-2016    WrinklyNinja

    This file is part of LOOT.

    LOOT is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    LOOT is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LOOT.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <loot/api.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "wrapper_version.h"

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

PYBIND11_PLUGIN(loot_api) {
  using loot::CreateDatabase;
  using loot::DatabaseInterface;
  using loot::IsCompatible;
  using loot::GameType;
  using loot::LanguageCode;
  using loot::LootVersion;
  using loot::MessageType;
  using loot::MasterlistInfo;
  using loot::PluginCleanliness;
  using loot::SimpleMessage;
  using loot::PluginTags;
  using loot::WrapperVersion;

  using pybind11::arg;
  using pybind11::enum_;
  using pybind11::class_;

  const std::string emptyString;

  pybind11::module module("loot_api", "A Python module that wraps the LOOT API, generated by pybind11.");

  enum_<GameType>(module, "GameType")
    .value("tes4", GameType::tes4)
    .value("tes5", GameType::tes5)
    .value("fo3", GameType::fo3)
    .value("fonv", GameType::fonv)
    .value("fo4", GameType::fo4);

  enum_<LanguageCode>(module, "LanguageCode")
    .value("english", LanguageCode::english)
    .value("spanish", LanguageCode::spanish)
    .value("russian", LanguageCode::russian)
    .value("french", LanguageCode::french)
    .value("chinese", LanguageCode::chinese)
    .value("polish", LanguageCode::polish)
    .value("brazilian_portuguese", LanguageCode::brazilian_portuguese)
    .value("finnish", LanguageCode::finnish)
    .value("german", LanguageCode::german)
    .value("danish", LanguageCode::danish)
    .value("korean", LanguageCode::korean);

  enum_<MessageType>(module, "MessageType")
    .value("say", MessageType::say)
    .value("warn", MessageType::warn)
    .value("error", MessageType::error);

  enum_<PluginCleanliness>(module, "PluginCleanliness")
    .value("clean", PluginCleanliness::clean)
    .value("dirty", PluginCleanliness::dirty)
    .value("do_not_clean", PluginCleanliness::do_not_clean)
    .value("unknown", PluginCleanliness::unknown);

  class_<MasterlistInfo>(module, "MasterlistInfo")
    .def_readwrite("revision_id", &MasterlistInfo::revision_id)
    .def_readwrite("revision_date", &MasterlistInfo::revision_date)
    .def_readwrite("is_modified", &MasterlistInfo::is_modified);

  class_<SimpleMessage>(module, "Message")
    .def_readwrite("type", &SimpleMessage::type)
    .def_readwrite("language", &SimpleMessage::language)
    .def_readwrite("text", &SimpleMessage::text);

  class_<PluginTags>(module, "PluginTags")
    .def_readwrite("added", &PluginTags::added)
    .def_readwrite("removed", &PluginTags::removed)
    .def_readwrite("userlist_modified", &PluginTags::userlist_modified);

  class_<LootVersion>(module, "Version")
    .def_readonly_static("major", &LootVersion::major)
    .def_readonly_static("minor", &LootVersion::minor)
    .def_readonly_static("patch", &LootVersion::patch)
    .def_readonly_static("revision", &LootVersion::revision)
    .def_static("string", LootVersion::string);

  class_<WrapperVersion>(module, "WrapperVersion")
    .def_readonly_static("major", &WrapperVersion::major)
    .def_readonly_static("minor", &WrapperVersion::minor)
    .def_readonly_static("patch", &WrapperVersion::patch)
    .def_readonly_static("revision", &WrapperVersion::revision)
    .def_static("string", WrapperVersion::string);

  class_<DatabaseInterface, std::shared_ptr<DatabaseInterface>>(module, "DatabaseInterface")
    .def("load_lists", &DatabaseInterface::LoadLists, arg("masterlist_path"), arg("userlist_path") = emptyString)
    .def("eval_lists", &DatabaseInterface::EvalLists)
    .def("sort_plugins", &DatabaseInterface::SortPlugins)
    .def("update_masterlist", &DatabaseInterface::UpdateMasterlist)
    .def("get_masterlist_revision", &DatabaseInterface::GetMasterlistRevision)
    .def("get_plugin_tags", &DatabaseInterface::GetPluginTags)
    .def("get_plugin_messages", &DatabaseInterface::GetPluginMessages)
    .def("get_plugin_cleanliness", &DatabaseInterface::GetPluginCleanliness)
    .def("write_minimal_list", &DatabaseInterface::WriteMinimalList);

  module.def("is_compatible", &IsCompatible);

  module.def("create_database", &CreateDatabase, arg("game"), arg("game_path") = emptyString, arg("game_local_path") = emptyString);

  return module.ptr();
}
