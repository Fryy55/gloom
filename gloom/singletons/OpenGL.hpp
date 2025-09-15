#pragma once

#include "exceptions.hpp"

#include <array>
#include <variant>
#include <optional>
#include <format>

	#include <GLFW/glfw3.h>
	template <typename T>
	concept AutoOrValueHint = requires(T a) {
		{ a.value() };
		{ a.has_value() } -> std::same_as<bool>;
	};
namespace gloom {

class OpenGL final {
public:
	static OpenGL* get() noexcept;

	OpenGL(OpenGL const&) = delete;
	OpenGL& operator=(OpenGL const&) = delete;
	OpenGL(OpenGL&&) = delete;
	OpenGL& operator=(OpenGL&&) = delete;

private:
	OpenGL() noexcept = default;
	~OpenGL() = default;

public:
	// Init/Termination
	bool init() const noexcept;
	void terminate() const noexcept;

	// Version
	struct Version {
		int major;
		int minor;
		int patch;
	};
	Version getGLFWVersion() const noexcept;

	// Hints
	enum Hint {
		// Init hints
		Init_ExposeJoystickHatButtons,
		MACOS_Init_SetContentsResourcesDir,
		MACOS_Init_CreateMenuBar,
		WAYLAND_Init_PreferLibDecor,

		// Window hints
		Window_ResizableByUser,
		Window_InitiallyVisible,
		Window_Decorated,
		Window_InitiallyFocused,
		Window_MinimizeOnFocusLoss,
		Window_Topmost,
		Window_InitiallyMaximized,
		Window_InitiallyCenterCursor,
		Window_TransparentFramebuffer,
		Window_FocusOnShow,
		Window_ScaleToMonitor,
		MACOS_Window_UseRetinaFramebuffer,
		MACOS_Window_FrameName,
		MACOS_Window_GraphicsSwitching,
		X11_Window_ClassName,
		X11_Window_InstanceName,

		// Framebuffer hints
		Framebuffer_RedBitDepth,
		Framebuffer_GreenBitDepth,
		Framebuffer_BlueBitDepth,
		Framebuffer_AlphaBitDepth,
		Framebuffer_BitDepth,
		Framebuffer_StencilBitDepth,
		Framebuffer_AccumBuffer_RedBitDepth,
		Framebuffer_AccumBuffer_GreenBitDepth,
		Framebuffer_AccumBuffer_BlueBitDepth,
		Framebuffer_AccumBuffer_AlphaBitDepth,
		Framebuffer_AuxBuffers,
		Framebuffer_Samples,
		Framebuffer_StereoscopicRendering,
		Framebuffer_IsSRGBCapable,
		Framebuffer_UseDoubleBuffer,

		// Monitor hints
		Monitor_RefreshRate,

		// Context hints
		Context_ClientAPI,
		Context_ContextCreationAPI,
		Context_MajorVersion,
		Context_MinorVersion,
		Context_IsForwardCompatible,
		Context_Debug,
		Context_Profile,
		Context_Robustness,
		Context_ReleaseBehavior,
		Context_NoError,

		COUNT
	};
	enum class ClientAPI {
		None,
		OpenGL,
		OpenGLES
	};
	enum class ContextCreationAPI {
		Native,
		EGL,
		OSMesa
	};
	enum class Robustness {
		None,
		NoResetNotification,
		LoseContextOnReset
	};
	enum class ReleaseBehavior {
		None,
		Flush
	};
	enum class Profile {
		Compatibility,
		Core
	};

	static constexpr auto Auto = std::nullopt;
	template <typename T>
	using AutoOrValue = std::optional<T>;

private:
	static constexpr std::array boolHints{
		Init_ExposeJoystickHatButtons,
		MACOS_Init_SetContentsResourcesDir,
		MACOS_Init_CreateMenuBar,
		WAYLAND_Init_PreferLibDecor,
		Window_ResizableByUser,
		Window_InitiallyVisible,
		Window_Decorated,
		Window_InitiallyFocused,
		Window_MinimizeOnFocusLoss,
		Window_Topmost,
		Window_InitiallyMaximized,
		Window_InitiallyCenterCursor,
		Window_TransparentFramebuffer,
		Window_FocusOnShow,
		Window_ScaleToMonitor,
		MACOS_Window_UseRetinaFramebuffer,
		MACOS_Window_GraphicsSwitching,
		Framebuffer_StereoscopicRendering,
		Framebuffer_IsSRGBCapable,
		Framebuffer_UseDoubleBuffer,
		Context_IsForwardCompatible,
		Context_Debug,
		Context_NoError
	};
	static constexpr std::array autoUIntHints{
		Framebuffer_RedBitDepth,
		Framebuffer_GreenBitDepth,
		Framebuffer_BlueBitDepth,
		Framebuffer_AlphaBitDepth,
		Framebuffer_BitDepth,
		Framebuffer_StencilBitDepth,
		Framebuffer_AccumBuffer_RedBitDepth,
		Framebuffer_AccumBuffer_GreenBitDepth,
		Framebuffer_AccumBuffer_BlueBitDepth,
		Framebuffer_AccumBuffer_AlphaBitDepth,
		Framebuffer_AuxBuffers,
		Framebuffer_Samples,
		Monitor_RefreshRate
	};
	static constexpr std::array stringHints{
		MACOS_Window_FrameName,
		X11_Window_ClassName,
		X11_Window_InstanceName
	};
	static constexpr std::array initHints{
		Init_ExposeJoystickHatButtons,
		MACOS_Init_SetContentsResourcesDir,
		MACOS_Init_CreateMenuBar,
		WAYLAND_Init_PreferLibDecor
	};

	template <Hint hint>
	static consteval auto getHintType() {
			if constexpr (
				std::ranges::find(boolHints, hint) != boolHints.end()
			) return std::type_identity<bool>{};
			else if constexpr (
				std::ranges::find(autoUIntHints, hint) != autoUIntHints.end()
			) return std::type_identity<AutoOrValue<std::uint32_t>>{};
			else if constexpr (
				std::ranges::find(stringHints, hint) != stringHints.end()
			) return std::type_identity<std::string_view>{};
			else if constexpr (hint == Context_ClientAPI)
				return std::type_identity<ClientAPI>{};
			else if constexpr (hint == Context_ContextCreationAPI)
				return std::type_identity<ContextCreationAPI>{};
			else if constexpr (hint == Context_Robustness)
				return std::type_identity<Robustness>{};
			else if constexpr (hint == Context_ReleaseBehavior)
				return std::type_identity<AutoOrValue<ReleaseBehavior>>{};
			else if constexpr (hint == Context_Profile)
				return std::type_identity<AutoOrValue<Profile>>{};
			else
				static_assert(false, "Unhandled enum in getHintType");
		}
		static consteval int enumForHint(Hint hint) {
		switch (hint) {
			// Init hints
			case Init_ExposeJoystickHatButtons:
				return GLFW_JOYSTICK_HAT_BUTTONS;

			case MACOS_Init_SetContentsResourcesDir:
				return GLFW_COCOA_CHDIR_RESOURCES;

			case MACOS_Init_CreateMenuBar:
				return GLFW_COCOA_MENUBAR;

			case WAYLAND_Init_PreferLibDecor:
				return GLFW_WAYLAND_LIBDECOR;

			// Window hints
			case Window_ResizableByUser:
				return GLFW_RESIZABLE;

			case Window_InitiallyVisible:
				return GLFW_VISIBLE;

			case Window_Decorated:
				return GLFW_DECORATED;

			case Window_InitiallyFocused:
				return GLFW_FOCUSED;

			case Window_MinimizeOnFocusLoss:
				return GLFW_AUTO_ICONIFY;

			case Window_Topmost:
				return GLFW_FLOATING;

			case Window_InitiallyMaximized:
				return GLFW_MAXIMIZED;

			case Window_InitiallyCenterCursor:
				return GLFW_CENTER_CURSOR;

			case Window_TransparentFramebuffer:
				return GLFW_TRANSPARENT_FRAMEBUFFER;

			case Window_FocusOnShow:
				return GLFW_FOCUS_ON_SHOW;

			case Window_ScaleToMonitor:
				return GLFW_SCALE_TO_MONITOR;

			case MACOS_Window_UseRetinaFramebuffer:
				return GLFW_COCOA_RETINA_FRAMEBUFFER;

			case MACOS_Window_FrameName:
				return GLFW_COCOA_FRAME_NAME;

			case X11_Window_ClassName:
				return GLFW_X11_CLASS_NAME;

			case X11_Window_InstanceName:
				return GLFW_X11_INSTANCE_NAME;

			// Framebuffer hints
			case Framebuffer_RedBitDepth:
				return GLFW_RED_BITS;

			case Framebuffer_GreenBitDepth:
				return GLFW_GREEN_BITS;

			case Framebuffer_BlueBitDepth:
				return GLFW_BLUE_BITS;

			case Framebuffer_AlphaBitDepth:
				return GLFW_ALPHA_BITS;

			case Framebuffer_BitDepth:
				return GLFW_DEPTH_BITS;

			case Framebuffer_StencilBitDepth:
				return GLFW_STENCIL_BITS;

			case Framebuffer_AccumBuffer_RedBitDepth:
				return GLFW_ACCUM_RED_BITS;

			case Framebuffer_AccumBuffer_GreenBitDepth:
				return GLFW_ACCUM_GREEN_BITS;

			case Framebuffer_AccumBuffer_BlueBitDepth:
				return GLFW_ACCUM_BLUE_BITS;

			case Framebuffer_AccumBuffer_AlphaBitDepth:
				return GLFW_ACCUM_ALPHA_BITS;

			case Framebuffer_AuxBuffers:
				return GLFW_AUX_BUFFERS;

			case Framebuffer_Samples:
				return GLFW_SAMPLES;

			case Framebuffer_StereoscopicRendering:
				return GLFW_STEREO;

			case Framebuffer_IsSRGBCapable:
				return GLFW_SRGB_CAPABLE;

			case Framebuffer_UseDoubleBuffer:
				return GLFW_DOUBLEBUFFER;

			// Monitor hints
			case Monitor_RefreshRate:
				return GLFW_REFRESH_RATE;

			// Context hints
			case Context_ClientAPI:
				return GLFW_CLIENT_API;

			case Context_ContextCreationAPI:
				return GLFW_CONTEXT_CREATION_API;

			case Context_MajorVersion:
				return GLFW_CONTEXT_VERSION_MAJOR;

			case Context_MinorVersion:
				return GLFW_CONTEXT_VERSION_MINOR;

			case Context_IsForwardCompatible:
				return GLFW_OPENGL_FORWARD_COMPAT;

			case Context_Debug:
				return GLFW_CONTEXT_DEBUG;

			case Context_Profile:
				return GLFW_OPENGL_PROFILE;

			case Context_Robustness:
				return GLFW_CONTEXT_ROBUSTNESS;

			case Context_ReleaseBehavior:
				return GLFW_CONTEXT_RELEASE_BEHAVIOR;

			case Context_NoError:
				return GLFW_CONTEXT_NO_ERROR;

			default:
				throw except::UnhandledEnum("Unhandled enum in enumForHint");
		}
	}
	template <typename T>
	static consteval int enumForValue(T value) {
		if constexpr (std::is_same_v<T, ClientAPI>)
			switch (value) {
				case ClientAPI::None:
					return GLFW_NO_API;

				case ClientAPI::OpenGL:
					return GLFW_OPENGL_API;

				case ClientAPI::OpenGLES:
					return GLFW_OPENGL_ES_API;

				default:
					throw except::UnhandledEnum("Unhandled enum in enumForValue (ClientAPI branch)");
			}
		else if constexpr (std::is_same_v<T, ContextCreationAPI>)
			switch (value) {
				case ContextCreationAPI::Native:
					return GLFW_NATIVE_CONTEXT_API;

				case ContextCreationAPI::EGL:
					return GLFW_EGL_CONTEXT_API;

				case ContextCreationAPI::OSMesa:
					return GLFW_OSMESA_CONTEXT_API;
			
				default:
					throw except::UnhandledEnum("Unhandled enum in enumForValue (ContextCreationAPI branch)");
			}
		else if constexpr (std::is_same_v<T, Robustness>)
			switch (value) {
				case Robustness::None:
					return GLFW_NO_ROBUSTNESS;

				case Robustness::NoResetNotification:
					return GLFW_NO_RESET_NOTIFICATION;

				case Robustness::LoseContextOnReset:
					return GLFW_LOSE_CONTEXT_ON_RESET;
			
				default:
					throw except::UnhandledEnum("Unhandled enum in enumForValue (Robustness branch)");
			}
		else if constexpr (std::is_same_v<T, ReleaseBehavior>)
			switch (value) {
				case ReleaseBehavior::None:
					return GLFW_RELEASE_BEHAVIOR_NONE;

				case ReleaseBehavior::Flush:
					return GLFW_RELEASE_BEHAVIOR_FLUSH;
			
				default:
					throw except::UnhandledEnum("Unhandled enum in enumForValue (ReleaseBehavior branch)");
			}
		else if constexpr (std::is_same_v<T, Profile>)
			switch (value) {
				case Profile::Compatibility:
					return GLFW_OPENGL_COMPAT_PROFILE;

				case Profile::Core:
					return GLFW_OPENGL_CORE_PROFILE;

				default:
					throw except::UnhandledEnum("Unhandled enum in enumForValue (Profile branch)");
			}
		else
			throw except::UnhandledEnum("Unhandled enum in enumForValue");
	}

public:
	using Value = std::variant<
		bool,
		AutoOrValue<std::uint32_t>,
		std::string_view,
		ClientAPI,
		ContextCreationAPI,
		Robustness,
		AutoOrValue<ReleaseBehavior>,
		AutoOrValue<Profile>
	>;

	template <Hint hint, Value value>
	void setHint() const {
		using ValueType = decltype(getHintType<hint>())::type;

		if (!std::holds_alternative<ValueType>(value))
			throw except::TypeMismatch(
				std::format(
					"Mismatching type: expected '{}', got '{}'\n",
					typeid(ValueType).name(),
					std::visit([](auto&& x) { return typeid(x).name(); }, value)
				).c_str()
			);

		auto castedValue = std::get<ValueType>(value);
		constexpr int hintEnum = enumForHint(hint);

		if constexpr (
			std::ranges::find(initHints, hint) != initHints.end()
		) {
			if constexpr (hint == WAYLAND_Init_PreferLibDecor)
				glfwInitHint(
					hintEnum,
					castedValue ?
						GLFW_WAYLAND_PREFER_LIBDECOR
						:
						GLFW_WAYLAND_DISABLE_LIBDECOR
				);
			else
				glfwInitHint(hintEnum, castedValue);
		} else { // window hint
			if constexpr (AutoOrValueHint<ValueType>) {
				if (castedValue == Auto) {
					switch (hint) {
					case Context_ReleaseBehavior:
						glfwWindowHint(hintEnum, GLFW_ANY_RELEASE_BEHAVIOR);
						break;

					case Context_Profile:
						glfwWindowHint(hintEnum, GLFW_OPENGL_ANY_PROFILE);
						break;

					default:
						glfwWindowHint(hintEnum, GLFW_DONT_CARE);
						break;
					}
				} else { // not Auto/std::nullopt
					if constexpr (std::is_same_v<decltype(castedValue.value()), std::uint32_t>)
						glfwWindowHint(hintEnum, castedValue.value());
					else // enums
						glfwWindowHint(hintEnum, enumForValue(castedValue.value()));
				}
			} else if constexpr (std::is_same_v<ValueType, std::string>) {
				glfwWindowHintString(hintEnum, castedValue.c_str());
			} else if constexpr (
				std::ranges::find(boolHints, hint) != boolHints.end()
			) {
				glfwWindowHint(hintEnum, castedValue);
			} else { // non optional enums
				glfwWindowHint(hintEnum, enumForValue(hint, castedValue));
			}
		}

		return;
	}

	void resetWindowHints() const noexcept;

private:
	// Fields
	
};

} // namespace gloom