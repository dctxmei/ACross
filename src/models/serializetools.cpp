#include "serializetools.h"

using namespace across::config;

SerializeTools::SerializeTools() {}

std::optional<SIP008>
SerializeTools::sip008Parser(const std::string& data)
{
  JsonTools json_tools;
  if (!json_tools.setData(data)) {
    return {};
  }

  auto root = json_tools.getRoot();
  SIP008 sip008;

  try {
    if (root.empty() || !root.isMember("version")) {
      return {};
    }
  } catch (Json::Exception e) {
    return {};
  }

  sip008.version = root["version"].asInt();
  sip008.username = root["username"].asString();
  sip008.user_uuid = root["id"].asString();
  sip008.bytes_used = root["bytes_used"].asLargestUInt();
  sip008.bytes_remaining = root["bytes_remaining"].asLargestUInt();

  for (auto& item : root["servers"]) {
    SIP008::Server server{
      .id = item["id"].asString(),
      .remarks = item["remarks"].asString(),
      .server = item["server"].asString(),
      .server_port = item["server_port"].asUInt(),
      .password = item["password"].asString(),
      .method = item["method"].asString(),
      .plugin = item["plugin"].asString(),
      .plugin_opts = item["plugin_opts"].asString(),
    };

    sip008.servers.emplace_back(server);
  }

  return sip008;
}

bool
SerializeTools::decodeOutboundFromURL(NodeInfo& node, const QString& raw_url)
{
  bool result = false;

  do {
    QUrl url(raw_url);
    auto scheme = magic_enum::enum_cast<SchemeType>(url.scheme().toStdString());

    if (!scheme.has_value()) {
      break;
    }

    switch (scheme.value()) {
      case SchemeType::ss:
        result = SerializeTools::setShadowsocksOutboundFromURL(node, url);
        break;
      case SchemeType::vmess:
        result = SerializeTools::setVMessOutboundFromBase64(
          node, raw_url.toStdString());
        break;
      case SchemeType::trojan:
        result = SerializeTools::setTrojanOutboundFromURL(node, url);
        break;
      default:
        break;
    }
  } while (false);

  return result;
}

bool
SerializeTools::setShadowsocksOutboundFromURL(NodeInfo& node, const QUrl& url)
{
  bool result = false;

  do {
    auto sip008_server = SerializeTools::sip002Decode(url);
    if (!sip008_server.has_value()) {
      break;
    }

    ShadowsocksObject::OutboundSettingObject outbound_setting;
    outbound_setting.fromSIP008Server(sip008_server.value());

    OutboundObject outbound_object;
    outbound_object.appendShadowsocksObject(outbound_setting);

    node.protocol = across::EntryType::shadowsocks;
    node.name = QString::fromStdString(sip008_server->remarks);
    node.address = QString::fromStdString(outbound_setting.address);
    node.port = outbound_setting.port;
    node.password = QString::fromStdString(outbound_setting.password);
    node.raw =
      QString::fromStdString(outbound_object.toObject().toStyledString());

    result = true;
  } while (false);

  return result;
}

bool
SerializeTools::setVMessOutboundFromBase64(NodeInfo& node,
                                           const std::string& data)
{
  bool result = false;

  do {
    auto vmess_meta = SerializeTools::vmessBase64Decode(data);
    if (!vmess_meta.has_value()) {
      break;
    }
    auto outbound_object = vmess_meta->outbound_object;

    node.protocol = across::EntryType::vmess;
    node.name = QString::fromStdString(vmess_meta->name);
    node.address = QString::fromStdString(vmess_meta->address);
    node.port = vmess_meta->port;
    node.password = QString::fromStdString(vmess_meta->password);
    node.raw =
      QString::fromStdString(outbound_object.toObject().toStyledString());

    result = true;
  } while (false);

  return result;
}

bool
SerializeTools::setTrojanOutboundFromURL(NodeInfo& node, const QUrl& url)
{
  bool result = false;

  do {
    auto trojan_meta = SerializeTools::trojanDecode(url);
    if (!trojan_meta.has_value()) {
      break;
    }
    auto outbound_object = trojan_meta->outbound_object;

    node.protocol = across::EntryType::trojan;
    node.name = QString::fromStdString(trojan_meta->name);
    node.address = QString::fromStdString(trojan_meta->address);
    node.port = trojan_meta->port;
    node.password = QString::fromStdString(trojan_meta->password);
    node.raw =
      QString::fromStdString(outbound_object.toObject().toStyledString());

    result = true;
  } while (false);

  return result;
}

std::optional<SIP008::Server>
SerializeTools::sip002Decode(const QUrl& url)
{
  // url scheme:
  // ss://<websafe-base64-encode-utf8(method:password)>@hostname:port/?plugin"#"tag
  SIP008::Server server;

  QString user_info = QByteArray::fromBase64(url.userInfo().toUtf8());

  server.method = user_info.split(":").first().toStdString();
  server.password = user_info.split(":").last().toStdString();
  server.server = url.host().toStdString();
  server.server_port = url.port();
  server.remarks = url.fragment(QUrl::FullyDecoded).toStdString();

  if (server.method.empty() || server.password.empty() ||
      server.server.empty()) {
    return {};
  }

  return server;
}

std::optional<QUrl>
SerializeTools::sip002Encode(const SIP008::Server& node)
{
  QUrl url;
  QString user_info = QString("%1:%2")
                        .arg(QString::fromStdString(node.method),
                             QString::fromStdString(node.password))
                        .toUtf8()
                        .toBase64(QByteArray::Base64UrlEncoding);

  url.setScheme("ss");
  url.setHost(QString::fromStdString(node.server));
  url.setPort(node.server_port);
  url.setUserInfo(user_info);
  url.setFragment(QString::fromStdString(node.remarks));

  return url;
}

std::optional<URLMetaObject>
SerializeTools::trojanDecode(const QUrl& url)
{
  // url scheme:
  // trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>
  do {
    OutboundObject outbound_object;

    TrojanObject::OutboundSettingObject outbound_setting;
    outbound_setting.address = url.host().toStdString();
    outbound_setting.port = url.port();
    outbound_setting.password = url.userInfo().toStdString();

    outbound_object.appendTrojanObject(outbound_setting);

    // query need to be verified
    if (url.hasQuery()) {
      QUrlQuery url_query(url);
      TransportObject::OutboundStreamObject outbound_stream;
      TransportObject::TLSObject tls;

      if (url_query.hasQueryItem("sni")) {
        tls.server_name = url_query.queryItemValue("sni").toStdString();
      }

      if (url_query.hasQueryItem("allowinsecure")) {
        tls.allow_insecure =
          url_query.queryItemValue("allowinsecure") == "true";
      }

      if (url_query.hasQueryItem("alpn")) {
        auto alpn = url_query.queryItemValue("alpn", QUrl::FullyDecoded);

        QStringList values;

        if (alpn.contains(",")) {
          values = alpn.split(",");
        } else if (alpn.contains("\n")) {
          values = alpn.split("\n");
        }

        if (values.empty()) {
          tls.appendAlpn(alpn.toStdString());
        } else {
          for (auto& item : values) {
            tls.appendAlpn(item.toStdString());
          }
        }
      }

      outbound_stream.setTLSSetting(tls);
      outbound_object.setTransportStreamObject(outbound_stream);
    }

    return URLMetaObject{ url.fragment(QUrl::FullyDecoded).toStdString(),
                          outbound_setting.address,
                          outbound_setting.port,
                          outbound_setting.password,
                          outbound_object };
  } while (false);

  return {};
}

std::optional<URLMetaObject>
SerializeTools::vmessBase64Decode(const std::string& data)
{
  do {
    QString info = QString::fromStdString(data).split("://").takeLast();
    if (info.isEmpty()) {
      break;
    }

    QString base64_str = QByteArray::fromBase64(info.toUtf8());

    JsonTools json_tools;
    if (!json_tools.setData(base64_str.toStdString())) {
      break;
    }
    auto root = json_tools.getRoot();

    VMessObject::UserObject user_object;
    user_object.id = root["id"].asString();
    user_object.alter_id = root["aid"].asInt();

    VMessObject::OutboundSettingObject outbound_setting;
    outbound_setting.address = root["add"].asString();
    outbound_setting.port = root["port"].asInt();
    outbound_setting.appendUserObject(user_object);

    TransportObject::OutboundStreamObject outbound_stream;
    auto network =
      magic_enum::enum_cast<TransportObject::Network>(root["net"].asString());
    if (!network.has_value()) {
      break;
    } else {
      outbound_stream.network = network.value();
    }

    TransportObject::WebSocketObject websocket;

    switch (outbound_stream.network) {
      case TransportObject::Network::ws:
        websocket.setHost(root["host"].asString());
        websocket.path = root["path"].asString();
        outbound_stream.setWebsocketSetting(websocket);
      default:
        break;
    }

    if (!root["tls"].empty()) {
      outbound_stream.security = root["tls"].asString();
    }

    if (!root["sni"].empty()) {
      TransportObject::TLSObject tls;
      tls.server_name = root["sni"].asString();
      outbound_stream.setTLSSetting(tls);
    }

    OutboundObject outbound_object;
    outbound_object.appendVMessObject(outbound_setting);
    outbound_object.setTransportStreamObject(outbound_stream);

    return URLMetaObject{ root["ps"].asString(),
                          outbound_setting.address,
                          outbound_setting.port,
                          user_object.id,
                          outbound_object };
  } while (false);

  return {};
}
