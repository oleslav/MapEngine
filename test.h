//
// Created by Oleslav Boychuk on 15.09.2023.
//

#ifndef MAPENGINE_TEST_H
#define MAPENGINE_TEST_H

#include "transportation/tools.h"
#include "memory.h"
#include <iostream>
#include <chrono>

#if CSV_LIB_TYPE == 0
#include "fast_csv/csv.hpp"

void test_connector()
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::vector<Connector> connectors;
    connectors.reserve(70000);
    io::CSVReader<14, io::trim_chars<' ', '\t'>, io::double_quote_escape<'|','\"'>> in(CSV_PATH"connectors.csv");
    in.read_header(io::ignore_extra_column, "id", "updatetime", "version", "level", "subtype", "connectors", "road", "sources", "bbox", "geometry", "filename", "theme", "type", "wkt_geometry");
    std::string _, id, bbox, wkt_geometry;

    while(in.read_row(id, _, _, _, _, _, _, _, bbox, _, _, _, _, wkt_geometry))
    {
        connectors.emplace_back(id, getPoint(wkt_geometry), getBoundingBox(bbox));
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double>(stop - start);
    std::cout << "Connector time takes: " << duration.count() << " sec" << std::endl;
}

void test_segment()
{
    const auto start = std::chrono::high_resolution_clock::now();
    std::vector<Segment> segments;
    segments.reserve(50000);
    io::CSVReader<14, io::trim_chars<' ', '\t'>, io::double_quote_escape<'|','\"'>> in(CSV_PATH"segments.csv");
    in.read_header(io::ignore_extra_column, "id", "updatetime", "version", "level", "subtype", "connectors", "road", "sources", "bbox", "geometry", "filename", "theme", "type", "geometry:1");
    std::string id, _, version, level, subtype, connectors, road, sources, bbox, geometry, filename, theme, type, geometry_1;
    while(in.read_row(id, _, _, _, _, connectors, _, _, bbox, _, _, _, _, geometry_1))
    {
        Segment segment{id, getLineString(geometry_1), getBoundingBox(bbox)};
        segment.connectors.emplace_back(connectors);
        segments.push_back(segment);
    }
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double>(stop - start);
    std::cout << "Segments time takes: " << duration.count() << " sec" << std::endl;
}

#elif CSV_LIB_TYPE == 1
#include "csv-parser/csv.hpp"

void test_connector()
{
    std::vector<Connector> connectors;
    connectors.reserve(70000);
    auto start = std::chrono::system_clock::now();
    csv::CSVReader reader(CSV_PATH"connectors.csv");
    for (csv::CSVRow& row: reader)
    {
        connectors.emplace_back(row["id"].get(), getPoint(row["wkt_geometry"].get()), getBoundingBox(row["bbox"].get()));
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Connector time takes: " << duration.count() << " sec" << std::endl;
}


void test_segment()
{
    std::vector<Segment> segments;
    segments.reserve(50000);
    auto start = std::chrono::system_clock::now();
    csv::CSVReader reader(CSV_PATH"segments.csv");
    for (csv::CSVRow& row: reader)
    {
        Segment segment{row["id"].get(), getLineString(row["geometry:1"].get()), getBoundingBox(row["bbox"].get())};
        segment.connectors.emplace_back(row["connectors"].get());
        segments.push_back(segment);
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Segments time takes: " << duration.count() << " sec" << std::endl;
}
#elif CSV_LIB_TYPE == 2
#include "rapidcsv/csv.hpp"

void test_connector()
{
    auto start = std::chrono::system_clock::now();
    rapidcsv::Document doc(CSV_PATH"connectors.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams('|'));
    std::vector<std::string> ids = doc.GetColumn<std::string>("id");
    std::vector<std::string> wkt_geometry = doc.GetColumn<std::string>("wkt_geometry");
    std::vector<std::string> bbox = doc.GetColumn<std::string>("bbox");
    std::vector<Connector> connectors;
    connectors.reserve(ids.size());
    for (int i = 0; i < ids.size(); ++i)
    {
        connectors.emplace_back(ids[i], getPoint(wkt_geometry[i]), getBoundingBox(bbox[i]));
    }
    ids.clear(); wkt_geometry.clear(); bbox.clear();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Connector time takes: " << diff.count() << " sec" << std::endl;
}

void test_segment()
{
    auto start = std::chrono::system_clock::now();
    rapidcsv::Document doc(CSV_PATH"segments.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams('|'));
    std::vector<std::string> ids = doc.GetColumn<std::string>("id");
    std::vector<std::string> geometry_1 = doc.GetColumn<std::string>("geometry:1");
    std::vector<std::string> bbox = doc.GetColumn<std::string>("bbox");
    std::vector<Segment> connectors;
    connectors.reserve(ids.size());
    for (int i = 0; i < ids.size(); ++i)
    {
        connectors.emplace_back(ids[i], getLineString(geometry_1[i]), getBoundingBox(bbox[i]));
    }
    ids.clear(); geometry_1.clear(); bbox.clear();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Connector time takes: " << diff.count() << " sec" << std::endl;
}

#endif


void test()
{
    print_memory_stat();
    test_connector();
    test_segment();
    print_memory_stat();
}
#endif //MAPENGINE_TEST_H
