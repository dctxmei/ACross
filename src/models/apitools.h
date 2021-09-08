#ifndef APITOOLS_H
#define APITOOLS_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "v2ray_api.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using v2ray::core::app::stats::command::GetStatsRequest;
using v2ray::core::app::stats::command::GetStatsResponse;
using v2ray::core::app::stats::command::QueryStatsRequest;
using v2ray::core::app::stats::command::QueryStatsResponse;
using v2ray::core::app::stats::command::Stat;
using v2ray::core::app::stats::command::StatsService;
using v2ray::core::app::stats::command::SysStatsRequest;
using v2ray::core::app::stats::command::SysStatsResponse;

namespace across {
namespace core {
class APITools
{
public:
  explicit APITools(std::shared_ptr<Channel> channel);

  explicit APITools(uint port);

  std::pair<bool, std::string> isOk() const;

private:
  std::unique_ptr<StatsService::Stub> p_stub;

  std::string ipv4_local_address = "127.0.0.1:";
};
}
}

#endif // APITOOLS_H
