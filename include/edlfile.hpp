// Stefan Olivier
// Description: EDL class object for parsing EDL files

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Standard library
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// Project library
#include "errors.hpp"
#include "timecode.hpp"
#include "traits.hpp"
#include "utility.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VTM_EDLFILE_MACROS
#define VTM_EDLFILE_MACROS

#define __AVIDPTEDL_VALUE_TO_STRING(in) GET_ENUM_MAPPING_1(__AVIDPTEDL_STRING(), in)
#define __AVIDPTEDL_STRING_TO_VALUE(in) GET_ENUM_MAPPING_2(__AVIDPTEDL_STRING(), in)
#define __AVIDPTEDL_STRING() DECLARE_ENUM_MAPPING(vtm::edl::internal::__AvidPTEDLSection,            \
                                                  std::string_view,                                  \
                                                  []() { VTM_WARN("unknown Avid PT EDL section"); }, \
                                                  []() { VTM_WARN("unknown Avid PT EDL section"); }, \
                                                  vtm::edl::internal::__AvidPTEDLSection::none,      \
                                                  "NONE",                                            \
                                                  "HEADER",                                          \
                                                  "M A R K E R S  L I S T I N G",                    \
                                                  "O F F L I N E  F I L E S  I N  S E S S I O N",    \
                                                  "O N L I N E  C L I P S  I N  S E S S I O N",      \
                                                  "O N L I N E  F I L E S  I N  S E S S I O N",      \
                                                  "P L U G - I N S  L I S T I N G",                  \
                                                  "T R A C K  L I S T I N G")

#endif // @END OF VTM_EDLFILE_MACROS

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm::edl::internal {

enum class __AvidPTEDLSection : int
{
    header,
    markers_listing,
    offline_files,
    online_clips,
    online_files,
    plugin_listing,
    track_listing,
    none
};

template<vtm::traits::StringLike T>
inline constexpr auto edl_file_default_display() -> T
{
    return "EDL File Empty";
}

template<typename T>
concept EDLFileStringAliases = requires (T t) {
    vtm::traits::StringLike<typename T::string_t>;
    vtm::traits::StringLike<typename T::string_view_t>;
};

template<typename T>
concept EDLFileCompatible = EDLFileStringAliases<T>
                         && vtm::traits::InterfaceClear<T>
                         && vtm::traits::InterfaceDisplay<T>
                         && requires {
                            typename T::display_t;
                         };

enum class TimelineUnitFormat {
    samples,
    frames,
    barsbeats,
};

template<vtm::traits::StringLike TString, vtm::traits::StringLike TView>
class EDLFile : public vtm::traits::__clear,
                public vtm::traits::__implicit_string_overloads<TString, TView>,
                public vtm::traits::__display<typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type>
{
public:
    using string_t        = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_type;
    using string_view_t   = typename vtm::traits::__implicit_string_overloads<TString, TView>::string_view_type;
    using display_t       = string_view_t;

    EDLFile() = default;
    virtual ~EDLFile() noexcept = default;
    EDLFile(const EDLFile& edlfile) = delete;
    EDLFile(EDLFile&& edlfile) noexcept = delete;
    EDLFile& operator=(const EDLFile& edlfile) = delete;
    EDLFile& operator=(EDLFile&& edlfile) noexcept = delete;

    // File access 
    virtual auto parse_file(const string_view_t& path) -> void = 0;
    virtual auto write_file(const string_view_t& path) const -> void = 0;

    // Default Interfaces
    virtual auto display() const -> display_t
    {
        return edl_file_default_display<display_t>();
    }

    virtual operator string_t() const
    {
        return edl_file_default_display<string_t>();
    }

    virtual operator string_view_t() const
    {
        return edl_file_default_display<string_view_t>();
    }
};

template<typename Tstring, typename _Tstring = std::remove_cvref_t<Tstring>>
struct __AvidPTTrackEvent : public vtm::traits::__clear
{
    using string_t   = _Tstring;
    using timecode_t = vtm::chrono::float64_t;

    // Virtual interfaces
    void clear() noexcept;

    uint32_t id;
    uint32_t channel;
    string_t name;
    timecode_t start_time;
    timecode_t end_time;
    timecode_t duration;
    string_t state;
};

// TODO: EDLTrackDataInterface concept
template<typename T>
concept EDLTrackDataInterface = true;

template<typename Tstring = std::string, typename _Tstring = std::remove_cvref_t<Tstring>>
struct __AvidPTTrack : public vtm::traits::__clear
{
    using string_t          = _Tstring;
    using event_t           = __AvidPTTrackEvent<string_t>;
    using data_t            = std::vector<std::pair<std::size_t, event_t>>;
    using timeline_format_t = TimelineUnitFormat;

    
    // Virtual interfaces
    void clear() noexcept;

    // Accessor methods
    event_t operator[](std::size_t index);
    event_t& operator[](std::size_t index) const;

    string_t name;
    string_t comments;

    struct user_delay {
        uint32_t delay;
        timeline_format_t unit;
    };

    uint64_t state;
    data_t events;
};

// TODO: EDLTrackDataContainer concept
template<typename T>
concept EDLTrackDataContainer = true;

template<typename T>
concept EDLDataInterface =
    EDLFileStringAliases<T>
 && std::default_initializable<T>
 && std::destructible<T>
 && !std::copyable<T>
 && !std::equality_comparable<T>
 && requires (T t) {
        // Type Aliases
        typename T::track_t;
        typename T::event_t;
        typename T::data_t;
        typename T::timecode_t;
        typename T::timecode_fmt_t;
        
        // Member variables
        requires std::same_as<decltype(t.session_name), typename T::string_t>;
        requires std::unsigned_integral<decltype(t.sample_rate)>;
        requires std::unsigned_integral<decltype(t.bit_depth)>;
        requires std::unsigned_integral<decltype(t.audio_clips)>;
        requires std::unsigned_integral<decltype(t.audio_files)>;
        requires std::unsigned_integral<decltype(t.audio_tracks)>;
        requires std::same_as<decltype(t.session_start), typename T::timecode_t>;
        requires std::same_as<decltype(t.timecode_format), typename T::timecode_fmt_t>;
        requires std::same_as<decltype(t.tracks), typename T::data_t>;

        // Data type constraints
        requires EDLTrackDataContainer<typename T::data_t>;

        // Track data constraints
        requires EDLTrackDataInterface<typename T::track_t>;
    };

template<vtm::traits::StringLike TString,
         vtm::traits::StringLike TView>
struct __AvidPTEDLData
{
    using string_t = std::remove_cvref_t<TString>;
    using string_view_t = std::remove_cvref_t<TView>;
    using track_t  = __AvidPTTrack<string_t>;
    using event_t  = typename track_t::event_t;
    using data_t   = std::vector<std::pair<std::size_t, track_t>>; // TODO: template for container type
    using timecode_t = vtm::chrono::float64_t;
    using timecode_fmt_t = typename vtm::fps::type;

    __AvidPTEDLData() = default;
    ~__AvidPTEDLData() = default;
    __AvidPTEDLData(const __AvidPTEDLData& edlfile) = delete;
    __AvidPTEDLData(__AvidPTEDLData&& edlfile) noexcept = delete;
    __AvidPTEDLData& operator=(const __AvidPTEDLData& edlfile) = delete;
    __AvidPTEDLData& operator=(__AvidPTEDLData&& edlfile) noexcept = delete;
    bool operator==(const __AvidPTEDLData&) const = delete;

    // Data members
    string_t session_name;
    uint32_t sample_rate;
    uint32_t bit_depth;
    uint32_t audio_clips;
    uint32_t audio_files;
    uint32_t audio_tracks;
    timecode_t session_start;
    timecode_fmt_t timecode_format;
    data_t tracks;
};

template< vtm::traits::StringLike TString = std::string,
          vtm::traits::StringLike TView = std::string_view,
          EDLFileCompatible Interface = EDLFile<TString, TView>,
          EDLDataInterface TData = internal::__AvidPTEDLData<typename Interface::string_t, typename Interface::string_view_t> >
class __AvidPTEDLFile : public Interface
{
public:
    using string_t      = typename Interface::string_t;
    using string_view_t = typename Interface::string_view_t;
    using display_t     = typename Interface::display_t;
    using data_t        = std::remove_cvref_t<TData>;
    using track_t       = typename data_t::track_t;
    using event_t       = typename data_t::event_t;

    __AvidPTEDLFile() = default;
    ~__AvidPTEDLFile() = default;
    __AvidPTEDLFile(const __AvidPTEDLFile& edl_file) = default;
    __AvidPTEDLFile(__AvidPTEDLFile&& edl_file) noexcept = default;
    __AvidPTEDLFile& operator=(const __AvidPTEDLFile& edl_file) = default;
    __AvidPTEDLFile& operator=(__AvidPTEDLFile&& edl_file) noexcept = default;

    // Path constructors
    explicit __AvidPTEDLFile(const string_view_t& path) noexcept
    {
        try {
            this->parse_file(path);
        } catch (const std::runtime_error& e) {
            VTM_WARN(e.what());
        }
    }

public:
    // Virtual interfaces
    void parse_file(const string_view_t& path)
    {
        // Get size of file & reset position
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);

        if (!file)
            throw std::runtime_error(fmt::format("could not open file at specified path: {}", path));

        const std::size_t file_length = file.tellg();
        file.seekg(0);

    }

    void write_file(const string_view_t& path) const
    {
        VTM_TODO("not implemented");
    }

    virtual auto clear() noexcept -> void
    {
        VTM_TODO("not implemented");
    }
    
    virtual auto display() const noexcept -> display_t
    {
        VTM_TODO("not implemented");
    }
    
    // Member access
    track_t get_track(const std::string_view& name)
    {
        VTM_TODO("not implemented");
    }

    track_t& get_track(const std::string_view& name) const
    {
        VTM_TODO("not implemented");
    }

    track_t operator[](std::size_t index)
    {
        VTM_TODO("not implemented");
    }

    track_t& operator[](std::size_t index) const
    {
        VTM_TODO("not implemented");
    }

    event_t get_event(const std::string_view& name)
    {
        VTM_TODO("not implemented");
    }

    event_t& get_event(const std::string_view& name) const
    {
        VTM_TODO("not implemented");
    }

private:
    data_t _data;
};

} // @END OF namespace vtm::edl::internal

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace vtm {

    // Avid Pro Tools EDL file parser
    using avidpt_edl = edl::internal::__AvidPTEDLFile< std::string,
                                                       std::string_view >;
} // @END OF namespace vtm

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
