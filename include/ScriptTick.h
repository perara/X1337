//
// Created by per on 4/19/21.
//

#ifndef X1337_SCRIPTTICK_H
#define X1337_SCRIPTTICK_H

class ScriptTick
{
public:
    int delay;
    int repeat;
    int type;
    std::queue<VectorN> pathQueue;
    std::list<std::pair<int, std::string>> emoteQueue;

    ScriptTick(
            int delay,
            std::queue<VectorN> pathQueue,
            std::list<std::pair<int, std::string>> emoteQueue,
            int type,
            int repeat):
            delay(delay),
            repeat(repeat),
            type(type),
            pathQueue(pathQueue),
            emoteQueue(emoteQueue)
    {};
};

#endif //X1337_SCRIPTTICK_H
