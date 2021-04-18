#pragma once

#include <Arduino.h>

class Schedule {
  private:
    uint32_t interval, last;
    bool enabled, immediate;

  public:
    Schedule(uint32_t _interval, bool _enabled = true)
      : interval(_interval), last(millis()), enabled(_enabled), immediate(false) {
    }

    void set_interval(uint32_t _interval) {
      this->interval = _interval;
      this->reset();
    }

    void set_immediate() {
      this->immediate = true;
    }

    bool execute() {
      if(!enabled)
        return false;

      if(immediate) {
        this->immediate = false;
        this->last = millis();
        return true;
      }

      // overflow
      if(this->last > millis()) {
        if(UINT32_MAX - last + millis() > this->interval) {
          this->immediate = false;
          this->last = millis();
          return true;
        } else {
          return false;
        }
      }

      // normal
      if(millis() >= (this->interval + this->last) || immediate) {
        this->last = millis();
        this->immediate = false;
        return true;
      }
      return false;
    }

    void set_enabled(bool e) {
      this->enabled = e;
      if(e)
        this->last = millis();
    }

    bool is_enabled() {
      return this->enabled;
    }

    void reset() {
      this->last = millis();
    }
  };

class Onetime {
    uint32_t next, delay;
    bool enabled;

  public:
    Onetime(uint32_t delay) : next(0), delay(delay), enabled(false) {
    }

    void set_delay(uint32_t delay) {
      this->delay = delay;
    }

    void enable() {
      next = millis() + delay;
      enabled = true;
    }

    void disable() {
      next = 0;
      enabled = false;
    }

    bool execute() {
      if(enabled && next != 0 && (millis() >= next)) {
        enabled = false;
        next = 0;
        return true;
      }
      return false;
    }

    bool is_enabled() {
      return this->enabled;
    }
  };
