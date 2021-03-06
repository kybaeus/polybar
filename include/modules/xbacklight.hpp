#pragma once

#include "components/config.hpp"
#include "config.hpp"
#include "modules/meta/event_handler.hpp"
#include "modules/meta/input_handler.hpp"
#include "modules/meta/static_module.hpp"
#include "x11/extensions/randr.hpp"

POLYBAR_NS

class connection;

namespace modules {
  /**
   * Backlight module built using the RandR X extension.
   *
   * This is built as a replacement for the old backlight
   * module that was set up using with inotify watches listening
   * for changes to the raw file handle.
   *
   * This module is alot faster, it's more responsive and it will
   * be dormant until new values are reported. Inotify watches
   * are a bit random when it comes to proc-/sysfs.
   *
   * TODO: Implement backlight configuring using scroll events
   */
  class xbacklight_module : public static_module<xbacklight_module>,
                            public event_handler<evt::randr_notify>,
                            public input_handler {
   public:
    explicit xbacklight_module(const bar_settings& bar, string name_);

    void update();
    string get_output();
    bool build(builder* builder, const string& tag) const;

   protected:
    void handle(const evt::randr_notify& evt);
    bool on(const input_event_t& evt);

   private:
    static constexpr const char* TAG_LABEL{"<label>"};
    static constexpr const char* TAG_BAR{"<bar>"};
    static constexpr const char* TAG_RAMP{"<ramp>"};

    static constexpr const char* EVENT_SCROLLUP{"xbacklight+"};
    static constexpr const char* EVENT_SCROLLDOWN{"xbacklight-"};

    connection& m_connection;
    monitor_t m_output;
    xcb_window_t m_proxy{};

    ramp_t m_ramp;
    label_t m_label;
    progressbar_t m_progressbar;

    bool m_scroll{true};
    int m_percentage{0};
  };
}

POLYBAR_NS_END
