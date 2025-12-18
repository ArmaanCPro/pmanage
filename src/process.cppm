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

    [[nodiscard]] static std::expected<std::unique_ptr<process>, std::string> find_by_pid(pid_t pid) noexcept;
    [[nodiscard]] static std::expected<std::unique_ptr<process>, std::string> find_by_name(const std::string &name) noexcept;

    virtual std::expected<void, std::string> kill() noexcept = 0;
    [[nodiscard]] virtual pid_t pid() const noexcept = 0;
};

} // namespace pmng
