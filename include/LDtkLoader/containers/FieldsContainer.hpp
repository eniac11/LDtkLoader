// Created by Modar Nasser on 27/01/2021.

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>


#include <nlohmann/json_fwd.hpp>

#include "LDtkLoader/defs/FieldDef.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Field.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class World;

    /**
     * @brief Base class for objects that can have fields.
     *
     * Fields are pairs of (name, value) where the value can be of one of the following types: `int`,
     * `float`, `bool`, `@ref std::string`, `@ref ldtk::Color` , `@ref ldtk::IntPoint`, `@ref ldtk::Enum` or `@ref ldtk::FilePath`.
     **/
    class FieldsContainer
    {
    public:
        FieldsContainer() = default;

        /**
        * @brief Get the field matching the given @p name and @p T.
        *
        * This overload allows to get either single value fields or array fields:
        *
        * @code{.cpp}
        * // get a single value field
        * const auto& field = entity.getField<ldtk::FieldType::Color>("hair_color");
        *
        * if (!field.is_null()) {
        *     // get the field value
        *     const auto& hair_color = field.value();
        * }
        * @endcode
        *
        * @code{.cpp}
        * // get an array field
        * const auto& array_field = level.getField<ldtk::FieldType::ArrayPoint>("spawns");
        *
        * // iterate on the array field
        * for (const auto& field : array_field) {
        *     if (!field.is_null()) {
        *     // get the field value
        *     const auto& point = field.value();
        *     }
        * }
        * @endcode
        *
         * @tparam T type - must be one of the values of the @ref FieldType enum.
         * @param name
         * @return a field can be null.
         */
        template <FieldType T>
        auto getField(const std::string& name) const -> const getFieldType<T>&;

        /**
        *  Get the field matching the given @p name and @p T type.
        *
        * `T` must be one of the following types : `int`, `float`, `bool`, `std::string`, `ldtk::Color`,
        * `ldtk::IntPoint`, `ldtk::Enum`, `ldtk::FilePath`.
        *
        * For example, if your FieldsContainer has a field of type Color named "color", you can write :
        *
        * @code{.cpp}
        * const FieldsContainer& object = ...; // get the FieldsContainer
        *
        * // get the field
        * const auto& field = object.getField<ldtk::Color>("color");
        *
        * if (!field.is_null()) {
        *     // get the field value
        *     const auto& color = field.value();
        * }
        * @endcode
        *
        * @tparam T type of field
        * @param name name of field
        * @return a field can be null.
        */
        template <typename T>
        auto getField(const std::string& name) const -> const Field<T>&;

        /**
        * Get the array field matching the given @p name and @p T type.
        *
        * `ldtk::ArrayField<T>` is equivalent to `std::vector<ldtk::Field<T>>` and can be iterated over like
        * a normal vector. Fields can be null.
        *
        * `T` must be one of the following types : `int`, `float`, `bool`, `std::string`, `ldtk::Color`,
        * `ldtk::IntPoint`, `ldtk::Enum`, `ldtk::FilePath`.
        *
        * For example, if your FieldsContainer has a field of type ArrayPoint named "spawns", you can write :
        *
        * @code{.cpp}
        * const FieldsContainer& object = ...; // get the FieldsContainer
        *
        * // get the field
        * const auto& array_field = object.getArrayField<ldtk::IntPoint>("spawns");
        *
        * // iterate on the array field
        * for (const auto& field : array_field) {
        *     if (!field.is_null()) {
        *         // get the field value
        *         const auto& point = field.value();
        *     }
        * }
        * @endcode
        *
         * @tparam T
         * @param name
         * @return
         */
        template <typename T>
        auto getArrayField(const std::string& name) const -> const ArrayField<T>&;

    protected:
        FieldsContainer(const nlohmann::json& j, const World* w);

        template <typename T>
        auto addField(const std::string& name, const T& field) -> Field<T>&;

        template <typename T>
        auto addField(const std::string& name, const Field<T>& field) -> Field<T>&;

        template <typename T>
        auto addArrayField(const std::string& name, const std::vector<Field<T>>& field) -> ArrayField<T>&;

        template <typename T>
        auto addArrayField(const std::string& name, const ArrayField<T>& field) -> ArrayField<T>&;

    private:
        void parseFields(const nlohmann::json& j, const World* w);
        void parseArrayField(
            const nlohmann::json& field,
            const std::string& type,
            const std::string& name,
            const World* w
        );
        void parseValueField(
            const nlohmann::json& field,
            const std::string& type,
            const std::string& name,
            const World* w
        );

        std::vector<std::unique_ptr<IField>> m_gc;
        std::unordered_map<std::string, IField*> m_fields;
        std::unordered_map<std::string, IField*> m_array_fields;
    };

    template <FieldType T>
    auto FieldsContainer::getField(const std::string& name) const -> const getFieldType<T>&
    {
        using FieldT = getFieldType<T>;
        const auto is_array = std::is_base_of<FieldT, ArrayField<typename FieldT::value_type>>::value;
        if (is_array) {
            return getArrayField<typename FieldT::value_type>(name);
        }
        else {
            return getField<typename FieldT::value_type>(name);
        }
    }

    template <typename T>
    auto FieldsContainer::getField(const std::string& name) const -> const Field<T>&
    {
        if (m_fields.count(name) > 0) {
            const auto* field = m_fields.at(name);
            const auto* ret = dynamic_cast<const Field<T>*>(field);
            if (ret) {
                return *ret;
            }
            ldtk_error("Field \"" + name + "\" is not of type " + typeid(T).name() + ".");
        }
        ldtk_error("Field \"" + name + "\" does not exist.");
    }

    template <typename T>
    auto FieldsContainer::getArrayField(const std::string& name) const -> const ArrayField<T>&
    {
        if (m_array_fields.count(name) > 0) {
            const auto* field = m_array_fields.at(name);
            const auto* ret = dynamic_cast<const ArrayField<T>*>(field);
            if (ret) {
                return *ret;
            }
            ldtk_error("ArrayField \"" + name + "\" is not of type " + typeid(T).name() + ".");
        }
        ldtk_error("ArrayField \"" + name + "\" does not exist.");
    }

    template <typename T>
    auto FieldsContainer::addField(const std::string& name, const T& field) -> Field<T>&
    {
        auto* new_field = new Field<T>(field);
        m_fields.emplace(name, new_field);
        m_gc.emplace_back(new_field);
        return *new_field;
    }

    template <typename T>
    auto FieldsContainer::addField(const std::string& name, const Field<T>& field) -> Field<T>&
    {
        auto* new_field = new Field<T>(field);
        m_fields.emplace(name, new_field);
        m_gc.emplace_back(new_field);
        return *new_field;
    }

    template <typename T>
    auto FieldsContainer::addArrayField(const std::string& name, const std::vector<Field<T>>& field)
        -> ArrayField<T>&
    {
        auto* new_field = new ArrayField<T>(field);
        m_array_fields.emplace(name, new_field);
        m_gc.emplace_back(new_field);
        return *new_field;
    }

    template <typename T>
    auto FieldsContainer::addArrayField(const std::string& name, const ArrayField<T>& field) -> ArrayField<T>&
    {
        auto* new_field = new ArrayField<T>(field);
        m_array_fields.emplace(name, new_field);
        m_gc.emplace_back(new_field);
        return *new_field;
    }

} // namespace ldtk
