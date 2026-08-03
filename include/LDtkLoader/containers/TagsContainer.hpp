// Created by Modar Nasser on 22/02/2022.

#pragma once

#include <string>
#include <vector>

#include <nlohmann/json_fwd.hpp>

namespace ldtk {

    /**
     * @brief Base class for objects that can be tagged. Tags are strings.
     */
    class TagsContainer
    {
    public:
        /**
         * @return `true` if the TagsContainer contains the given tag, returns `false` otherwise.
         */
        auto hasTag(const std::string& tag) const -> bool;
        /**
         * @return a vector containing all the tags in the @ref TagsContainer.
         */
        auto allTags() const -> const std::vector<std::string>&;

    protected:
        explicit TagsContainer(const nlohmann::json& j);

    private:
        const std::vector<std::string> m_tags;
    };

} // namespace ldtk
