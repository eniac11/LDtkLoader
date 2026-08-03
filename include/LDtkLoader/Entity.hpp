// Created by Modar Nasser on 22/11/2020.

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/containers/FieldsContainer.hpp"
#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/defs/FieldDef.hpp"

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;

    class Layer;

    /**
     * Represents an Entity instance.
     *
     * > [!TIP]
     * > To get the correct sprite data of the Entity as it is shown in the editor,
     * > you should use `hasSprite` to check if the Entity has a sprite,
     * > and then `getTexturePath` and `getTextureRect` to get the texture data.
     */
    class Entity : public FieldsContainer
    {
    public:
        ~Entity() = default;
        Entity(const Entity&) = delete;
        Entity(Entity&&) noexcept = default;
        auto operator=(const Entity&) -> Entity& = delete;
        auto operator=(Entity&&) -> Entity& = delete;

        /**
         * Pointer to the @ref Layer object that contains the @ref Entity.
         */
        const Layer* const layer;
        /**
         * Unique instance ID of the @ref Entity.
         */
        const IID iid;

        /**
         * Returns the name of the Entity.
         */
        auto getName() const -> const std::string&;
        /**
         * Returns the size in pixels of the Entity.
         */
        auto getSize() const -> const IntPoint&;
        /**
         * Returns the color of the Entity.
         */
        auto getColor() const -> const Color&;

        /**
         * Returns the position in pixels of the Entity relative to the parent Layer.
         *
         * If the layer has an offset different from (0, 0), then the layer's total offset should be
         * added to get the position in pixels relative to the parent Level.
         *
         * @code{.cpp}
         * auto entity_level_pos = entity.getPosition() + entity.layer->getOffset();
         * @endcode
         */
        auto getPosition() const -> const IntPoint&;
        /**
         * Returns the position in grid coordinates of the Entity.
         */
        auto getGridPosition() const -> const IntPoint&;
        /**
         * Returns the computed position in pixels of the @ref Entity relative to the @ref World.
         */
        auto getWorldPosition() const -> IntPoint;

        /**
         * Returns the pivot of the @ref Entity, a point from (0.f, 0.f) to (1.f, 1.f).
         */
        auto getPivot() const -> const FloatPoint&;

        /**
         * Returns true if the @ref Entity has a sprite associated to it, returns false otherwise.
         */
        auto hasSprite() const -> bool;
        /**
         * Returns the path to the texture of the sprite. Returns an empty string if the Entity has no sprite.
         * @return relative path of texture
         */
        auto getTexturePath() const -> const std::string&;
        /**
         * Returns the texture rectangle of this Entity's sprite.
         */
        auto getTextureRect() const -> const IntRect&;

        /**
         * Returns true if the @ref Entity "Entity's" sprite has a 9-slices scaling, returns false otherwise.
         */
        auto hasNineSlice() const -> bool;
        /**
         * Returns the @ref Entity "Entity's" 9-slices borders.
         * @sa NineSliceBorders.
         */
        auto getNineSliceBorders() const -> const NineSliceBorders&;

        /**
         * Returns `true` if the @ref Entity has the given tag, returns `false` otherwise.
         * @param tag name of tag
         */
        auto hasTag(const std::string& tag) const -> bool;
        /**
         * Returns a vector containing all tags of the Entity.
         */
        auto allTags() const -> const std::vector<std::string>&;

        auto allFields() const -> const std::vector<FieldDef>&;

        explicit Entity(const nlohmann::json& j, const World* w, const Layer* l);

    private:
        const EntityDef* const m_definition;

        const IntPoint m_size;
        const IntPoint m_position;
        const IntPoint m_grid_pos;
        const Color m_color;

        const Tileset* const m_tileset;
        const IntRect m_texture_rect;
    };

} // namespace ldtk
