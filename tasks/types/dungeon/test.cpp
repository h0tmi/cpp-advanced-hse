#include <catch.hpp>
#include <util.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "dungeon.h"

#include <rogue.h>

std::vector<std::unique_ptr<Room>> ReadDungeon(std::istream& in) {
    int num_rooms, num_open_doors, num_closed_doors, num_keys;
    in >> num_rooms;

    std::vector<std::unique_ptr<Room>> dungeon(num_rooms);
    for (size_t i = 0; i < dungeon.size(); ++i) {
        dungeon[i].reset(new Room(i + 1 == dungeon.size()));
    }

    in >> num_keys;
    for (int i = 0; i < num_keys; ++i) {
        int room;
        std::string key;
        in >> room >> key;
        dungeon[room]->AddKey(key);
    }

    in >> num_open_doors;
    for (int i = 0; i < num_open_doors; ++i) {
        int first_room, second_room;
        in >> first_room >> second_room;
        dungeon[first_room]->AddDoor(dungeon[second_room].get());
    }

    in >> num_closed_doors;
    for (int i = 0; i < num_closed_doors; ++i) {
        int first_room, second_room;
        std::string key;
        in >> first_room >> second_room >> key;
        dungeon[first_room]->AddDoorWithKey(dungeon[second_room].get(), key);
    }

    return dungeon;
}

TEST_CASE("SimpleDungeons") {
    // clang-format off
    std::vector<std::string> test_dungeons = {
R"EOF(
1
0
0
0
)EOF",
R"EOF(
2
0
1
0 1
0
)EOF",
R"EOF(
2
1
0 abc
0
1
0 1 abc
)EOF",
R"EOF(
3
1
1 abc
1
0 1
1
0 2 abc
)EOF",
R"EOF(
3
1
1 abc
1
0 1
1
1 2 abc
)EOF",
R"EOF(
2
0
0
0
)EOF",
R"EOF(
2
1
0 abc
0
1
0 1 bcd
)EOF",
R"EOF(
2
1
1 abc
0
1
0 1 abc
)EOF",
R"EOF(
4
0
3
0 1
1 2
2 1
0
)EOF",
    };
    // clang-format on

    std::vector<bool> connected = {true, true, true, true, true, false, false, false, false};
    REQUIRE(connected.size() == test_dungeons.size());

    for (size_t i = 0; i < test_dungeons.size(); ++i) {
        std::stringstream dungeon_stream(test_dungeons[i]);
        auto dungeon = ReadDungeon(dungeon_stream);
        Room* exit_room = FindFinalRoom(dungeon[0].get());

        bool ok = connected[i] ? exit_room == dungeon.back().get() : exit_room == nullptr;
        if (!ok) {
            std::cerr << "Wrong answer. ";
            if (connected[i]) {
                std::cerr << "Expected room " << dungeon.size() << ", ";
            } else {
                std::cerr << "Expected nullptr, ";
            }

            for (size_t i = 0; i < dungeon.size(); ++i) {
                if (dungeon[i].get() == exit_room) {
                    std::cerr << "Got room " << i << std::endl;
                }
            }
            if (exit_room == nullptr) {
                std::cerr << "Got nullptr" << std::endl;
            }

            std::cerr << "Test input:" << test_dungeons[i];

            exit(1);
        }
    }
}

TEST_CASE("GeneratedDungeons") {
    RandomGenerator rnd(7364745);

    const int num_tests = 10;
    const int num_rooms = 100;
    const int num_doors = 200;
    const int num_closed_doors = 100;

    for (int k = 0; k < num_tests; ++k) {
        std::vector<std::unique_ptr<Room>> dungeon(num_rooms);
        for (size_t i = 0; i < dungeon.size(); ++i) {
            dungeon[i].reset(new Room(i + 1 == dungeon.size()));
        }

        for (int i = 0; i < num_doors; ++i) {
            int from = rnd.GenInt(0, num_rooms - 1);
            int to = rnd.GenInt(0, num_rooms - 1);

            dungeon[from]->AddDoor(dungeon[to].get());
        }

        for (int i = 0; i < num_closed_doors; ++i) {
            int from = rnd.GenInt(0, num_rooms - 1);
            int to = rnd.GenInt(0, num_rooms - 1);
            int key_location = rnd.GenInt(0, num_rooms - 1);
            std::string key = std::to_string(i);

            dungeon[key_location]->AddKey(key);
            dungeon[from]->AddDoorWithKey(dungeon[to].get(), key);
        }

        Room* final_room = FindFinalRoom(dungeon[0].get());
        if (final_room && final_room != dungeon.back().get()) {
            std::cerr << "Wrong answer on generated dungeon" << std::endl;
            exit(1);
        }
    }
}
