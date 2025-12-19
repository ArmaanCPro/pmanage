module;

#if defined(_WIN32)

#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#else
#if defined(_MSC_VER)
#pragma message("Compiling process_win32.cpp not allowed on non-windows platforms is not allowed!")
#else
#warning "Compiling process_win32.cpp not allowed on non-windows platforms is not allowed!"
#endif

#endif

#include <expected>
#include <memory>
#include <string>

module pmanage;

namespace pmng
{

struct win32_process : public process
{
    win32_process(DWORD pid) : m_Pid(pid)
    {
    }

    [[nodiscard]] virtual std::expected<void, std::string> kill() noexcept override
    {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, m_Pid);

        if (!hProcess)
        {
            return std::unexpected{ "Could not open process." };
        }

        BOOL result = TerminateProcess(hProcess, 0);

        CloseHandle(hProcess);

        if (!result)
        {
            return std::unexpected{ "Could not terminate process." };
        }

        return {};
    }

    [[nodiscard]] virtual Pid_T pid() const noexcept override
    {
        return static_cast<Pid_T>(m_Pid);
    }

private:
   DWORD m_Pid;
};

namespace
{
    DWORD get_parent_pid(DWORD current_pid)
    {
        DWORD ppid = 0;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hSnapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(hSnapshot, &pe))
            {
                do
                {
                    if (pe.th32ProcessID == current_pid)
                    {
                        ppid = pe.th32ParentProcessID;
                        break;
                    }
                } while (Process32Next(hSnapshot, &pe));
            }
            CloseHandle(hSnapshot);
        }

        return ppid;
    }
} // namespace

[[nodiscard]] std::expected<std::unique_ptr<process>, std::string> process::find_by_name(std::string name) noexcept
{
    if (!name.ends_with(".exe"))
    {
        name += ".exe";
    }

    DWORD pid = 0;
    // takes a snapshot of all processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &pe))
        {
            do
            {
                if (_tcsicmp(pe.szExeFile, name.c_str()) == 0)
                {
                    pid = pe.th32ProcessID;
                    break;
                }
            }
            while (Process32Next(hSnapshot, &pe));
        }

        CloseHandle(hSnapshot);
    }

    return std::make_unique<win32_process>(pid);
}

[[nodiscard]] std::expected<std::unique_ptr<process>, std::string> process::find_by_pid(Pid_T pid) noexcept
{
    return std::make_unique<win32_process>(static_cast<DWORD>(pid));
}

[[nodiscard]] Pid_T process::current_pid() noexcept
{
    return static_cast<Pid_T>(GetCurrentProcessId());
}

[[nodiscard]] Pid_T process::parent_pid() noexcept
{
    return static_cast<Pid_T>(get_parent_pid(GetCurrentProcessId()));
}

} // namespace pmng
