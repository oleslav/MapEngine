//
// Created by Oleslav Boychuk on 15.09.2023.
//

#ifndef MAPENGINE_TYPES_H
#define MAPENGINE_TYPES_H

#include <vector>
#include <string>

using point = std::pair<double, double>;
using bounding_box = std::pair<point, point>;
using line_string = std::vector<point>;

static point getPoint(const std::string& str)
{
    // TODO: Use std::string_view to optimize memory
    static const char start_char = '(', middle_char = ' ', end_char  = ')';
    const std::size_t start = str.find(start_char) + 1;
    const std::size_t middle = str.find(middle_char, start) + 1;
    const std::size_t end = str.find(end_char, middle);
    const long double x = std::stold(str.substr(start, middle - start - 1));
    const long double y = std::stold(str.substr(middle, end - middle));
    return {x, y};
}

static bounding_box getBoundingBox(const std::string& str)
{
    // TODO: Use std::string_view to optimize memory
    static const std::string start_chars{": "}, end_char{','}, final_char{'}'};

    std::size_t start = str.find(start_chars) + 2;
    std::size_t end = str.find(end_char, start);
    const long double min_x = std::stold(str.substr(start, end - start));

    start = str.find(start_chars, end) + 2;
    end = str.find(end_char, start);
    const long double max_x = std::stold(str.substr(start, end - start));

    start = str.find(start_chars, end) + 2;
    end = str.find(end_char, start);
    const long double min_y = std::stold(str.substr(start, end - start));

    start = str.find(start_chars, end) + 2;
    end = str.find(final_char, start);
    const long double max_y = std::stold(str.substr(start, end - start));

    return {{min_x, max_x}, {min_y, max_y}};
}

static line_string getLineString(const std::string& str)
{
    // TODO: Use std::string_view to optimize memory
    static const char start_char = '(', middle_char = ' ', end_char  = ',', final_char  = ')';
    line_string result;
    std::size_t start = str.find(start_char) + 1;
    std::size_t middle = str.find(middle_char, start) + 1;
    std::size_t end = str.find(end_char, middle);

    while(end != std::string::npos)
    {
        const long double x = std::stold(str.substr(start, middle - start - 1));
        const long double y = std::stold(str.substr(middle, end - middle));
        result.emplace_back(x, y);
        start = str.find(middle_char, end) + 1;
        middle = str.find(middle_char, start) + 1;
        end = str.find(end_char, middle);
    }

    const std::size_t final = str.find(final_char, middle);
    const long double x = std::stold(str.substr(start, middle - start - 1));
    const long double y = std::stold(str.substr(middle, final - middle));
    result.emplace_back(x, y);

    return std::move(result);
}

#endif //MAPENGINE_TYPES_H
