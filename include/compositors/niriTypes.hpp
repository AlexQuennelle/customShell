#pragma once

#ifndef __clangd__
#include <meta>
#endif

#include <array>
#include <cstdint>
#include <glaze/forward.hpp>
#include <glaze/glaze.hpp>
#include <glaze/json/flatten_map.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <variant>

template <typename... Ts> struct Overload : Ts...
{
	using Ts::operator()...;
};
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;

struct glzOpts : glz::opts
{
	bool reflect_enums = true;
	bool skip_null_members = false;
};

namespace niri
{

struct FocusTimestamp
{
	uint64_t secs;
	uint64_t nanos;
};
struct LayoutData
{
	std::optional<std::array<int64_t, 2>> pos_in_scrolling_layout;
	std::array<double, 2> tile_size{};
	std::array<int32_t, 2> window_size{};
	std::optional<std::array<double, 2>> tile_pos_in_workspace_view;
	std::array<double, 2> window_offset_in_tile{};
};
struct LayoutChange
{
	uint64_t id{};
	LayoutData data{};
};
struct WindowData
{
	uint64_t id{};
	std::optional<std::string> title;
	std::optional<std::string> app_id;
	std::optional<int64_t> pid;
	std::optional<uint64_t> workspace_id;
	bool is_focused{};
	bool is_floating{};
	bool is_urgent{};
	LayoutData layout;
	std::optional<FocusTimestamp> focus_timestamp;
};
struct WindowID
{
	uint64_t id;
};

struct WorkspaceData
{
	uint64_t id{};
	uint8_t idx{};
	std::optional<std::string> name;
	std::optional<std::string> output;
	bool is_urgent{};
	bool is_active{};
	bool is_focused{};
	std::optional<uint64_t> active_window_id;
};

struct OutputMode
{
	uint16_t width;
	uint16_t height;
	uint32_t refresh_rate;
	bool is_preferred;
};
enum class OutputTransform : uint8_t
{
	Normal,
	_90,
	_180,
	_270,
	Flipped,
	Flipped90,
	Flipped180,
	Flipped270,
};
struct LogicalOutput
{
	int32_t x;
	int32_t y;
	uint32_t width;
	uint32_t height;
	double scale;
	OutputTransform transform;
};
struct OutputData
{
	std::string name;
	std::string make;
	std::string model;
	std::optional<std::string> serial;
	std::optional<std::array<uint32_t, 2>> physical_size;
	std::vector<OutputMode> modes;
	std::optional<uint64_t> currentMode;
	bool is_custom_mode;
	bool vrr_supported;
	bool vrr_enabled;
	std::optional<LogicalOutput> logical;
	std::optional<uint8_t> max_bpc;
};
struct OutputID
{
	std::string name;
};

enum class Layer : uint8_t
{
	Background,
	Bottom,
	Top,
	Overlay,
};
enum class LayerSurfaceKeyboardInteractivity : uint8_t
{
	None,
	Exclusive,
	OnDemand,
};
struct LayerSurface
{
	std::string _namespace;
	std::string output;
	Layer layer;
	LayerSurfaceKeyboardInteractivity keyboard_interactivity;
};

struct KeyboardLayouts_t
{
	std::vector<std::string> names;
	uint8_t current_idx;
};

struct PickedColor_t
{
	std::array<double, 3> rbg;
};

enum class OutputConfigChanged_t : uint8_t
{
	Applied,
	OutputWasMissing,
};

struct OverviewData
{
	bool is_open;
};

enum class CastKind : uint8_t
{
	PipeWire,
	WlrScreencopy,
};
enum class EmptyCastTarget : uint8_t
{
	Nothing
};
using CastTargetType = std::variant<EmptyCastTarget, OutputID, WindowID>;
struct CastTarget
{ // TODO: fix outputID and windowID objects
	CastTargetType data;
};
struct CastData
{
	uint64_t stream_id{};
	uint64_t session_id{};
	CastKind kind{};
	CastTarget target{};
	bool is_dynamic_target{};
	bool is_active{};
	std::optional<int32_t> pid{std::nullopt};
	std::optional<uint32_t> pw_node_id{std::nullopt};
};

struct ErrResponse
{
	std::string Err;
};

struct VersionResponse
{
	std::string Version;
};
struct OutputsResponse
{
	std::unordered_map<std::string, OutputData> Outputs;
};
struct WorkspacesResponse
{
	std::vector<WorkspaceData> Workspaces;
};
struct WindowsResponse
{
	std::vector<WindowData> Windows;
};
struct LayersResponse
{
	std::vector<LayerSurface> Layers;
};
struct KeyboardLayoutsResponse
{
	KeyboardLayouts_t KeyboardLayouts;
};
struct FocusedOutputResponse
{
	std::optional<OutputData> FocusedOutput;
};
struct FocusedWindowResponse
{
	std::optional<WindowData> FocusedWindow;
};
struct PickedWindowResponse
{
	std::optional<WindowData> PickedWindow;
};
struct PickedColorResponse
{
	std::optional<PickedColor_t> PickedColor;
};
struct OutputConfigChangedResponse
{
	OutputConfigChanged_t OutputConfigChanged;
};
struct OverviewStateResponse
{
	OverviewData OverviewState;
};
struct CastsResponse
{
	std::vector<CastData> Casts;
};

using ResponsePayload
	= std::variant<std::string, VersionResponse, OutputsResponse,
				   WindowsResponse, WindowsResponse, LayersResponse,
				   KeyboardLayoutsResponse, FocusedOutputResponse,
				   FocusedWindowResponse, PickedWindowResponse,
				   PickedColorResponse, OutputConfigChangedResponse,
				   OverviewStateResponse, CastsResponse>;

struct OkResponse
{
	ResponsePayload Ok;
};

struct Response
{
	std::variant<OkResponse, ErrResponse> data;
};

struct WorkspacesChangedEvent
{
	std::vector<WorkspaceData> workspaces;
};
struct WorkspaceUrgencyChangedEvent
{
	uint64_t id;
	bool urgent;
};
struct WorkspaceActivatedEvent
{
	uint64_t id;
	bool focused;
};
struct WorkspaceActiveWindowChangedEvent
{
	uint64_t workspace_id{};
	std::optional<uint64_t> active_window_id;
};
struct WindowsChangedEvent
{
	std::vector<WindowData> windows;
};
struct WindowOpenedOrChangedEvent
{
	WindowData window;
};
struct WindowClosedEvent
{
	uint64_t id;
};
struct WindowFocusChangedEvent
{
	std::optional<uint64_t> id;
};
struct WindowFocusTimestampChangedEvent
{
	uint64_t id{};
	std::optional<FocusTimestamp> focus_timestamp;
};
struct WindowUrgencyChangedEvent
{
	uint64_t id;
	bool urgent;
};
struct WindowLayoutsChangedEvent
{ // TODO: niri passes as mixed array, not map
	std::vector<LayoutChange> changes;
};
struct KeyboardLayoutsChangedEvent
{
	KeyboardLayouts_t keyboard_layouts;
};
struct KeyboardLayoutSwitchedEvent
{
	uint8_t idx;
};
struct OverviewOpenedOrClosedEvent
{
	bool is_open;
};
struct ConfigLoadedEvent
{
	bool failed;
};
struct ScreenshotCapturedEvent
{
	std::optional<std::string> path;
};
struct CastsChangedEvent
{
	std::vector<CastData> casts;
};
struct CastStartedOrChangedEvent
{
	CastData cast;
};
struct CastStoppedEvent
{
	uint64_t stream_id;
};

using EventType = std::variant<
	WorkspacesChangedEvent, WorkspaceUrgencyChangedEvent,
	WorkspaceActivatedEvent, WorkspaceActiveWindowChangedEvent,
	WindowsChangedEvent, WindowOpenedOrChangedEvent, WindowClosedEvent,
	WindowFocusChangedEvent, WindowFocusTimestampChangedEvent,
	WindowUrgencyChangedEvent, WindowLayoutsChangedEvent,
	KeyboardLayoutsChangedEvent, KeyboardLayoutSwitchedEvent,
	OverviewOpenedOrClosedEvent, ConfigLoadedEvent, ScreenshotCapturedEvent,
	CastsChangedEvent, CastStartedOrChangedEvent, CastStoppedEvent>;
struct Event
{
	EventType event;
};

using Message = std::variant<Response, Event>;

} // namespace niri

namespace glz
{

template <> struct meta<niri::LayerSurface>
{
	using T = niri::LayerSurface;
	static constexpr auto value = glz::object(
		"namespace", &T::_namespace, "output", &T::output, "layer", &T::layer,
		"keyboard_interactivity", &T::keyboard_interactivity);
};

template <> struct meta<niri::LayoutChange>
{
	using T = niri::LayoutChange;
	static constexpr auto value = glz::array(&T::id, &T::data);
};
// template <> struct meta<niri::WindowLayoutsChangedEvent>
// {
// 	static constexpr auto value = object(
// 		"changes", glz::flatten_map<&niri::WindowLayoutsChangedEvent::changes>);
// };

template <> struct meta<niri::CastTarget>
{
	using T = niri::CastTarget;
	static constexpr auto custom_read
		= [](T& self, const generic& input, context& ctx) -> void
	{
		ctx.error = error_code::no_matching_variant_type;
		ctx.custom_error_message = "";

		if (!input.is_object())
		{
			niri::EmptyCastTarget target{};
			auto err = read<glzOpts{}>(target, input);
			ctx.error = err.ec;
			ctx.custom_error_message = err.custom_error_message;
			if (err.ec == error_code::none)
			{
				self.data = target;
			}
			return;
		}

		const auto& obj = input.get_object().begin()->second;
		niri::OutputID outID;
		auto err = read<glzOpts{}>(outID, obj);
		if (err.ec == error_code::none)
		{
			self.data = outID;
			ctx.error = err.ec;
			ctx.custom_error_message = err.custom_error_message;
			return;
		}
		niri::WindowID winID{};
		err = read<glzOpts{}>(winID, obj);
		if (err.ec == error_code::none)
		{
			self.data = winID;
			ctx.error = err.ec;
			ctx.custom_error_message = err.custom_error_message;
			return;
		}
	};
	using FullTarget = std::variant<niri::OutputID, niri::WindowID>;
	using OutType = std::variant<niri::EmptyCastTarget,
								 std::map<std::string_view, FullTarget>>;
	static constexpr auto custom_write = [](T& self) -> OutType
	{
		if (self.data.index() == 0)
		{
			return std::get<0>(self.data);
		}
		else if (self.data.index() == 1)
		{
			return std::map<std::string_view, FullTarget>{
				{"Output", niri::OutputID(std::get<1>(self.data))}};
		}
		else
		{
			return std::map<std::string_view, FullTarget>{
				{"Window", niri::WindowID(std::get<2>(self.data))}};
		}
	};
	static constexpr auto value = custom<custom_read, custom_write>;
};

template <> struct meta<niri::Response>
{
	static constexpr auto value = &niri::Response::data;
};
#ifndef __clangd__
template <> struct meta<niri::Event>
{
	using T = niri::Event;
	static constexpr size_t varSize = std::variant_size_v<niri::EventType>;
	static constexpr auto dealiased = std::meta::dealias(^^niri::EventType);
	static constexpr auto types
		= std::define_static_array(std::meta::template_arguments_of(dealiased));

	static constexpr auto custom_read
		= [](niri::Event& self, const generic& input, context& ctx) -> void
	{
		namespace rv = std::ranges::views;

		if (!input.is_object())
			return;

		ctx.error = error_code::no_matching_variant_type;
		ctx.custom_error_message = "Could not find Variant type";

		const auto& obj = input.get_object();
		const auto& var = obj.begin()->second;

		template for (constexpr auto I : rv::iota(0u, varSize))
		{
			constexpr auto V = types[I];

			std::string_view typeName = std::meta::identifier_of(V);
			typeName.remove_suffix(5);
			if (obj.begin()->first == typeName)
			{
				using VarType = [:V:];
				VarType event;
				auto err = read<glzOpts{}>(event, var);
				if (err.ec != error_code::none)
				{
					ctx.error = err.ec;
					ctx.custom_error_message = err.custom_error_message;
					// std::cout
					// 	<< "Error: "
					// 	<< err.custom_error_message
					// 	<< " | "
					// 	<< typeName
					// 	<< '\n';
				}
				else
				{
					ctx.error = error_code::none;
					ctx.custom_error_message = "";
					self.event = event;
				}
			}
		}
		// if (ctx.error != error_code::none)
		// {
		// 	std::cout
		// 		<< "Error: "
		// 		<< ctx.custom_error_message
		// 		<< " | "
		// 		<< obj.begin()->first
		// 		<< '\n';
		// }
	};
	static constexpr auto custom_write
		= [](T& self) -> std::map<std::string_view, niri::EventType>
	{
		namespace rv = std::ranges::views;

		const size_t idx = self.event.index();

		std::string_view eventType;
		template for (constexpr auto I : rv::iota(0u, varSize))
		{
			constexpr auto V = types[I];
			if (idx == I)
			{
				std::string_view typeName = std::meta::identifier_of(V);
				typeName.remove_suffix(5);
				eventType = typeName;
			}
		}
		return {{eventType, self.event}};
	};
	static constexpr auto value = glz::custom<custom_read, custom_write>;
};
#endif

} // namespace glz
