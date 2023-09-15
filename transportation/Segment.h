//
// Created by Oleslav Boychuk on 15.09.2023.
//

#ifndef MAPENGINE_SEGMENT_H
#define MAPENGINE_SEGMENT_H
#include "types.h"

struct Segment
{
    Segment(const std::string &_id, const line_string &x_y, const bounding_box &box) : id(_id), line_string(x_y), b_box(box) {}
    Segment(std::string &&_id, line_string &&x_y, bounding_box &&box) : id(std::move(_id)), line_string(std::move(x_y)), b_box(std::move(box)) {}

    std::string id;
    std::vector<std::string> connectors;
    line_string line_string;
    bounding_box b_box;
};

#endif //MAPENGINE_SEGMENT_H
