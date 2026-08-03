// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "LDtkLoader/containers/FieldsContainer.hpp"
#include <nlohmann/json_fwd.hpp>
#include <optional>

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Layer.hpp"

namespace ldtk {

    class World;

    /**
     * A @ref ldtk::Level represents a single tilemap in the ldtk::World.
     *
     * It contains one or multiple @ref ldtk::Layer "Layers".
     */
    class Level : public FieldsContainer
    {
        friend World;

    public:

        /**
         * Contains information about the Level background image.
         */
        struct BgImage
        {
            /**
             * The relative path to the background image file.
             */
            FilePath path;
            /**
             * The cropped sub rectangle of the displayed image.
             */
            IntPoint pos;
            /**
             * The position of the top left corner of the background image in the level.
             */
            FloatPoint scale;
            /**
             * The scale of the background image.
             */
            IntRect crop;
        };

        ~Level() = default;
        Level(const Level&) = delete;
        Level(Level&&) noexcept = default;
        auto operator=(const Level&) -> Level& = delete;
        auto operator=(Level&&) -> Level& = delete;

        /**
         * Pointer to the World object that contains the Level.
         */
        const World* const world;

        /**
         * Name of the Level.
         */
        const std::string name;
        /**
         * Unique instance ID of the Level.
         */
        const IID iid;
        /**
         * Unique identifier of the Level.
         */
        const int uid;
        /**
         * Size of the Level in pixels.
         */
        const IntPoint size;
        /**
         * Position in pixels of the Level relatively to the World.
         */
        const IntPoint position;
        /**
         * Background color of the Level.
         */
        const Color bg_color;
        /**
         * Depth of the Level. 0 by default.
         */
        const int depth;

        /**
         *
         * @return the vector containing all Layers of the Level.
         */
        auto allLayers() const -> const std::vector<Layer>&;
        /**
         * Get layer by name
         * @throws std::invalid_argument if no Layer is found
         * @param layer_name name of layer
         * @return the Layer matching the given name.
         */
        auto getLayer(const std::string& layer_name) const -> const Layer&;

        /**
         * Get layer by name iid
         * @throws std::invalid_argument if no Layer is found
         * @param iid iid of layer
         * @return the Layer matching the given name.
         */
        auto getLayer(const IID& iid) const -> const Layer&;

        /**
         * 
         * @return true if the @ref Level has a background image, returns false otherwise.
         */
        auto hasBgImage() const -> bool;
        /**
         * 
         * @return the background image data of the Level.
         * @sa ldtk::Level::BgImage
         */
        auto getBgImage() const -> const BgImage&;

        /**
         *
         * @return a vector containing all the neighbour @ref ldtk::Level "Level".
         */
        auto allNeighbours() const -> const std::vector<ref_wrapper<const Level>>&;
        /**
         *
         * @param direction
         * @return a vector containing all the neighbour @ref ldtk::Level "Levels" placed at the given direction.
         */
        auto getNeighbours(const Dir& direction) const -> const std::vector<ref_wrapper<const Level>>&;
        /**
         * Get the direction of a neighbour Level.
         *
         * @param level
         * @return If the given Level is not a neighbour, returns `ldtk::Dir::None`.
         * @sa ldtk::Dir
         */
        auto getNeighbourDirection(const Level& level) const -> Dir;

        explicit Level(const nlohmann::json& j, World* w);

    private:
        std::vector<Layer> m_layers;
        std::optional<BgImage> m_bg_image;

        std::unordered_set<IID> m_neighbours_iid;
        std::map<Dir, std::unordered_set<IID>> m_neighbours_iid_by_dir;

        std::vector<ref_wrapper<const Level>> m_neighbours;
        std::map<Dir, std::vector<ref_wrapper<const Level>>> m_neighbours_by_dir;
    };

} // namespace ldtk
