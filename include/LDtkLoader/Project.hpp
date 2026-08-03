// Created by Modar Nasser on 25/02/2022.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/defs/LayerDef.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/World.hpp"

namespace ldtk {

    /**
     * A Project represents the whole LDtk project file and can contain one or multiple worlds.
     */
    class Project
    {
    public:
        /**
         *  Default constructor.
         */
        Project() = default;
        ~Project() = default;
        Project(const Project&) = delete;
        Project(Project&&) noexcept = default;
        auto operator=(const Project&) -> Project& = delete;
        auto operator=(Project&&) -> Project& = delete;

        IID iid;

        /**
         * Load a LDtk project from a given file path.
         *
         * After this method is executed, everything will be loaded in the Project object.
         *
         * @throws std::invalid_argument Throws an exception in case of failure.
         * @param filepath ldtk project file
         */
        void loadFromFile(const std::string& filepath);
        /**
         * This overload allows loading a LDtk project using a custom stream.
         *
         * This allows for example, to load a file from a virtual filesystem.
         *
         * @param filepath
         * @param file_loader
         * @sa ldtk::FileLoader
         */
        void loadFromFile(const std::string& filepath, const FileLoader& file_loader);

        /**
         * Load a LDtk project from a string in memory.
         */
        void loadFromMemory(const std::vector<std::uint8_t>& bytes);
        /**
         * @copydoc Project::loadFromMemory(const std::vector<std::uint8_t>&)
         */
        void loadFromMemory(const unsigned char* data, size_t size);

        /**
         *
         * @return the path of the file from which the Project was loaded.
         */
        auto getFilePath() const -> const FilePath&;

        /**
         *
         * @return the default pivot, a point from (0.f, 0.f) to (1.f, 1.f).
         */
        auto getDefaultPivot() const -> const FloatPoint&;

        /**
         * Cell dimension is `(size, size)`.
         *
         * @return the default size of a cell in the Project's world grid.
         */
        auto getDefaultCellSize() const -> int;

        /**
         * @return the default background color of the Project.
         */
        auto getBgColor() const -> const Color&;


        /**
         * @return  Returns the LayerDef matching the given @p id.
         * @throws std::invalid_argument If no LayerDef is found
         */
        auto getLayerDef(int id) const -> const LayerDef&;

        /**
         * @return  Returns the LayerDef matching the given @p id.
         * @throws std::invalid_argument If no LayerDef is found
         */
        auto getLayerDef(const std::string& name) const -> const LayerDef&;

        /**
         * @return  the @ref ldtk::EntityDef "EntityDef" matching the given @p id.
         * @throws std::invalid_argument If no EntityDef is found.
         */
        auto getEntityDef(int id) const -> const EntityDef&;
        /**
        * @return  the @ref ldtk::EntityDef "EntityDef" matching the given @p name.
        * @throws std::invalid_argument If no EntityDef is found.
        */
        auto getEntityDef(const std::string& name) const -> const EntityDef&;

        /**
         * @return a vector containing all the @ref ldtk::Tileset "tilesets" of the @ref ldtk::Project "project".
         */
        auto allTilesets() const -> const std::vector<Tileset>&;
        /**
         *
         * @throws std::invalid_argument if no @ref ldtk::Tileset "Tileset" is found.
         *
         * @return Returns the @ref ldtk::Tileset "Tileset" matching the given @p id
         */
        auto getTileset(int id) const -> const Tileset&;
        /**
         *
         * @throws std::invalid_argument if no @ref ldtk::Tileset "Tileset" is found.
         *
         * @return Returns the @ref ldtk::Tileset "Tileset" matching the given @p name
         */
        auto getTileset(const std::string& name) const -> const Tileset&;

        auto getEnum(int id) const -> const Enum&;
        auto getEnum(const std::string& name) const -> const Enum&;

        /**
         * @return a vector containing all the Worlds of the Project.
         */
        auto allWorlds() const -> const std::vector<World>&;
        /**
         * For @ref ldtk::Project "project's" with only one world,
         *
         * @return the @ref ldtk::World.
         *
         * @throws std::invalid_argument If the project has multiple worlds.
         */
        auto getWorld() const -> const World&;
        /**
         * For @ref ldtk::Project "projects's" with multi-worlds enabled
         *
         * @param name name of the world, in a multi-worlds project.
         * @return the @ref ldtk::World matching the given @p name.
         */
        auto getWorld(const std::string& name) const -> const World&;
            /**
              * For @ref ldtk::Project "projects's" with multi-worlds enabled
              *
              * @param iid iid of the world, in a multi-worlds project.
              * @return the @ref ldtk::World matching the given @p iid.
              */
        auto getWorld(const IID& iid) const -> const World&;

        /**
         * @return a vector containing the @ref ldtk::EntityRef "EntityRefs" that are located in the ToC.
         */
        auto allTocEntities() const -> const std::vector<EntityRef>&;
        /**
         *
         * @param name name of entity in the ToC
         * @return a vector containing the @ref ldtk::EntityRef "EntityRefs" with the given @p name, that are located in the ToC.
         */
        auto getTocEntitiesByName(const std::string& name) const -> const std::vector<EntityRef>&;

    private:
        void load(const nlohmann::json& j, const FileLoader& file_loader, bool from_memory);

        FilePath m_file_path;
        FloatPoint m_default_pivot;
        int m_default_cell_size = 0;
        Color m_background_color;
        std::string m_json_version;

        std::vector<LayerDef> m_layers_defs;
        std::unordered_map<int, ref_wrapper<LayerDef>> m_layers_defs_by_uid;
        std::unordered_map<std::string, ref_wrapper<LayerDef>> m_layers_defs_by_name;

        std::vector<EntityDef> m_entities_defs;
        std::unordered_map<int, ref_wrapper<EntityDef>> m_entities_defs_by_uid;
        std::unordered_map<std::string, ref_wrapper<EntityDef>> m_entities_defs_by_name;

        std::vector<Tileset> m_tilesets;
        std::unordered_map<int, ref_wrapper<Tileset>> m_tilesets_by_uid;
        std::unordered_map<std::string, ref_wrapper<Tileset>> m_tilesets_by_name;

        std::vector<Enum> m_enums;
        std::unordered_map<int, ref_wrapper<Enum>> m_enums_by_uid;
        std::unordered_map<std::string, ref_wrapper<Enum>> m_enums_by_name;

        std::vector<World> m_worlds;

        std::vector<EntityRef> m_toc;
        mutable std::map<std::string, std::vector<EntityRef>> m_toc_map;
    };

} // namespace ldtk
