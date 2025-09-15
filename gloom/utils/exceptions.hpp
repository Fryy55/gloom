#pragma once

#include <exception>
#include <string>


namespace gloom::except {

class UnhandledEnum final : public std::exception {
public:
	UnhandledEnum(std::string_view message) : m_message(message) {}
	UnhandledEnum(UnhandledEnum const&) = default;
	UnhandledEnum& operator=(UnhandledEnum const&) = default;
	UnhandledEnum(UnhandledEnum&&) noexcept = default;
	UnhandledEnum& operator=(UnhandledEnum&&) noexcept = default;
	~UnhandledEnum() override = default;

	char const* what() const noexcept override { return m_message.c_str(); }

private:
	// Fields
	std::string m_message;
};

class TypeMismatch final : public std::exception {
public:
	TypeMismatch(std::string_view message) : m_message(message) {}
	TypeMismatch(TypeMismatch const&) = default;
	TypeMismatch& operator=(TypeMismatch const&) = default;
	TypeMismatch(TypeMismatch&&) noexcept = default;
	TypeMismatch& operator=(TypeMismatch&&) noexcept = default;
	~TypeMismatch() override = default;

	char const* what() const noexcept override { return m_message.c_str(); }

private:
	// Fields
	std::string m_message;
};

} // namespace gloom::except