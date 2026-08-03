// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <streambuf>
#include <string>

namespace ldtk {

    /**
    * Represents a two dimentional coordinate.
    *
     * @tparam T can be one of the following types: `float`, `int` or `unsigned int`.
     */
    template <typename T>
    struct Point
    {
        Point() : x(0), y(0) {}
        Point(T x, T y) : x(x), y(y) {}
        /**
         * X coordinate of the Point.
         */
        T x;
        /**
         * Y coordinate of the Point.
         */
        T y;
    };

    template <typename T>
    auto operator==(const Point<T>& lhs, const Point<T>& rhs) -> bool
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }

    using IntPoint = Point<int>;
    using UIntPoint = Point<unsigned int>;
    using FloatPoint = Point<float>;

    /**
     * Represents an axis aligned rectangle.
     * @tparam T can be one of the following types: `float` or `int`.
     */
    template <typename T>
    struct Rect
    {
        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(T x, T y, T w, T h) : x(x), y(y), width(w), height(h) {}
        Rect(const Point<T>& pos, const Point<T>& size) : x(pos.x), y(pos.y), width(size.x), height(size.y) {}
        /**
         * X coordinate of the top left corner of the Rect.
         */
        T x;
        /**
         * Y coordinate of the top left corner of the Rect.
         */
        T y;
        /**
         * Width of the Rect.
         */
        T width;
        /**
         * Height of the Rect.
         */
        T height;
    };

    template <typename T>
    auto operator==(const Rect<T>& lhs, const Rect<T>& rhs) -> bool
    {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height);
    }

    template <typename T>
    auto operator!=(const Rect<T>& lhs, const Rect<T>& rhs) -> bool
    {
        return !(lhs == rhs);
    }

    using IntRect = Rect<int>;
    using UIntRect = Rect<unsigned int>;
    using FloatRect = Rect<float>;

    /**
     * Represents the dimensions of the 9-slice tile render.
     */
    struct NineSliceBorders
    {
        int top;
        int right;
        int bottom;
        int left;
    };

    /**
     * Represents a color value in the 32 bits RGBA format.
     */
    struct Color
    {
        Color() = default;
        explicit Color(const std::string& hex);
        explicit Color(int hex);
        Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 0xffu);

        /**
         * Red component of the Color.
         */
        std::uint8_t r = 0;
        /**
         * Green component of the Color.
         */
        std::uint8_t g = 0;
        /**
         * Blue component of the Color.
         */
        std::uint8_t b = 0;
        /**
         * Alpha component of the Color (opacity).
         */
        std::uint8_t a = 0xffu;
    };

    auto operator==(const Color& lhs, const Color& rhs) -> bool;

    /**
    *  Represents a vertex composed of a position and a texture coordinate.
    *
    * The graphic representation of a Tile is composed of 4 vertices.
     */
    struct Vertex
    {
        /**
         * Coordinate of the vertex in pixels, relative to the @ref Level.
         */
        FloatPoint pos = {0.f, 0.f};
        /**
         * Texture coordinate of the vertex, in pixels.
         */
        IntPoint tex = {0, 0};
    };

    /**
     * Represents the grid values that can be painted on an IntGrid layer.
     */
    struct IntGridValue
    {
        const int value;
        const std::string name;
        const Color color;
        static const IntGridValue None;
    };

    /**
     * Contains enum values representing all the possible types of @ref World layouts.
     */
    enum class WorldLayout
    {
        Free,
        GridVania,
        LinearHorizontal,
        LinearVertical
    };

    /**
     * Contains enum values representing all the possible types of @ref ldtk::Layer "Layers".
     */
    enum class LayerType
    {
        IntGrid,
        Entities,
        Tiles,
        AutoLayer
    };

    /**
     * Contains enum values representing all the possible Directions of Level neighbours.
     * It is used by @ref ldtk::Level::getNeighbours and returned by
     * @ref ldtk::Level::getNeighbourDirection.
     */
    enum class Dir
    {
        None,
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
        Overlap,
        Over,
        Under,
    };

    /**
     * Contains enum values representing all the possible types of a @ref Field.
     *
     * These enum values can be used to get a field from an Entity. For example:
     *
     * @code{.cpp}
     * const auto& field_value = entity.getField<ldtk::FieldType::ArrayEntityRef>();
     * // The type of field_value is ldtk::ArrayField<ldtk::EntityRef>
     * @endcode
     */
    enum class FieldType
    {
        Int,
        Float,
        Bool,
        String,
        Color,
        Point,
        Enum,
        FilePath,
        Tile,
        EntityRef,
        ArrayInt,
        ArrayFloat,
        ArrayBool,
        ArrayString,
        ArrayColor,
        ArrayPoint,
        ArrayEnum,
        ArrayFilePath,
        ArrayTile,
        ArrayEntityRef,
    };

    /**
     * A string that contains file path information.
     */
    class FilePath : std::string
    {
    public:
        /**
         * Default constructor. Constructs an empty FilePath.
         */
        FilePath() = default;
        /**
         * Construct a FilePath from a valid provided string.
         * @param str filepath string
         */
        FilePath(const std::string& str);
        using std::string::basic_string;
        using std::string::c_str;

        /**
         * Returns the path to the directory containing the file.
         */
        auto directory() const -> std::string;
        /**
         * Returns the filename (including the extension) of the FilePath.
         */
        auto filename() const -> std::string;
        /**
         * Returns only the extension of the file (e.g. png, json, txt ...).
         */
        auto extension() const -> std::string;
    };

    /**
    * @ref FileLoader is a kind of function that takes the path of a file as parameter and returns a
    * unique pointer to a standard stream buffer for that file.
    *
    * This allows to load a @ref Project from a custom source stream (e.g. from a virtual filesystem).
     */
    using FileLoader = std::function<std::unique_ptr<std::streambuf>(const std::string&)>;

    class Tileset; // forward declaration

    /**
     * Represents a rectangle of one or multiple tiles. Used in TileField.
     */
    struct TileRect
    {
        TileRect(const Tileset& tileset, IntRect bounds);

        const IntRect bounds;

        /**
         * Returns the Tileset to which the tile belongs.
         */
        auto getTileset() const -> const Tileset&;

    private:
        const Tileset* m_tileset;
    };

    struct IID
    {
        IID() = default;
        explicit IID(std::string iid);
        auto str() const -> const std::string&;

    private:
        std::string m_iid;
    };

    auto operator==(const IID& lhs, const IID& rhs) -> bool;
    auto operator!=(const IID& lhs, const IID& rhs) -> bool;

    class Entity; // forward declaration

    /**
     * Represents a reference to an @ref Entity.
     */
    struct EntityRef
    {
        EntityRef(IID ent, IID layer, IID level, IID world);

        /**
         * This operator allows to access the Entity reference
         *
         * @code{.cpp}
         * entity_ref->getName(); // returns the name of the referenced Entity
         * @endcode
         *
         * @returns a **pointer** to the Entity referenced by the EntityRef.
         * @return
         */
        auto operator->() const -> const Entity*;

        IID entity_iid;
        IID layer_iid;
        IID level_iid;
        IID world_iid;

    private:
        friend class Project;
        const Entity* ref = nullptr;
    };

} // namespace ldtk

template <typename T>
auto operator<<(std::ostream& os, const ldtk::Point<T>& point) -> std::ostream&
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

template <typename T>
auto operator<<(std::ostream& os, const ldtk::Rect<T>& rect) -> std::ostream&
{
    os << "(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ")";
    return os;
}

auto operator<<(std::ostream& os, const ldtk::Color& color) -> std::ostream&;

auto operator<<(std::ostream& os, const ldtk::FilePath& path) -> std::ostream&;

auto operator<<(std::ostream& os, const ldtk::IID& iid) -> std::ostream&;

namespace std {

    // specialization for the IID structure, allows to create unordered_set<IID>
    template <>
    struct hash<ldtk::IID>
    {
        std::size_t operator()(const ldtk::IID& iid) const noexcept
        {
            return std::hash<std::string>{}(iid.str());
        }
    };

} // namespace std
