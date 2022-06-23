// main.cpp
//
// ICS 46 Spring 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include <iostream>
#include "RoadMapWriter.hpp"
#include "TripReader.hpp"
#include "RoadMapReader.hpp"
#include <stack>

namespace
{
    void print_converted_time(double time) //time is in hours
    {
        int hour = time;
        int min = (time - hour)*60;
        double sec = ((time-hour) - (min/60.0))*3600;
     
        if(hour == 0)
        {
            if(min == 0)
            {
                printf("%.1f secs", sec);
            }
            else
            {
                printf("%d mins %.1f secs", min, sec);
            }
        }
        else
        {
            printf("%d hrs %d mins %.1f secs", hour, min, sec);
        }
        
    }
}

int main()
{
    InputReader reader = InputReader(std::cin);
    RoadMapReader road_reader;
    //RoadMapWriter road_writer;
    RoadMap roadmap = road_reader.readRoadMap(reader);
    //road_writer.writeRoadMap(std::cout, roadmap);
    TripReader trip_reader;
    std::vector<Trip> trip_vec = trip_reader.readTrips(reader);
    if(roadmap.isStronglyConnected())
    {
        std::stack<int> shortest_path;
        for(const auto& t: trip_vec)
        {
            // std::cout << "Start Vertex: " << t.startVertex; 
            // std::cout << " End Vertex: " << t.endVertex; 
            // if(t.metric == TripMetric::Distance)
            //     std::cout << " Trip Metric: Distance" << std::endl; 
            // else
            //     std::cout << " Trip Metric: Time" << std::endl; 
            if(t.metric == TripMetric::Distance)
            {
                double total_dist = 0.0;
                std::map<int, int> dist_map = roadmap.findShortestPaths(t.startVertex, [](RoadSegment r) {return r.miles;});
                std::cout << "Shortest distance from " << roadmap.vertexInfo(t.startVertex) << " to " << roadmap.vertexInfo(t.endVertex) << std::endl;
                std::cout << "  Begin at " << roadmap.vertexInfo(t.startVertex) << std::endl;
                int curr_vertex = dist_map[t.endVertex];
                shortest_path.push(t.endVertex);
                while(roadmap.vertexInfo(curr_vertex) != roadmap.vertexInfo(t.startVertex))
                {
                    shortest_path.push(curr_vertex);
                    curr_vertex = dist_map[curr_vertex];
                }
                
                while(not shortest_path.empty())
                {
                    curr_vertex = shortest_path.top();
                    std::cout << "  Continue to " << roadmap.vertexInfo(curr_vertex);
                    printf(" (%.1f miles)\n", roadmap.edgeInfo(dist_map[curr_vertex], curr_vertex).miles);
                    total_dist+=roadmap.edgeInfo(dist_map[curr_vertex], curr_vertex).miles;
                    shortest_path.pop();
                }
                printf("Total distance: %.1f miles\n", total_dist);
            }
            else if (t.metric == TripMetric::Time)
            {
                double total_time = 0.0;
                std::map<int, int> time_map = roadmap.findShortestPaths(t.startVertex, [](RoadSegment r){return r.miles/r.milesPerHour;});
                std::cout << "Shortest driving time from " << roadmap.vertexInfo(t.startVertex) << " to " << roadmap.vertexInfo(t.endVertex) << std::endl;
                std::cout << "  Begin at " << roadmap.vertexInfo(t.startVertex) << std::endl;
                int curr_vertex = time_map[t.endVertex];
                double time;
                shortest_path.push(t.endVertex);
                while(roadmap.vertexInfo(curr_vertex) != roadmap.vertexInfo(t.startVertex))
                {
                    shortest_path.push(curr_vertex);
                    curr_vertex = time_map[curr_vertex];
                }

                while(not shortest_path.empty())
                {
                    curr_vertex = shortest_path.top();
                    std::cout << "  Continue to " << roadmap.vertexInfo(curr_vertex);
                    RoadSegment curr_road = roadmap.edgeInfo(time_map[curr_vertex], curr_vertex);
                    printf(" (%.1f miles @ %.1fmph = ", curr_road.miles, curr_road.milesPerHour);
                    time = curr_road.miles/curr_road.milesPerHour;
                    total_time+=time;
                    print_converted_time(time); std::cout << ")" << std::endl;
                    shortest_path.pop();
                }

                std::cout << "Total time: ";
                print_converted_time(total_time); std::cout << std::endl;
            }
            std::cout << std::endl;
        }

    }
    else
    {
        std::cout << "Disconnected Map" << std::endl;
    }

    return 0;
}

