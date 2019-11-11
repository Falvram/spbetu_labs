#include <exception>
#include <string>

class ExpException : std::exception
{
public:
    ExpException(std::string &&whatStr) noexcept : whatStr(std::move(whatStr)) { }
    ExpException(const std::string &whatStr) noexcept : whatStr(whatStr) { }
    ~ExpException() noexcept = default;

    const char* what() const noexcept override;

private:
    std::string whatStr;
};

const char* what();
