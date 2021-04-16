//
// Created by per on 4/16/21.
//

#ifndef X1337_HIGHSCOREITEM_H
#define X1337_HIGHSCOREITEM_H

/// <summary>
/// This class keeps a record a highscore item, this is used in the ResourceHandler
/// </summary>
class HighScoreItem
{
public:
    /// Integer which describe the stage this HighScoreItem belongs to
    int stage;

    /// Contains the Playername for current HighScoreItem
    std::string playerName{};

    /// Contains the score achieved for this HighScoreItem
    float score;

    /// Contains the date for when this HighScoreItem was obtained (Written in ResourceHandler)
    std::string date{};

    /// Constructor
    HighScoreItem(int stage, std::string playerName,
                  float score,
                  std::string date) : stage(stage), playerName(playerName), score(score), date(date){};
};
#endif //X1337_HIGHSCOREITEM_H
