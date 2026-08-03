// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/containers/TagsContainer.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class Project;

    /**
     * @brief A Tileset contains information about a texture used to draw tiles.
     */
    class Tileset : public TagsContainer
    {
    public:
        /**
         * Name of the Tileset.
         */
        const std::string name;
        /**
         * Unique identifier of the Tileset.
         */
        const int uid;
        /**
         * Relative path to the Tileset's texture image file.
         */
        const std::string path;
        /**
         * Size in pixels of the Tileset's texture.
         */
        const IntPoint texture_size;
        /**
         * Size in pixels of a tile in the Tileset. Tiles are always square shaped.
         */
        const int tile_size;
        /**
         * Spacing in pixels between tiles in the Tileset.
         */
        const int spacing;
        /**
         * Padding in pixels to the first row and column of tiles in the Tileset.
         */
        const int padding;

        /**
         * @return the tile id at the given position in pixels.
         */
        auto getTileIdAt(int pos_x, int pos_y) const -> int;

        /**
         * @return the texture coordinates of the Tile ID.
         */
        auto getTileTexturePos(int tile_id) const -> IntPoint;
        /**
         * @return the custom data associated with the Tile ID.
         */
        auto getTileCustomData(int tile_id) const -> const std::string&;
        /**
         * @return a vector containing the @ref ldtk::EnumValue "EnumTags" associated with the Tile ID.
         */
        auto getTileEnumTags(int tile_id) const -> const std::vector<ref_wrapper<const EnumValue>>&;

        /**
         * 
         * @returns `true` if the Tileset has enum tags associated to tiles, `false` otherwise.
         */
        auto hasEnumTags() const -> bool;
        /**
         * 
         * @return the @ref ldtk::Enum "Enum" type used by this Tileset's @ref ldtk::EnumValue "EnumTags".
         */
        auto getEnumTagsEnum() const -> const Enum&;
        /**
         * Get a vector of all tiles ID tagged with a given EnumValue.
         *
         * @throws std::invalid_argument if the @ref ldtk::EnumValue "EnumValue" argument does not have the right @ref ldtk::Enum "Enum" type.
         */
        auto getTilesByEnumTag(const EnumValue& enumvalue) const -> const std::vector<int>&;

        Tileset(const nlohmann::json& j, Project* p);

    private:
        const Enum* const m_enumtags_enum;
        std::unordered_map<int, std::string> m_custom_data_map;
        std::unordered_map<int, std::vector<ref_wrapper<const EnumValue>>> m_enumtags_by_tile;
        std::unordered_map<std::string, std::vector<int>> m_tiles_by_enumtag;
    };

} // namespace ldtk
