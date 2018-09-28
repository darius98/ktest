#include "description.hpp"

using namespace std;

namespace matcher {

Description* Description::createForExpect(
        const string& file,
        int line,
        const string &extra) {
    auto description = new Description();
    description->append(string(file) + ":" + to_string(line) + ": ");
    description->append(extra);
    if (extra.empty()) {
        description->append("Expected ");
    }
    return description;
}

string Description::toString() const {
    return stream.str();
}

}