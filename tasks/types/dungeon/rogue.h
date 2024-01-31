#pragma once

#include "dungeon.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>

#include <iostream>

Room* FindFinalRoom(Room* starting_room) {

    std::unordered_set<std::string> collected_keys;
    std::unordered_set<Door*> collected_unopened_doors;
    std::unordered_set<Room*> visited_rooms;
    std::queue<Room*> q;

    q.push(starting_room);
    while (!q.empty()) {
        auto current_room = q.front();

        q.pop();

        if (current_room->IsFinal()) {
            return current_room;
        }
        for (size_t i = 0; i < current_room->NumKeys(); ++i) {
            collected_keys.insert(current_room->GetKey(i));
        }
        for (size_t i = 0; i < current_room->NumDoors(); ++i) {
            if (current_room->GetDoor(i)->IsOpen() &&
                !visited_rooms.contains(current_room->GetDoor(i)->GoThrough())) {
                visited_rooms.insert(current_room->GetDoor(i)->GoThrough());
                q.push(current_room->GetDoor(i)->GoThrough());
            } else {
                collected_unopened_doors.insert(current_room->GetDoor(i));
            }
        }
        for (auto door : collected_unopened_doors) {
            for (auto key : collected_keys) {
                if (door->TryOpen(key)) {
                    if (!visited_rooms.contains(door->GoThrough())) {
                        visited_rooms.insert(door->GoThrough());
                        q.push(door->GoThrough());
                    }
                }
            }
        }
    }
    return nullptr;
}
