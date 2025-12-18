module;

#include <expected>
#include <memory>
#include <string>

#include "common.h"

export module pmanage;

export namespace pmng
{
    using Pid_T = int64_t;

    /*
     * Not using this for ABI consistency across platforms
#if defined(_WIN32)
    using pid_t = uint32_t;
#else
    using pid_t = int;
#endif
    */

struct PMANAGE_API process
{
    virtual ~process() = default;

    [[nodiscard]] static std::expected<std::unique_ptr<process>, std::string> find_by_pid(Pid_T pid) noexcept;
    [[nodiscard]] static std::expected<std::unique_ptr<process>, std::string> find_by_name(const std::string &name) noexcept;
    [[nodiscard]] static Pid_T current_pid() noexcept;

    virtual std::expected<void, std::string> kill() noexcept = 0;
    [[nodiscard]] virtual Pid_T pid() const noexcept = 0;

};

} // namespace pmng
