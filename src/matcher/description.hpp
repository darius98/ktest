#ifndef RUNTIME_TESTING_MATCHER_DESCRIPTION_H_
#define RUNTIME_TESTING_MATCHER_DESCRIPTION_H_

#include <sstream>
#include <string>

#include "src/utils/streamer.hpp"


namespace matcher {

class Description {
public:
    static Description* createForExpectation(
            const char* fileName,
            const int& lineNumber,
            const char* extra);

    template<class T>
    Description* append(T obj) {
        runtime_testing::utils::Streamer<T>::send(this->stream, obj);
        return this;
    }

    template<class T, class... Args>
    Description* append(T obj, const Args... args) {
        this->append(obj);
        this->append(args...);
        return this;
    }

    std::string toString() const;
private:
    std::stringstream stream;
};

}

#endif
