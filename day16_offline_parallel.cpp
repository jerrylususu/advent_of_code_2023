#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>


using namespace std;

vector<vector<char>> readPatternFromFile(const string& filePath) {
    vector<vector<char>> pattern;
    ifstream file(filePath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) {
                continue; // Ignore empty lines
            }
            vector<char> row;
            for (char c : line) {
                if (c != '\n') {
                    row.push_back(c);
                }
            }
            pattern.push_back(row);
        }
        file.close();
    }
    return pattern;
}

vector<tuple<int, int, string>> getNextBeams(int beamRow, int beamCol, const string& direction, char c) {
    vector<tuple<int, int, string>> nextBeams;

    if (c == '.') {
        if (direction == "right") {
            nextBeams.push_back(make_tuple(beamRow, beamCol + 1, direction));
        } else if (direction == "left") {
            nextBeams.push_back(make_tuple(beamRow, beamCol - 1, direction));
        } else if (direction == "up") {
            nextBeams.push_back(make_tuple(beamRow - 1, beamCol, direction));
        } else if (direction == "down") {
            nextBeams.push_back(make_tuple(beamRow + 1, beamCol, direction));
        }
    } else if (c == '/') {
        if (direction == "right") {
            nextBeams.push_back(make_tuple(beamRow - 1, beamCol, "up"));
        } else if (direction == "left") {
            nextBeams.push_back(make_tuple(beamRow + 1, beamCol, "down"));
        } else if (direction == "up") {
            nextBeams.push_back(make_tuple(beamRow, beamCol + 1, "right"));
        } else if (direction == "down") {
            nextBeams.push_back(make_tuple(beamRow, beamCol - 1, "left"));
        }
    } else if (c == '\\') {
        if (direction == "right") {
            nextBeams.push_back(make_tuple(beamRow + 1, beamCol, "down"));
        } else if (direction == "left") {
            nextBeams.push_back(make_tuple(beamRow - 1, beamCol, "up"));
        } else if (direction == "up") {
            nextBeams.push_back(make_tuple(beamRow, beamCol - 1, "left"));
        } else if (direction == "down") {
            nextBeams.push_back(make_tuple(beamRow, beamCol + 1, "right"));
        }
    } else if (c == '|') {
        if (direction == "right" || direction == "left") {
            nextBeams.push_back(make_tuple(beamRow + 1, beamCol, "down"));
            nextBeams.push_back(make_tuple(beamRow - 1, beamCol, "up"));
        } else if (direction == "up") {
            nextBeams.push_back(make_tuple(beamRow - 1, beamCol, "up"));
        } else if (direction == "down") {
            nextBeams.push_back(make_tuple(beamRow + 1, beamCol, "down"));
        }
    } else if (c == '-') {
        if (direction == "up" || direction == "down") {
            nextBeams.push_back(make_tuple(beamRow, beamCol + 1, "right"));
            nextBeams.push_back(make_tuple(beamRow, beamCol - 1, "left"));
        } else if (direction == "right") {
            nextBeams.push_back(make_tuple(beamRow, beamCol + 1, "right"));
        } else if (direction == "left") {
            nextBeams.push_back(make_tuple(beamRow, beamCol - 1, "left"));
        }
    }
    return nextBeams;
}

tuple<int, int> getNextPosition(int beamRow, int beamCol, const string& direction) {
    if (direction == "right") {
        return make_tuple(beamRow, beamCol + 1);
    } else if (direction == "left") {
        return make_tuple(beamRow, beamCol - 1);
    } else if (direction == "up") {
        return make_tuple(beamRow - 1, beamCol);
    } else if (direction == "down") {
        return make_tuple(beamRow + 1, beamCol);
    }
}

int countNumberOfNonZero(const vector<vector<int>>& energized) {
    int count = 0;
    for (const auto& row : energized) {
        for (int value : row) {
            if (value != 0) {
                count++;
            }
        }
    }
    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./program <file_path> <edge_index>" << endl;
        return 1;
    }

    string filePath = argv[1];
    int edgeIndex = stoi(argv[2]);

    vector<vector<char>> pattern = readPatternFromFile(filePath);

    cout << "Shape of pattern: " << pattern.size() << " rows x " << pattern[0].size() << " columns" << endl;


    vector<tuple<int, int, string>> edgeStartBeams;
    if (edgeIndex == 0) {
        for (int idx = 0; idx < pattern[0].size(); idx++) {
            edgeStartBeams.push_back(make_tuple(0, idx, "down"));
        }
    } else if (edgeIndex == 1) {
        for (int idx = 0; idx < pattern.back().size(); idx++) {
            edgeStartBeams.push_back(make_tuple(pattern.size() - 1, idx, "up"));
        }
    } else if (edgeIndex == 2) {
        for (int idx = 0; idx < pattern.size(); idx++) {
            edgeStartBeams.push_back(make_tuple(idx, 0, "right"));
        }
    } else if (edgeIndex == 3) {
        for (int idx = 0; idx < pattern.size(); idx++) {
            edgeStartBeams.push_back(make_tuple(idx, pattern[0].size() - 1, "left"));
        }
    }

    vector<int> edgeEnergized;

    for (int outerIdx = 0; outerIdx < edgeStartBeams.size(); outerIdx++) {
        cout << "calculating edge: " << outerIdx << " " << get<0>(edgeStartBeams[outerIdx]) << " " << get<1>(edgeStartBeams[outerIdx]) << endl;
        vector<vector<int>> energized(pattern.size(), vector<int>(pattern[0].size(), 0));

        vector<tuple<int, int, string>> beams = {edgeStartBeams[outerIdx]};
        vector<tuple<int, int, string>> tracedBeams;
        int simStep = 0;
        int currentEnergized = 0;

        while (!beams.empty() && simStep <= 100000) {
            simStep++;
            int lastEnergized = currentEnergized;
            tuple<int, int, string> currentBeam = beams.front();
            beams.erase(beams.begin());
            int beamRow = get<0>(currentBeam);
            int beamCol = get<1>(currentBeam);
            string direction = get<2>(currentBeam);

            if (simStep % 1000 == 0) {
                cout << "step: " << simStep << " beam: " << beamRow << " " << beamCol << " direction: " << direction << " len beam: " << beams.size() << endl;
            }

            if (beamRow < 0 || beamRow >= pattern.size() || beamCol < 0 || beamCol >= pattern[beamRow].size()) {
                continue;
            }

            energized[beamRow][beamCol]++;

            char nextChar = pattern[beamRow][beamCol];
            vector<tuple<int, int, string>> nextBeams = getNextBeams(beamRow, beamCol, direction, nextChar);

            for (const auto& nextBeam : nextBeams) {
                if (find(tracedBeams.begin(), tracedBeams.end(), nextBeam) != tracedBeams.end()) {
                    continue;
                } else {
                    tracedBeams.push_back(nextBeam);
                    beams.push_back(nextBeam);
                }
            }

            currentEnergized = countNumberOfNonZero(energized);
        }

        edgeEnergized.push_back(currentEnergized);
        cout << "calculating edge: " << outerIdx << " " << get<0>(edgeStartBeams[outerIdx]) << " " << get<1>(edgeStartBeams[outerIdx]) << " done, energized=" << currentEnergized << endl;
    }

    int maxEnergized = *max_element(edgeEnergized.begin(), edgeEnergized.end());
    cout << "edge energized: ";
    for (int energized : edgeEnergized) {
        cout << energized << " ";
    }
    cout << endl;
    cout << "max: " << maxEnergized << endl;

    return 0;
}
