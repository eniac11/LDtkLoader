// Created by Modar Nasser on 22/02/2022.

#pragma once

#include <array>

#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class Layer;

    /**
     * Represents a tile on a @ref ldtk::Layer "Layer"
     */
    class Tile
    {
    public:
        /**
         * Pointer to the Layer object that contains the Tile.
         */
        const Layer* const layer;

        /**
         * Coordinate ID of the @ref Tile. Used to retrieve grid position of the @ref Tile.
         */
        const int coordId = 0;
        /**
         * Unique ID that identifies the @ref Tile on its @ref Tileset.
         */
        const int tileId = 0;

        /**
         * Equals `true` if the Tile is flipped alongside the X axis. Equals `false` otherwise.
         */
        const bool flipX = false;
        /**
         * Equals `true` if the Tile is flipped alongside the Y axis. Equals `false` otherwise.
         */
        const bool flipY = false;

        /**
         * A value between 0.0 and 1.0 that represents the opacity of the Tile.
         */
        const float alpha;

        /**
         * @return the position of the @ref Tile in pixels, relatively to the @ref ldtk::Level "Level" (i.e. after applying @ref ldtk::Layer "Layer" offset).
         */
        auto getPosition() const -> IntPoint;
        /**
         * @return the grid position of the @ref Tile.
         */
        auto getGridPosition() const -> IntPoint;
        /**
         *
        * @return the position of the @ref Tile in pixels, relatively to the @ref ldtk::World "World"
        * (i.e. after applying @ref ldtk::Layer "Layer" and @ref ldtk::Level "Level" offsets).
         */
        auto getWorldPosition() const -> IntPoint;

        /**
         *
         * @return the @ref IntRect corresponding to the @ref Tile area on the @ref Tileset texture.
         */
        auto getTextureRect() const -> IntRect;

        /**
         * @return an array containing 4 vertices, corresponding the graphical representation of a Tile quad.
         * @sa ldtk::Vertex
         */
        auto getVertices() const -> std::array<Vertex, 4>;

        static const Tile None;

        Tile(const Layer* l, IntPoint pos, int coord_id, int tile_id, int flips, float a);

    private:
        IntPoint m_position;
    };

    auto operator==(const Tile& l, const Tile& r) -> bool;
    auto operator!=(const Tile& l, const Tile& r) -> bool;

} // namespace ldtk
