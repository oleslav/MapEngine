//
// Created by Oleslav Boychuk on 14.09.2023.
//

#ifndef MAPENGINE_CONNECTOR_H
#define MAPENGINE_CONNECTOR_H
#include "types.h"

struct Connector
{
    Connector(const std::string &_id, const point &_x_y, const bounding_box &box) : id(_id), x_y(_x_y), b_box(box) {}
    Connector(std::string &&_id, point &&_x_y, bounding_box &&box) : id(std::move(_id)), x_y(std::move(_x_y)), b_box(std::move(box)) {}

    std::string id;
    point x_y;
    bounding_box b_box;
};


#endif //MAPENGINE_CONNECTOR_H
