INSTALL spatial;
INSTALL httpfs;

LOAD spatial;
LOAD httpfs;

SET s3_region='us-west-2';

-- Fetch connectors (nodes)
COPY ( 
    SELECT type, id, ST_GeomFromWkb(geometry) AS geometry 
    FROM read_parquet('s3://overturemaps-us-west-2/release/2023-07-26-alpha.0/theme=transportation/type=connector/*', filename=true, hive_partitioning=1) 
    WHERE ST_Within(ST_GeomFromWkb(geometry), ST_GeomFromText('POLYGON ((23.878066163834433 49.93497633271076, 23.878066163834433 49.75085562193263, 24.178554868040123 49.75085562193263, 24.178554868040123 49.93497633271076, 23.878066163834433 49.93497633271076))')) 
)
TO 'connectors.csv' (FORMAT CSV, DELIMITER '|', HEADER); 
 

-- Fetch segments (edges) 
COPY (
       SELECT id, connectors, ST_GeomFromWkb(geometry) AS geometry
       FROM read_parquet('s3://overturemaps-us-west-2/release/2023-07-26-alpha.0/theme=transportation/type=segment/*', filename=true, hive_partitioning=1) 
       WHERE ST_Within(ST_GeomFromWkb(geometry), ST_GeomFromText('POLYGON ((23.878066163834433 49.93497633271076, 23.878066163834433 49.75085562193263, 24.178554868040123 49.75085562193263, 24.178554868040123 49.93497633271076, 23.878066163834433 49.93497633271076))')) 
)
TO 'segment.csv' (FORMAT CSV, DELIMITER '|', HEADER); 