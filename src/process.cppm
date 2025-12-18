module;

#include <expected>
#include <memory>
#include <string>

#include "common.h"

export module pmanage;

export namespace pmng
{
using pid_t = int;
struct PMANAGE_API process
{
    virtual ~process() = default;

    static std::expected<std::unique_ptr<process>, std::string> find_by_pid(pid_t pid);
    static std::expected<std::unique_ptr<process>, std::string> find_by_name(const std::string &name);

    virtual std::expected<void, std::string> kill() = 0;
    virtual pid_t pid() const = 0;
};
} // namespace pmng
