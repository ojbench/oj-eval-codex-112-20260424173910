// src.hpp - Implementation for Problem 112 (shell 模拟)
// The OJ provides its own main.cpp which includes this header.
// Implement arguments constructor/destructor only; do not add headers or members.

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
        static char* dup_cstr(const string& s) {
            char* p = new char[s.size() + 1];
            std::memcpy(p, s.c_str(), s.size() + 1);
            return p;
        }
        static char** alloc_argv_from_parts(const vector<string>& parts) {
            int n = static_cast<int>(parts.size());
            if (n <= 0) return nullptr;
            char** a = new char*[n];
            for (int i = 0; i < n; ++i) a[i] = dup_cstr(parts[(size_t)i]);
            return a;
        }
        static void free_argv(char** a, int n) {
            if (!a) return;
            for (int i = 0; i < n; ++i) delete[] a[i];
            delete[] a;
        }
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) : _argc(0), _argv(nullptr) {
            vector<string> parts;
            parts.reserve(8);
            const size_t n = cmd.size();
            size_t i = 0;
            while (i < n) {
                while (i < n && cmd[i] == ' ') ++i;
                if (i >= n) break;
                size_t j = i;
                while (j < n && cmd[j] != ' ') ++j;
                parts.emplace_back(cmd.substr(i, j - i));
                i = j;
            }
            _argc = static_cast<int>(parts.size());
            _argv = alloc_argv_from_parts(parts);
        }
        // Deep copy constructor
        arguments(const arguments& other) : _argc(other._argc), _argv(nullptr) {
            if (_argc > 0 && other._argv) {
                _argv = new char*[_argc];
                for (int i = 0; i < _argc; ++i) {
                    _argv[i] = dup_cstr(other._argv[i]);
                }
            }
        }
        // Move constructor
        arguments(arguments&& other) noexcept : _argc(other._argc), _argv(other._argv) {
            other._argc = 0;
            other._argv = nullptr;
        }
        // Copy assignment
        arguments& operator=(const arguments& other) {
            if (this == &other) return *this;
            free_argv(_argv, _argc);
            _argc = other._argc;
            _argv = nullptr;
            if (_argc > 0 && other._argv) {
                _argv = new char*[_argc];
                for (int i = 0; i < _argc; ++i) {
                    _argv[i] = dup_cstr(other._argv[i]);
                }
            }
            return *this;
        }
        // Move assignment
        arguments& operator=(arguments&& other) noexcept {
            if (this == &other) return *this;
            free_argv(_argv, _argc);
            _argc = other._argc;
            _argv = other._argv;
            other._argc = 0;
            other._argv = nullptr;
            return *this;
        }
        ~arguments() {
            free_argv(_argv, _argc);
            _argv = nullptr;
            _argc = 0;
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}

