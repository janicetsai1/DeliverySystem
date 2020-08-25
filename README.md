# DeliverySystem
Uses [OpenStreetMap](https://www.openstreetmap.org/#map=16/34.0708/-118.4461) data to find the best delivery route for one or more points. Utilizes the A* search algorithm to find the optimized path. 

## Description
This program takes the files with specified delivery points and outputs a turn-by-turn set of directions to the console, that starts and ends at the initial location. In addition, both distance (in miles) for each direction and total distance of the delivery is included in the directions.

## Data Files
#### Street Map Data File (mapdata.txt)
* Name of street
* Number of segments there are for this street
* Following lines include geocoordinates for these segments

#### Deliveries Data File (delieveries.txt)
* Starting point geocoordinates
* Following lines of delivery points in geocoordinates, followed by the item delievered

#### Note
In main.cpp (line 55), you will have to change the path to mapdata.txt, based on where your file lives. Using your command line, you can get the path by going into the folder that contains mapdata.txt and using the **pwd** command.
