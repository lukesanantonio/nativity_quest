/*
 * Copyright (C) 2014 Luke San Antonio
 * All rights reserved.
 */
#pragma once
#include <string>
namespace game
{
  struct Scoped_Log_Init
  {
    Scoped_Log_Init() noexcept;
    ~Scoped_Log_Init() noexcept;
  };
  void init_log() noexcept;
  void uninit_log() noexcept;
  void flush_log() noexcept;

  enum class Log_Severity
  {
    Debug, Info, Warning, Error
  };

  void set_log_level(Log_Severity level) noexcept;

  void log_e(std::string msg) noexcept;
  void log_w(std::string msg) noexcept;
  void log_i(std::string msg) noexcept;
  void log_d(std::string msg) noexcept;
}
