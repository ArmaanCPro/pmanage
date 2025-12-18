module;

#ifndef _WIN32
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include <expected>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#endif

module pmanage;

namespace pmng
{
struct posix_process : public process
{
    posix_process(pid_t pid) : m_Pid(pid)
    {
    }

    [[nodiscard]] virtual std::expected<void, std::string> kill() noexcept override
    {
        if (::kill(m_Pid, SIGTERM) == -1)
        {
            if (errno == EPERM)
            {
                return std::unexpected{"Permission denied. Cannot kill this process."};
            }
            else if (errno == ESRCH)
            {
                return std::unexpected{std::format("Process not found. This process has an invalid PID: {}\n", m_Pid)};
            }

            return std::unexpected{
                std::format("Error sending SIGTERM to PID {}, error: {}", m_Pid, std::strerror(errno))};
        }

        return {};
    }

    [[nodiscard]] virtual pid_t pid() const noexcept override
    {
        return m_Pid;
    }

  private:
    pid_t m_Pid;
};

namespace
{
[[nodiscard]] std::expected<pid_t, std::string> get_pid_by_name(const char *process_name) noexcept
{
    char buf[512];
    // Use pidof to find the PID. The -s flag ensures only one PID is returned if multiple match.
    // Replace 'pidof -s' with 'pgrep -o' to use pgrep and get the oldest process's PID.
    char command[256];
    snprintf(command, sizeof(command), "pidof -s %s", process_name);

    FILE *cmd_pipe = popen(command, "r");
    if (!cmd_pipe)
    {
        return std::unexpected{"popen failed"};
    }

    // Read the output (PID string)
    if (fgets(buf, sizeof(buf), cmd_pipe) != NULL)
    {
        pid_t pid = static_cast<pid_t>(strtoul(buf, NULL, 10));
        pclose(cmd_pipe);
        return pid;
    }

    pclose(cmd_pipe);
    return std::unexpected{"Process not found or error."};
}
} // namespace

[[nodiscard]] std::expected<std::unique_ptr<process>, std::string> process::find_by_pid(pid_t pid) noexcept
{
    return std::make_unique<posix_process>(pid);
}

[[nodiscard]] std::expected<std::unique_ptr<process>, std::string> process::find_by_name(const std::string &name) noexcept
{
    auto found_pid = get_pid_by_name(name.c_str());

    if (found_pid.has_value())
        return std::make_unique<posix_process>(found_pid.value());
    return std::unexpected{found_pid.error()};
}

} // namespace pmng

