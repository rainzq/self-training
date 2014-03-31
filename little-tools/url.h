#ifndef __URL_H__
#define __URL_H__

#include <string>
#include <algorithm>
#include <cctype>

class Url {
    public:
        Url(const std::string &url) { parse(url); }
        
        std::string protocol() { return protocol_; }
        std::string host() { return host_; };
        std::string path() { return path_; };
        std::string query() { return query_; };

    private:
        inline void parse(const std::string &url) {
            /** set protocol */
            const std::string protocol_end("://");
            auto host_i = std::search(url.begin(), url.end(), protocol_end.begin(), protocol_end.end());
            protocol_.reserve(std::distance(url.begin(), host_i));
            std::transform(url.begin(), host_i, back_inserter(protocol_), ::tolower);
            if (host_i == url.end()) {
                return;
            }
            /** set host */
            std::advance(host_i, protocol_end.size());
            auto path_i = std::find(host_i, url.end(), '/');
            host_.reserve(std::distance(host_i, path_i));
            std::transform(host_i, path_i, back_inserter(host_), ::tolower);
            /** set path */
            auto query_i = std::find(path_i, url.end(), '?');
            path_.assign(path_i, query_i);
            /** set query */
            if (query_i != url.end()) {
                ++query_i;
            }
            query_.assign(query_i, url.end());
        }

    private:
        std::string protocol_;
        std::string host_;
        std::string path_;
        std::string query_;
};

#endif
