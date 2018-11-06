#ifndef KKTEST_RUNNER_OS_PATH_H_
#define KKTEST_RUNNER_OS_PATH_H_

#include <string>
#include <vector>

#include <utils/comparable.hpp>
#include "error.hpp"


namespace kktest_runner {

class Path: public Comparable<std::string> {
public:
    static const char  SEP;
    static const char* SELF_FOLDER;
    static const char* PARENT_FOLDER;

    static Path parent(const Path& path);

    static Path join(const std::string& a, const std::string& b);
    static Path join(const Path& a, const std::string& b);
    static Path join(const std::string& a, const Path& b);
    static Path join(const Path& a, const Path& b);

    template<typename... Args>
    static Path join(const std::string& a,
                     const std::string& b,
                     const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const Path& a, const std::string& b, const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const std::string& a, const Path& b, const Args... args) {
        return join(join(a, b), args...);
    }

    template<typename... Args>
    static Path join(const Path& a, const Path& b, const Args... args) {
        return join(join(a, b), args...);
    }

    Path(const std::string& path);

    Path(const Path& other);

    Path(Path&& other);

    template<typename... Args>
    Path(const Path& a, const Path& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const Path& a, const std::string& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const std::string& a, const Path& b, const Args... args):
            Path(join(a, b, args...)) {}

    template<typename... Args>
    Path(const std::string& a, const std::string& b, const Args... args):
            Path(join(a, b, args...)) {}

    Path& operator=(const Path& other);
    Path& operator=(Path&& other);

    bool operator==(const Path& other) const;

    bool isRelative() const;
    bool isAbsolute() const;

    Path absolute() const;

    std::string last() const;
    std::string toString() const;
    std::vector<std::string> getParts() const;

    Path& operator+=(const Path& other);
    Path& operator+=(const std::string& other);
    Path operator+(const Path& other) const;
    Path operator+(const std::string& other) const;

    Path parent() const;
    Path& operator--();
    Path operator--(int);

private:
    bool applySpecialPart(const std::string& part);
    void addPart(const std::string& part);
    void addPart(std::string&& part);

    std::string key() const override;

    std::vector<std::string> parts;
    bool relative;

};

}

#endif