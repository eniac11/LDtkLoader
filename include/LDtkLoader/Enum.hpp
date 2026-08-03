// Created by Modar Nasser on 13/11/2020.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LDtkLoader/containers/TagsContainer.hpp"
#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class Enum;

    class Tileset;

    struct EnumValue
    {
        /**
         * Name of the @ref EnumValue.
         */
        const std::string name;
        /**
         * Color of the @ref EnumValue.
         */
        const Color color;
        /**
        * Enum object owning this EnumValue.
        *
        * Allows to access the name of the Enum, for example:
        *
        * @code
        * const auto& enum_value = project.getEnum("Items")["SilverSword"];
        *
        * std::cout << enum_value.type.name;
        * // output: Items
        * @endcode
        */
        const Enum& type;

        /**
         * @return `true` if the @ref EnumValue has an icon, returns `false` otherwise.
         */
        auto hasIcon() const -> bool;
        /**
         *
         * @returns the @ref ldtk::Tileset "tileset" of the icon.
         */
        auto getIconTileset() const -> const Tileset&;
        /**
         * @return the texture rectangle of this @ref ldtk::EnumValue "EnumValue's" icon.
         */
        auto getIconTextureRect() const -> const IntRect&;

    private:
        friend Enum;
        friend auto operator==(const EnumValue& l, const EnumValue& r) -> bool;
        EnumValue(
            std::string name,
            int id,
            const IntRect& tile_rect,
            const Color& color,
            const Enum& enum_type
        );
        const int id;
        const IntRect tile_rect;
    };

    auto operator==(const EnumValue& l, const EnumValue& r) -> bool;
    auto operator!=(const EnumValue& l, const EnumValue& r) -> bool;

    class Enum : public TagsContainer
    {
        friend class Project;

    public:
        ~Enum() = default;
        Enum(const Enum&) = delete;
        Enum(Enum&&) noexcept = default;
        auto operator=(const Enum&) -> Enum& = delete;
        auto operator=(Enum&&) -> Enum& = delete;

        /**
         * Name of the Enum.
         */
        const std::string name;
        /**
         * Unique identifier of the Enum.
         */
        const int uid;

        /**
         * @returns the @ref EnumValue matching the provided name.
         * @throws std::invalid_argument If no EnumValue is found
         */
        auto operator[](const std::string& val_name) const -> const EnumValue&;

        /**
         * @return `true` if the @ref Enum has icons, returns `false` otherwise.
         */
        auto hasIcons() const -> bool;
        /**
         * @return the @ref ldtk::Tileset "tileset" used by this Enum's icons.
         */
        auto getIconsTileset() const -> const Tileset&;

        explicit Enum(const nlohmann::json& j);

    private:
        const int m_tileset_id;
        const Tileset* m_tileset = nullptr;
        std::unordered_map<std::string, EnumValue> m_values;
    };

} // namespace ldtk

auto operator<<(std::ostream& os, const ldtk::EnumValue& enum_value) -> std::ostream&;
