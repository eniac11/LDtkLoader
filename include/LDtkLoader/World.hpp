// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/defs/LayerDef.hpp"
#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class Project;

    /**
     * A World represents one world in the @ref ldtk::Project "LDtk project" and can contain one or multiple @ref ldtk::Level "levels".
     */
    class World
    {
    public:
        ~World() = default;
        World(const World&) = delete;
        World(World&&) noexcept = default;
        auto operator=(const World&) -> World& = delete;
        auto operator=(World&&) -> World& = delete;

        /**
         * Unique instance ID of the @ref World.
         */
        const IID iid;

        /**
         * Returns the name of the World.
         */
        auto getName() const -> const std::string&;

        /**
         * Returns the default pivot, a point from (0.f, 0.f) to (1.f, 1.f).
         */
        auto getDefaultPivot() const -> const FloatPoint&;

        /**
         * Returns the default size of a cell in the World grid. Cell dimension is be `size`x`size`.
         */
        auto getDefaultCellSize() const -> int;

        /**
         * Returns the default background color of the World.
         */
        auto getBgColor() const -> const Color&;

        /**
         * Returns the layout of the world (Free, GridVania, LinearHorizontal or LinearVertical).
         * @sa ldtk::WorldLayout
         */
        auto getLayout() const -> const WorldLayout&;

        /**
        * Returns the @ref LayerDef matching the given @p id.
        * @param id id of @ref ldtk::LayerDef "layerdef"
        * @throws std::invalid_argument  if no @ref LayerDef is found
        */
        auto getLayerDef(int id) const -> const LayerDef&;
        /**
        * Returns the @ref LayerDef matching the given @p name.
        * @param name name of @ref ldtk::LayerDef "layerdef"
        * @throws std::invalid_argument  if no @ref LayerDef is found
        */
        auto getLayerDef(const std::string& name) const -> const LayerDef&;

        /**
        * Returns the @ref EntityDef matching the given @p id.
        * @param id id of @ref ldtk::LayerDef "entitydef"
        * @throws std::invalid_argument  if no @ref EntityDef is found
        */
        auto getEntityDef(int id) const -> const EntityDef&;
        /**
        * Returns the @ref EntityDef matching the given @p name.
        * @param name name of @ref ldtk::LayerDef "entitydef"
        * @throws std::invalid_argument  if no @ref EntityDef is found
        */
        auto getEntityDef(const std::string& name) const -> const EntityDef&;

        /**
         * Returns a vector containing all the @ref ldtk::Tileset "Tilesets" of the @ref World.
         */
        auto allTilesets() const -> const std::vector<Tileset>&;
        /**
         * Returns the Tileset matching the given @p id.
         * @param id id of @ref ldtk::Tileset "tileset"
         * @throws std::invalid_argument  if no @ref Tileset is found
         */
        auto getTileset(int id) const -> const Tileset&;
        /**
         * Returns the @ref ldtk::Tileset "tileset" matching the given @p name.
         * @param name name of @ref ldtk::Tileset "tileset"
         * @throws std::invalid_argument  if no @ref Tileset is found
         */
        auto getTileset(const std::string& name) const -> const Tileset&;

        /**
         * Returns the Enum matching the given @p id.
         * @param id id of the @ref Enum
         * @throws std::invalid_argument if no @ref Enum is found.
         */
        auto getEnum(int id) const -> const Enum&;
        /**
         * Returns the Enum matching the given @p name.
         * @param name name of the @ref Enum
         * @throws std::invalid_argument if no @ref Enum is found.
         */
        auto getEnum(const std::string& name) const -> const Enum&;

        /**
         * Returns a vector containing all the @ref ldtk::Level "Levels" of the World.
         */
        auto allLevels() const -> const std::vector<Level>&;
        /**
         * Returns the @ref Level matching the given @p id.
         * @param id id of the @ref ldtk::Level "level"
         * @throws std::invalid_argument if no Level is found
         */
        auto getLevel(int id) const -> const Level&;
        /**
        * Returns the @ref Level matching the given @p name.
        * @param name name of the @ref ldtk::Level "level"
        * @throws std::invalid_argument if no Level is found
        */
        auto getLevel(const std::string& name) const -> const Level&;
        auto getLevel(const IID& iid) const -> const Level&;

        World(const nlohmann::json& j, Project* p, const FileLoader& file_loader, bool external_levels);

    private:
        const Project* const m_project;
        std::string m_name;

        WorldLayout m_layout = WorldLayout::Free;
        std::vector<Level> m_levels;
    };

} // namespace ldtk
