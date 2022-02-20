// Stefan Olivier
// Description: EDL class object for parsing EDL files

#pragma once

// Standard library
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <concepts>
#include <string>
#include <string_view>
#include <fstream>
#include <utility>
#include <vector>
#include <iterator>

// Project library
#include "io.hxx"
#include "locale.hxx"
#include "errors.hxx"

namespace sostd
{
    template<typename Tstring>
    class EDLFile 
    {
    public:
        using string_t = std::remove_cvref_t<Tstring>;

        EDLFile() = default;
        ~EDLFile() = default;
        EDLFile(const EDLFile& edlfile);
        EDLFile(EDLFile&& edlfile) noexcept;
        EDLFile& operator=(const EDLFile& edlfile);
        EDLFile& operator=(EDLFile&& edlfile) noexcept;

        // Virtual interfaces
        virtual int parse_file(const string_t& path) = 0;
        virtual int write_file(const string_t& path) const = 0;
    };

    namespace internal
    {
        enum class TimelineUnitFormat {
            samples,
            frames,
            barsbeats,
        };

        template<typename Tstring>
        struct AvidPTTrackEvent
        {
            using string_t = std::remove_cvref_t<Tstring>;

            uint32_t id;
            uint32_t channel;
            string_t name;
            timecode_t start_time;
            timecode_t end_time;
            timecode_t duration;
            string_t state;
        };

        template<typename Tstring>
        struct AvidPTTrack
        {
            using string_t          = std::remove_cvref_t<Tstring>;
            using event_t           = AvidPTTrackEvent<string_t>;
            using data_t            = std::vector<std::pair<std::size_t, event_t>>;
            using timeline_format_t = TimelineUnitFormat;

            // Iterators
            struct iterator {
                using iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = void;
                using pointer           = void*;
                using reference         = void;

                // TODO
            };

            string_t name;
            string_t comments;

            struct user_delay {
                uint32_t delay;
                timeline_format_t unit;
            };

            uint64_t state;
            data_t events;
        };
        
        template<typename Tstring>
        struct AvidPTEDLData 
        {
            using string_t = std::remove_cvref_t<Tstring>;
            using track_t  = AvidPTTrack<string_t>;
            using event_t  = typename track_t::event_t;
            using data_t   = std::vector<std::pair<std::size_t, track_t>>;

            // Iterators
            struct iterator { 
                using iterator_category = std::forward_iterator_tag;
                using difference_type   = std::ptrdiff_t;
                using value_type        = void;
                using pointer           = void*;
                using reference         = void;

                // TODO
            };

            string_t name;
            uint32_t sample_rate;
            uint32_t bit_depth;
            timecode_t session_start;
            timecode_format_t timecode_format;
            uint32_t total_audio_tracks;
            uint32_t total_audio_clips;
            uint32_t total_audio_files;
            data_t tracks;
        };
    }

    template<class Tstring>
    class EDLFileAvidPT : EDLFile<Tstring>
    {
    public:
        using interface_t = EDLFile<Tstring>;
        using string_t    = typename interface_t::string_t;
        using data_t      = internal::AvidPTEDLData<string_t>;
        using track_t     = typename data_t::track_t;
        using event_t     = typename data_t::event_t;

        EDLFileAvidPT() = default;
        ~EDLFileAvidPT() = default;
        EDLFileAvidPT(const EDLFileAvidPT& edlfile);
        EDLFileAvidPT(EDLFileAvidPT&& edlfile) noexcept;
        EDLFileAvidPT& operator=(const EDLFileAvidPT& edlfile);
        EDLFileAvidPT& operator=(EDLFileAvidPT& edlfile) noexcept;

        // Virtual interfaces
        int parse_file(const string_t& path);
        int write_file() const;

        // Accessor methods
        data_t& get_all_tracks() const;
        data_t& get_all_events() const;

        track_t& get_track(std::size_t index) const;
        track_t& get_track(const string_t& track_name) const;

        template<std::invocable Tpredicate>
        track_t& get_track(const std::remove_cvref_t<Tpredicate>& predicate) const;

        event_t& get_event(std::size_t index) const;
        event_t& get_event(const string_t& event_name) const;

        template<std::invocable Tpredicate>
        event_t& get_event(const std::remove_cvref_t<Tpredicate>& predicate) const;

    private:
        data_t _data;
    };
}















