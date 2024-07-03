#include <exception>
#include <stdexcept>

#pragma once

class BadConfigError : std::runtime_error
{
public:
  BadConfigError() : 
    std::runtime_error("Configuration file have bad info, it will be restarted")
  {}
};

class ConfigFileNotFound : std::runtime_error
{
public:
  ConfigFileNotFound() :
    std::runtime_error("Configuration file wasn't found, it will be created")
  {}
};

class BadPositionDataError : std::runtime_error
{
public:
  BadPositionDataError() :
    std::runtime_error("The opened position doesn't contain valid array JSON")
  {}
};